#ifndef IFT_BSpline_H_
#define IFT_BSpline_H_

#include "iftMatrix.h"
#include "iftImage.h"


iftMatrix *iftBlendingMatrix(float tension);
iftMatrix *iftColorImagePointMatrix(iftImage *img, iftVoxel *u);
iftMatrix *iftGrayImagePointMatrix(iftImage *img, iftVoxel *u);
iftMatrix *iftBSplineInterp(float u, iftMatrix *Bm, iftMatrix *Pm);





#endif
