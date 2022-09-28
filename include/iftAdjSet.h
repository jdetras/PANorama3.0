#ifndef IFT_ADJSET_H_
#define IFT_ADJSET_H_

#include "iftCommon.h"

typedef struct ift_adjset {
  int   node;
  float arcw;
  struct ift_adjset *next;
} iftAdjSet;

void iftInsertAdjSet(iftAdjSet **S, int node, float arcw);
int  iftRemoveAdjSet(iftAdjSet **S, float *arcw);
void iftRemoveAdjSetNode(iftAdjSet **S, int node);
void iftDestroyAdjSet(iftAdjSet **S);

#endif

