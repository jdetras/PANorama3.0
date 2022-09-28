#include "ift.h"

/* Data structure for panicle measurements */

typedef struct ift_panicle {
  iftImage   *skel;     // corresponding skeleton image
  iftSegment *maxis;    // its main axis
  iftSet     *E;        // its end points
  iftSegment **pbran;   // its primary branches
  int  num_of_pbran;    // number of primary branches
  float       scale;    // number of pixels per cm
} iftPanicle;

/* Panicle creator */

iftPanicle *iftCreatePanicle()
{
  iftPanicle *pan=(iftPanicle *)calloc(1,sizeof(iftPanicle));

  pan->skel         = NULL;
  pan->maxis        = iftCreateSegment();
  pan->E            = NULL;
  pan->pbran        = NULL;
  pan->num_of_pbran = 0;
  pan->scale        = 0.0;

  return(pan);
}

/* Panicle destroyer */

void iftDestroyPanicle(iftPanicle **pan)
{
  iftPanicle *aux=*pan;
  int i;

  if (aux != NULL) {
    iftDestroyImage(&(aux->skel));
    iftDestroySegment(&(aux->maxis));
    iftDestroySet(&(aux->E));
    for (i=0; i < aux->num_of_pbran; i++){
      iftDestroySegment(&(aux->pbran[i]));
    }
    free(aux->pbran);
    free(aux);
    *pan = NULL;
  }
}

/* Auxiliary function to read xml tag */

char iftReadXMLTag(FILE **fp, char *tag, char *label)
{
  fscanf((*fp),"%s",tag);
  if (strcmp(tag,label)!=0)
    return(0);
  else
    return(1);
}

/* Read skeleton with its end points from an xml file */

iftPanicle *iftReadPanicleInfo(char *filename)
{
  FILE *fp=fopen(filename,"r");
  int p,npts,xsize,ysize;
  iftVoxel u;
  iftPanicle *pan=iftCreatePanicle();
  char tag[100];
 
  if (!iftReadXMLTag(&fp,tag,"<Image>"))
    iftError("Missing <Image> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"<FileInfo>"))
    iftError("Missing <FileInfo> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"<ImageWidth>"))
    iftError("Missing <ImageWidth> in xml file","iftReadPanicleInfo");

  fscanf(fp,"%d",&xsize);

  if (!iftReadXMLTag(&fp,tag,"</ImageWidth>"))
    iftError("Missing </ImageWidth> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"<ImageHeight>"))
    iftError("Missing <ImageHeight> in xml file","iftReadPanicleInfo");

  fscanf(fp,"%d",&ysize);

  if (!iftReadXMLTag(&fp,tag,"</ImageHeight>"))
    iftError("Missing </ImageHeight> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"<Scale>"))
    iftError("Missing <Scale> in xml file","iftReadPanicleInfo");
    
  fscanf(fp,"%f",&pan->scale);

  if (!iftReadXMLTag(&fp,tag,"</Scale>"))
    iftError("Missing </Scale> in xml file","iftReadPanicleInfo");
  
  if (!iftReadXMLTag(&fp,tag,"</FileInfo>"))
    iftError("Missing </FileInfo> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"<ProcessingInfo>"))
    iftError("Missing <ProcessingInfo> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"<SkeletonPoints>"))
    iftError("Missing <SkeletonPoints> in xml file","iftReadPanicleInfo");

  pan->skel = iftCreateImage(xsize,ysize,1);

  if (!iftReadXMLTag(&fp,tag,"<SklPtNumber>"))
    iftError("Missing <SklPtNumber> in xml file","iftReadPanicleInfo");

  fscanf(fp,"%d",&npts);

  if (!iftReadXMLTag(&fp,tag,"</SklPtNumber>"))
    iftError("Missing </SklPtNumber> in xml file","iftReadPanicleInfo");

  u.z=0;
  while(npts > 0){
    if (!iftReadXMLTag(&fp,tag,"<SklPt>"))
      iftError("Missing <SklPt> in xml file","iftReadPanicleInfo");
    fscanf(fp,"%d %d",&u.x,&u.y);
    p = iftGetVoxelIndex(pan->skel,u);
    pan->skel->val[p]=255;
    npts--;
    if (!iftReadXMLTag(&fp,tag,"</SklPt>"))
      iftError("Missing </SklPt> in xml file","iftReadPanicleInfo");
  }

  if (!iftReadXMLTag(&fp,tag,"</SkeletonPoints>"))
    iftError("Missing </SkeletonPoints> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"<TerminalPoints>"))
    iftError("Missing <TerminalPoints> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"<TermPtNumber>"))
    iftError("Missing <TermPtNumber> in xml file","iftReadPanicleInfo");
  
  fscanf(fp,"%d",&npts);

  if (!iftReadXMLTag(&fp,tag,"</TermPtNumber>"))
    iftError("Missing </TermPtNumber> in xml file","iftReadPanicleInfo");
  
  u.z=0;
  while (npts > 0){
    if (!iftReadXMLTag(&fp,tag,"<TermPt>"))
      iftError("Missing <TermPt> in xml file","iftReadPanicleInfo");
    fscanf(fp,"%d %d",&u.x,&u.y);
    p = iftGetVoxelIndex(pan->skel,u);
    iftInsertSet(&(pan->E),p);
    npts--;
    if (!iftReadXMLTag(&fp,tag,"</TermPt>"))
      iftError("Missing </TermPt> in xml file","iftReadPanicleInfo");
  }

  if (!iftReadXMLTag(&fp,tag,"</TerminalPoints>"))
    iftWarning("Missing </TerminalPoints> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"</ProcessingInfo>"))
    iftError("Missing </ProcessingInfo> in xml file","iftReadPanicleInfo");

  if (!iftReadXMLTag(&fp,tag,"</Image>"))
    iftError("Missing </Image> in xml file","iftReadPanicleInfo");
  
  fclose(fp);

  return(pan);
}

/* Search the closest junction point within 10 pixels around the
   selected point u. This is an auxiliary function for iftMainAxis */

void iftLastPointOnMainAxis(iftPanicle *pan, iftVoxel u)
{
  iftAdjRel *A=iftCircular(10.0);
  iftVoxel   v;
  int        i, q, dist, qmin, dmin;
  
  dmin = INFINITY_INT; qmin = NIL;

  for (i=0; i < A->n; i++) {
    v.x = u.x + A->dx[i];
    v.y = u.y + A->dy[i];
    v.z = 0;
    if (iftValidVoxel(pan->skel,v)){
      q   = iftGetVoxelIndex(pan->skel,v);
      if (pan->skel->val[q]!=0){ /* skeleton point */
	dist = (u.x - v.x)*(u.x - v.x) + (u.y - v.y)*(u.y - v.y);
	if (dist < dmin){
	  dmin = dist;
	  qmin = q;	    
	}
      }
    }
  }
  
  if (qmin != NIL) {
    pan->maxis->last = qmin;
    iftDestroyAdjRel(&A);
    return;
  }
  
  iftError("Could not find terminal point on main axis","iftLastPointOnMainAxis");  
}

/* Read terminal point of the main axis and compute it */

void iftMainAxis(iftPanicle *pan, char *basename)
{
  FILE      *fp=NULL;
  char       filename[100];
  iftSet    *Eaux;
  iftPoint   pt;
  iftVoxel   u;
  iftAdjRel *A=iftCircular(sqrtf(2.0));
  int        p, pmax, ymax;

  if (pan->skel == NULL) 
    iftError("There is no skeleton related to this panicle","iftMainAxis");

  // Read the selected point

  sprintf(filename,"./skeletons/%s_skel_mainaxis.txt",basename);
  fp = fopen(filename,"r");
  if (fp == 0) 
    iftError("First execute iftPanicleSkel and MainAxis.py","iftMainAxis");

  fscanf(fp,"%f %f",&pt.x,&pt.y);
  fclose(fp);

  /* Find the last point on the main axis as the skeleton point
     closest to the selected point */
  
  u.x = (int)pt.x; u.y = (int)pt.y;
  iftLastPointOnMainAxis(pan,u);

  /* Find the first point on the main axis as the lowest one in the image */

  pmax = pan->E->elem;
  ymax = iftGetYCoord(pan->skel,pmax);    
  Eaux = pan->E;
  while (Eaux != NULL) {
    p    = Eaux->elem;
    u.y  = iftGetYCoord(pan->skel,p);    
    if (u.y > ymax){
      ymax = u.y;
      pmax = p;
    }
    Eaux = Eaux->next;
  }
  pan->maxis->first = pmax;

  /* Find the main axis */

  pan->maxis->S = iftFindPathOnSkeleton(pan->skel,A,pan->maxis->first,pan->maxis->last);
  iftDestroyAdjRel(&A);
}

/* Find seed points to compute primary branches and mark junction
   points on the main axis */

iftSet *iftSeedPointsForPrimaryBranches(iftPanicle *pan, iftBMap *maxis)
{
  iftSet  *Saux=NULL;
  int      p,q,i;
  iftAdjRel *A=iftCircular(sqrtf(2.0));
  iftVoxel u,v;

  
  for (p=0; p < pan->skel->n; p++) {
    if ((pan->skel->val[p]!=0)&&(iftBMapValue(maxis,p)==0)){
      u.x = iftGetXCoord(pan->skel,p);
      u.y = iftGetYCoord(pan->skel,p);
      u.z = 0;
      for (i=1; i < A->n; i++){
	v.x = u.x + A->dx[i];
	v.y = u.y + A->dy[i];
	v.z = 0;
	if (iftValidVoxel(pan->skel,v)){
	  q = iftGetVoxelIndex(pan->skel,v);
	  if ((pan->skel->val[q]!=0)&&(iftBMapValue(maxis,q)==1)){
	    // p is a seed point
	    iftInsertSet(&Saux,p);
	    break;
	  }
	}
      }
    }
  }
  iftDestroyAdjRel(&A);

  return(Saux);
}

/* Find the primary branches of the panicle */
 
void iftPrimaryBranches(iftPanicle *pan)
{
  iftFIFO    *F=iftCreateFIFO(pan->skel->n);
  iftSet     *Saux;
  int        *pred, *label, *dist, *leaf, *dmax, p, q, i, j;
  iftVoxel    u,v;
  iftAdjRel  *A=iftCircular(sqrtf(2.0));
  iftBMap    *maxis=iftCreateBMap(pan->skel->n);
  iftBMap    *junction=iftCreateBMap(pan->skel->n);
  iftBMap    *pbran=iftCreateBMap(pan->skel->n);
  int         num_of_pbran=0;
  iftSegment *pbranch;
 
  if (pan->maxis->S == NULL)
    iftError("There is no main axis related to the skeleton","iftPrimaryBranches");

  /* Initialize forest maps */

  pred  = iftAllocIntArray(pan->skel->n);
  label = iftAllocIntArray(pan->skel->n);
  dist  = iftAllocIntArray(pan->skel->n);

  /* Mark pixels on the main axis */

  Saux = pan->maxis->S;
  while (Saux != NULL) {
    iftBMapSet1(maxis,Saux->elem);
    Saux = Saux->next;
  }
  
  /* Find seed points as the skeleton points that are connected to the
     main axis, but they are not part of the main axis. The number of
     seed points will be the number of candidates for primary
     branch. Insert the seed points in a FIFO queue. */


  Saux=iftSeedPointsForPrimaryBranches(pan, maxis);
  while (Saux != NULL) {
    p = iftRemoveSet(&Saux);
    iftInsertFIFO(F,p);
    pred[p]=NIL;
    label[p]=p;
    dist[p]=0;
  }

  /* Compute the skeleton forest rooted at the seed points, but
     exclude main axis from the computation */

  while(!iftEmptyFIFO(F)){

    p   = iftRemoveFIFO(F);

    u.x = iftGetXCoord(pan->skel,p);
    u.y = iftGetYCoord(pan->skel,p);
    u.z = 0;
    
    for (i=1; i < A->n; i++){
      v.x = u.x + A->dx[i];
      v.y = u.y + A->dy[i];
      v.z = 0;
      if (iftValidVoxel(pan->skel,v)){
	q = iftGetVoxelIndex(pan->skel,v);
	if (iftBMapValue(maxis,q)==0){
	  if ((pan->skel->val[q]!=0)&&(F->color[q] == WHITE)){
	    pred[q]=p; label[q]=label[p]; dist[q]=dist[p]+1;
	    iftInsertFIFO(F,q);
	  }
	}
      }
    }
  }

  iftDestroyFIFO(&F);

  /* Each root of the forest determines a candidate for primary
     branch. Label them with subsequent integer numbers.*/

  i=0;
  num_of_pbran = 0;
  for (p=0; p < pan->skel->n; p++)
    if (pred[p]==NIL){
      i++; label[p]=i;      
    }
  num_of_pbran=i;

  if (num_of_pbran <= 1){
    iftError("Invalid number of primary branches","iftPrimaryBranch");
  }

  pbranch  = (iftSegment *)calloc(num_of_pbran,sizeof(iftSegment));

  /* Propagate labels to the remaining nodes of each tree */
  
  for (p=0; p < pan->skel->n; p++)
    if (pan->skel->val[p]!=0){
      if ((pred[p]!=NIL)&&(iftBMapValue(maxis,p)==0))
      label[p]=label[label[p]];
    }

  /* For each tree, find the most distant leaf node (end point out of
     the main axis) from its root. Find the primary branch candidate
     as constituting of this end point and all path points that
     connect it to a last point before reaching the main axis. Store
     the first and last points on each primary branch candidate and
     mark its points. */
  
  leaf = iftAllocIntArray(num_of_pbran);
  dmax = iftAllocIntArray(num_of_pbran);
  for (i=0; i < num_of_pbran; i++) {
    leaf[i]=NIL;
    dmax[i]=-INFINITY_INT;
  }
  
  Saux = pan->E;
  while(Saux != NULL) {
    p = Saux->elem;
    if (iftBMapValue(maxis,p)==0){ 
      if (dist[p]>dmax[label[p]-1]){ 
	leaf[label[p]-1]=p; 
	dmax[label[p]-1]=dist[p]; 
      }
    }
    Saux = Saux->next;
  }

  /* Note that, if leaf[i]==NIL => the candidate is a trivial tree,
     and so, it is not a primary branch for sure. */

  for (i=0; i < num_of_pbran; i++) {
    pbranch[i].last = leaf[i];
    p = leaf[i];
    if (p != NIL) {
      while((pred[p]!=NIL)&&(iftBMapValue(maxis,p)==0)){
	iftInsertSet(&(pbranch[i].S),p);
	iftBMapSet1(pbran,p);
	p = pred[p];
      }
      iftInsertSet(&(pbranch[i].S),p);
      iftBMapSet1(pbran,p);
      pbranch[i].first = p;
    }
  }
  free(leaf);
  free(dmax);
  free(dist);
  free(label);

  /* For each remaining end point, which is neither on a primary
     branch candidate nor on the main axis, follow its path toward a
     primary branch candidate and mark the corresponding junction
     point. Note that this end point cannot be a root point (trivial
     tree previously discarded as primary branch candidate) */

  Saux = pan->E;
  while(Saux != NULL) {
    p = Saux->elem;
    if ((pred[p]!=NIL)&&
	(iftBMapValue(maxis,p)==0)&&
	(iftBMapValue(pbran,p)==0)){ 
      q = p;
      /* If the skeleton is broken on thin parts, due to digitization,
	 then you will need to add (q !=
	 0)&&(iftBMapValue(pbran,q)==0) in the while command below in
	 order to discard the corresponding sequences of end points
	 and avoid infinite loop. This is not being used because I am
	 selecting the largest component after skeleton digitization
	 in iftPanicleSkel.c */
      while((iftBMapValue(pbran,q)==0)){ 
	q = pred[q];
      }
      iftBMapSet1(junction,q);
    }
    Saux = Saux->next;
  }
  iftDestroyBMap(&pbran);	
  free(pred);

  /* Insert junction points on their corresponding primary branches */

  for (i=0; i < num_of_pbran; i++) {
    Saux = pbranch[i].S;
    while (Saux != NULL) {
      p = Saux->elem;
      if (iftBMapValue(junction,p)==1){
	iftInsertSet(&(pbranch[i].J),p);
      }
      Saux = Saux->next;
    }
    iftInvertSet(&(pbranch[i].J));
  }
  
  /* Eliminate candidates without junction points and save the true
     primary branches */

  pan->num_of_pbran=0;
  for (i=0; i < num_of_pbran; i++) 
    if (pbranch[i].J != NULL)
      pan->num_of_pbran++;

  if (num_of_pbran <= 1){
    iftError("Invalid number of real primary branches","iftPrimaryBranch");
  }

  pan->pbran  = (iftSegment **)calloc(pan->num_of_pbran,sizeof(iftSegment *));
  for (i=0; i < pan->num_of_pbran; i++) 
    pan->pbran[i] = iftCreateSegment();

  j=0;
  for (i=0; i < num_of_pbran; i++) 
    if (pbranch[i].J != NULL){
      pan->pbran[j]->S     = pbranch[i].S;
      pan->pbran[j]->J     = pbranch[i].J;
      pan->pbran[j]->first = pbranch[i].first;
      pan->pbran[j]->last  = pbranch[i].last;
      j++;
    }

  /* Mark and save junction points on the main axis */

  for (j=0; j < pan->num_of_pbran; j++){
    p   = pan->pbran[j]->first;
    u.x = iftGetXCoord(pan->skel,p);
    u.y = iftGetYCoord(pan->skel,p);
    u.z = 0;
    
    for (i=1; i < A->n; i++){
      v.x = u.x + A->dx[i];
      v.y = u.y + A->dy[i];
      v.z = 0;
      if (iftValidVoxel(pan->skel,v)){
	q = iftGetVoxelIndex(pan->skel,v);
	if ((iftBMapValue(maxis,q)==1)&&
	    (q!=pan->maxis->last)&&
	    (q!=pan->maxis->first)){
	  iftBMapSet1(junction,q);
	  break;
	}
      }
    }
  }

  Saux = pan->maxis->S;
  while (Saux != NULL) {
    p=Saux->elem;
    if (iftBMapValue(junction,p)==1){
      iftInsertSet(&(pan->maxis->J),p);
    }
    Saux = Saux->next;
  }
  iftInvertSet(&(pan->maxis->J));

  iftDestroyBMap(&junction);
  iftDestroyBMap(&maxis);
  iftDestroyAdjRel(&A);
 
  for (i=0; i < num_of_pbran; i++) 
    if ((pbranch[i].J == NULL)&&(pbranch[i].first !=NIL)){
      iftDestroySet(&pbranch[i].S);
    }
  free(pbranch);

}

int iftNumberOfEndPoints(iftPanicle *pan)
{
  iftSet *E=pan->E;
  int n;

  n=0;
  while (E!=NULL){ 
    n++;
    E = E->next;
  }
  return(n);
}

void iftWritePanicleMeasures(iftPanicle *pan, char *basename)
{
  FILE *fp=NULL;
  iftAdjRel *A=iftCircular(sqrtf(2.0));
  int i;
  iftSegment *seg;
  
  /* if ((fp = fopen("PanicleMeasures.csv","r"))) { */
  /*   fclose(fp); */
  /* }else{ // File does not exist, so create it with a first line */
  /*   fp = fopen("PanicleMeasures.csv","w"); */
  /*   fprintf(fp, "Basename;"); */
  /*   fprintf(fp, "Term. points (#);"); */
  /*   fprintf(fp, "Length of main axis (cm);"); */
  /*   fprintf(fp, "Subsegments on main axis (#);"); */
  /*   for (i=1; i <= iftNumberOfJunctionPoints(pan->maxis)+1; i++){ */
  /*     fprintf(fp, "Length %d between nodes (cm);",i); */
  /*   } */
  /*   fprintf(fp, "Prim. branches (#);"); */
  /*   for (i=0; i < pan->num_of_pbran; i++){ */
  /*     fprintf(fp, "Length of branch %d (cm);",i+1); */
  /*     fprintf(fp, "Nodes on branch %d (#);",i+1); */
  /*   } */
  /*   fprintf(fp,"\n"); */
  /*   fclose(fp); */
  /* } */

  fp = fopen("PanicleMeasures.csv","a");

  fprintf(fp, "Basename,"); 
  fprintf(fp, "Term. points (#),"); 
  fprintf(fp, "Length of main axis (cm),"); 
  fprintf(fp, "Subsegments on main axis (#),"); 
  for (i=1; i <= iftNumberOfJunctionPoints(pan->maxis)+1; i++){ 
    fprintf(fp, "Length %d between nodes (cm),",i); 
  } 
  fprintf(fp, "Prim. branches (#),"); 
  for (i=0; i < pan->num_of_pbran; i++){ 
    fprintf(fp, "Length of branch %d (cm),",i+1); 
  } 
  for (i=0; i < pan->num_of_pbran; i++){ 
    fprintf(fp, "Nodes on branch %d (#),",i+1); 
  }
  fprintf(fp,"\n"); 

  fprintf(fp, "%s,",basename);

  fprintf(fp, "%d,",iftNumberOfEndPoints(pan));

  fprintf(fp, "%7.4f,",iftSegmentLength(pan->maxis,pan->skel,A,pan->scale,0.5));

  fprintf(fp, "%d,",iftNumberOfJunctionPoints(pan->maxis)+1);

  for (i=1; i <= iftNumberOfJunctionPoints(pan->maxis)+1; i++){
    seg = iftGetSubSegment(pan->maxis,i);
    fprintf(fp, "%7.4f,",iftSegmentLength(seg,pan->skel,A,pan->scale,0.5));
    iftDestroySegment(&seg);
  }

  fprintf(fp, "%d,",pan->num_of_pbran);
  for (i=0; i < pan->num_of_pbran; i++){
    fprintf(fp, "%7.4f,",iftSegmentLength(pan->pbran[i],pan->skel,A,pan->scale,0.5));
  }

  for (i=0; i < pan->num_of_pbran; i++){
    fprintf(fp, "%d,",iftNumberOfJunctionPoints(pan->pbran[i]));
  }

  fprintf(fp,"\n");
  iftDestroyAdjRel(&A);
  fclose(fp);
}



int main(int argc, char *argv[]) 
{
  iftImage       *orig=NULL;
  char            filename[400],command[500],*basename;
  iftColor        RGB,YCbCr;
  iftAdjRel      *B=NULL;
  iftPanicle     *pan;
  int             i;
  timer          *t1=NULL,*t2=NULL;

  /*--------------------------------------------------------*/

  void *trash = malloc(1);                 
  struct mallinfo info;   
  int MemDinInicial, MemDinFinal;
  free(trash); 
  info = mallinfo();
  MemDinInicial = info.uordblks;

  /*--------------------------------------------------------*/


  if (argc!=2)
    iftError("Usage must be: iftPanicleMeasures <image.jpg>","main");

  // Read image file of the panicle

  basename = strtok(argv[1],".");
  sprintf(filename,"./skeletons/%s_skel.jpg",basename);
  sprintf(command,"convert %s temp.ppm",filename);
  system(command);
  orig    = iftReadImageP6("temp.ppm");
  sprintf(command,"rm -f temp.ppm");
  system(command);

  // Read file with skeleton, junction, and terminal points 


  sprintf(filename,"./skeletons/%s.xml",basename);
  pan = iftReadPanicleInfo(filename);


  t1     = iftTic();

  // Read file with the terminal point of the main axis and compute
  // main axis and terminal points.

  iftMainAxis(pan,basename);


  // Find primary branches and junction points of the main axis

  iftPrimaryBranches(pan); 


  // Draw acquired information

  RGB.val[0]=255; RGB.val[1]=255; RGB.val[2]=0;
  YCbCr = iftRGBtoYCbCr(RGB); 
  B = iftCircular(1.0);
  iftDrawObject(orig,pan->skel,YCbCr,B); /* Draw skeleton */


  RGB.val[0]=255; RGB.val[1]=150; RGB.val[2]=0;
  YCbCr = iftRGBtoYCbCr(RGB); 
  iftDrawPoints(orig, pan->maxis->S, YCbCr, B); /* Draw main axis */


  RGB.val[0]=0; RGB.val[1]=255; RGB.val[2]=255;
  YCbCr = iftRGBtoYCbCr(RGB); 
  for (i=0; i < pan->num_of_pbran; i++) /* Draw primary branches */
    iftDrawPoints(orig, pan->pbran[i]->S, YCbCr, B); 


  RGB.val[0]=0; RGB.val[1]=0; RGB.val[2]=255;
  YCbCr = iftRGBtoYCbCr(RGB); 
  iftDestroyAdjRel(&B);
  B = iftCircular(3.0);  
  iftDrawPoints(orig,pan->E,YCbCr,B); /* Draw end points */


  RGB.val[0]=255; RGB.val[1]=0; RGB.val[2]=255; /* Draw junction on
						   the primary
						   branches */
  YCbCr = iftRGBtoYCbCr(RGB); 
  iftDestroyAdjRel(&B);
  B = iftCircular(3.0); 
  for (i=0; i < pan->num_of_pbran; i++) {
    iftDrawPoints(orig,pan->pbran[i]->J,YCbCr,B);
  }


  RGB.val[0]=0; RGB.val[1]=255; RGB.val[2]=0; /* Draw junction on the
						 main axis */
  YCbCr = iftRGBtoYCbCr(RGB); 
  iftDestroyAdjRel(&B);
  B = iftCircular(3.0); 
  iftDrawPoints(orig,pan->maxis->J,YCbCr,B);


  RGB.val[0]=0; RGB.val[1]=0; RGB.val[2]=255; /* Draw terminal point
						 of the main axis */
  YCbCr = iftRGBtoYCbCr(RGB); 
  iftDestroyAdjRel(&B);
  B = iftCircular(3.0);
  iftVoxel u;
  u.x = iftGetXCoord(orig,pan->maxis->last);
  u.y = iftGetYCoord(orig,pan->maxis->last);
  u.z = 0;
  iftDrawPoint(orig,u,YCbCr,B);

  iftWritePanicleMeasures(pan, basename);


  t2     = iftToc();
  fprintf(stdout,"Measure extraction in %f ms\n",iftCompTime(t1,t2));

  iftWriteImageP6(orig,"temp.ppm");
  sprintf(command,"convert temp.ppm ./skeletons/%s_skel.jpg",basename);
  system(command);
  sprintf(command,"rm -f temp.ppm");
  system(command);

  iftDestroyAdjRel(&B);
  iftDestroyImage(&orig);  
  iftDestroyPanicle(&pan);  

  /* ---------------------------------------------------------- */

  info = mallinfo();
  MemDinFinal = info.uordblks;
  if (MemDinInicial!=MemDinFinal)
    printf("\n\nDynamic memory was not completely deallocated (%d, %d)\n",
	   MemDinInicial,MemDinFinal);   

  return(0);
}



