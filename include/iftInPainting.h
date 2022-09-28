#ifndef IFT_INPAINTING_H_
#define IFT_INPAINTING_H_

#include "iftCommon.h"
#include "iftAdjacency.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftImageMath.h"
#include "iftMathMorph.h"
#include "iftSet.h"

/* Method for in-painting implemented as proposed in

A. Criminisi, P. Perez and K. Toyama, Region Filling and Object
Removal by Exemplar-Based Image Inpainting, \emph{IEEE TRANSACTIONS ON
IMAGE PROCESSING}, VOL. 13, NO. 9, SEP 2004.

*/



iftImage  *iftInPainting(iftImage *img, iftImage *mask, float adj_radius);

iftImage *iftInPaintBoundary(iftImage *img, iftImage *mask);

#endif
