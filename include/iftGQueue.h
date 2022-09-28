#ifndef IFT_GQUEUE_H_
#define IFT_GQUEUE_H_

#include "iftCommon.h"

#define MINVALUE   0  /* define queue to remove node with minimum value */
#define MAXVALUE   1  /* define queue to remove node with maximum value */
#define FIFOBREAK  0  /* define queue to solve ambiguity by FIFO */
#define LIFOBREAK  1  /* define queue to solve ambiguity by LIFO */
#define QSIZE      65536

#define iftSetTieBreak(a,b) a->C.tiebreak=b 
#define iftSetRemovalPolicy(a,b) a->C.removal_policy=b 

typedef struct ift_gqnode { 
  int  next;  /* next node */
  int  prev;  /* prev node */
  char color; /* WHITE=0, GRAY=1, BLACK=2 */ 
} iftGQNode;

typedef struct _gdoublylinkedlists {
  iftGQNode *elem;  /* all possible doubly-linked lists of the circular queue */
  int nelems;  /* total number of elements */
  int *value;   /* the value of the nodes in the graph */
} iftGDoublyLinkedLists; 

typedef struct ift_gcircularqueue { 
  int  *first;   /* list of the first elements of each doubly-linked list */
  int  *last;    /* list of the last  elements of each doubly-linked list  */
  int  nbuckets; /* number of buckets in the circular queue */
  int  minvalue;  /* minimum value of a node in queue */
  int  maxvalue;  /* maximum value of a node in queue */
  char tiebreak; /* 1 is LIFO, 0 is FIFO (default) */
  char removal_policy; /* 0 is MINVALUE and 1 is MAXVALUE */
} iftGCircularQueue;

typedef struct ift_gqueue { /* Priority queue by Dial implemented as
                           proposed by A. Falcao */
  iftGCircularQueue C;
  iftGDoublyLinkedLists L;
} iftGQueue;

iftGQueue *iftCreateGQueue(int nbuckets, int nelems, int *value);
void   iftDestroyGQueue(iftGQueue **Q);
void   iftResetGQueue(iftGQueue *Q);
int    iftEmptyGQueue(iftGQueue *Q);
void   iftInsertGQueue(iftGQueue **Q, int elem);
int    iftRemoveGQueue(iftGQueue *Q);
void   iftRemoveGQueueElem(iftGQueue *Q, int elem);
void   iftUpdateGQueue(iftGQueue **Q, int elem, int newvalue);
iftGQueue *iftGrowGQueue(iftGQueue **Q, int nbuckets);

#endif







