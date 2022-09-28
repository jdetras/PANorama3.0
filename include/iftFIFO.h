#ifndef IFT_FIFO_H_
#define IFT_FIFO_H_

#include "iftCommon.h"

typedef struct ift_fifo {
  int  *FIFO;
  int   first,last;
  char *color;
  int   n;
} iftFIFO;

iftFIFO *iftCreateFIFO(int n);
void     iftDestroyFIFO(iftFIFO **F);
char     iftInsertFIFO(iftFIFO *F, int node);
int      iftRemoveFIFO(iftFIFO *F);
char     iftFullFIFO(iftFIFO *F);
char     iftEmptyFIFO(iftFIFO *F);
void     iftResetFIFO(iftFIFO *F);

#endif
