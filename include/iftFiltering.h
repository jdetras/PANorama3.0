#ifndef IFT_FILTERING_H_
#define IFT_FILTERING_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftMImage.h"
#include "iftAdjacency.h"
#include "iftSort.h"
#include "iftMatrix.h"
#include "iftKernel.h"

iftImage  *iftLinearFilter(iftImage *img, iftKernel *K);
iftImage  *iftLinearFilterInRegion(iftImage *img, iftImage *mask, iftKernel *K);
iftImage  *iftFastLinearFilter(iftImage *img, iftKernel *K, char crop); 
iftImage  *iftCroppedFastLinearFilterByMatrixMult(iftImage *img, iftKernel *K);
iftFImage *iftFLinearFilter(iftFImage *img, iftKernel *K);
iftFImage  *iftFLinearFilterInRegion(iftFImage *img, iftImage *mask, iftKernel *K);
iftFImage *iftFastFLinearFilter(iftFImage *img, iftKernel *K, char crop);
iftMImage *iftMLinearFilter(iftMImage *img, iftMKernel *K);
iftMImage *iftMMLinearFilter(iftMImage *img, iftMMKernel *k_bank);
iftImage  *iftMedianFilter(iftImage *img, iftAdjRel *A);
iftImage  *iftModaFilter(iftImage *img, iftAdjRel *A);
iftImage  *iftSobelGradientMagnitude(iftImage *img);
iftMatrix *iftImageToMatrix(iftImage *img, iftFastAdjRel *F, char crop);
iftImage  *iftMatrixToImage(iftMatrix *M, int xsize, int ysize, int zsize);
iftMatrix *iftFImageToMatrix(iftFImage *img, iftFastAdjRel *F, char crop);
iftFImage  *iftMatrixToFImage(iftMatrix *M, int xsize, int ysize, int zsize);
iftMatrix *iftMImageToMatrix(iftMImage *img, iftFastAdjRel *F);
iftMImage *iftMatrixToMImage(iftMatrix *M, int xsize, int ysize, int zsize, int nbands);
iftMatrix *iftKernelToMatrix(iftKernel *K);
iftMatrix *iftMKernelToMatrix(iftMKernel *K);
iftMatrix *iftMMKernelToMatrix(iftMMKernel *k_bank);
iftImage  *iftSmoothImage(iftImage *img, iftAdjRel *A, float sigma);
iftImage  *iftSmoothImageInRegion(iftImage *img, iftImage *mask, iftAdjRel *A, float sigma);


#endif
