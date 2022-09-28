#ifndef IFT_IMAGEMATH_H_
#define IFT_IMAGEMATH_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftFImage.h"

iftImage  *iftAdd(iftImage *img1, iftImage *img2);
iftImage  *iftSub(iftImage *img1, iftImage *img2);
iftImage  *iftAnd(iftImage *img1, iftImage *img2);
iftImage  *iftOr(iftImage *img1, iftImage *img2);
iftImage  *iftMult(iftImage *img1, iftImage *img2);
iftImage  *iftAbs(iftImage *img);
iftImage  *iftComplement(iftImage *img);
iftImage  *iftMask(iftImage *img, iftImage *bin);
iftImage  *iftAddValue(iftImage *img, int value);
iftFImage *iftSQRT(iftImage *img1);

#endif
