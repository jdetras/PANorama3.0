#ifndef IFT_SVM_H_
#define IFT_SVM_H_

#include "iftDataSet.h"
#include "../libsvm/svm.h"

typedef struct svm_node      svmNode;
typedef struct svm_problem   svmProblem;
typedef struct svm_parameter svmParameter;
typedef struct svm_model     svmModel;

typedef struct ift_svm {
  svmParameter  *params;    // libsvm structure for svm parameters
  svmProblem    *problem;
  svmModel     **model;     // libsvm structure to represent svm models
  double        *truelabel;
  int            nmodels;
} iftSVM;


iftSVM *iftCreateLinearSVC(double C);
iftSVM *iftCreateRBFSVC(double C, double sigma);
iftSVM *iftCreatePreCompSVC(double C);
void    iftDestroySVM(iftSVM *svm);

void    iftSVMTrainOVO(iftSVM *svm, iftDataSet *Z);
int     iftSVMClassifyOVO(iftSVM *svm, iftDataSet *Z, uchar predStatus);
void 	iftClassSVMTrainOVA(iftSVM *svm, iftDataSet *Z, int classtrain, int idxtrain);
void iftClassSVMTrainOVAParallel(iftSVM *svm, iftDataSet *Z, int classtrain, int idxtrain);
void    iftSVMTrainOVA(iftSVM *svm, iftDataSet *Z);
float *iftClassSVMLinearClassifyOVA(iftSVM* svm, int idxmodel, iftDataSet *Z, iftDataSet* Ztrain, uchar sampleStatus);
float *iftClassSVMLinearClassifyOVAParallel(iftSVM* svm, int idxmodel, iftDataSet *Ztest, iftDataSet* Ztrain, uchar sampleStatus);
void    iftSVMLinearClassifyOVAParallel(iftSVM* svm, iftDataSet *Z, iftDataSet* Ztrain, uchar sampleStatus);
int     iftSVMClassifyOVA(iftSVM *svm, iftDataSet *Z, uchar predStatus);
int     iftSVMLinearClassifyOVA(iftSVM *svm, iftDataSet *Z,iftDataSet* Ztrain,
                                uchar predStatus);
float **iftSVMLinearClassifyOVA2(iftSVM* svm, iftDataSet *Ztest,iftDataSet* Ztrain,
								   uchar sampleStatus);

iftSample iftSVMGetNormalHyperplane(iftSVM* svm,int idxModel,
                                    iftDataSet* Ztrain, float* rho);

iftDataSet *iftKernelizeDataSet(iftDataSet *Z,
                                int kFunction,
                                uchar traceNormalize);

iftDataSet *iftKernelizeDataSet2(iftDataSet *Zref,
                                 iftDataSet *Zin,
                                 int kFunction,
                                 uchar traceNormalize,
                                 float *ktrace);

int *iftExtractSupportVectorIndices(iftSVM *svm, int idxModel, int *n);
int **iftExtractAllSupportVectorIndices(iftSVM *svm, int **n);

#endif
