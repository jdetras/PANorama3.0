#ifndef IFT_SEEDS_H_
#define IFT_SEEDS_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftAdjacency.h"
#include "iftSet.h"
#include "iftLabeledSet.h"
#include "iftGQueue.h"
#include "iftLIFO.h"
#include "iftFIFO.h"
#include "iftBMap.h"
#include "iftMathMorph.h"
#include "iftDataSet.h"
#include "iftRegion.h"
#include "iftMImage.h"

iftLabeledSet *iftReadSeeds(char *filename, iftImage *img);
iftLabeledSet *iftReadSeeds2D(char *filename, iftImage *img);
iftLabeledSet *iftLabelObjBorderSet(iftImage *bin, iftAdjRel *A);
iftLabeledSet *iftImageBorderLabeledSet(iftImage *img);
iftLabeledSet *iftLabelCompSet(iftImage *bin, iftAdjRel *A);
iftLabeledSet *iftFuzzyModelToLabeledSet(iftImage *model);
iftLabeledSet *iftMAdjustSeedCoordinates(iftLabeledSet *S, iftMImage *input, iftMImage *output);
iftLabeledSet *iftAdjustSeedCoordinates(iftLabeledSet *Sin, iftImage *orig, iftMImage *output);

iftSet        *iftImageBorderSet(iftImage *img);
iftLabeledSet *iftMultiObjectBorderLabeledSet(iftImage *img, iftAdjRel *A);
iftSet        *iftObjectBorderSet(iftImage *bin, iftAdjRel *A);
iftSet        *iftEndPoints(iftImage *skel, iftAdjRel *A);
iftSet        *iftFindPathOnSkeleton(iftImage *skel, iftAdjRel *A, int src, int dst);
iftSet        *iftSkeletonPoints(iftImage *skel);
iftImage      *iftObjectBorders(iftImage *bin, iftAdjRel *A);
iftImage      *iftLabelObjBorders(iftImage *bin, iftAdjRel *A);
iftImage      *iftLabelComp(iftImage *bin, iftAdjRel *A);
iftImage      *iftSelectLargestComp(iftImage *bin, iftAdjRel *A);
iftImage      *iftSelectSmallestComp(iftImage *bin, iftAdjRel *A);
iftImage      *iftSelectKLargestComp(iftImage *bin, iftAdjRel *A, int K);
iftImage      *iftComponentArea(iftImage *bin, iftAdjRel *A);
iftImage      *iftSelectCompAboveArea(iftImage *bin, iftAdjRel *A, int thres);
iftImage      *iftSelectCompBelowArea(iftImage *bin, iftAdjRel *A, int thres);
iftImage      *iftSelectCompInAreaInterval(iftImage *bin, iftAdjRel *A, int thres_min, int thres_max);
iftImage      *iftSelectLargestRegion(iftImage *label);
iftImage      *iftSelectSmallestRegion(iftImage *label);
iftImage      *iftLabelContPixel(iftImage *bin);
char           iftValidContPoint(iftImage *bin, iftAdjRel *A, iftAdjRel *L, iftAdjRel *R, int p);

void iftWriteSeeds2D(char* filename, iftLabeledSet* seed, iftImage* image);
void iftWriteSeeds(char* filename, iftLabeledSet* seed, iftImage* image);

iftImage	  *iftSeedImageFromLabeledSet(iftLabeledSet* labeled_set, iftImage *image);

iftLabeledSet *iftLabeledSetFromSeedImage(iftImage* seed_image);

iftLabeledSet *iftBorderMarkersForPixelSegmentation(iftImage *grad_image, iftImage *gt_image, float border_distance);
iftLabeledSet *iftGeodesicMarkersForSegmentation(iftImage *gt_image, iftImage *classification_image);
iftLabeledSet* iftBorderMarkersForSuperpixelSegmentation(iftImage* label_image,iftImage* gt_image, iftDataSet* dataset);

//Pops the first "nelem" elements from "lset" with label "label"
iftLabeledSet* iftGetSeeds(iftLabeledSet* S, int nelem, int label);
iftLabeledSet* iftGetMisclassifiedSeeds(iftLabeledSet* S, int nelem, int label, iftImage* gt_image, iftImage* cl_image);

// Binary Segmentation only
int iftCheckNewSeeds(int *nelem, int length);
//int iftMarkersFromMisclassifiedSeeds(iftImage* seed_image, iftLabeledSet* all_seeds, iftBMap* used_seeds, int nseeds,iftImage* gt_image, iftImage* cl_image, int dist_border, int max_marker_radius, int min_marker_radius);
int iftMarkersFromMisclassifiedSeeds(iftImage* seed_image, iftLabeledSet* all_seeds, iftBMap* used_seeds, int nseeds, int number_of_objects, iftImage* gt_image, iftImage* cl_image, int dist_border, int max_marker_radius, int min_marker_radius);

void iftWriteSeedsOnImage(iftImage* image, iftLabeledSet* seed);

int iftRootVoxel(iftImage *pred, int p);

iftImage  *iftFastLabelComp(iftImage *bin, iftAdjRel *A);

#endif
