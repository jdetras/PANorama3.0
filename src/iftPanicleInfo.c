#include "iftPANorama.h"


/* 
   Author: Alexandre Falcao.  

   Description: This program computes the informations about the
   panicle, such as main axis, extrusion, nodes on the main axis, and
   stores these informations in a .inf file for future measure
   computation.

*/

void        iftMainAxisAndExtrusion(iftPanicle *pan, char *filename);
void        iftPrimaryBranches(iftPanicle *pan);
void        iftFirstPointOnEachBranch(iftPanicle *pan, iftImage *label, iftImage *mainaxis);
char        iftDeleteShortBranches(iftImage *label, iftImage *sknoaxis, int thres);
void        iftJunctionsOnMainAxis(iftPanicle *pan);
iftPanicle *iftReadSkeletonOnly(char *filename);


iftPanicle *iftReadSkeletonOnly(char *filename)
{
  iftPanicle *pan=iftCreatePanicleInfo();
  FILE *fp=NULL;
  int i,p,n,*buffer;
  
  fp = fopen(filename,"rb");

  if (fp == NULL) 
    iftError(MSG2,"iftReadSkeletonOnly");

  /* Read skeleton scale, domain, number of points, point addresses,
     and number of branches */   

  fread(&pan->scale,sizeof(float),1,fp);
  fread(&n,sizeof(int),1,fp);
  buffer = iftAllocIntArray(n);
  fread(buffer,sizeof(int),n,fp);
  pan->skel = iftCreateImage(buffer[0],buffer[1],1);
  pan->nskelpts = n-3;
  for (i=2; i < pan->nskelpts+2; i++) {
    p = buffer[i];
    pan->skel->val[p] = 1;
  }
  free(buffer);

  fclose(fp);

  return(pan);
}


void iftMainAxisAndExtrusion(iftPanicle *pan, char *filename)
{
  FILE      *fp=fopen(filename,"r");
  float      val;
  iftVoxel   P[3],u,v;
  int        i,p,q,p1,p2;
  iftAdjRel *A=iftCircular(sqrtf(2.0));
  iftFIFO   *F=iftCreateFIFO(pan->skel->n);
  int       *pred=iftAllocIntArray(pan->skel->n);
  int       *length=iftAllocIntArray(pan->skel->n);

  if (fp == NULL) 
    iftError(MSG2,"iftMainAxisAndExtrusion");

  /* Read the three given points */

  for (i=0; i < 3; i++) { 
    fscanf(fp,"%f",&val); P[i].x = (int)val; 
    fscanf(fp,"%f",&val); P[i].y = (int)val; 
    P[i].z = 0;
    if (!iftValidVoxel(pan->skel,P[i]))
      iftError("Point is outside the image domain","iftMainAxisAndExtrusion");

    /* If it is the case, correct point selected outside the skeleton */
    if (pan->skel->val[iftGetVoxelIndex(pan->skel,P[i])]==0) 
      P[i] = iftGetClosestSkeletonPoint(pan, P[i]);
  }
  fclose(fp);

  pan->mainaxis.last  = P[2];
  pan->mainaxis.first = P[1];

  /* Starting from the last point, compute points along the main axis
     by assuming that the skeleton is one-pixel-wide and connected,
     and that the first point will be found before the initial
     extrusion point. */


  p = iftGetVoxelIndex(pan->skel,pan->mainaxis.last);
  iftInsertFIFO(F,p);
  pred[p]   = NIL;
  length[p] = 1;
  p1        = iftGetVoxelIndex(pan->skel,pan->mainaxis.first);

  while(!iftEmptyFIFO(F)){
    p = iftRemoveFIFO(F);
    if (p == p1)
      break;
    u = iftGetVoxelCoord(pan->skel,p);
    for (i=1; i < A->n; i++) {
      v = iftGetAdjacentVoxel(A,u,i);
      if (iftValidVoxel(pan->skel,v)){
	q = iftGetVoxelIndex(pan->skel,v);
	if (pan->skel->val[q]!=0){
	  if (F->color[q]==WHITE){
	    iftInsertFIFO(F,q);
	    pred[q]=p;
	    length[q]=length[p]+1;
	  }
	}
      }
    }
  }


  /* Store points along the main axis */

  if (p != p1){
    iftError("Could not define main axis","iftMainAxisAndExtrusion");
  }
  
  pan->mainaxis.npoints = length[p];
  pan->mainaxis.point   = (iftVoxel *)calloc(pan->mainaxis.npoints,sizeof(iftVoxel));
  q=p; i=0;
  while(q != NIL){
    pan->mainaxis.point[i]=iftGetVoxelCoord(pan->skel,q); i++;      
    q = pred[q];
  }    
  
  if (iftVoxelDistance(P[0],pan->mainaxis.first) < (pan->scale*MINEXTSZ)){ /* Consider
									   no
									   extrusion */
    p2 = p;

  }else{
    p2 = iftGetVoxelIndex(pan->skel,P[0]) ;
    /* Starting from the first point, compute points along the extrusion
       by assuming that the skeleton is one-pixel-wide and connected,
       and that the initial extrusion point will be found. */

    p = iftGetVoxelIndex(pan->skel,pan->mainaxis.first);
    iftResetFIFO(F);
    iftInsertFIFO(F,p);
    pred[p]   = NIL;
    length[p] = 0;

    while(!iftEmptyFIFO(F)){
      p = iftRemoveFIFO(F);
      if (p == p2)
	break;
      u = iftGetVoxelCoord(pan->skel,p);
      for (i=1; i < A->n; i++) {
	v = iftGetAdjacentVoxel(A,u,i);
	if (iftValidVoxel(pan->skel,v)){
	  q = iftGetVoxelIndex(pan->skel,v);
	  if (pan->skel->val[q]!=0){
	    if (F->color[q]==WHITE){
	      iftInsertFIFO(F,q);
	      pred[q]=p;
	      length[q]=length[p]+1;
	    }
	  }
	}
      }
    }
  
    if (p != p2)
      iftError("Could not define extrusion","iftMainAxisAndExtrusion");

    pan->nextpts      = length[p];
    pan->extrusion    = (iftVoxel *)calloc(pan->nextpts,sizeof(iftVoxel));
    q=p; i=0;
    while(pred[q] != NIL){ 
      pan->extrusion[i]=iftGetVoxelCoord(pan->skel,q); i++;
      q = pred[q];
    }
  }

  iftDestroyAdjRel(&A);
  iftDestroyFIFO(&F);
  free(pred);
  free(length);
}

/* void iftMainAxisAndExtrusion(iftPanicle *pan, char *filename) */
/* { */
/*   FILE      *fp=fopen(filename,"r"); */
/*   float      val; */
/*   iftVoxel   P[3],u,v; */
/*   int        i,j,p,q,d[3],p1,p2,e; */
/*   iftAdjRel *A=iftCircular(sqrtf(2.0)); */
/*   iftFIFO   *F=iftCreateFIFO(pan->skel->n); */
/*   int       *pred=iftAllocIntArray(pan->skel->n); */
/*   int       *length=iftAllocIntArray(pan->skel->n); */

/*   if (fp == NULL)  */
/*     iftError(MSG2,"iftMainAxisAndExtrusion"); */

/*   /\* Read the three given points *\/ */

/*   for (i=0; i < 3; i++) {  */
/*     fscanf(fp,"%f",&val); P[i].x = (int)val;  */
/*     fscanf(fp,"%f",&val); P[i].y = (int)val;  */
/*     P[i].z = 0; */
/*     if (!iftValidVoxel(pan->skel,P[i])) */
/*       iftError("Point is outside the image domain","iftMainAxisAndExtrusion"); */

/*     /\* If it is the case, correct point selected outside the skeleton *\/ */
/*     if (pan->skel->val[iftGetVoxelIndex(pan->skel,P[i])]==0)  */
/*       P[i] = iftGetClosestSkeletonPoint(pan, P[i]); */
/*   } */
/*   fclose(fp); */

/*   /\* Force consistency among them in case the panicle image was in */
/*      arbitrary position. The last point on the main axis is defined as */
/*      the most distance to the others *\/ */
  
/*   for (i=0; i < 3; i++) { */
/*     d[i]=0; */
/*     for (j=0; j < 3; j++) */
/*       if (i!=j) */
/*   	d[i] += iftVoxelSquareDistance(P[i],P[j]); */
/*   } */

/*   p1 = p2 = NIL; */
/*   if ((d[0] > d[1])&&(d[0]>d[2])){ /\* P[0] is the last point *\/ */
/*     pan->mainaxis.last = P[0]; */
/*     p1 = iftGetVoxelIndex(pan->skel,P[1]); */
/*     p2 = iftGetVoxelIndex(pan->skel,P[2]); */
/*   }else{ */
/*     if ((d[1] > d[0])&&(d[1]>d[2])){ /\* P[1] is the last point *\/ */
/*       pan->mainaxis.last = P[1]; */
/*       p1 = iftGetVoxelIndex(pan->skel,P[0]); */
/*       p2 = iftGetVoxelIndex(pan->skel,P[2]); */
/*     }else{ */
/*       if ((d[2] > d[0])&&(d[2]>d[1])){ /\* P[2] is the last point *\/ */
/*   	pan->mainaxis.last = P[2]; */
/*   	p1 = iftGetVoxelIndex(pan->skel,P[0]); */
/*   	p2 = iftGetVoxelIndex(pan->skel,P[1]); */
/*       }else{ */
/*   	iftError("Could not identify the last point","iftMainAxisAndExtrusion"); */
/*       } */
/*     } */
/*   } */


/*   /\* Starting from the last point, compute points along the main axis */
/*      by assuming that the skeleton is one-pixel-wide and connected, */
/*      and that the first point will be found before the initial */
/*      extrusion point. *\/ */


/*   p = iftGetVoxelIndex(pan->skel,pan->mainaxis.last); */
/*   iftInsertFIFO(F,p); */
/*   pred[p]   = NIL; */
/*   length[p] = 1; */

/*   while(!iftEmptyFIFO(F)){ */
/*     p = iftRemoveFIFO(F); */
/*     if ( (p == p1) || (p == p2) ) */
/*       break; */
/*     u = iftGetVoxelCoord(pan->skel,p); */
/*     for (i=1; i < A->n; i++) { */
/*       v = iftGetAdjacentVoxel(A,u,i); */
/*       if (iftValidVoxel(pan->skel,v)){ */
/* 	q = iftGetVoxelIndex(pan->skel,v); */
/* 	if (pan->skel->val[q]!=0){ */
/* 	  if (F->color[q]==WHITE){ */
/* 	    iftInsertFIFO(F,q); */
/* 	    pred[q]=p; */
/* 	    length[q]=length[p]+1; */
/* 	  } */
/* 	} */
/*       } */
/*     } */
/*   } */


/*   /\* Store points along the main axis *\/ */

/*   e = NIL; */
/*   if (p == p1){ */
/*     pan->mainaxis.first   = iftGetVoxelCoord(pan->skel,p1); */
/*     e = p2; */
/*   }else{ */
/*     if (p == p2){ */
/*       pan->mainaxis.first = iftGetVoxelCoord(pan->skel,p2); */
/*       e = p1; */
/*     }else{ */
/*       iftError("Could not define main axis","iftMainAxisAndExtrusion"); */
/*     } */
/*   } */


/*   pan->mainaxis.npoints = length[p]; */
/*   pan->mainaxis.point   = (iftVoxel *)calloc(pan->mainaxis.npoints,sizeof(iftVoxel)); */
/*   q=p; i=0; */
/*   while(q != NIL){ */
/*     pan->mainaxis.point[i]=iftGetVoxelCoord(pan->skel,q); i++;       */
/*     q = pred[q]; */
/*   }     */
  

/*   u = iftGetVoxelCoord(pan->skel,e); */
/*   if (iftVoxelDistance(u,pan->mainaxis.first) < (pan->scale*MINEXTSZ)){ /\* Consider */
/* 									   no */
/* 									   extrusion *\/ */
/*     e = p; */
/*   } */

/*   if (p != e) { /\* Extrusion *\/ */

/*   /\* Starting from the first point, compute points along the extrusion */
/*      by assuming that the skeleton is one-pixel-wide and connected, */
/*      and that the initial extrusion point will be found. *\/ */

/*     p = iftGetVoxelIndex(pan->skel,pan->mainaxis.first); */
/*     iftResetFIFO(F); */
/*     iftInsertFIFO(F,p); */
/*     pred[p]   = NIL; */
/*     length[p] = 0; */

/*     while(!iftEmptyFIFO(F)){ */
/*       p = iftRemoveFIFO(F); */
/*       if (p == e) */
/* 	break; */
/*       u = iftGetVoxelCoord(pan->skel,p); */
/*       for (i=1; i < A->n; i++) { */
/* 	v = iftGetAdjacentVoxel(A,u,i); */
/* 	if (iftValidVoxel(pan->skel,v)){ */
/* 	  q = iftGetVoxelIndex(pan->skel,v); */
/* 	  if (pan->skel->val[q]!=0){ */
/* 	    if (F->color[q]==WHITE){ */
/* 	      iftInsertFIFO(F,q); */
/* 	      pred[q]=p; */
/* 	      length[q]=length[p]+1; */
/* 	    } */
/* 	  } */
/* 	} */
/*       } */
/*     } */

/*     if (p == e){ */
/*       pan->nextpts      = length[p]; */
/*       pan->extrusion    = (iftVoxel *)calloc(pan->nextpts,sizeof(iftVoxel)); */
/*       q=e; i=0; */
/*       while(pred[q] != NIL){  */
/*   	pan->extrusion[i]=iftGetVoxelCoord(pan->skel,q); i++; */
/*   	q = pred[q]; */
/*       } */
/*     }else{ */
/*       iftError("Could not define extrusion","iftMainAxisAndExtrusion"); */
/*     } */
/*   } */

/*   iftDestroyAdjRel(&A); */
/*   iftDestroyFIFO(&F); */
/*   free(pred); */
/*   free(length); */
/* } */



void iftFirstPointOnEachBranch(iftPanicle *pan, iftImage *label, iftImage *mainaxis)
{
  int p,q,i;
  iftVoxel u,v;
  iftAdjRel *A=iftCircular(sqrtf(2.0));

  /* The first point on each branch is a point of the skeleton without
     main axis (in label), which has an 8-neighbor on the mainaxis. */

  for (p=0; p < label->n; p++) 
    if (label->val[p]>0){
      u = iftGetVoxelCoord(label,p);
      for (i=1; i < A->n; i++) {
	v = iftGetAdjacentVoxel(A,u,i);
	if (iftValidVoxel(label,v)){
	  q = iftGetVoxelIndex(label,v);
	  if (mainaxis->val[q]!=0){
	    pan->primary[label->val[p]-1].first = u;
	    break; 
	  }
	}
      }
    }

  iftDestroyAdjRel(&A);
}



char iftDeleteShortBranches(iftImage *label, iftImage *sknoaxis, int thres)
{
  int p,*hist = iftAllocIntArray(iftMaximumValue(label)+1);
  char short_branch=0;

  for (p=0; p < label->n; p++) 
    hist[label->val[p]]++;

  for (p=0; p < label->n; p++) {
    if (hist[label->val[p]] <= thres){
      sknoaxis->val[p]=0;
      short_branch = 1;
    }
  }

  free(hist);
  return(short_branch);
}
  
void iftPrimaryBranches(iftPanicle *pan)
{
  iftImage   *mainaxis=NULL,*label=NULL;
  iftImage   *endpts=NULL,*sknoaxis=NULL;
  iftAdjRel  *A=iftCircular(sqrtf(2.0));
  int        *pred=NULL,*length=NULL,i,j,p,q;
  iftFIFO    *F=NULL;
  iftVoxel    u,v;
  
  mainaxis  = iftImageOfMainAxis(pan);
  sknoaxis  = iftSkeletonWithoutMainAxis(pan);
  label     = iftFastLabelComp(sknoaxis,A);
  if (iftDeleteShortBranches(label,sknoaxis,MINBRANCSZ*pan->scale)){
    iftDestroyImage(&label);
    label = iftFastLabelComp(sknoaxis,A);
  }
  iftDestroyImage(&sknoaxis);
  pan->nbranches = iftMaximumValue(label);
  pan->primary   = (iftBranch *)calloc(pan->nbranches,sizeof(iftBranch));
  endpts         = iftImageOfEndPoints(pan->skel);
  iftFirstPointOnEachBranch(pan, label, mainaxis);

  /* Compute all paths in each labeled component in a predecessor map
     and define the longest ones as the primary branches. The length
     of each path is also computed on the fly. Whenever a terminal
     point is reached, if it is in the same component and the path
     length to it is higher than the current value, it becomes the
     next candidate for last point on the respective primary
     branch. At the end of this process, the points along each primary
     branch are obtained from the predecessor map, backwards, starting
     at the last last point of each branch and ending at the first
     point. */

  F       = iftCreateFIFO(label->n);
  pred    = iftAllocIntArray(label->n);
  length  = iftAllocIntArray(label->n);

  for (i=0; i < pan->nbranches; i++) {
    p = iftGetVoxelIndex(label,pan->primary[i].first);
    if (p != 0) { /* valid branch */
      iftInsertFIFO(F,p);
      pred[p]   = NIL;
      length[p] = 1;
    }
  }

  while(!iftEmptyFIFO(F)){
    p = iftRemoveFIFO(F);
    u = iftGetVoxelCoord(label,p);

    /* Select last point in each branch */
    
    v = pan->primary[label->val[p]-1].last;
    q = iftGetVoxelIndex(label,v);
    if ((endpts->val[p]!=0)&&(length[p]>length[q])){
      pan->primary[label->val[p]-1].last = u;
    }

    /* Continue path propagation */

    for (i=1; i < A->n; i++) {
      v = iftGetAdjacentVoxel(A,u,i);
      if (iftValidVoxel(label,v)){
	q = iftGetVoxelIndex(label,v);
	if (label->val[q]==label->val[p]){
	  if (F->color[q]==WHITE){
	    iftInsertFIFO(F,q);
	    pred[q]=p;
	    length[q]=length[p]+1;
	  }
	}
      }
    }
  }


  iftDestroyFIFO(&F);
  iftDestroyAdjRel(&A);

  /* Retrieve points along each primary branch */

  for (i=0; i < pan->nbranches; i++) {
    q = iftGetVoxelIndex(label,pan->primary[i].last);
    if (q != 0) {
      pan->primary[i].npoints = length[q];
      pan->primary[i].point = (iftVoxel *)calloc(pan->primary[i].npoints,sizeof(iftVoxel));
      if (pan->primary[i].point == NULL) 
	iftError(MSG1,"iftPrimaryBranches");
      j = pan->primary[i].npoints-1;
      p = iftGetVoxelIndex(label,pan->primary[i].first);
      while (q != p){
	pan->primary[i].point[j]=iftGetVoxelCoord(label,q);
	q = pred[q]; j--;
      }
      pan->primary[i].point[j]=iftGetVoxelCoord(label,q);
    }
    iftInvertVoxelSequence(pan->primary[i].point,pan->primary[i].npoints);
  }

  iftDestroyImage(&mainaxis);
  iftDestroyImage(&label);
  iftDestroyImage(&endpts);
  free(length);
  free(pred);

}

void iftJunctionsOnMainAxis(iftPanicle *pan)
{

  if ((pan->mainaxis.point == NULL)||(pan->primary == NULL)) 
    iftError("First find primary branches and main axis","iftJunctionsOnMainAxis");

  float dist[pan->nbranches][pan->nbranches];
  int i,j,root[pan->nbranches],r1,r2;

  /* Compute distances between all pairs of first points on primary
     branches */

  for (i=0; i < pan->nbranches; i++){   
    root[i]=i;
    for (j=0; j < pan->nbranches; j++)
      if (i != j) 
	dist[i][j] = iftVoxelDistance(pan->primary[i].first,pan->primary[j].first);
  }

  /* Group nearby points which should indicate a same junction point
     on the main axis (Apply a simple union-find approach) */

  for (i=0; i < pan->nbranches; i++){   
    for (j=0; j < pan->nbranches; j++){
      if (i != j) {
	if (dist[i][j] <= (MINJCTDIST*pan->scale)){ 
	  r1=i;    // find set root 
	  while(root[r1]!=r1)
	    r1=root[r1];
	  r2=j;    // find set root
	  while(root[r2]!=r2)
	    r2=root[r2];
	  if (r1 != r2) { // set union
	    root[r2]=r1;
	  }
	}
      }
    }
  }

  /* Compress root information */
  for (i=0; i < pan->nbranches; i++) {
    r1=i;
    while(root[r1]!=r1) 
      r1 = root[r1];
    root[i]=r1;
  }
  


  iftSet *S=NULL;

  pan->njunctions = 0; /* Compute roots and number of groups */
  for (i=0; i < pan->nbranches; i++){ 
    if (root[i]==i){
      pan->njunctions++;
      iftInsertSet(&S,i);
    }
  }
  pan->junction = iftAllocIntArray(pan->njunctions);

  /* Compute the geometric center of each group */

  iftPoint center[pan->njunctions];
  int npts[pan->njunctions];
  int     k;

  for (k=0; k < pan->njunctions; k++) {
    npts[k]     = 0; 
    center[k].x = center[k].y = center[k].z = 0.0;
  }

  k = 0;
  while (S != NULL) {
    r1   = iftRemoveSet(&S);
    for (j=0; j < pan->nbranches; j++) {
      if (root[j]==r1){ /* pan->primary[j].first belongs to the group of r1 */
	center[k].x += pan->primary[j].first.x;
	center[k].y += pan->primary[j].first.y;
	npts[k]++;
      }
    }
    k++;
  }
  
  for (k=0; k < pan->njunctions; k++) {
    center[k].x /= npts[k];
    center[k].y /= npts[k];
  }

  /* For each group, compute the point along the main axis, which is
     closest to its geometric center, and define it as the junction
     point. */

   for (k=0; k < pan->njunctions; k++) { 
     pan->junction[k] = iftClosestPointInSegment(pan->mainaxis.point,pan->mainaxis.npoints,center[k]);
   }
 
  /* float    dmin,d;  */
  /* iftPoint P; */

  /* P.z = 0.0; */
  /* for (k=0; k < pan->njunctions; k++) { */
  /*   dmin=INFINITY_FLT; */
  /*   for (i=0; i < pan->mainaxis.npoints; i++) { */
  /*     P.x = (float) pan->mainaxis.point[i].x; */
  /*     P.y = (float) pan->mainaxis.point[i].y; */
  /*     d = iftPointDistance(center[k],P); */
  /*     if (d < dmin){ */
  /* 	dmin = d;  */
  /* 	pan->junction[k] = i; */
  /*     } */
  /*   } */
  /* } */
  
  /* Sort the junction points from the closest to the farthest with
     respect to the first point along the main axis */
	  
  int index[pan->njunctions];

  for (k=0; k < pan->njunctions; k++){
    index[k]=k;
  }
  iftQuickSort(pan->junction,index,0,pan->njunctions-1,INCREASING);

}

int main(int argc, char *argv[]) 
{
  iftImage       *orig,*panimg,*seeds;
  iftPanicle     *pan;
  char            command[200],filename[120],*basename;
  timer          *t1=NULL,*t2=NULL;

  /*--------------------------------------------------------*/

  /* void *trash = malloc(1);                  */
  /* struct mallinfo info;    */
  /* int MemDinInicial, MemDinFinal; */
  /* free(trash);  */
  /* info = mallinfo(); */
  /* MemDinInicial = info.uordblks; */

  /*--------------------------------------------------------*/


  if (argc!=2)
    iftError("Usage: iftPanicleInfo <image.jpg>","main");

  // Read image file of the panicle

  basename = strtok(argv[1],".");
  sprintf(filename,"./originals/%s.jpg",basename);
  sprintf(command,"convert %s temp.pgm",filename);
  system(command);
  orig    = iftReadImageP5("temp.pgm");
  sprintf(command,"rm -f temp.pgm");
  system(command);

  // Read panicle information with its skeleton

  sprintf(filename,"./skeletons/%s.inf",basename);
  pan = iftReadSkeletonOnly(filename);

  t1     = iftTic();
  
  sprintf(filename,"./skeletons/%s_skel_mainaxis.txt",basename);
  iftMainAxisAndExtrusion(pan,filename);  
  iftPrimaryBranches(pan);
  iftJunctionsOnMainAxis(pan);
  iftSortBranches(pan);
  sprintf(filename,"./skeletons/%s.inf",basename);
  iftWritePanicleInfo(pan,filename);

  seeds = iftSeedSegmentation(pan,orig);
  iftImage *temp = iftColorizeCompOverImage(orig,seeds);
  iftWriteImageP6(temp,"temp.ppm");
  sprintf(filename,"./skeletons/%s_seeds.jpg",basename);
  sprintf(command,"convert temp.ppm %s",filename);
  system(command);
  sprintf(command,"rm -f temp.ppm");
  system(command);
  iftDestroyImage(&seeds);
  iftDestroyImage(&temp);


  t2     = iftToc();
  fprintf(stdout,"Panicle information was extracted in %f ms\n",iftCompTime(t1,t2));

  panimg = iftDrawPanicleInfo(orig, pan);
  iftWriteImageP6(panimg,"temp.ppm");
  sprintf(filename,"./skeletons/%s_skel.jpg",basename);
  sprintf(command,"convert temp.ppm %s",filename);
  system(command);
  sprintf(command,"rm -f temp.ppm");
  system(command);


  iftDestroyImage(&orig);  
  iftDestroyPanicleInfo(&pan);  
  iftDestroyImage(&panimg);  

  /* ---------------------------------------------------------- */

  /* info = mallinfo(); */
  /* MemDinFinal = info.uordblks; */
  
  /* if (MemDinInicial!=MemDinFinal) */
  /*   printf("\n\nDynamic memory was not completely deallocated (%d, %d)\n", */
  /* 	   MemDinInicial,MemDinFinal);   */ 
  
  return(0);
}



