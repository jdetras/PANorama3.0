#include "iftPANorama.h"


/* 
   Author: Alexandre Falcao.  

   Description: This program computes measures of the panicles,
   resulting the PanicleMeasures.csv file. It also outputs the
   measures of each panicle and of each segment between nodes in a
   special file ExtraPanicleMeasures.csv.

*/


/* Methods to extract measures */

float iftLengthOfExtrusion(iftPanicle *pan);
float iftLengthOfMainAxis(iftPanicle *pan);

float iftMeanLengthBetweenNodes(iftPanicle *pan);
float iftStdevLengthBetweenNodes(iftPanicle *pan);
float iftMaxLengthBetweenNodes(iftPanicle *pan);
float iftMinLengthBetweenNodes(iftPanicle *pan);

float iftMeanLengthOfPrimaryBranches(iftPanicle *pan);
float iftStdevLengthOfPrimaryBranches(iftPanicle *pan);
float iftMaxLengthOfPrimaryBranches(iftPanicle *pan);
float iftMinLengthOfPrimaryBranches(iftPanicle *pan);

float iftMeanLengthOfPrimaryBranchesInInferiorSlab(iftPanicle *pan);
float iftStdevLengthOfPrimaryBranchesInInferiorSlab(iftPanicle *pan);
float iftMaxLengthOfPrimaryBranchesInInferiorSlab(iftPanicle *pan);
float iftMinLengthOfPrimaryBranchesInInferiorSlab(iftPanicle *pan);

float iftMeanLengthOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan);
float iftStdevLengthOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan);
float iftMaxLengthOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan);
float iftMinLengthOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan);

float iftMeanThicknessOfMainAxis(iftPanicle *pan, iftImage *dist);
float iftStdevThicknessOfMainAxis(iftPanicle *pan, iftImage *dist);
float iftMaximumThicknessOfMainAxis(iftPanicle *pan, iftImage *dist);
float iftMinimumThicknessOfMainAxis(iftPanicle *pan, iftImage *dist);

float iftMeanThicknessOfExtrusion(iftPanicle *pan, iftImage *dist);
float iftStdevThicknessOfExtrusion(iftPanicle *pan, iftImage *dist);
float iftMaximumThicknessOfExtrusion(iftPanicle *pan, iftImage *dist);
float iftMinimumThicknessOfExtrusion(iftPanicle *pan, iftImage *dist);

float iftAsymmetryOfPrimaryBranches(iftPanicle *pan);
float iftAsymmetryOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan);
float iftAsymmetryOfPrimaryBranchesInInferiorSlab(iftPanicle *pan);

float iftAsymmetryInNumberOfBranches(iftPanicle *pan);
float iftAsymmetryInNumberOfBranchesInSuperiorSlab(iftPanicle *pan);
float iftAsymmetryInNumberOfBranchesInInferiorSlab(iftPanicle *pan);

int   iftNumberOfTips(iftPanicle *pan);
int   iftNumberOfSeeds(iftPanicle *pan, iftImage *orig);

float iftMeanNumberOfBranchesPerNode(iftPanicle *pan);
float iftStdevNumberOfBranchesPerNode(iftPanicle *pan);
float iftMeanNumberOfBranchesPerNodeInSuperiorSlab(iftPanicle *pan);
float iftStdevNumberOfBranchesPerNodeInSuperiorSlab(iftPanicle *pan);
float iftMeanNumberOfBranchesPerNodeInInferiorSlab(iftPanicle *pan);
float iftStdevNumberOfBranchesPerNodeInInferiorSlab(iftPanicle *pan);

float iftMaxNumberOfBranchesPerNode(iftPanicle *pan);
float iftMaxNumberOfBranchesPerNodeInSuperiorSlab(iftPanicle *pan);
float iftMaxNumberOfBranchesPerNodeInInferiorSlab(iftPanicle *pan);

int   iftNumberOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan);
int   iftNumberOfPrimaryBranchesInInferiorSlab(iftPanicle *pan);

int   iftNumberOfNodesOnMainAxisInSuperiorSlab(iftPanicle *pan);
int   iftNumberOfNodesOnMainAxisInInferiorSlab(iftPanicle *pan);

float iftInflorescenceLength(iftPanicle *pan);

/* Methods to write panicle measures */

void  iftWritePanicleMeasures(iftPanicle *pan, iftImage *orig, char *filename, char *basename);
void iftWriteExtraPanicleMeasures(iftPanicle *pan, char *filename, char *basename);

/*------------------------------------------------------------------------------------*/


float iftLengthOfExtrusion(iftPanicle *pan)
{
  if (pan->nextpts > 0){
    return(iftSegmentLength(pan->extrusion,0,pan->nextpts-1));
  }else{
    return(0.0);
  }
}

float iftLengthOfMainAxis(iftPanicle *pan)
{
  return(iftSegmentLength(pan->mainaxis.point,0,pan->mainaxis.npoints-1));  
}

float iftInflorescenceLength(iftPanicle *pan)
{
  int j, i;
  float len, maxlen=0;

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if (j==(pan->njunctions-1)){
      len = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
      if (len > maxlen) 
	maxlen = len;
    }
  }

  return(maxlen+iftSegmentLength(pan->mainaxis.point,0,pan->mainaxis.npoints-1));
}

float iftMeanLengthBetweenNodes(iftPanicle *pan)
{
  int i,n=0; 
  float mean=0.0; 

  if (pan->junction[0] > 0){ /* the first point on the main axis is
				not a junction point */

    mean = iftSegmentLength(pan->mainaxis.point,0,pan->junction[0]); 

    if (mean > MINFSTJCTLEN) { /* the first segment is valid */
      n    = 1;
    }else{
      mean = 0.0;
    }
  }
    
  for (i=0; i < pan->njunctions-1; i++) {
    mean += iftSegmentLength(pan->mainaxis.point,pan->junction[i],pan->junction[i+1]);
    n++;
  }

  mean /= n;
  
  return(mean);
}

float iftStdevLengthBetweenNodes(iftPanicle *pan)
{
  
  int i,n=0; 
  float mean=0.0,stdev=0.0,aux; 

  if (pan->junction[0] > 0){ /* the first point on the main axis is
				not a junction point */

    mean = iftMeanLengthBetweenNodes(pan);

    aux  = iftSegmentLength(pan->mainaxis.point,0,pan->junction[0]);
    if (aux > MINFSTJCTLEN) { /* the first segment is valid */
      stdev = powf((aux - mean),2.0); 
      n    = 1;
    }
    
    
    for (i=0; i < pan->njunctions-1; i++) {
      stdev += powf((iftSegmentLength(pan->mainaxis.point,pan->junction[i],pan->junction[i+1])-mean),2.0);
      n++;
    }
  
    if (n > 1)
      stdev = sqrtf(stdev/(n-1)); 
    else
      stdev = sqrtf(stdev);
  }

  return(stdev);

}

float iftMaxLengthBetweenNodes(iftPanicle *pan)
{
  int i; 
  float maxlen=INFINITY_FLT_NEG, len; 

  if (pan->njunctions > 0) {
    maxlen  = iftSegmentLength(pan->mainaxis.point,0,pan->junction[0]); 
    for (i=0; i < pan->njunctions-1; i++) {
      len = iftSegmentLength(pan->mainaxis.point,pan->junction[i],pan->junction[i+1]);
      if (len > maxlen) 
	maxlen=len;
    }
  }
  
  if (maxlen == INFINITY_FLT_NEG)
    maxlen = 0.0;

  return(maxlen);
}

float iftMinLengthBetweenNodes(iftPanicle *pan)
{
  int i; 
  float minlen=INFINITY_FLT, len; 

  if (pan->njunctions > 0) {
    if (pan->junction[0] > 0){ /* the first point on the main axis is
				 not a junction point */   
      minlen  = iftSegmentLength(pan->mainaxis.point,0,pan->junction[0]); 
      if (minlen <= MINFSTJCTLEN) { /* the first segment is not valid */
	minlen = INFINITY_FLT;
      }
    }

    for (i=0; i < pan->njunctions-1; i++) {
      len = iftSegmentLength(pan->mainaxis.point,pan->junction[i],pan->junction[i+1]);
      if (len < minlen) 
	minlen=len;
    }
  }
  
  if (minlen == INFINITY_FLT)
    minlen = 0.0;

  return(minlen);
}


float iftMeanLengthOfPrimaryBranches(iftPanicle *pan)
{
  int i;
  float mean=0.0; 
    
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) 
      mean += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
    mean /= pan->nbranches;
  }
  
  return(mean);

}

float iftStdevLengthOfPrimaryBranches(iftPanicle *pan)
{
  int i;
  float mean=0.0, stdev=0.0, *len=NULL;
    
  if (pan->nbranches > 0){

    len=iftAllocFloatArray(pan->nbranches); 

    for (i=0; i < pan->nbranches; i++) {
      len[i] = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
      mean  += len[i];
    }

    mean /= pan->nbranches;

    for (i=0; i < pan->nbranches; i++) 
      stdev += powf((len[i]-mean),2.0);

    if (pan->nbranches > 1)
      stdev = sqrtf(stdev/(pan->nbranches-1));
    else
      stdev = sqrtf(stdev);

    free(len);
  }
  

  return(stdev);
}

float iftMaxLengthOfPrimaryBranches(iftPanicle *pan)
{
  int i;
  float len=0.0,maxlen=INFINITY_FLT_NEG; 
    
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      len = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
      if (len > maxlen) 
	maxlen = len;
    }
  }

  if (maxlen == INFINITY_FLT_NEG) 
    maxlen = 0.0;

  return(maxlen);
}

float iftMinLengthOfPrimaryBranches(iftPanicle *pan)
{
  int i;
  float len=0.0,minlen=INFINITY_FLT; 
    
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      len = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
      if (len < minlen) 
	minlen = len;
    }
  }

  if (minlen == INFINITY_FLT) 
    minlen = 0.0;

  return(minlen);
}

float iftMeanLengthOfPrimaryBranchesInInferiorSlab(iftPanicle *pan)
{
  int i,center_pos,j,n;
  float mean=0.0; 

  center_pos = iftCenterOfMainAxis(pan);

  n = 0;
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j >= 0)&&(pan->junction[j] < center_pos)) {
	mean += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	n++;
      }
    }
    if (n > 0)
      mean /= n;
  }

  return(mean);
}

float iftStdevLengthOfPrimaryBranchesInInferiorSlab(iftPanicle *pan)
{
  int i,center_pos,j,k,n;
  float mean=0.0,stdev=0.0,*len=NULL; 

  center_pos = iftCenterOfMainAxis(pan);
  n = 0;

  if (pan->nbranches > 0){

    for (i=0; i < pan->nbranches; i++) { /* Number of branches in inferior slab */
      j = iftGetBranchJunction(pan,i);
      if ((j >=0)&&(pan->junction[j] < center_pos)) {
	n++;
      }
    }
    if (n > 0) {

      len = iftAllocFloatArray(n);
      k   = 0;
      for (i=0; i < pan->nbranches; i++) { /* mean length of the branches in the inferior slab */
	j = iftGetBranchJunction(pan,i);
	if ((j>=0)&&(pan->junction[j] < center_pos)) {
	  len[k] = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	  mean  += len[k];
	  k++;
	}
      }      
      mean /= n;
      
      for (k=0; k < n; k++) {  /* standard deviation of the branch length in the inferior slab */
	stdev += powf(len[k]-mean,2.0);
      }
      if (n > 1) 
	stdev = sqrtf(stdev/(n-1));
      else
	stdev = sqrtf(stdev);

      free(len);

    }
  }

  return(stdev);
}

float iftMaxLengthOfPrimaryBranchesInInferiorSlab(iftPanicle *pan)
{
  int i, center_pos,j;
  float len=0.0,maxlen=INFINITY_FLT_NEG; 
    
  center_pos = iftCenterOfMainAxis(pan);

  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] < center_pos)) {
	len = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	if (len > maxlen) 
	  maxlen = len;
      }
    }
  }

  if (maxlen == INFINITY_FLT_NEG) 
    maxlen = 0.0;

  return(maxlen);
}

float iftMinLengthOfPrimaryBranchesInInferiorSlab(iftPanicle *pan)
{
  int i, j, center_pos;
  float len=0.0,minlen=INFINITY_FLT; 
    
  center_pos = iftCenterOfMainAxis(pan);

  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] < center_pos)) {
	len = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	if (len < minlen) 
	  minlen = len;
      }
    }
  }

  if (minlen == INFINITY_FLT) 
    minlen = 0.0;

  return(minlen);
}

float iftMeanLengthOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan)
{
  int i,j,center_pos,n;
  float mean=0.0; 

  center_pos = iftCenterOfMainAxis(pan);

  n = 0;
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] >= center_pos)) {
	mean += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	n++;
      }
    }
    if (n > 0)
      mean /= n;
  }

  return(mean);
}

float iftStdevLengthOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan)
{
  int i,center_pos,j,k,n;
  float mean=0.0,stdev=0.0,*len=NULL; 

  center_pos = iftCenterOfMainAxis(pan);
  n = 0;

  if (pan->nbranches > 0){

    for (i=0; i < pan->nbranches; i++) { /* Number of branches in superior slab */
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] >= center_pos)) {
	n++;
      }
    }
    if (n > 0) {

      len = iftAllocFloatArray(n);
      k   = 0;
      for (i=0; i < pan->nbranches; i++) { /* mean length of the branches in the superior slab */
	j = iftGetBranchJunction(pan,i);
	if ((j>=0)&&(pan->junction[j] >= center_pos)) {
	  len[k] = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	  mean  += len[k];
	  k++;
	}
      }      
      mean /= n;
      
      for (k=0; k < n; k++) {  /* standard deviation of the branch length in the superior slab */
	stdev += powf(len[k]-mean,2.0);
      }
      if (n > 1) 
	stdev = sqrtf(stdev/(n-1));
      else
	stdev = sqrtf(stdev);

      free(len);

    }
    
  }

  return(stdev);
}

float iftMaxLengthOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan)
{
  int i, center_pos, j;
  float len=0.0,maxlen=INFINITY_FLT_NEG; 
    
  center_pos = iftCenterOfMainAxis(pan);

  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] >= center_pos)) {
	len = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	if (len > maxlen) 
	  maxlen = len;
      }
    }
  }

  if (maxlen == INFINITY_FLT_NEG) 
    maxlen = 0.0;

  return(maxlen);
}

float iftMinLengthOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan)
{
  int i, center_pos, j;
  float len=0.0,minlen=INFINITY_FLT; 
    
  center_pos = iftCenterOfMainAxis(pan);

  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] >= center_pos)) {
	len = iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	if (len < minlen) 
	  minlen = len;
      }
    }
  }

  if (minlen == INFINITY_FLT) 
    minlen = 0.0;

  return(minlen);
}

float iftAsymmetryInNumberOfBranches(iftPanicle *pan)
{
  int i;
  float asymmetry=0.0, n1=0, n2=0; 
    
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      if (iftVoxelLinePosition2D(pan->primary[i].last,pan->mainaxis.first,pan->mainaxis.last) <= 0){
	n1++;
      }else{
	n2++;
      }
    }
  }

  if ((n1+n2) > 0)
    asymmetry = fabs((n1-n2)/(n1+n2));

  return(asymmetry);
}

float iftAsymmetryInNumberOfBranchesInSuperiorSlab(iftPanicle *pan)
{
  int i,center_pos,j;
  float asymmetry=0, n1=0, n2=0; 

  center_pos = iftCenterOfMainAxis(pan);

  n1 = n2 = 0;
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] >= center_pos)) {
	if (iftVoxelLinePosition2D(pan->primary[i].last,pan->mainaxis.first,pan->mainaxis.last) <= 0){ /* Left Side */
	  n1++;
	}else{ /* Right Side */
	  n2++;
	}
      }
    }
  }

  if ((n1+n2) > 0)
    asymmetry = fabs((n1-n2)/(n1+n2));

  return(asymmetry);
}

float iftAsymmetryInNumberOfBranchesInInferiorSlab(iftPanicle *pan)
{
  int i,center_pos,j;
  float asymmetry=0, n1=0, n2=0; 

  center_pos = iftCenterOfMainAxis(pan);

  n1 = n2 = 0;
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] < center_pos)) {
	if (iftVoxelLinePosition2D(pan->primary[i].last,pan->mainaxis.first,pan->mainaxis.last) <= 0){ /* Left Side */
	  n1++;
	}else{ /* Right Side */
	  n2++;
	}
      }
    }
  }

  if ((n1+n2) > 0)
    asymmetry = fabs((n1-n2)/(n1+n2));

  return(asymmetry);
}


float iftAsymmetryOfPrimaryBranches(iftPanicle *pan)
{
  int i, n1=0, n2=0;
  float mean1=0.0, mean2=0.0,asymmetry=0.0; 
    
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      if (iftVoxelLinePosition2D(pan->primary[i].last,pan->mainaxis.first,pan->mainaxis.last) <= 0){
	mean1 += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	n1++;
      }else{
	mean2 += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	n2++;
      }
    }
    
    if (n1 > 0) mean1 /= n1;
    if (n2 > 0) mean2 /= n2;
    
  }

  if ((mean1+mean2) > 0)
    asymmetry = fabs((mean1-mean2)/(mean1+mean2));

  return(asymmetry);
}


float iftAsymmetryOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan)
{
  int i,center_pos,j;
  float asymmetry=0, n1=0, n2=0, mean1 = 0, mean2 = 0; 

  center_pos = iftCenterOfMainAxis(pan);

  n1 = n2 = 0;
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] >= center_pos)) {
	if (iftVoxelLinePosition2D(pan->primary[i].last,pan->mainaxis.first,pan->mainaxis.last) <= 0){ /* Left Side */
	  mean1 += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	  n1++;
	}else{ /* Right Side */
	  mean2 += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	  n2++;
	}
      }
    }
    if (n1 > 0) mean1 /= n1;
    if (n2 > 0) mean2 /= n2;
  }

  if ((mean1+mean2) > 0)
    asymmetry = fabs((mean1-mean2)/(mean1+mean2));

  return(asymmetry);
}

float iftAsymmetryOfPrimaryBranchesInInferiorSlab(iftPanicle *pan)
{
  int i,center_pos,j;
  float asymmetry=0, n1=0, n2=0, mean1 = 0, mean2 = 0; 

  center_pos = iftCenterOfMainAxis(pan);

  n1 = n2 = 0;
  if (pan->nbranches > 0){
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] < center_pos)) {
	if (iftVoxelLinePosition2D(pan->primary[i].last,pan->mainaxis.first,pan->mainaxis.last) <= 0){ /* Left Side */
	  mean1 += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	  n1++;
	}else{ /* Right Side */
	  mean2 += iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1);
	  n2++;
	}
      }
    }
    if (n1 > 0) mean1 /= n1;
    if (n2 > 0) mean2 /= n2;
  }

  if ((mean1+mean2) > 0)
    asymmetry = fabs((mean1-mean2)/(mean1+mean2));

  return(asymmetry);
}

float iftMeanThicknessOfMainAxis(iftPanicle *pan, iftImage *dist)
{
  int i, p;
  float mean; 


  mean = 0;
  for (i=0; i < pan->mainaxis.npoints; i++) {
    p = iftGetVoxelIndex(dist,pan->mainaxis.point[i]);
    mean += 2.0*sqrtf(dist->val[p])/pan->scale;
  }
  mean /= pan->mainaxis.npoints;

  return(mean);
}

float iftStdevThicknessOfMainAxis(iftPanicle *pan, iftImage *dist)
{
  int i, p;
  float mean=0.0,stdev=0.0,*thick=iftAllocFloatArray(pan->mainaxis.npoints); 
 
  for (i=0; i < pan->mainaxis.npoints; i++) {
    p = iftGetVoxelIndex(dist,pan->mainaxis.point[i]);
    thick[i] = 2.0*sqrtf(dist->val[p])/pan->scale;
    mean    += thick[i];
  }
  mean /= pan->mainaxis.npoints;

  for (i=0; i < pan->mainaxis.npoints; i++) {
    stdev += powf((thick[i]-mean),2.0);
  }
  stdev = sqrtf(stdev/(pan->mainaxis.npoints-1));

  free(thick);

  return(stdev);
}


float iftMaxThicknessOfMainAxis(iftPanicle *pan, iftImage *dist)
{
  int i, p;
  float max; 

  /* Compute the maximum thickness of the main axis */

  max = INFINITY_FLT_NEG;
  for (i=0; i < pan->mainaxis.npoints; i++) {
    p = iftGetVoxelIndex(dist,pan->mainaxis.point[i]);
    if (dist->val[p] > max) 
      max = dist->val[p];
  }

  return(2.0*sqrtf(max)/pan->scale);
}

float iftMinThicknessOfMainAxis(iftPanicle *pan, iftImage *dist)
{
  int i, p;
  float min; 

  /* Compute the minimum thickness of the main axis */

  min = INFINITY_FLT;
  for (i=0; i < pan->mainaxis.npoints; i++) {
    p = iftGetVoxelIndex(dist,pan->mainaxis.point[i]);
    if (dist->val[p] < min)
      min = dist->val[p];
  }

  return(2.0*sqrtf(min)/pan->scale);
}

float iftMeanThicknessOfExtrusion(iftPanicle *pan, iftImage *dist)
{
  int i, p;
  float mean=0.0; 

  if (pan->nextpts == 0) 
    return(mean);

  mean = 0;
  for (i=0; i < pan->nextpts; i++) {
    p = iftGetVoxelIndex(dist,pan->extrusion[i]);
    mean += sqrtf(dist->val[p]);
  }
  mean /= pan->nextpts;


  return(2.0*mean/pan->scale);
}

float iftStdevThicknessOfExtrusion(iftPanicle *pan, iftImage *dist)
{
  int i, p;
  float mean=0.0, stdev=0.0, *thick=NULL; 

  if (pan->nextpts == 0) 
    return(stdev);

  thick = iftAllocFloatArray(pan->nextpts);
  mean  = 0;
  for (i=0; i < pan->nextpts; i++) {
    p = iftGetVoxelIndex(dist,pan->extrusion[i]);
    thick[i] = 2.0*sqrtf(dist->val[p])/pan->scale;
    mean    += thick[i];
  }
  mean /= pan->nextpts;

  for (i=0; i < pan->nextpts; i++) {
    stdev += powf(thick[i]-mean,2.0);
  }
  
  if (pan->nextpts > 1)
    stdev = sqrtf(stdev/(pan->nextpts-1));
  else
    stdev = sqrtf(stdev);

  free(thick);

  return(stdev);
}

float iftMaxThicknessOfExtrusion(iftPanicle *pan, iftImage *dist)
{
  int i, p;
  float max=INFINITY_FLT_NEG; 

  if (pan->nextpts == 0) 
    return(0);

  /* Compute the maximum thickness of the extrusion */

  for (i=0; i < pan->nextpts; i++) {
    p = iftGetVoxelIndex(dist,pan->extrusion[i]);
    if (dist->val[p] > max)
      max = dist->val[p];
  }

  return(2.0*sqrtf(max)/pan->scale);
}

float iftMinThicknessOfExtrusion(iftPanicle *pan, iftImage *dist)
{
  int i, p;
  float min=INFINITY_FLT; 

  if (pan->nextpts == 0) 
    return(0);


  /* Compute the minimum thickness of the extrusion */

  for (i=0; i < pan->nextpts; i++) {
    p = iftGetVoxelIndex(dist,pan->extrusion[i]);
    if (dist->val[p] < min)
      min = dist->val[p];
  }

  return(2.0*sqrtf(min)/pan->scale);
}

int iftNumberOfTips(iftPanicle *pan)
{
  iftAdjRel *A=iftCircular(sqrtf(2.0));
  iftSet *S=iftEndPoints(pan->skel,A);
  int nendpts=0;

  while (S != NULL){
    iftRemoveSet(&S);
    nendpts++;
  }
  iftDestroyAdjRel(&A);

  return(nendpts);
}

int iftNumberOfSeeds(iftPanicle *pan, iftImage *orig)
{
  iftImage *aux[2];
  int nseeds;
  char command[200];

  /* Compute panicle mask */
  
  aux[0] = iftPanicleMask(orig);

  /* Remove the awns (thin parts) and label connected components, but
     use slightly higher threshold to obtain the seeds. */

  aux[1] = iftRemoveAwns(aux[0],(int)(powf(SEEDTHRES*SCALE,2.0)));

  if (DEBUG==1) {
    iftImage *temp=iftColorizeCompOverImage(orig,aux[1]);
    iftWriteImageP6(temp,"temp.ppm");
    sprintf(command,"convert temp.ppm panicle_seeds_over_image.jpg");
    system(command);
    sprintf(command,"rm -f temp.ppm");
    system(command);
    iftDestroyImage(&temp);
    temp=iftColorizeComp(aux[1]);
    iftWriteImageP6(temp,"temp.ppm");
    sprintf(command,"convert temp.ppm panicle_seeds.jpg");
    system(command);
    sprintf(command,"rm -f temp.ppm");
    system(command);
    iftDestroyImage(&temp);
  }

  iftDestroyImage(&aux[0]);

  nseeds = iftMaximumValue(aux[1])-1;  
  iftDestroyImage(&aux[1]);

  return(nseeds);
}

float iftMeanNumberOfBranchesPerNode(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions],j; 
  float mean=0.0;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute number of branches per node along the main axis */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if (j>=0) nbranches_per_node[j]++;
  }
  
  /* Compute the mean value of the number of branches per node */

  for (i=0; i < pan->njunctions; i++) {
    mean += nbranches_per_node[i];
  }
  mean /= pan->njunctions;

  return(mean);
}

float iftStdevNumberOfBranchesPerNode(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions],j; 
  float mean=0.0, stdev=0.0;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute number of branches per node along the main axis */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if (j>=0) nbranches_per_node[j]++;
  }
  
  /* Compute the mean value of the number of branches per node */

  for (i=0; i < pan->njunctions; i++) {
    mean += nbranches_per_node[i];
  }
  mean /= pan->njunctions;

  /* Compute the standard deviation of the number of branches per node */

  for (i=0; i < pan->njunctions; i++) {
    stdev += powf((nbranches_per_node[i]-mean),2.0);
  }
  stdev = sqrtf(stdev/(pan->njunctions-1));

  return(stdev);
}

float iftMeanNumberOfBranchesPerNodeInSuperiorSlab(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions]; 
  float mean=0.0, n=0;
  int center_pos, j;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of branches per node along the main axis in the
     superior slab */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if ((j>=0)&&(pan->junction[j] >= center_pos))
      nbranches_per_node[j]++;
  }
  
  /* Compute the mean number of branches per node in the superior slab */

  for (i=0; i < pan->njunctions; i++) {
    if (pan->junction[i] >= center_pos){
      mean += nbranches_per_node[i];
      n++;
    }
  }
  if (n > 0)
    mean /= n;

  return(mean);
}

float iftStdevNumberOfBranchesPerNodeInSuperiorSlab(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions]; 
  float mean=0.0, stdev=0.0,n=0;
  int center_pos, j;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of branches per node along the main axis in the
     superior slab */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if ((j>=0)&&(pan->junction[j] >= center_pos))
      nbranches_per_node[j]++;
  }
  
  /* Compute the mean number of branches per node in the superior slab */

  for (i=0; i < pan->njunctions; i++) {
    if (pan->junction[i] >= center_pos){
      mean += nbranches_per_node[i];
      n++;
    }
  }

  if (n > 0) {
    mean /= n;

    /* Compute the standard deviation of the number of branches per node in the superior slab */

    for (i=0; i < pan->njunctions; i++) {
      if (pan->junction[i] >= center_pos){
	stdev += powf((nbranches_per_node[i]-mean),2.0);
      }
    }
    if (n > 1)
      stdev = sqrtf(stdev/(n-1));
    else
      stdev = sqrtf(stdev);
  }

  return(stdev);
}

float iftMeanNumberOfBranchesPerNodeInInferiorSlab(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions]; 
  float mean=0.0, n=0;
  int center_pos, j;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of branches per node along the main axis in the
     inferior slab */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if ((j>=0)&&(pan->junction[j] < center_pos))
      nbranches_per_node[j]++;
  }
  
  /* Compute the mean number of branches per node in the inferior slab */

  for (i=0; i < pan->njunctions; i++) {
    if (pan->junction[i] < center_pos){
      mean += nbranches_per_node[i];
      n++;
    }
  }
  if (n > 0)
    mean /= n;

  return(mean);
}

float iftStdevNumberOfBranchesPerNodeInInferiorSlab(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions]; 
  float mean=0.0, stdev=0.0, n=0;
  int center_pos, j;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of branches per node along the main axis in the
     inferior slab */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if ((j>=0)&&(pan->junction[j] < center_pos))
      nbranches_per_node[j]++;
  }
  
  /* Compute the mean number of branches per node in the inferior slab */

  for (i=0; i < pan->njunctions; i++) {
    if (pan->junction[i] < center_pos){
      mean += nbranches_per_node[i];
      n++;
    }
  }

  if (n > 0) {

    mean /= n;

    /* Compute the standard deviation of the number of branches per node in the inferior slab */

    for (i=0; i < pan->njunctions; i++) {
      if (pan->junction[i] < center_pos){
	stdev += powf((nbranches_per_node[i]-mean),2.0);
      }
    }
    if (n > 1)
      stdev = sqrtf(stdev/(n-1));
    else
      stdev = sqrtf(stdev);
  }

  return(stdev);
}


float iftMaxNumberOfBranchesPerNode(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions],j; 
  int max;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute number of branches per node along the main axis */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if (j>=0) nbranches_per_node[j]++;
  }
  
  /* Compute the mean value of the number of branches per node */

  max = 0;
  for (i=0; i < pan->njunctions; i++) {
    if (nbranches_per_node[i] > max) 
      max = nbranches_per_node[i];
  }

  return(max);
}

float iftMaxNumberOfBranchesPerNodeInSuperiorSlab(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions]; 
  int center_pos, j, max;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of branches per node along the main axis in the
     superior slab */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if ((j>=0)&&(pan->junction[j] >= center_pos))
      nbranches_per_node[j]++;
  }
  
  /* Compute the mean number of branches per node in the superior slab */

  max = 0;
  for (i=0; i < pan->njunctions; i++) {
    if (pan->junction[i] >= center_pos){
      if (nbranches_per_node[i] > max) 
	max = nbranches_per_node[i];
    }
  }

  return(max);
}

float iftMaxNumberOfBranchesPerNodeInInferiorSlab(iftPanicle *pan)
{
  int i, nbranches_per_node[pan->njunctions]; 
  int center_pos, j, max;

  /* Initialize number of branches per node */

  for (i=0; i < pan->njunctions; i++) 
    nbranches_per_node[i]=0;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of branches per node along the main axis in the
     inferior slab */

  for (i=0; i < pan->nbranches; i++) {
    j = iftGetBranchJunction(pan,i);
    if ((j>=0)&&(pan->junction[j] < center_pos))
      nbranches_per_node[j]++;
  }
  
  /* Compute the mean number of branches per node in the inferior slab */

  max = 0;
  for (i=0; i < pan->njunctions; i++) {
    if (pan->junction[i] < center_pos){
      if (nbranches_per_node[i] > max) 
	max = nbranches_per_node[i];
    }
  }

  return(max);
}

int iftNumberOfPrimaryBranchesInSuperiorSlab(iftPanicle *pan)
{
  int i, j, n=0; 
  int center_pos;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of branches in the superior slab */
  
  if (pan->nbranches > 0) {
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] >= center_pos))
	n++;
    }
  }
  
  return(n);

}

int iftNumberOfPrimaryBranchesInInferiorSlab(iftPanicle *pan)
{
  int i, j, n=0; 
  int center_pos;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of branches in the inferior slab */
  
  if (pan->nbranches > 0) {
    for (i=0; i < pan->nbranches; i++) {
      j = iftGetBranchJunction(pan,i);
      if ((j>=0)&&(pan->junction[j] < center_pos))
	n++;
    }
  }
  
  return(n);

}

int iftNumberOfNodesOnMainAxisInSuperiorSlab(iftPanicle *pan)
{
  int i, n=0; 
  int center_pos;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of junctions in the superior slab */
  
  for (i=0; i < pan->njunctions; i++) {
    if (pan->junction[i] >= center_pos)
      n++;
  }
  
  return(n);
}

int iftNumberOfNodesOnMainAxisInInferiorSlab(iftPanicle *pan)
{
  int i, n=0; 
  int center_pos;

  /* Compute center of the main axis */

  center_pos = iftCenterOfMainAxis(pan);

  /* Compute number of junctions in the inferior slab */
  
  for (i=0; i < pan->njunctions; i++) {
    if (pan->junction[i] < center_pos)
      n++;
  }
  
  return(n);
}



void iftWritePanicleMeasures(iftPanicle *pan, iftImage *orig, char *filename, char *basename)
{
  FILE *fp=NULL;
  char *token, lineID[40], lineID_aux[20];
  char  msg[40], basename_aux[100]; 
  int exp, panicle, rep, date;
  iftImage *dist;

  strcpy(basename_aux,basename);

  /* Compute EDT (Euclidean Distance Transform) of the panicle */
  dist = iftPanicleEDT(orig);

  token = strtok(basename_aux,"_");
  sprintf(msg,"Invalid basename: %s",basename_aux);
 
  if (strcmp(token,"exp")==0){
    token = strtok(NULL,"_");  
    exp = atoi(token);    
    token = strtok(NULL,"_");  
    strcpy(lineID,token); 
    token = strtok(NULL,"_"); 
    strcpy(lineID_aux,token);
    strcat(lineID,lineID_aux);
    token = strtok(NULL,"_");  
    if (strcmp(token,"rep")!=0){
      iftError(msg,"iftWritePanicleMeasures");
    }
    token = strtok(NULL,"_");  
    rep = atoi(token);
    token = strtok(NULL,"_");  
    if (strcmp(token,"pan")!=0){
      iftError(msg,"iftWritePanicleMeasures");
    }
    token = strtok(NULL,"_");  
    panicle = atoi(token);
    token = strtok(NULL,"_");  
    if (strcmp(token,"date")!=0){
      iftError(msg,"iftWritePanicleMeasures");
    }
    token = strtok(NULL,"_");  
    //strcpy(date,token); 
    date = atoi(token);
  }else{
    iftError(msg,"iftWritePanicleMeasures");
  }

  fp = fopen(filename,"r");
  if (fp == NULL) {
    fp = fopen(filename,"w");

    /* Write Header */

    fprintf(fp,"LineID(v%2.1f);",VERSION);
    fprintf(fp,"Experiment;");
    fprintf(fp,"Replicate;");
    fprintf(fp,"Panicle;");
    fprintf(fp,"Date;");
    fprintf(fp,"LengthOfExtrusion(cm);");
    fprintf(fp,"LengthOfMainAxis(cm);");
    fprintf(fp,"InflorescenceLength(cm);");
    fprintf(fp,"MeanLengthBetweenNodes(cm);");
    fprintf(fp,"StdevLengthBetweenNodes(cm);");
    fprintf(fp,"MaxLengthBetweenNodes(cm);");
    fprintf(fp,"MinLengthBetweenNodes(cm);");
    fprintf(fp,"MeanLengthOfPrimaryBranches(cm);");
    fprintf(fp,"StdevLengthOfPrimaryBranches(cm);");
    fprintf(fp,"MaxLengthOfPrimaryBranches(cm);");
    fprintf(fp,"MinLengthOfPrimaryBranches(cm);");
    fprintf(fp,"MeanLengthOfPrimaryBranchesInInferiorSlab(cm);");
    fprintf(fp,"StdevLengthOfPrimaryBranchesInInferiorSlab(cm);");
    fprintf(fp,"MaxLengthOfPrimaryBranchesInInferiorSlab(cm);");
    fprintf(fp,"MinLengthOfPrimaryBranchesInInferiorSlab(cm);");
    fprintf(fp,"MeanLengthOfPrimaryBranchesInSuperiorSlab(cm);");
    fprintf(fp,"StdevLengthOfPrimaryBranchesInSuperiorSlab(cm);");
    fprintf(fp,"MaxLengthOfPrimaryBranchesInSuperiorSlab(cm);");
    fprintf(fp,"MinLengthOfPrimaryBranchesInSuperiorSlab(cm);");
    fprintf(fp,"AsymmetryOfPrimaryBranches;");
    fprintf(fp,"AsymmetryOfPrimaryBranchesInInferiorSlab;");
    fprintf(fp,"AsymmetryOfPrimaryBranchesInSuperiorSlab;");
    fprintf(fp,"AsymmetryInNumberOfBranches;");
    fprintf(fp,"AsymmetryInNumberOfBranchesInInferiorSlab;");
    fprintf(fp,"AsymmetryInNumberOfBranchesInSuperiorSlab;");
    fprintf(fp,"MeanThicknessOfMainAxis(cm);");
    fprintf(fp,"StdevThicknessOfMainAxis(cm);");
    fprintf(fp,"MaxThicknessOfMainAxis(cm);");
    fprintf(fp,"MinThicknessOfMainAxis(cm);");
    fprintf(fp,"MeanThicknessOfExtrusion(cm);");
    fprintf(fp,"StdevThicknessOfExtrusion(cm);");
    fprintf(fp,"MaxThicknessOfExtrusion(cm);");
    fprintf(fp,"MinThicknessOfExtrusion(cm);");
    fprintf(fp,"NumberOfPrimaryBranches;");
    fprintf(fp,"NumberOfPrimaryBranchesInSuperiorSlab;");
    fprintf(fp,"NumberOfPrimaryBranchesInInferiorSlab;");
    fprintf(fp,"NumberOfNodesOnMainAxis;");
    fprintf(fp,"NumberOfNodesOnMainAxisInSuperiorSlab;");
    fprintf(fp,"NumberOfNodesOnMainAxisInInferiorSlab;");
    fprintf(fp,"NumberOfSeeds;");
    fprintf(fp,"NumberOfTips;");
    fprintf(fp,"MeanNumberOfBranchesPerNode;");
    fprintf(fp,"StdevNumberOfBranchesPerNode;");
    fprintf(fp,"MeanNumberOfBranchesPerNodeInSuperiorSlab;");
    fprintf(fp,"StdevNumberOfBranchesPerNodeInSuperiorSlab;");
    fprintf(fp,"MeanNumberOfBranchesPerNodeInInferiorSlab;");
    fprintf(fp,"StdevNumberOfBranchesPerNodeInInferiorSlab;");
    fprintf(fp,"MaxNumberOfBranchesPerNode;");
    fprintf(fp,"MaxNumberOfBranchesPerNodeInSuperiorSlab;");
    fprintf(fp,"MaxNumberOfBranchesPerNodeInInferiorSlab\n");
    fclose(fp);
  }else{
    fclose(fp);
  }

  fp = fopen(filename,"a");

  /* Write Data */

  fprintf(fp,"%s;",lineID);
  fprintf(fp,"%d;",exp);
  fprintf(fp,"%d;",rep);
  fprintf(fp,"%d;",panicle);
  //  fprintf(fp,"%s;",date);
  fprintf(fp,"%08d;",date);
  fprintf(fp,"%f;",iftLengthOfExtrusion(pan));
  fprintf(fp,"%f;",iftLengthOfMainAxis(pan));
  fprintf(fp,"%f;",iftInflorescenceLength(pan));
  fprintf(fp,"%f;",iftMeanLengthBetweenNodes(pan));
  fprintf(fp,"%f;",iftStdevLengthBetweenNodes(pan));
  fprintf(fp,"%f;",iftMaxLengthBetweenNodes(pan));
  fprintf(fp,"%f;",iftMinLengthBetweenNodes(pan));
  fprintf(fp,"%f;",iftMeanLengthOfPrimaryBranches(pan));
  fprintf(fp,"%f;",iftStdevLengthOfPrimaryBranches(pan));
  fprintf(fp,"%f;",iftMaxLengthOfPrimaryBranches(pan));
  fprintf(fp,"%f;",iftMinLengthOfPrimaryBranches(pan));
  fprintf(fp,"%f;",iftMeanLengthOfPrimaryBranchesInInferiorSlab(pan)); 
  fprintf(fp,"%f;",iftStdevLengthOfPrimaryBranchesInInferiorSlab(pan)); 
  fprintf(fp,"%f;",iftMaxLengthOfPrimaryBranchesInInferiorSlab(pan)); 
  fprintf(fp,"%f;",iftMinLengthOfPrimaryBranchesInInferiorSlab(pan)); 
  fprintf(fp,"%f;",iftMeanLengthOfPrimaryBranchesInSuperiorSlab(pan)); 
  fprintf(fp,"%f;",iftStdevLengthOfPrimaryBranchesInSuperiorSlab(pan)); 
  fprintf(fp,"%f;",iftMaxLengthOfPrimaryBranchesInSuperiorSlab(pan)); 
  fprintf(fp,"%f;",iftMinLengthOfPrimaryBranchesInSuperiorSlab(pan)); 
  fprintf(fp,"%f;",iftAsymmetryOfPrimaryBranches(pan));
  fprintf(fp,"%f;",iftAsymmetryOfPrimaryBranchesInInferiorSlab(pan));
  fprintf(fp,"%f;",iftAsymmetryOfPrimaryBranchesInSuperiorSlab(pan));
  fprintf(fp,"%f;",iftAsymmetryInNumberOfBranches(pan));
  fprintf(fp,"%f;",iftAsymmetryInNumberOfBranchesInInferiorSlab(pan));
  fprintf(fp,"%f;",iftAsymmetryInNumberOfBranchesInSuperiorSlab(pan));
  fprintf(fp,"%f;",iftMeanThicknessOfMainAxis(pan,dist));
  fprintf(fp,"%f;",iftStdevThicknessOfMainAxis(pan,dist));
  fprintf(fp,"%f;",iftMaxThicknessOfMainAxis(pan,dist));
  fprintf(fp,"%f;",iftMinThicknessOfMainAxis(pan,dist));
  fprintf(fp,"%f;",iftMeanThicknessOfExtrusion(pan,dist));
  fprintf(fp,"%f;",iftStdevThicknessOfExtrusion(pan,dist));
  fprintf(fp,"%f;",iftMaxThicknessOfExtrusion(pan,dist));
  fprintf(fp,"%f;",iftMinThicknessOfExtrusion(pan,dist));
  fprintf(fp,"%d;",pan->nbranches);
  fprintf(fp,"%d;",iftNumberOfPrimaryBranchesInSuperiorSlab(pan));
  fprintf(fp,"%d;",iftNumberOfPrimaryBranchesInInferiorSlab(pan));
  fprintf(fp,"%d;",pan->njunctions);
  fprintf(fp,"%d;",iftNumberOfNodesOnMainAxisInSuperiorSlab(pan));
  fprintf(fp,"%d;",iftNumberOfNodesOnMainAxisInInferiorSlab(pan));
  fprintf(fp,"%d;",iftNumberOfSeeds(pan,orig));
  fprintf(fp,"%d;",iftNumberOfTips(pan));
  fprintf(fp,"%f;",iftMeanNumberOfBranchesPerNode(pan));
  fprintf(fp,"%f;",iftStdevNumberOfBranchesPerNode(pan));
  fprintf(fp,"%f;",iftMeanNumberOfBranchesPerNodeInSuperiorSlab(pan));
  fprintf(fp,"%f;",iftStdevNumberOfBranchesPerNodeInSuperiorSlab(pan));
  fprintf(fp,"%f;",iftMeanNumberOfBranchesPerNodeInInferiorSlab(pan));
  fprintf(fp,"%f;",iftStdevNumberOfBranchesPerNodeInInferiorSlab(pan));
  fprintf(fp,"%f;",iftMaxNumberOfBranchesPerNode(pan));
  fprintf(fp,"%f;",iftMaxNumberOfBranchesPerNodeInSuperiorSlab(pan));
  fprintf(fp,"%f\n",iftMaxNumberOfBranchesPerNodeInInferiorSlab(pan));

  iftDestroyImage(&dist);

  fclose(fp);
}

void iftWriteExtraPanicleMeasures(iftPanicle *pan, char *filename, char *basename)
{
  FILE *fp=NULL;
  char *token, lineID[40], lineID_aux[20];
  char  msg[40], basename_aux[100]; 
  int exp, panicle, rep, date, i;
  float first_segm_len;

  strcpy(basename_aux,basename);

  token = strtok(basename_aux,"_");
  sprintf(msg,"Invalid basename: %s",basename_aux);
 
  if (strcmp(token,"exp")==0){
    token = strtok(NULL,"_");  
    exp = atoi(token);    

    token = strtok(NULL,"_");  
    strcpy(lineID,token); 
    token = strtok(NULL,"_"); 
    strcpy(lineID_aux,token);
    strcat(lineID,lineID_aux);
    token = strtok(NULL,"_");  
    if (strcmp(token,"rep")!=0){
      iftError(msg,"iftWriteExtraPanicleMeasures");
    }
    token = strtok(NULL,"_");  
    rep = atoi(token);
    token = strtok(NULL,"_");  
    if (strcmp(token,"pan")!=0){
      iftError(msg,"iftWriteExtraPanicleMeasures");
    }
    token = strtok(NULL,"_");  
    panicle = atoi(token);
    token = strtok(NULL,"_");  
    if (strcmp(token,"date")!=0){
      iftError(msg,"iftWriteExtraPanicleMeasures");
    }
    token = strtok(NULL,"_");  
    //strcpy(date,token); 
    date = atoi(token);
  }else{
    iftError(msg,"iftWriteExtraPanicleMeasures");
  }

  fp = fopen(filename,"a");

  /* Write Header */

  fprintf(fp,"LineID(v%2.1f);",VERSION);
  fprintf(fp,"Experiment;");
  fprintf(fp,"Replicate;");
  fprintf(fp,"Panicle;");
  fprintf(fp,"Date;");

  fprintf(fp, "NumberOfSubsegmentsOnMainAxis;"); 

  if (pan->junction[0] > 0){ /* the first point on the main axis is
				not a junction point */

    first_segm_len = iftSegmentLength(pan->mainaxis.point,0,pan->junction[0]); 
    
    if (first_segm_len > MINFSTJCTLEN){ /* first segment is valid */
      
      fprintf(fp, "LengthOfSegment1(cm);"); 
      for (i=0; i < pan->njunctions-1; i++){ 
	fprintf(fp, "LengthOfSegment%d(cm);",i+2); 
      }
    }else{
      first_segm_len = 0.0;
      for (i=0; i < pan->njunctions-1; i++){ 
	fprintf(fp, "LengthOfSegment%d(cm);",i+1); 
      } 
    }
  }else{
    first_segm_len = 0.0;
    for (i=0; i < pan->njunctions-1; i++){ 
      fprintf(fp, "LengthOfSegment%d(cm);",i+1); 
    } 
  }

  fprintf(fp, "NumberOfPrimaryBranches;"); 
  for (i=0; i < pan->nbranches-1; i++){ 
    fprintf(fp, "LengthOfBranch%d(cm);",i+1); 
  } 
  fprintf(fp, "LengthOfBranch%d(cm)\n",i+1); 

  /* Write Data */

  fprintf(fp,"%s;",lineID);
  fprintf(fp,"%d;",exp);
  fprintf(fp,"%d;",rep);
  fprintf(fp,"%d;",panicle);
  fprintf(fp,"%08d;",date);
  if (pan->junction[0] > 0){ /* the first point on the main axis is
				not a junction point */
    
    if (first_segm_len > MINFSTJCTLEN){ /* first segment is valid */

      fprintf(fp,"%d;",pan->njunctions);
      fprintf(fp,"%f;",iftSegmentLength(pan->mainaxis.point,0,pan->junction[0])); 
      for (i=0; i < pan->njunctions-1; i++) {
	fprintf(fp,"%f;",iftSegmentLength(pan->mainaxis.point,pan->junction[i],pan->junction[i+1]));
      }
    }else{
      fprintf(fp,"%d;",pan->njunctions-1);
      for (i=0; i < pan->njunctions-1; i++) {
	fprintf(fp,"%f;",iftSegmentLength(pan->mainaxis.point,pan->junction[i],pan->junction[i+1]));
      }
    }
  }else{
    fprintf(fp,"%d;",pan->njunctions-1);
    for (i=0; i < pan->njunctions-1; i++) {
      fprintf(fp,"%f;",iftSegmentLength(pan->mainaxis.point,pan->junction[i],pan->junction[i+1]));
    }
  }
  fprintf(fp,"%d;",pan->nbranches);
  for (i=0; i < pan->nbranches-1; i++) {
    fprintf(fp,"%f;",iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1));
  }
  fprintf(fp,"%f\n",iftSegmentLength(pan->primary[i].point,0,pan->primary[i].npoints-1));

  fclose(fp);

}



int main(int argc, char *argv[]) 
{
  iftImage       *orig;
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


  if (argc!=3)
    iftError("Usage: iftPanicleMeasures <dirname> <image.jpg>","main");

  // Read image file of the panicle

  basename = strtok(argv[2],".");
  sprintf(filename,"%s/originals/%s.jpg",argv[1],basename);
  sprintf(command,"convert %s temp.pgm",filename);
  system(command);
  orig    = iftReadImageP5("temp.pgm");
  sprintf(command,"rm -f temp.pgm");
  system(command);

  

  // Read panicle information with its skeleton

  sprintf(filename,"%s/skeletons/%s.inf",argv[1],basename);
  pan = iftReadPanicleInfo(filename);
  if (pan->nbranches==0)
    iftError("First execute iftPanicleInfo","main");


  t1     = iftTic();
  
  sprintf(filename,"./PanicleMeasures.csv");
  iftWritePanicleMeasures(pan,orig,filename,basename);

  sprintf(filename,"./ExtraPanicleMeasures.csv");
  iftWriteExtraPanicleMeasures(pan,filename,basename);

  t2     = iftToc();

  fprintf(stdout,"Panicle measures were extracted in %f ms\n",iftCompTime(t1,t2));


  iftDestroyImage(&orig);  
  iftDestroyPanicleInfo(&pan);  

  /* ---------------------------------------------------------- */

  /* info = mallinfo(); */
  /* MemDinFinal = info.uordblks; */
  
  /* if (MemDinInicial!=MemDinFinal) */
  /*   printf("\n\nDynamic memory was not completely deallocated (%d, %d)\n", */
  /* 	   MemDinInicial,MemDinFinal);    */
  
  return(0);
}



