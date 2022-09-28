#ifndef IFT_BMAP_H_
#define IFT_BMAP_H_

#include "iftCommon.h"

#define   iftBMapValue(b,n) ((b->val[n>>3]&(1<<(n&0x07)))!=0)
#define   iftBMapSet0(b,n) b->val[n>>3]&=((~0)^(1<<(n&0x07)));
#define   iftBMapSet1(b,n) b->val[n>>3]|=(1<<(n&0x07));
#define   iftBMapToggle(b,n) b->val[n>>3]^=(1<<(n&0x07));

typedef struct ift_bitmap {
  char *val;
  int   n, nbytes;
} iftBMap;

iftBMap  *iftCreateBMap(int n);
void      iftDestroyBMap(iftBMap **b);
void      iftFillBMap(iftBMap *b, int value);
iftBMap  *iftCopyBMap(iftBMap *src);

#endif
