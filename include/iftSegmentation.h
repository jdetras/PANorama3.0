#ifndef IFT_SEGMENTATION_H_
#define IFT_SEGMENTATION_H_

#include "iftCommon.h"
#include "iftAdjacency.h"
#include "iftFiltering.h"
#include "iftGQueue.h"
#include "iftImage.h"
#include "iftMImage.h"
#include "iftImageForest.h"
#include "iftFImage.h"
#include "iftSeeds.h"
#include "iftBMap.h"
#include "iftFIFO.h"
#include "iftRadiometric.h"
#include "iftDataSet.h"
#include "iftClustering.h"
#include "iftClassification.h"
#include "iftImageMath.h"
#include "iftMathMorph.h"
#include "iftRepresentation.h"
#include "iftMetrics.h"

/* Pre-processing for image segmentation by watershed transforms */


iftImage      *iftEnhanceObject(iftImage *img, iftLabeledSet *seed, int obj);
iftImage      *iftEnhanceEdges(iftImage *img, iftAdjRel *A, iftLabeledSet *seed, float alpha);
iftImage      *iftMEnhanceObject(iftMImage *img, iftLabeledSet *seed, int obj);
iftImage      *iftMEnhanceEdges(iftMImage *img, iftAdjRel *A, iftLabeledSet *seed, float alpha);

/* Watershed transforms from labeled markers */

iftImage  *iftImageDomes(iftImage  *img, iftAdjRel *A);
iftImage  *iftImageBasins(iftImage *img, iftAdjRel *A);
iftImage  *iftWatershed(iftImage *img, iftAdjRel *A, iftLabeledSet *seed);
void       iftDiffWatershedTree(iftImageForest *fst, iftLabeledSet *seed);
void       iftDiffWatershedComp(iftImageForest *fst, iftLabeledSet *seed);
void       iftIncWatershed(iftImageForest *fst, iftLabeledSet *seed);
iftImage  *iftRelaxedWatershed(iftImage *basins, iftAdjRel *A, iftLabeledSet *seed, int num_smooth_iterations, float smooth_factor);
void 	   iftDiffRelaxedWatershed(iftImageForest *fst, iftLabeledSet *seed, int num_smooth_iterations, float smooth_factor);
iftImage  *iftEnhanceWhenCrIsGreaterThanCb(iftImage *img);
iftImage  *iftWatershedOnPixelDist(iftDataSet *dataset, iftAdjRel *A,iftLabeledSet *seed);

/* Fuzzy Connectdness */
iftImage* iftRelaxBoundaries(iftImageForest *fst, iftSet **Frontier, iftBMap *inFrontier, iftFImage *border_weight, iftFImage *norm_factor, iftFImage *prev_weight, iftFImage *next_weight, iftSet **dilated_boundary, int num_smooth_iterations);
void 	  iftFixConnectivity (iftImageForest *fst, iftImage *filtered_labels, iftSet *dilated_boundary);
iftBMap   *iftDiffFuzzyConnectedness(iftImageForest *fst, iftLabeledSet *seed);
void 	  iftDiffRelaxedFuzzyConnectedness(iftImageForest *fst, iftLabeledSet *seed, iftFImage *border_weight, iftFImage *norm_factor, iftFImage *prev_weight, iftFImage *next_weight, int num_smooth_iterations);
iftImage  *iftIRFC(iftImage *basins, iftAdjRel *A, iftLabeledSet *seed);
iftFImage *iftWeightNormFactor(iftFImage *weight, iftAdjRel *A);

/* Watershed transforms from grayscale markers: Note that, in order to
   obtain a connected operator for some graph topology, the adjacency
   relation must be the same used to compute the input image.  */

iftImage  *iftWaterGray(iftImage *basins, iftImage *marker, iftAdjRel *A);
void       iftWaterGrayForest(iftImageForest *fst, iftImage *marker);
iftImage  *iftWaterDist(iftImage *dist, iftImage *label, int H, iftAdjRel *A);
iftImage  *iftDualWaterGray(iftImage *domes, iftImage *marker, iftAdjRel *A);

/* Compute kappa-connected segmentation for multiple objects with seed
   competition. The image of markers indicate labels 0 for background,
   1,2,..., for objects and NIL for available voxels. */

iftImage *iftKappaConnectedSegm(iftImage *basins, iftAdjRel *A, iftImage *markers, int area_thres);

iftImage *iftOrientedWatershed(iftImage *img, iftImage *basins, iftAdjRel *A, iftLabeledSet *seed, char orientation);

/* Thresholding */

int        iftCumHistogramThres(iftImage *img, float perc);
int        iftOtsu(iftImage *img);
int        iftOtsuInRegion(iftImage *img, iftImage *mask);

iftImage  *iftThreshold(iftImage *img, int lowest, int highest, int value);
iftImage  *iftFThreshold(iftFImage *img, float lowest, float highest, int value);
iftImage  *iftBinarizeByOPF(iftImage *orig, iftImage *enha, int init_thres, float train_perc);

iftImage  *iftAboveAdaptiveThreshold(iftImage *img, iftAdjRel *A, float perc, int niters, int value); /* Computes the mean value within the adjacency region A and selects pixels that are above perc*mean, by assigning value (positive number) to them. Repeats this process for a certain number of iterations (niters), excluding pixels already selected. */

iftImage  *iftBelowAdaptiveThreshold(iftImage *img, iftAdjRel *A, float perc, int niters, int value); /* Computes the mean value within the adjacency region A and selects pixels that are below perc*mean, by assigning value (positive number) to them. Repeats this process for a certain number of iterations (niters), excluding pixels already selected. */

/* post-processing for relaxed IFT (see also iftNonUsedCode.c) */

iftFImage *iftSmoothWeightImage(iftImage *basins, float beta);
iftImage  *iftFastSmoothObjects(iftImage *labelIn, iftFImage *weight, \
				int niters);

iftImage* iftBorderImageToLabelImage(iftImage* basins, iftImage* border, float border_radius);

/* Similarity measures for binary segmentation */

float iftFBetaScore(iftImage *bin, iftImage *gt, float beta);
float iftDiceSimilarity(iftImage *bin, iftImage *gt);
float *iftDiceSimilarityMultiLabel (iftImage *mlabel, iftImage *mlgt, int number_of_objects);

float iftFScoreError(iftImage *bin, iftImage *gt);
float *iftFScoreMultiLabel (iftImage *mlabel, iftImage *mlgt, int number_of_objects);

/* Error statistics */
float *iftASDError(iftImage *current_segmentation, iftFImage **object_edt_gt, int number_of_objects);
float *iftASSDError(iftImage *current_segmentation, iftImage *gt_truth, int number_of_objects);
iftErrorClassification iftSegmentationErrors(iftImage* gt_image, iftImage* cl_image);

float iftBoundaryRecall(iftImage *gt_image, iftImage *label_image, iftAdjRel *A);
float iftBoundaryRecallFromBImage(iftImage *border_image, iftImage *label_image, iftAdjRel *A);
float iftUnderSegmentation(iftImage *gt_image, iftImage *label_image, float tol_per);
float iftUnderSegmentationSLIC2(iftImage *gt_image, iftImage *label_image, float tol_per);
float iftUnderSegmentationTurbopixel(iftImage *gt_image, iftImage *label_image);
float iftUnderSegmentationMin(iftImage *gt_image, iftImage *label_image);


#endif
