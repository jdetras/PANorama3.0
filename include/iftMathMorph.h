#ifndef IFT_MATHMORPH_H_
#define IFT_MATHMORPH_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftImageMath.h"
#include "iftSeeds.h"
#include "iftGQueue.h"
#include "iftAdjacency.h"
#include "iftFiltering.h"
#include "iftCompTree.h"

/* Morphological operations with non-planar structuring elements */

iftImage *iftDilateWithKernel(iftImage *img, iftKernel *K);
iftImage *iftErodeWithKernel(iftImage *img, iftKernel *K);
iftImage *iftCloseWithKernel(iftImage *img, iftKernel *K);
iftImage *iftOpenWithKernel(iftImage *img, iftKernel *K);

/* Morphological operations with planar structuring elements */

iftImage *iftDilate(iftImage *img, iftAdjRel *A);
iftImage *iftErode(iftImage *img, iftAdjRel *A);
iftImage *iftClose(iftImage *img, iftAdjRel *A);
iftImage *iftOpen(iftImage *img, iftAdjRel *A);
iftImage *iftMorphGrad(iftImage *img, iftAdjRel *A);

iftImage *iftSupRec(iftImage *img, iftImage *marker);
iftImage *iftInfRec(iftImage *img, iftImage *marker);
iftImage *iftOpenRec(iftImage *img, iftAdjRel *A);
iftImage *iftCloseRec(iftImage *img, iftAdjRel *A);
iftImage *iftAsfCORec(iftImage *img, iftAdjRel *A);
iftImage *iftAsfOCRec(iftImage *img, iftAdjRel *A);
iftImage *iftCloseBasins(iftImage *img);
iftImage *iftOpenDomes(iftImage *img);

iftImage *iftAreaClose(iftImage *img, int area_thres);
iftImage *iftAreaOpen(iftImage *img, int area_thres);
iftImage *iftFastAreaClose(iftImage *img, int thres);
iftImage *iftFastAreaOpen(iftImage *img, int thres);
iftImage *iftVolumeOpen(iftImage *img, int volume_thres);
iftImage *iftVolumeClose(iftImage *img, int volume_thres);

iftImage *iftDilateInRegion(iftImage *img, iftImage *mask, iftAdjRel *A);
iftImage *iftErodeInRegion(iftImage *img, iftImage *mask, iftAdjRel *A);
iftImage *iftCloseInRegion(iftImage *img, iftImage *mask, iftAdjRel *A);
iftImage *iftOpenInRegion(iftImage *img, iftImage *mask, iftAdjRel *A);
iftImage *iftCloseRecInRegion(iftImage *img, iftImage *mask, iftAdjRel *A);
iftImage *iftOpenRecInRegion(iftImage *img, iftImage *mask, iftAdjRel *A);
iftImage *iftCloseBasinsInRegion(iftImage *img, iftImage *mask);
iftImage *iftOpenDomesInRegion(iftImage *img, iftImage *mask);

iftImage *iftDilateBin(iftImage *bin, iftSet **seed, float radius);
iftImage *iftErodeBin(iftImage *bin, iftSet **seed, float radius);
iftImage *iftCloseBin(iftImage *bin, float radius);
iftImage *iftOpenBin(iftImage *bin, float radius);

iftImage *iftHBasins(iftImage *img, int H);
iftImage *iftHDomes(iftImage *img, int H);

#endif
