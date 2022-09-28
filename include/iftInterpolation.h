#ifndef IFT_INTERPOLATION_H_
#define IFT_INTERPOLATION_H_

#include "iftCommon.h"
#include "iftColor.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftMImage.h"
#include "iftDataSet.h"
#include "iftAdjacency.h"
#include "iftPlane.h"
#include "iftMatrix.h"
#include "iftSegmentation.h"
#include "iftRepresentation.h"
#include "iftMSPS.h"

iftPlane *iftFindBestCutPlane(iftImage *weight, iftPoint pos, int xviewsize, int yviewsize);
iftPlane *iftFindBestObjectCutPlane(iftImage *obj, iftImage *weight);
iftImage  *iftResliceImage(iftImage *img, iftPlane *pl, int xsize, int ysize, int zsize);
iftImage  *iftGetSlice(iftImage *img, iftPlane *pl, int xsize, int ysize);
iftImage  *iftInterp2D(iftImage *img, float sx, float sy);
iftImage  *iftInterp(iftImage *img, float sx, float sy, float sz);
iftFImage  *iftFGetSlice(iftFImage *img, iftPlane *pl, int xsize, int ysize);
iftFImage *iftFInterp2D(iftFImage *img, float sx, float sy);
iftMImage *iftMInterp2D(iftMImage *mimg, float sx, float sy);
iftMImage *iftMInterp(iftMImage *img, float sx, float sy, float sz);
iftFImage *iftFInterp(iftFImage *img, float sx, float sy, float sz);
iftImage  *iftShapeBasedInterp2D(iftImage *label, float sx, float sy);
iftImage  *iftShapeBasedInterp(iftImage *label, float sx, float sy, float sz);
iftImage  *iftResliceOnPrincipalAxis(iftImage *img, iftImage *bin);


#endif

