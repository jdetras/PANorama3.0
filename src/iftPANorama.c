#include "iftPANorama.h"


iftImage *iftSegmentPanicle(iftImage *orig)
{
  iftAdjRel *A;
  iftImage *aux[2];
  int brightness_thres=ROUND(OTSUTHRES*iftOtsu(orig));
  char command[200];

  /* Segment the panicle with its awns, whenever they exist */
  

  aux[0] = iftPanicleMask(orig);

  if (DEBUG==1) { 
    iftWriteImageP2(aux[0],"temp.pgm");
    sprintf(command,"convert temp.pgm panicle_mask.jpg");
    system(command);
    sprintf(command,"rm -f temp.pgm");
    system(command);
  }

  /* Remove the awns (thin parts) and label connected components */

  aux[1] = iftRemoveAwns(aux[0],(int)(powf(ANWSTHICK*SCALE,2.0)));

  if (DEBUG==1) {
    iftImage *temp=iftColorizeCompOverImage(orig,aux[1]);
    iftWriteImageP6(temp,"temp.ppm");
    sprintf(command,"convert temp.ppm panicle_noawns_mask_over_image.jpg");
    system(command);
    sprintf(command,"rm -f temp.ppm");
    system(command);
    iftDestroyImage(&temp);
    temp=iftColorizeComp(aux[1]);
    iftWriteImageP6(temp,"temp.ppm");
    sprintf(command,"convert temp.ppm panicle_noawns_mask.jpg");
    system(command);
    sprintf(command,"rm -f temp.ppm");
    system(command);
    iftDestroyImage(&temp);
  }

  /* Reconstruct the panicle, without awns, by reconnecting
     components */

  iftDestroyImage(&aux[0]);  
  aux[0] = iftReconnectPanicle(orig,brightness_thres,aux[1]);
  A = iftCircular(ANWSTHICK*SCALE);
  iftDestroyImage(&aux[1]);
  aux[1] = iftDilate(aux[0],A);

  if (DEBUG==1) { 
    iftWriteImageP2(aux[1],"temp.pgm");
    sprintf(command,"convert temp.pgm panicle_reconnected_mask.jpg");
    system(command);
    sprintf(command,"rm -f temp.pgm");
    system(command);
  }

  /* Close holes inside the panicle in order to avoid skiz in
     skeletonization */

  iftDestroyImage(&aux[0]);
  aux[0] = iftCloseBasins(aux[1]);

  if (DEBUG==1){
    iftWriteImageP2(aux[0],"temp.pgm");
    sprintf(command,"convert temp.pgm panicle_skeletonization_mask.jpg");
    system(command);
    sprintf(command,"rm -f temp.pgm");
    system(command);
  }
  iftDestroyImage(&aux[1]);
  iftDestroyAdjRel(&A);

  return(aux[0]);
}

iftPanicle *iftExtractPanicleSkeleton(iftImage *bin)
{
  iftFImage  *msskel=NULL;
  iftImage   *aux=NULL;
  iftAdjRel  *A=NULL;
  iftPanicle *pan=iftCreatePanicleInfo();
  int         p;

  A      = iftCircular(sqrtf(2.0));
  msskel = iftMSSkel2D(bin,A,INTERIOR);    /* Multi-Scale Skeletonization */

  aux    = iftFThreshold(msskel,SKELTHRES,100.0,255);  /* Skeleton binarization */   
  iftDestroyFImage(&msskel);

  pan->skel = iftSelectLargestComp(aux,A);  /* Maximal component
					       selection. This should
					       never be
					       necessary. However the
					       contour pixel labeling
					       algorithm might break
					       thin and small parts of
					       the shape into a
					       separated component,
					       smoothing out the
					       panicle shape. When
					       this happens, those
					       components will also
					       produce undesirable
					       small skeletons. The
					       largest component
					       selection removes
					       them. */

  pan->nskelpts=0; /* compute number of skeleton points */
  for (p=0; p < pan->skel->n; p++) 
    if (pan->skel->val[p]!=0)
      pan->nskelpts++;

  iftDestroyImage(&aux);
  iftDestroyAdjRel(&A);
  
  return(pan);
}


iftPanicle *iftCreatePanicleInfo()
{
  iftPanicle *pan=(iftPanicle *)calloc(1,sizeof(iftPanicle));

  pan->primary   = NULL;
  pan->nbranches = 0;
  pan->skel      = NULL; 
  pan->scale     = SCALE;
  pan->mainaxis.point    = NULL;
  pan->junction  = NULL;
  pan->njunctions  = 0;
  pan->mainaxis.npoints  = 0;
  pan->extrusion = NULL;
  pan->nextpts = 0;

  return(pan);
}

void iftDestroyPanicleInfo(iftPanicle **pan)
{
  iftPanicle *aux=*pan;
  int i;

  if (aux != NULL) { 
    if (aux->skel != NULL) 
      iftDestroyImage(&aux->skel);
    if (aux->primary != NULL){
      for (i=0; i < aux->nbranches; i++) {
	if (aux->primary[i].point!=NULL)    free(aux->primary[i].point);
      }
      free(aux->primary);
    }
    if (aux->extrusion != NULL)
      free(aux->extrusion);
    if (aux->mainaxis.point!=NULL)
      free(aux->mainaxis.point);
    if (aux->junction!=NULL)
      free(aux->junction);
    free(aux);
    *pan = NULL;
  }
  
}

iftPanicle *iftReadPanicleInfo(char *filename)
{
  iftPanicle *pan=iftCreatePanicleInfo();
  FILE *fp=NULL;
  int i,j,p,n,*buffer;
  
  fp = fopen(filename,"rb");

  if (fp == NULL) 
    iftError(MSG2,"iftReadPanicleInfo");

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
  pan->nbranches = buffer[i];
  free(buffer);


  if (pan->nbranches > 0) { 

    /* For each branch and main axis read: number of points, first,
       last, point addresses, number of junctions, junction
       addresses. Read also the extrusion point. */
  
    pan->primary = (iftBranch *) calloc(pan->nbranches, sizeof(iftBranch));
    if (pan->primary == NULL) {
      iftError(MSG1,"iftReadPanicleInfo");
    }
    
    int k=0;
    
    fread(&n,sizeof(int),1,fp);
    buffer = iftAllocIntArray(n);
    fread(buffer,sizeof(int),n,fp);

    for (i=0; i < pan->nbranches; i++) {
      pan->primary[i].npoints = buffer[k]; k++;
      p = buffer[k]; k++; 
      pan->primary[i].first = iftGetVoxelCoord(pan->skel,p);
      p = buffer[k]; k++; 
      pan->primary[i].last  = iftGetVoxelCoord(pan->skel,p);
      pan->primary[i].point = (iftVoxel *)calloc(pan->primary[i].npoints,sizeof(iftVoxel));
      if (pan->primary[i].point == NULL) {
	iftError(MSG1,"iftReadPanicleInfo");
      }      
      for (j=0; j < pan->primary[i].npoints; j++){
	p = buffer[k]; k++; 
	pan->primary[i].point[j] = iftGetVoxelCoord(pan->skel,p);
      }
    }

    pan->mainaxis.npoints = buffer[k]; k++;
    p = buffer[k]; k++; 
    pan->mainaxis.first = iftGetVoxelCoord(pan->skel,p);
    p = buffer[k]; k++; 
    pan->mainaxis.last  = iftGetVoxelCoord(pan->skel,p);
    pan->mainaxis.point = (iftVoxel *)calloc(pan->mainaxis.npoints,sizeof(iftVoxel));
    if (pan->mainaxis.point == NULL) {
      iftError(MSG1,"iftReadPanicleInfo");
    }      
    for (j=0; j < pan->mainaxis.npoints; j++){
      p = buffer[k]; k++; 
      pan->mainaxis.point[j] = iftGetVoxelCoord(pan->skel,p);
    }


    pan->njunctions = buffer[k]; k++; 
    pan->junction   = iftAllocIntArray(pan->njunctions);
    for (j=0; j < pan->njunctions; j++){
      pan->junction[j] = buffer[k]; k++; 
    }


    pan->nextpts    = buffer[k]; k++; 

    if (pan->nextpts > 0) {
      pan->extrusion  = (iftVoxel *)calloc(pan->nextpts,sizeof(iftVoxel));
      for (j=0; j < pan->nextpts; j++){    
	p = buffer[k]; k++;
	pan->extrusion[j] = iftGetVoxelCoord(pan->skel,p);
      }
    }

    free(buffer);
  }

  fclose(fp);
  
  return(pan);
}

void iftPrintPanicleInfo(iftPanicle *pan)
{
  int i,j;
  iftVoxel u;

  printf("Number of pixels per cm %f\n",pan->scale);  
  printf("Number of primary branches %d\n",pan->nbranches);

  if (pan->nbranches > 0) {

    for (i=0; i < pan->nbranches; i++) {
      printf("Branch %d with %d points (approx. %f cm):\n",i+1,pan->primary[i].npoints,pan->primary[i].npoints/pan->scale);
      u.x = pan->primary[i].first.x; u.y = pan->primary[i].first.y;
      printf(" first point (%d,%d) ",u.x,u.y);
      u.x = pan->primary[i].last.x; u.y = pan->primary[i].last.y;
      printf(" last point (%d,%d) \n",u.x,u.y);
    }
  }
  printf("Main axis with %d points (approx. %f cm):\n",pan->mainaxis.npoints,pan->mainaxis.npoints/pan->scale);
  u.x = pan->mainaxis.first.x; u.y = pan->mainaxis.first.y;
  printf(" first point (%d,%d) ",u.x,u.y);
  u.x = pan->mainaxis.last.x; u.y = pan->mainaxis.last.y;
  printf(" last point (%d,%d) \n",u.x,u.y);
  printf("Num. of nodes on main axis %d:\n",pan->njunctions);
  for (j=0; j < pan->njunctions; j++){
    u.x = pan->mainaxis.point[pan->junction[j]].x; 
    u.y = pan->mainaxis.point[pan->junction[j]].y; 
    printf(" node (%d,%d)\n",u.x,u.y); 	  
  }
  printf("Num. of extrusion points %d (approx. %f cm)\n",pan->nextpts,pan->nextpts/pan->scale);
  if (pan->nextpts > 0){
    u.x = pan->extrusion[0].x; u.y = pan->extrusion[0].y;
    printf(" first point (%d,%d)",u.x,u.y);
    u.x = pan->extrusion[pan->nextpts-1].x; u.y = pan->extrusion[pan->nextpts-1].y;
    printf(" last point (%d,%d)\n",u.x,u.y);
  }

}

void iftWritePanicleInfo(iftPanicle *pan, char *filename)
{
  FILE *fp;
  int i,j,p,*buffer;

  fp = fopen(filename,"wb");

  if (pan->skel == NULL) 
    iftError("First compute skeleton","iftWritePanicleInfo");

  /* Write skeleton scale, domain, number of points, point addresses,
     and number of branches (to indicate whether or not exists more
     information to be read) */   

  fwrite(&pan->scale,sizeof(float),1,fp);
 
  buffer    = iftAllocIntArray(pan->nskelpts+4);
  buffer[0] = pan->nskelpts+3;
  buffer[1] = pan->skel->xsize; 
  buffer[2] = pan->skel->ysize;   
  for (p=0,i=3; p < pan->skel->n; p++) 
    if (pan->skel->val[p]!=0) {
      buffer[i] = p;
      i++;
    }
  buffer[i] = pan->nbranches;
  fwrite(buffer,sizeof(int),pan->nskelpts+4,fp);
  free(buffer);


  if (pan->nbranches > 0) {
    int n = 1, k = 0; 

    /* compute the size of the buffer for writting */
    for (i=0; i < pan->nbranches; i++) { // primary branches
      n += (1+1+1+pan->primary[i].npoints); 
    }
    n += (1+1+1+pan->mainaxis.npoints); // main axis
    n += 1 + pan->njunctions;           // junctions along the main axis
    n += 1 + pan->nextpts;              // extrusion

    buffer = iftAllocIntArray(n);

    /* For each branch write: number of points, first, last, and point
       addresses. */
    
    buffer[k] = n; k++;
    for (i=0; i < pan->nbranches; i++) {
      buffer[k] = pan->primary[i].npoints; k++;
      p = iftGetVoxelIndex(pan->skel,pan->primary[i].first);
      buffer[k] = p; k++; 
      p = iftGetVoxelIndex(pan->skel,pan->primary[i].last);
      buffer[k] = p; k++;
      for (j=0; j < pan->primary[i].npoints; j++){ 	  
	p = iftGetVoxelIndex(pan->skel,pan->primary[i].point[j]);
	buffer[k] = p; k++; 
      }
    }
  
    /* Write number of points, first, last, and point addresses along
       the main axis; number of junctions and addresses to the
       junction points along the list of main axis points; and finally
       write the number of extrusion points and the extrusion
       points. */

    buffer[k] = pan->mainaxis.npoints; k++;
    p = iftGetVoxelIndex(pan->skel,pan->mainaxis.first);
    buffer[k] = p; k++;
    p = iftGetVoxelIndex(pan->skel,pan->mainaxis.last);
    buffer[k] = p; k++; 
    for (j=0; j < pan->mainaxis.npoints; j++){ 	  
      p = iftGetVoxelIndex(pan->skel,pan->mainaxis.point[j]);
      buffer[k] = p; k++;
    }
    
    buffer[k] = pan->njunctions; k++;
    for (j=0; j < pan->njunctions; j++){
      buffer[k] = pan->junction[j]; k++;
    }
    buffer[k] = pan->nextpts; k++;
    if (pan->nextpts > 0){
      for (j=0; j < pan->nextpts; j++){ 	  
	p = iftGetVoxelIndex(pan->skel,pan->extrusion[j]);
	buffer[k] = p; k++;
      }
    }
    fwrite(buffer,sizeof(int),n,fp);
    free(buffer);
  }
   
  fclose(fp);

  if (DEBUG==1){
    iftPrintPanicleInfo(pan);
  }

}

float iftSegmentLength(iftVoxel *point, int first, int last) 
{
  float length=0.0, maxdistseg=SCALE*MAXDISTSEG, dmax, dist;
  float length_1, length_2;
  int   i, farthest;


  if (first==last){
    return(0.0);
  }else{
    if ((first+1) == last){
      return(1.0/SCALE);
    }else{

      /* Find the farthest point to the segment, excluding the first and
	 the last points */
      
      length   = iftVoxelDistance(point[first],point[last]);
      farthest =  NIL;
      dmax     = INFINITY_FLT_NEG;
      
      for (i=first+1; i < last; i++) {
	dist = iftVoxelLineDist2D(point[i],point[first],point[last],length);
	if (dist > dmax) {
	  dmax = dist; farthest = i;
	}
      }
      
      if ((dmax < maxdistseg)&&(dmax >= 0)){ // end of recursion
      return(length/SCALE);
      }else{
	length_1 = iftSegmentLength(point, first,    farthest);
	length_2 = iftSegmentLength(point, farthest, last);
	return(length_1+length_2);
      }
    }
  }

}

iftImage *iftDrawPanicleInfo(iftImage *orig, iftPanicle *pan)
{
  iftImage  *panimg=iftCopyImage(orig);
  iftColor   RGB, YCbCr;
  iftSet    *endpts=NULL;
  iftAdjRel *A=NULL,*B=NULL;
  int        i,j;
  float      bulletsz = pan->scale*BULLETSZ;

  if (pan->skel == NULL) 
    iftError("First compute skeleton","iftDrawPanicleInfo");

  RGB.val[0]=R_SKL; RGB.val[1]=G_SKL; RGB.val[2]=B_SKL;
  YCbCr = iftRGBtoYCbCr(RGB); 
  B = iftCircular(1.0);
  iftDrawObject(panimg,pan->skel,YCbCr,B);  /* Draw skeleton */
  
  if (pan->mainaxis.point != NULL) {

    RGB.val[0]=R_AXS; RGB.val[1]=G_AXS; RGB.val[2]=B_AXS;
    YCbCr = iftRGBtoYCbCr(RGB); 
    for (j=0; j < pan->mainaxis.npoints; j++) /* Draw main axis */
      iftDrawPoint(panimg, pan->mainaxis.point[j], YCbCr, B);     
  }

  if (pan->extrusion != NULL) {
    RGB.val[0]=R_EXT; RGB.val[1]=G_EXT; RGB.val[2]=B_EXT;
    YCbCr = iftRGBtoYCbCr(RGB); 
    for (j=0; j < pan->nextpts; j++) /* Draw extrusion */
      iftDrawPoint(panimg, pan->extrusion[j], YCbCr, B); 
  }

  if (pan->primary != NULL) {
    RGB.val[0]=R_BCH; RGB.val[1]=G_BCH; RGB.val[2]=B_BCH;
    YCbCr = iftRGBtoYCbCr(RGB); 
    for (i=0; i < pan->nbranches; i++) /* Draw primary branches */
      for (j=0; j < pan->primary[i].npoints; j++)
	iftDrawPoint(panimg, pan->primary[i].point[j], YCbCr, B); 
  }

  RGB.val[0]=R_END; RGB.val[1]=G_END; RGB.val[2]=B_END;
  YCbCr = iftRGBtoYCbCr(RGB); 
  iftDestroyAdjRel(&B);
  B = iftCircular(bulletsz);
  A = iftCircular(sqrtf(2.0));
  endpts = iftEndPoints(pan->skel,A);   /* Compute its end points */  
  iftDrawPoints(panimg,endpts,YCbCr,B); /* Draw end points */

  iftDestroySet(&endpts);
  iftDestroyAdjRel(&A);


  if (pan->mainaxis.point != NULL) { /* Draw terminal points on the
					main axis and the initial
					extrusion point */
    RGB.val[0]=R_AXS; RGB.val[1]=G_AXS; RGB.val[2]=B_AXS;
    YCbCr = iftRGBtoYCbCr(RGB); 
    iftDrawPoint(panimg,pan->mainaxis.first,YCbCr,B);
    iftDrawPoint(panimg,pan->mainaxis.last,YCbCr,B);
    if (pan->extrusion!=NULL){
      RGB.val[0]=R_EXT; RGB.val[1]=G_EXT; RGB.val[2]=B_EXT;
      YCbCr = iftRGBtoYCbCr(RGB); 
      iftDrawPoint(panimg,pan->extrusion[0],YCbCr,B);
    }

    if (pan->junction != NULL) {
      RGB.val[0]=R_JCT; RGB.val[1]=G_JCT; RGB.val[2]=B_JCT; 
      YCbCr = iftRGBtoYCbCr(RGB); 
      for (j=0; j < pan->njunctions; j++) /* Draw junctions on main axis */
	iftDrawPoint(panimg,pan->mainaxis.point[pan->junction[j]],YCbCr,B);
    }

    /* Draw center of main axis */

    RGB.val[0]=R_AXS; RGB.val[1]=G_AXS; RGB.val[2]=B_AXS;
    YCbCr = iftRGBtoYCbCr(RGB); 
    j = iftCenterOfMainAxis(pan);
    iftDrawPoint(panimg, pan->mainaxis.point[j], YCbCr, B); 

  }

  iftDestroyAdjRel(&B);
    
  return(panimg);

}

iftImage *iftImageOfMainAxis(iftPanicle *pan)
{
  iftImage *mainaxis=iftCreateImage(pan->skel->xsize,pan->skel->ysize,1);
  int p,i;

  if (pan->mainaxis.npoints==0)
    iftError("First find main axis","iftMainAxis");

  for (i=0; i < pan->mainaxis.npoints; i++) {
    p = iftGetVoxelIndex(pan->skel,pan->mainaxis.point[i]);
    mainaxis->val[p]=1;
  }
  return(mainaxis); 
}

iftImage *iftImageOfExtrusion(iftPanicle *pan)
{
  iftImage *extimg=iftCreateImage(pan->skel->xsize,pan->skel->ysize,1);
  int p,i;

  if (pan->nextpts==0)
    iftError("First find main axis","iftImageOfExtrusion");

  for (i=0; i < pan->nextpts; i++) {
    p = iftGetVoxelIndex(pan->skel,pan->extrusion[i]);
    extimg->val[p]=1;
  }
  return(extimg); 
}

iftImage *iftSkeletonWithoutMainAxis(iftPanicle *pan)
{
  iftImage *sknoaxis,*sknoext=iftSkeletonWithoutExtrusion(pan);
  int p,i;
  char command[200];

  if (sknoext==NULL) 
    sknoaxis=iftCopyImage(pan->skel);
  else
    sknoaxis=sknoext;

  for (i=0; i < pan->mainaxis.npoints; i++) {
    p = iftGetVoxelIndex(pan->skel,pan->mainaxis.point[i]);
    sknoaxis->val[p]=0;
  }

  if (DEBUG==1){
    iftImage *label, *colorlabel, *dil;
    iftAdjRel *A=iftCircular(1.0);
    dil   = iftDilate(sknoaxis,A);
    iftDestroyAdjRel(&A);
    A = iftCircular(sqrtf(2.0));
    label = iftFastLabelComp(dil,A);
    colorlabel = iftColorizeComp(label);
    iftWriteImageP6(colorlabel,"temp.ppm");
    sprintf(command,"convert temp.ppm panicle_skeleton_withno_mainaxis.jpg");
    system(command);
    sprintf(command,"rm -f temp.ppm");
    system(command);
    iftDestroyAdjRel(&A);
    iftDestroyImage(&label);
    iftDestroyImage(&dil);
    iftDestroyImage(&colorlabel);
  }

  return(sknoaxis); 
}

iftImage *iftImageOfEndPoints(iftImage *skel)
{
  int p,q,i,counter;
  iftVoxel u,v;
  iftImage  *endpts=iftCreateImage(skel->xsize,skel->ysize,skel->zsize);
  iftAdjRel *A=iftCircular(sqrtf(2.0));

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
      if (counter==1){ // terminal point
	endpts->val[p]=1;
      }
    }
  }

  iftDestroyAdjRel(&A);

  return(endpts);   
}

iftImage *iftSkeletonWithoutExtrusion(iftPanicle *pan)
{
  iftImage  *ext=NULL,*sknoext=NULL,*aux=NULL;
  iftAdjRel *A=NULL;
  int p;
  char command[200];

  if (pan->nextpts > 1) {
    aux     = iftCopyImage(pan->skel);
    ext     = iftImageOfExtrusion(pan);
    for (p=0; p < aux->n; p++) { /* remove extrusion from skeleton */
      if (ext->val[p]!=0) 
	aux->val[p]=0; 
    }
    iftDestroyImage(&ext);
    A       = iftCircular(sqrtf(2.0));
    sknoext = iftSelectLargestComp(aux,A);
    iftDestroyImage(&aux);
    iftDestroyAdjRel(&A);
  }else{
    iftWarning("There is no extrusion","iftSkeletonWithoutExtrusion");
  }

  if (DEBUG==1){
    iftAdjRel *A=iftCircular(1.0);
    iftImage *dil = iftDilate(sknoext,A);
    iftDestroyAdjRel(&A);
    iftWriteImageP2(dil,"temp.pgm");
    sprintf(command,"convert temp.pgm panicle_skeleton_withno_extrusion.jpg");
    system(command);
    sprintf(command,"rm -f temp.pgm");
    system(command);
    iftDestroyImage(&dil);
  }



  return(sknoext);
}

iftPoint iftSegmentCenter(iftVoxel *point, int npoints)
{
  iftPoint center;
  int i;

  center.x = center.y = center.z = 0.0;

  for (i=0; i < npoints; i++) {
    center.x += point[i].x;
    center.y += point[i].y;
    center.z += point[i].z;
  }

  center.x /= npoints;
  center.y /= npoints;
  center.z /= npoints;
  
  return(center);
}

int iftClosestPointInSegment(iftVoxel *point, int npoints, iftPoint P)
{
  float dmin, d; 
  int i, closest;
  iftPoint  Q;

  closest = NIL; dmin = INFINITY_FLT; 

  for (i=0; i < npoints; i++) {
    Q.x = point[i].x; Q.y = point[i].y; Q.z = point[i].z;
    d = iftPointDistance(P,Q);
    if (d < dmin) {
      dmin    = d; 
      closest = i;
    }
  } 
  
  return(closest);
}

int iftFarthestPointInSegment(iftVoxel *point, int npoints, iftPoint P)
{
  float dmax, d; 
  int i, farthest;
  iftPoint  Q;

  farthest = NIL; dmax = INFINITY_FLT_NEG; 

  for (i=0; i < npoints; i++) {
    Q.x = point[i].x; Q.y = point[i].y; Q.z = point[i].z;
    d = iftPointDistance(P,Q);
    if (d > dmax) {
      dmax     = d; 
      farthest = i;
    }
  } 
  
  return(farthest);
}

iftImage *iftRemoveAwns(iftImage *bin, int distance_thres)
{
  iftImage  *dist=NULL, *objects=iftCopyImage(bin),*label=NULL;
  iftAdjRel *A=iftCircular(sqrt(2.0));
  int p;

  dist = iftDistTrans(bin,A,INTERIOR);

  for (p=0; p < bin->n; p++) 
    if (dist->val[p] <= distance_thres) 
      objects->val[p]=0;

  label = iftFastLabelComp(objects,A);

  iftDestroyImage(&objects);
  iftDestroyImage(&dist);
  iftDestroyAdjRel(&A);

  return(label);
}

iftImage *iftReconnectPanicle(iftImage *orig, int brightness_thres, iftImage *label)
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

iftImage *iftPanicleMask(iftImage *orig)
{
  iftAdjRel *A; 
  iftImage  *aux[2], *mask;

  /* Compute panicle mask */

  A      = iftCircular(1.0);
  aux[0] = iftErode(orig,A);
  aux[1] = iftThreshold(aux[0],0,ROUND(OTSUTHRES*iftOtsu(orig)),1);
  iftDestroyImage(&aux[0]);
  iftDestroyAdjRel(&A);
  A      = iftCircular(sqrtf(2.0));
  mask   = iftSelectLargestComp(aux[1],A);
  iftDestroyImage(&aux[1]);
  iftDestroyAdjRel(&A);
  
  return(mask);
}

int iftCenterOfMainAxis(iftPanicle *pan)
{
  iftPoint center;
  int center_pos;

  center     = iftSegmentCenter(pan->mainaxis.point, pan->mainaxis.npoints);
  center_pos = iftClosestPointInSegment(pan->mainaxis.point, pan->mainaxis.npoints,center);

  return(center_pos);
}

iftImage *iftPanicleEDT(iftImage *orig)
{
  iftAdjRel *A; 
  iftImage  *mask, *dist;

  /* Compute Panicle Mask */

  mask = iftPanicleMask(orig);

  /* Compute EDT */

  A    = iftCircular(sqrtf(2.0));
  dist = iftDistTrans(mask,A,INTERIOR);
  iftDestroyImage(&mask);
  iftDestroyAdjRel(&A);

  return(dist);
}

int iftGetBranchJunction(iftPanicle *pan, int branch)
{ 
  int j, junction; 
  iftVoxel J,P;

  if ((branch <0)||(branch >= pan->nbranches))
    iftError("Invalid branch","iftGetBranchJunction");

  P.x = pan->primary[branch].first.x; 
  P.y = pan->primary[branch].first.y; 
  P.z = 0;  

  /* Compute junction for this branch */

  J.z = 0; junction = NIL;
  for (j=0; j < pan->njunctions; j++) {
    J.x = pan->mainaxis.point[pan->junction[j]].x;
    J.y = pan->mainaxis.point[pan->junction[j]].y;
    if (iftVoxelDistance(P,J) <= MINJCTDIST*pan->scale){
      junction = j;
      break;
    }
  }
  
  if (junction==NIL)
    iftWarning("Could not find node on main axis. You may want to increase MINJCTDIST.","iftGetBranchJunction");

  return(junction);
}

void iftSortBranches(iftPanicle *pan)
{
  int i, j, k, *branch,*new_branch;
  float *dist;
  char *side;
  iftBranch *new_primary,*aux;     

  if (pan->nbranches==0)
    iftError("There are no primary branches","iftSortBranches");

  side       = iftAllocCharArray(pan->nbranches);
  dist       = iftAllocFloatArray(pan->nbranches);
  branch     = iftAllocIntArray(pan->nbranches);
  new_branch = iftAllocIntArray(pan->nbranches);
  
  /* compute the y-coordinate of the last point of each branch and
     also the side of the branch. */

  for (i=0; i < pan->nbranches; i++) {
    branch[i]=i;
    if (iftVoxelLinePosition2D(pan->primary[i].last,pan->mainaxis.first,pan->mainaxis.last) <= 0){ /* Left Side */
      side[i] = 1;
    }else{ /* right side */
      side[i]=2; 
    }  
    dist[i] = pan->primary[i].last.y; //iftVoxelDistance(pan->primary[i].first,pan->mainaxis.first);
  }

  /* Sort branches by distances */
  
  iftFQuickSort(dist, branch, 0, pan->nbranches-1, DECREASING);

  /* Separate left and right side branches in a clockwise manner */  

  j = 0;
  for (i=0; i < pan->nbranches; i++) {
    if (side[branch[i]]==1){ 
      new_branch[j]=branch[i];
      j++;
    }
  }
  for (i=pan->nbranches-1; i >= 0; i--) {
    if (side[branch[i]]==2){ 
      new_branch[j]=branch[i];
      j++;
    }
  }


  /* Put branches in the closewise order */

  new_primary = (iftBranch *) calloc(pan->nbranches, sizeof(iftBranch));
  if (new_primary == NULL) {
    iftError(MSG1,"iftSortBranches");
  }
    
  for (i=0; i < pan->nbranches; i++) {
    j = new_branch[i];
    new_primary[i].first   = pan->primary[j].first;
    new_primary[i].last    = pan->primary[j].last;
    new_primary[i].npoints = pan->primary[j].npoints;
    new_primary[i].point   = (iftVoxel *)calloc(new_primary[i].npoints,sizeof(iftVoxel));
    if (new_primary[i].point == NULL) 
      iftError(MSG1,"iftSortBranches");
    for (k=0; k < new_primary[i].npoints; k++){    
      new_primary[i].point[k] = pan->primary[j].point[k];
    }    
  }
  
  aux = pan->primary;
  pan->primary = new_primary;

  for (i=0; i < pan->nbranches; i++) {
    if (aux[i].point!=NULL) 
      free(aux[i].point);
  }
  free(aux);
  free(dist);
  free(side);
  free(branch);
  free(new_branch);
}

iftVoxel iftGetClosestSkeletonPoint(iftPanicle *pan, iftVoxel u)
{
  iftAdjRel *A=iftCircular(MAXSKDIST*pan->scale);
  int        i,q,qmin=NIL,dmin=INFINITY_INT,d;
  iftVoxel   v;
  iftImage  *skel=pan->skel;

  for (i=1; i < A->n; i++) {
    v = iftGetAdjacentVoxel(A,u,i);
    if (iftValidVoxel(skel,v)){
      q = iftGetVoxelIndex(skel,v);
      if (skel->val[q]!=0){
	d = iftSquaredVoxelDistance(u,v);
	if (d < dmin){
	  qmin = q;
	  dmin = d;
	}
      }
    }
  }
  
  iftDestroyAdjRel(&A);
  
  return(iftGetVoxelCoord(skel,qmin));
}

void iftInvertVoxelSequence(iftVoxel *seq, int npoints)
{
  int first, last;

  first = 0; last = npoints-1;
  while (first < last) {
    iftSwitchVoxels(&seq[first],&seq[last]);
    first++; last--;
  }
}

void iftAddBranch(iftPanicle *pan, iftVoxel pt)
{
  iftVoxel    u,v;
  iftBranch   newbranch, *newprimary;
  iftAdjRel  *A=iftCircular(sqrtf(2.0));
  int        *pred=NULL,i,p,q;
  iftFIFO    *F=NULL;
  char        first_point_found=0;
  iftImage   *mainaxis;

  /* Find the closest point on the skeleton and use it as the last
     point of the new branch */

  newbranch.last = iftGetClosestSkeletonPoint(pan,pt);

  /* Find the first point of the new branch and its number of points */

  mainaxis = iftImageOfMainAxis(pan);
  F        = iftCreateFIFO(pan->skel->n);
  p        = iftGetVoxelIndex(pan->skel,newbranch.last);
  iftInsertFIFO(F,p);
  pred     = iftAllocIntArray(pan->skel->n);
  pred[p]  = NIL;

  while(!iftEmptyFIFO(F)){
    p = iftRemoveFIFO(F);
    u = iftGetVoxelCoord(pan->skel,p);
    if (first_point_found) 
      break;

    for (i=1; i < A->n; i++) {
      v = iftGetAdjacentVoxel(A,u,i);
      if (iftValidVoxel(pan->skel,v)){
	q = iftGetVoxelIndex(pan->skel,v);
	if (pan->skel->val[q]!=0){ /* point is on the skeleton */
	  if (mainaxis->val[q]!=0) { /* point is on the main axis */
	    newbranch.first   = u;
	    first_point_found = 1;
	  }else{
	    if (F->color[q]==WHITE){
	      iftInsertFIFO(F,q);
	      pred[q]=p;
	    }
	  }
	}
      }
    }
  }
  iftDestroyAdjRel(&A);
  iftDestroyFIFO(&F);
  iftDestroyImage(&mainaxis);

  newbranch.npoints = 0;
  p                 = iftGetVoxelIndex(pan->skel,newbranch.first);
  while (p!=NIL){
    newbranch.npoints++;
    p=pred[p];
  }
  if (newbranch.npoints==0) 
    iftError("Could not find branch","iftAddBranch");

  /* Find the points along the new branch */

  newbranch.point = (iftVoxel *)calloc(newbranch.npoints,sizeof(iftVoxel));
  if (newbranch.point == NULL) 
    iftError(MSG1,"iftAddBranch");

  p               = iftGetVoxelIndex(pan->skel,newbranch.first);
  i = 0;
  while (p!=NIL){
    newbranch.point[i] = iftGetVoxelCoord(pan->skel,p); 
    i++;
    p=pred[p];
  }
  free(pred);
  
  /* Add new branch to the panicle informations */

  newprimary = (iftBranch *) calloc(pan->nbranches+1, sizeof(iftBranch));
  if (newprimary == NULL) {
    iftError(MSG1,"iftAddBranch");
  }  
  for (i=0; i < pan->nbranches; i++) {
    newprimary[i].point   = pan->primary[i].point;
    newprimary[i].first   = pan->primary[i].first;
    newprimary[i].last    = pan->primary[i].last;
    newprimary[i].npoints = pan->primary[i].npoints;
  }
  newprimary[i].point   = newbranch.point;
  newprimary[i].first   = newbranch.first;
  newprimary[i].last    = newbranch.last;
  newprimary[i].npoints = newbranch.npoints;

  free(pan->primary);
  pan->primary = newprimary;
  pan->nbranches += 1;
  iftSortBranches(pan);

}

void iftDeleteBranch(iftPanicle *pan, iftVoxel pt)
{
  int i,j,closest=NIL;
  float dist, mindist=INFINITY_FLT;
  iftBranch *newprimary;

  /* Find the closest last point for branch deletion */

  pt   = iftGetClosestSkeletonPoint(pan,pt);
  
  for (i=0; i < pan->nbranches; i++) {    
    dist = iftVoxelDistance(pt,pan->primary[i].last);
    if (dist < mindist) {
      mindist = dist;
      closest = i;
    }
  }
  
  /* Delete the branch */
  
  newprimary = (iftBranch *) calloc(pan->nbranches-1, sizeof(iftBranch));
  if (newprimary == NULL) {
    iftError(MSG1,"iftAddBranch");
  }  

  for (i=0,j=0; i < pan->nbranches; i++) {
    if (i != closest){
      newprimary[j].point   = pan->primary[i].point;
      newprimary[j].first   = pan->primary[i].first;
      newprimary[j].last    = pan->primary[i].last;
      newprimary[j].npoints = pan->primary[i].npoints;
      j++;
    }
  }

  free(pan->primary);
  pan->primary = newprimary;
  pan->nbranches -= 1;
  //  not needed, since the branches were already sorted
  //  iftSortBranches(pan);

}

iftImage *iftSeedSegmentation(iftPanicle *pan, iftImage *orig)
{
  iftImage      *open,*basins,*label,*mask;
  iftImage      *final_label;
  iftAdjRel     *A;
  iftLabeledSet *markers=NULL;
  

  /* Compute basins */

  A=iftCircular(sqrtf(2.0));
  basins = iftImageBasins(orig,A);

  /* Compute panicle mask */
  
  mask = iftPanicleMask(orig);
  
  /* Detect internal seed markers */

  open = iftOpenBin(mask,SEEDTHRES*SCALE/2.0); 
  iftDestroyImage(&mask); 

  label =  iftFastLabelComp(open,A); 
  iftDestroyImage(&open); 

  /* Segment seeds by watershed transform */

  markers=iftImageBorderLabeledSet(basins);
  for (int p=0; p < label->n; p++)
    if (label->val[p]>0) iftInsertLabeledSet(&markers,p,label->val[p]);

  iftDestroyImage(&label);
  label = iftWatershed(basins,A,markers);

  iftDestroyImage(&basins);
  iftDestroyLabeledSet(&markers);

  /* Pos-filtering: Remove all components on the main axis */
   
  int *lbrem = iftAllocIntArray(iftMaximumValue(label)+1); 
  for (int i=0; i < pan->mainaxis.npoints; i++) {
    iftVoxel u = pan->mainaxis.point[i]; 
    int p = iftGetVoxelIndex(orig,u);
    if (label->val[p]>0) /* mark for removal */
      lbrem[label->val[p]]=1;
  }
  for (int p=0; p < label->n; p++) 
    if (lbrem[label->val[p]]==1)
      label->val[p]=0; 

  final_label = iftRelabelRegions(label,A);
  iftDestroyAdjRel(&A);
  iftDestroyImage(&label);

  return(final_label);

}


