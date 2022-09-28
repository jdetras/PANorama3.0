#ifndef IFT_KERNEL_H_
#define IFT_KERNEL_H_

#include "iftCommon.h"
#include "iftAdjacency.h"
#include "iftMatrix.h"

/* Multiband kernel: iftKernel *K; K->A->n; K->A->dx[i],
   K->A->dy[i],K->A->dz[i]; K->weight[i]  */

typedef struct ift_kernel {
  iftAdjRel *A;
  float     *weight;
} iftKernel;

/* Multiband kernel: iftMKernel *K; K->A->n; K->A->dx[i],
   K->A->dy[i],K->A->dz[i]; K->weight[b].val[i]  */

typedef struct ift_mkernel {
  iftAdjRel *A;
  iftBand   *weight;
  int        nbands;
} iftMKernel;

/* Kernel bank of multiband kernels: iftMMKernel *K; K->A->n;
   K->A->dx[i], K->A->dy[i],K->A->dz[i]; K->nkernels; K->nbands;
   K->weight[k][b].val[i]  */

typedef struct ift_mmkernel {
  iftAdjRel *A;
  iftBand  **weight; /* one kernel per row and one band per column */
  float	    *bias; /* bias provided from SVM -> one bias per Kernel Multiband */
  int        nkernels;
  int        nbands;
  iftMatrix *W; /* Whitening transform */
  float     *mean; /* for centralization before whitening (size is W->ncols) */ 
} iftMMKernel;

iftKernel   *iftCreateKernel(iftAdjRel *A);
void         iftDestroyKernel(iftKernel **K);
iftKernel   *iftReadKernel(char *filename);
void         iftWriteKernel(iftKernel *K, char *filename); 
iftMKernel  *iftCreateMKernel(iftAdjRel *A, int nbands);
void         iftDestroyMKernel(iftMKernel **K);
iftMKernel  *iftRandomMKernel(iftAdjRel *A, int nbands);
iftMMKernel *iftCreateMMKernel(iftAdjRel *A, int nbands, int nkernels);
iftMMKernel *iftReadMMKernelFILE(FILE* fp);
void         iftWriteMMKernelFILE(iftMMKernel *k_bank,FILE* p);
void         iftWriteMMKernel(iftMMKernel *k_bank, char *filename);
iftMMKernel *iftCopyMMKernel(iftMMKernel *k);
iftMMKernel* iftUnionMMKernel(iftMMKernel *k1, iftMMKernel *k2);
iftMMKernel *iftRandomMMKernel(iftAdjRel *A, int nbands, int nkernels);
iftMMKernel* iftV1likeMMKernel2D(int sizex,int sizey, int nbands, int norients, int nfreqs, float *freqs);
iftMMKernel *iftRandomZMMMKernel(iftAdjRel *A, int nbands, int nkernels);
iftMMKernel *iftOnesMMKernel(iftAdjRel *A, int nbands, int nkernels);
void         iftDestroyMMKernel(iftMMKernel **k_bank);
iftKernel   *iftGaussianKernel(float radius, float stdev);
iftKernel   *iftGaussianKernel2D(float radius, float stdev);
iftKernel   *iftSobelXKernel(void);
iftKernel   *iftSobelYKernel(void);
iftKernel   *iftSobelZKernel(void);
iftKernel   *iftSobelXKernel2D(void);
iftKernel   *iftSobelYKernel2D(void);
iftKernel   *iftDoGKernel(float stdev1, float stdev2);
iftKernel   *iftDoGKernel2D(float stdev1, float stdev2);
iftKernel   *iftGabor2D(float gw,float gh,float gx0,float gy0,float wfreq,float worient,float wphase,iftAdjRel* A);

#endif
