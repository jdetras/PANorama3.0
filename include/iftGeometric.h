#ifndef IFT_GEOMETRIC_H_
#define IFT_GEOMETRIC_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftMatrix.h"
#include "iftInterpolation.h"
#include "iftDataSet.h"

iftImage  *iftRotateImage2D(iftImage *img, float theta);
iftImage  *iftScaleImage2D(iftImage *img, float sx, float sy);
iftImage  *iftRotateImage(iftImage *img, float theta_x, float theta_y);
iftImage  *iftScaleImage(iftImage *img, float sx, float sy, float sz);
iftImage  *iftTransformImage(iftImage *img, iftMatrix *M);
iftImage  *iftFlipImage(iftImage *img, char axis); /* AXIS_X, AXIS_Y, or AXIS_Z */
iftImage  *iftTransformImageClipping(iftImage *img, iftMatrix *InvE, int xsize, int ysize, int zsize);
iftFImage *iftTransformFImageClipping(iftFImage *img, iftMatrix *InvE, int xsize, int ysize, int zsize);
iftImage  *iftTransformImageClip(iftImage *img, iftMatrix *M);

#endif
