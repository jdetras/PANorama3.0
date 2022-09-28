#ifndef IFT_SORT_H_
#define IFT_SORT_H_

#include "iftCommon.h"
#include "iftGQueue.h"
#include "iftFHeap.h"

/* Suppose you have a list of objects, each with a given value. These
   methods can sort the indices of the objects in that list in the
   increasing (decreasing) order of values. They will return in index,
   the sorted indices, and in value, the sorted values. Note that
   value[i] will no longer correspond to value[index[i]] after
   sorting. The objects can be accessed in order by index[i] (e.g.,
   object[index[i]]), but their values must remain the same (e.g.,
   object[index[i]].value). The sorted list of values (i.e.,
   value[i]=object[index[i]].value), on the other hand, can be used
   when we want to sort values only, with no underlying object
   concept. In such a case, index[i] has no meaning. See examples in
   demo/iftSortObjects.c. 

*/

void iftBucketSort(int *value, int *index, int nelems, uchar order);
void iftFHeapSort(float *value, int *index, int nelems, uchar order);
void iftQuickSort( int *value, int *index, int i0, int i1, uchar order ); 
void iftSQuickSort( char **value, int *index, int i0, int i1, uchar order, int size ); 
void iftFQuickSort( float *value, int *index, int i0, int i1, uchar order); 

#endif
