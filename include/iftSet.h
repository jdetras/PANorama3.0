#ifndef IFT_SET_H_
#define IFT_SET_H_

#include "iftCommon.h"

typedef struct ift_set {
  int elem;
  struct ift_set *next;
} iftSet;

void    iftInsertSet(iftSet **S, int elem);
int     iftRemoveSet(iftSet **S);
void    iftRemoveSetElem(iftSet **S, int elem);
void    iftDestroySet(iftSet **S);
iftSet* iftSetUnion(iftSet *S1,iftSet *S2);
iftSet* iftSetConcat(iftSet *S1,iftSet *S2);
char    iftUnionSetElem(iftSet **S, int elem);
void    iftInvertSet(iftSet **S);
int 	iftSetSize(iftSet* S);
iftSet* iftSetCopy(iftSet* S);
int     iftSetHasElement(iftSet *S, int elem);

#endif

