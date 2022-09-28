#include "iftPANorama.h"


/* Note that by adjusting the threshold and counting the connected
   components, we may have a good estimate of the number of seeds */

iftImage *iftDeleteThinParts(iftImage *bin, int distance_thres)
{
  iftImage  *dist=NULL, *objects=iftCopyImage(bin),*label=NULL;
  iftAdjRel *A=iftCircular(sqrt(2.0));
  int p;

  dist = iftDistTrans(bin,A,INTERIOR);

  for (p=0; p < bin->n; p++) 
    if (dist->val[p] <= distance_thres) 
      objects->val[p]=0;

  label = iftLabelComp(objects,A);

  iftDestroyImage(&objects);
  iftDestroyImage(&dist);
  iftDestroyAdjRel(&A);

  return(label);
}

iftImage *iftReconstructMask(iftImage *orig, int brightness_thres, iftImage *label)
{
  iftImage   *pred = NULL;
  iftImage   *pathval=NULL,*mask=NULL;
  iftGQueue  *Q=NULL;
  int         i,p,q,r,tmp;
  char       **table;
  iftVoxel    u,v;
  iftAdjRel  *A=iftCircular(sqrtf(2.0));

  // Initialization
 
  table    = (char **)calloc(iftMaximumValue(label)+1,sizeof(char *));
  for (i=0; i <= label->maxval; i++) 
    table[i] = iftAllocCharArray(label->maxval+1);

  pathval  = iftCreateImage(orig->xsize,orig->ysize,orig->zsize);
  pred     = iftCreateImage(orig->xsize,orig->ysize,orig->zsize);
  mask     = iftCreateImage(orig->xsize,orig->ysize,orig->zsize);
  Q        = iftCreateGQueue(iftMaximumValue(orig)+1,orig->n,pathval->val);
  for (p=0; p < orig->n; p++) {
    pathval->val[p]=INFINITY_INT;
    if (label->val[p] > 0){
      pathval->val[p] = 0;
      pred->val[p]    = NIL;
      mask->val[p]    = 1;
      iftInsertGQueue(&Q,p);
    }
  }

  // Image Foresting Transform

  while(!iftEmptyGQueue(Q)) {
    p=iftRemoveGQueue(Q);

    u.x = iftGetXCoord(orig,p);
    u.y = iftGetYCoord(orig,p);
    u.z = iftGetZCoord(orig,p);

    for (i=1; i < A->n; i++){
      v.x = u.x + A->dx[i];
      v.y = u.y + A->dy[i];
      v.z = u.z + A->dz[i];
      if (iftValidVoxel(orig,v)){	
	q = iftGetVoxelIndex(orig,v);
	if (pathval->val[q] > pathval->val[p]){
	  if (orig->val[q]<=brightness_thres){
	    tmp = pathval->val[p]+orig->val[q];
	    if (tmp < pathval->val[q]){ 
	      if (pathval->val[q] != INFINITY_INT){
		iftRemoveGQueueElem(Q,q);
	      }
	      pathval->val[q]  = tmp;
	      pred->val[q]     = p;
	      label->val[q]    = label->val[p];
	      iftInsertGQueue(&Q, q);
	    }
	  }
	}else{
	  if ((Q->L.elem[q].color==BLACK)&&
	      (label->val[q]!=label->val[p])&&
	      (table[label->val[q]][label->val[p]]==0)){ // connect objects
	    r = q;
	    while (r != NIL) {
	      mask->val[r]=1;
	      r = pred->val[r];
	    }
	    r = p; 
	    while (r != NIL) {
	      mask->val[r]=1;
	      r = pred->val[r];
	    }
	    table[label->val[q]][label->val[p]]=1;
	    table[label->val[p]][label->val[q]]=1;	    
	  }
	}
      }
    }
  }

  iftDestroyGQueue(&Q);
  iftDestroyImage(&pathval);
  iftDestroyImage(&pred);
  iftDestroyAdjRel(&A);

  for (i=0; i <= label->maxval; i++) 
    free(table[i]);
  free(table);

  return(mask);
}

iftImage *iftPreprocPanicles(iftImage *orig, int brightness_thres, int distance_thres)
{
  iftAdjRel *A;
  iftImage *img,*aux;

  A      = iftCircular(1.0);
  img    = iftErode(orig,A);
  aux    = iftThreshold(img,0,brightness_thres,1);
  iftDestroyImage(&img);
  iftDestroyAdjRel(&A);
  A      = iftCircular(sqrtf(2.0));
  img    = iftSelectLargestComp(aux,A);
  iftDestroyImage(&aux);  
  aux = iftDeleteThinParts(img,distance_thres);
  iftDestroyImage(&img);  
  img = iftReconstructMask(orig,brightness_thres,aux);
  iftDestroyImage(&aux);
  iftDestroyAdjRel(&A);
  A      = iftCircular(2.0);
  aux = iftDilate(img,A);
  iftDestroyImage(&img);
  img = iftCloseBasins(aux);
  iftDestroyImage(&aux);
  iftDestroyAdjRel(&A);

  return(img);
}

/* Junction points cannot be determined based on this local rule,
   because my skeleton has points that can be removed without
   disconnecting it. Therefore, this function will create false
   junction points. */

iftSet *iftJunctionPoints(iftImage *skel, iftAdjRel *A)
{
  int p,q,i,counter;
  iftVoxel u,v;
  iftSet *S=NULL;

  for (p=0; p < skel->n; p++) {
    if (skel->val[p]!=0){
      u.x = iftGetXCoord(skel,p);
      u.y = iftGetYCoord(skel,p);
      u.z = iftGetZCoord(skel,p);
      counter=0;
      for (i=1; i < A->n; i++) {
	v.x = u.x + A->dx[i];
	v.y = u.y + A->dy[i];
	v.z = u.z + A->dz[i];
	if (iftValidVoxel(skel,v)){
	  q = iftGetVoxelIndex(skel,v);
	  if (skel->val[q]!=0){
	    counter++;
	  }
	}
      }
      if (counter>=3){ // junction point
	iftInsertSet(&S,p);
      }
    }
  }
  return(S);   
}

void iftWritePanicles(iftImage *bin, iftSet *E, char *filename)
{
  FILE *fp=fopen(filename,"w");
  int p,npts;
  iftVoxel u;
  iftSet *S;

  fprintf(fp,"<Image> ");
  fprintf(fp,"<FileInfo> ");
  fprintf(fp,"<ImageWidth> %d </ImageWidth> ",bin->xsize);
  fprintf(fp,"<ImageHeight> %d </ImageHeight> ",bin->ysize);
  fprintf(fp,"<Scale> 120.0 </Scale> ");
  fprintf(fp,"</FileInfo> ");
  fprintf(fp,"<ProcessingInfo> ");
  fprintf(fp,"<SkeletonPoints> ");

  npts=0;
  for (p=0; p < bin->n; p++) 
    if (bin->val[p]!=0)
      npts++;

  fprintf(fp,"<SklPtNumber> %d </SklPtNumber> ",npts);

  for (p=0; p < bin->n; p++) 
    if (bin->val[p]!=0){
      u.x = iftGetXCoord(bin,p);
      u.y = iftGetYCoord(bin,p);
      fprintf(fp,"<SklPt> %d %d </SklPt> ",u.x,u.y);
    }

  fprintf(fp,"</SkeletonPoints> ");

  fprintf(fp,"<TerminalPoints> ");

  npts=0;
  S = E;
  while(S != NULL) {
    npts++;
    S = S->next;
  }

  fprintf(fp,"<TermPtNumber> %d </TermPtNumber> ",npts);

  S = E;
  while(S != NULL) {
    u.x = iftGetXCoord(bin,S->elem);
    u.y = iftGetYCoord(bin,S->elem);
    fprintf(fp,"<TermPt> %d %d </TermPt> ",u.x,u.y);
    S = S->next;
  }

  fprintf(fp,"</TerminalPoints> ");

  fprintf(fp,"</ProcessingInfo> ");
  fprintf(fp,"</Image> ");

  fclose(fp);
}


int main(int argc, char *argv[]) 
{
  iftImage       *img=NULL,*bin=NULL;
  iftImage       *aux=NULL,*orig=NULL;
  iftAdjRel      *A=NULL,*B=NULL;
  iftFImage      *skel=NULL;
  iftSet         *E=NULL;
  char            filename[400],command[500],*basename;
  iftColor        RGB,YCbCr;
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
    iftError("Usage must be: iftPanicleSkel <image.jpg>","main");

  // Reading jpg file

  basename = strtok(argv[1],".");
  sprintf(filename,"./originals/%s.jpg",basename);
  sprintf(command,"convert %s temp.pgm",filename);
  system(command);
  orig  = iftReadImageP5("temp.pgm");
  sprintf(command,"rm -f temp.pgm");
  system(command);

  t1     = iftTic();

  // Image pre-processing

  img = iftPreprocPanicles(orig,ROUND(1.20*iftOtsu(orig)),16);

  // end of pre-processing

  A      = iftCircular(sqrtf(2.0));
  skel   = iftMSSkel2D(img,A,INTERIOR);   // Skeletonization

  // Drawing information and saving images 

  aux   = iftFThreshold(skel,0.1,100.0);    

  bin   = iftSelectLargestComp(aux,A);
  iftDestroyImage(&aux);

  E     = iftEndPoints(bin,A);  

  sprintf(filename,"./skeletons/%s.xml",basename);

  iftWritePanicles(bin,E,filename); // saving skeleton and terminal points

  RGB.val[0]=255; RGB.val[1]=255; RGB.val[2]=100;
  YCbCr = iftRGBtoYCbCr(RGB); 
  B = iftCircular(1.0);
  iftDrawObject(orig,bin,YCbCr,B); // Draw skeleton

  RGB.val[0]=0; RGB.val[1]=0; RGB.val[2]=255;
  YCbCr = iftRGBtoYCbCr(RGB); 
  iftDestroyAdjRel(&B);
  B = iftCircular(3.0);
  iftDrawPoints(orig,E,YCbCr,B); // Draw end points

  t2     = iftToc();

  fprintf(stdout,"skeletonization in %f ms\n",iftCompTime(t1,t2));

  iftWriteImageP6(orig,"temp.ppm");
  sprintf(command,"convert temp.ppm ./skeletons/%s_skel.jpg",basename);
  system(command);
  sprintf(command,"rm -f temp.ppm");
  system(command);
  iftDestroyAdjRel(&B);
  iftDestroyAdjRel(&A);
  iftDestroyImage(&orig);  
  iftDestroyImage(&img);  
  iftDestroyFImage(&skel);  
  iftDestroyImage(&bin);  
  iftDestroySet(&E);

  /* ---------------------------------------------------------- */

  info = mallinfo();
  MemDinFinal = info.uordblks;
  if (MemDinInicial!=MemDinFinal)
    printf("\n\nDynamic memory was not completely deallocated (%d, %d)\n",
	   MemDinInicial,MemDinFinal);   

  return(0);
}



