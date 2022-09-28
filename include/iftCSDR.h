#ifndef IFT_CSDR_H_
#define IFT_CSDR_H_

#include "iftCommon.h"
#include "iftDataSet.h"
#include "iftClassification.h"
#include "iftSort.h"
#include "iftMatrix.h"


/* Class-specific data representation (CSDR) based on partial least
   squares (PLS) */

typedef struct ift_CSDRPLS {
  iftMatrix  **w; // Projection matrix w[c] for each class c with 
                  // "nclasses" matrices, each with "nfeats" lines
                  // and "num_of_comps" columns.
  int    nclasses, nfeats, num_of_comps;
} iftCSDRPLS;

typedef struct ift_opfbank {
  iftCplGraph **graph; // a bank of trained classifiers, one per class
  int           nclassifiers;
} iftOPFBank;
  

iftOPFBank  *iftCreateOPFBank(int nclassifiers);
void         iftDestroyOPFBank(iftOPFBank **opfbank);

iftOPFBank  *iftTrainOPFBankBySupPCA(iftDataSet *Z, int num_of_comps);
void         iftClassifyByOPFBank(iftOPFBank *opfbank, iftDataSet *Z);

iftCSDRPLS  *iftCreateCSDRPLS(iftDataSet *Z, int num_of_comps);
void         iftDestroyCSDRPLS(iftCSDRPLS **csdr);
iftDataSet  *iftApplyCSDRPLS(iftDataSet *Z, iftMatrix *X, iftCSDRPLS *csdr, int truelabel);
iftOPFBank  *iftTrainOPFBankWithCSDRPLS(iftDataSet *Z, iftCSDRPLS *csdr);
void         iftClassifyByOPFBankWithCSDRPLS(iftOPFBank *opfbank, iftCSDRPLS *csdr, iftDataSet *Z);



#endif
