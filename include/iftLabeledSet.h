#ifndef IFT_LABELEDSET_H_
#define IFT_LABELEDSET_H_

#include "iftCommon.h"

typedef struct ift_labeledset {
  int elem;
  int label;
  struct ift_labeledset *next;
} iftLabeledSet;

void iftInsertLabeledSet(iftLabeledSet **S, int elem, int label);
int  iftRemoveLabeledSet(iftLabeledSet **S, int *label);
void iftRemoveLabeledSetElem(iftLabeledSet **S, int elem);
void iftDestroyLabeledSet(iftLabeledSet **S);

iftLabeledSet* iftCopyLabeledSet(iftLabeledSet *s);
iftLabeledSet* iftCopyOrderedLabeledSet(iftLabeledSet *s);

void iftConcatLabeledSet(iftLabeledSet **S1,iftLabeledSet **S2);
void iftRemoveSubsetLabeledSet(iftLabeledSet **S1,iftLabeledSet **S2);

int iftLabeledSetSize(iftLabeledSet *s);

#endif

