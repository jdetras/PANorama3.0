#ifndef IFT_FHEAP_H_
#define IFT_FHEAP_H_

#include "iftCommon.h"

#define MINVALUE   0 /* define heap to remove node with minimum value */
#define MAXVALUE   1 /* define heap to remove node with maximum value */

#define iftDad(i) ((i - 1) / 2)
#define iftLeftSon(i) (2 * i + 1)
#define iftRightSon(i) (2 * i + 2)
#define iftSetRemovalPolicyFHeap(a,b) a->removal_policy = b 

typedef struct ift_fheap {
  float *value;
  char  *color;
  int   *node;
  int   *pos;
  int    last;
  int    n;
  char removal_policy; /* 0 is MINVALUE and 1 is MAXVALUE */
} iftFHeap;

iftFHeap *iftCreateFHeap(int n, float *value);
void   iftDestroyFHeap(iftFHeap **H);
char   iftFullFHeap(iftFHeap *H);
char   iftEmptyFHeap(iftFHeap *H);
char   iftInsertFHeap(iftFHeap *H, int pixel);
int    iftRemoveFHeap(iftFHeap *H);
void   iftRemoveFHeapElem(iftFHeap *H, int pixel);
void   iftGoUpFHeap(iftFHeap *H, int i);
void   iftGoDownFHeap(iftFHeap *H, int i);
void   iftResetFHeap(iftFHeap *H);

#endif



