#ifndef IFT_REPRESENTATION_H_
#define IFT_REPRESENTATION_H_


#include "iftCommon.h"
#include "iftAdjacency.h"
#include "iftGQueue.h"
#include "iftImage.h"
#include "iftRadiometric.h"
#include "iftHistogram.h"
#include "iftFImage.h"
#include "iftSeeds.h"


iftImage       *iftDistTrans(iftImage *bin, iftAdjRel *A, char side);
iftImage       *iftShellDistTrans(iftImage *bin, iftAdjRel *A, char side, float max_dist);
void            iftDistTransRootMap(iftImage *bin, iftAdjRel *A, char side, iftImage **dist, iftImage **root);
iftFImage      *iftSignedDistTrans(iftImage *bin, iftAdjRel *A);
iftFImage      *iftShellSignedDistTrans(iftImage *bin, iftAdjRel *A,float max_dist);
iftFImage      *iftMSSkel(iftImage *bin);
iftImage       *iftSurfaceSkelByAngle(iftImage *bin);
iftFImage      *iftMSSkel2D(iftImage *bin, iftAdjRel *A, char side);
iftImage       *iftIntMSSkel2D(iftImage *bin, iftAdjRel *A, char side);
void            iftLabelRootPropagation(iftImage *bin, iftAdjRel *A, iftImage **cost, iftImage **root, iftImage **label, iftImage **pred); 
void            iftLabelRootPropagation2D(iftImage *bin, iftAdjRel *A, char side, iftImage **root, iftImage **label); 
iftImage       *iftRootPropagation(iftImage *bin, iftAdjRel *A, char side, float max_dist);
iftFImage      *iftLabelContPixelByGeoLen(iftImage *bin);
iftImage       *iftLiftImage(iftImage *img);
iftImage       *iftDropImage(iftImage *bin);
iftFImage      *iftIntegralImage(iftImage *img);
float           iftGetIntegralValueInRegion(iftFImage *integ, iftVoxel *v, int npts);
iftImage       *iftMarkGeometricCenters(iftImage *bin);
iftImage       *iftComponentSizes(iftImage *bin, iftAdjRel *A);
iftImage       *iftSurfaceArea(iftImage *bin);
iftImage       *iftPerimeter(iftImage *bin);


#endif
