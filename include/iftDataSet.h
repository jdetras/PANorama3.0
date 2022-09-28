#ifndef IFT_DATASET_H_
#define IFT_DATASET_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftMImage.h"
#include "iftAdjacency.h"
#include "iftLabeledSet.h"
#include "iftFImage.h"
#include "iftRadiometric.h"
#include "iftMatrix.h"
#include "iftSort.h"
#include "iftSet.h"
#include "iftKernel.h"
#include "iftParseInput.h"
#include "iftDescriptors.h"

#define TEST      0
#define TRAIN     1
#define OUTLIER   2
#define ERROR     3

#define WEIGHT    0
#define LABEL     1
#define CLASS     2
#define POINT     3
#define STATUS    4

#define K_distance5 20.0

typedef struct ift_minkowski_table {
  float *dist;    /* look-up table for Minkowski distance
		     computation */
  int    nelems;  /* number of elements: 2*mult_factor + 1 */ 
  int    mult_factor; /* multiplication factor */
  float  exp_factor;  /* exponential factor */ 
} iftMinkowskiTable;

extern iftMinkowskiTable *iftMinkowski; /* Use iftSetMinkowskiTable to
					   initialize it */


/* the coefficients alpha can be used to modify the arc weight
computation in many different ways. These coefficients must be found
by optimization. */

typedef float (*iftArcWeightFun)(float *f1, float *f2, float *alpha, int n);


/* Table of distances */

typedef struct ift_distance_table {
  float **distance_table; /* n x n table of distances, for n samples */
  int nsamples; 
} iftDistanceTable;

extern iftDistanceTable *iftDist; /* Use iftSetDistanceFunction
					  to change it to one of the
					  iftDistanceX, X=1,2...,7 */


/* Dataset */

typedef struct ift_sample {
  float *feat;    // feature values
  int    truelabel;   // 1,2,...,nclasses
  int    label;   // 1,2,....,nlabels
  int    id;      // identification which may be either a voxel of the
                  // related image or the position of an image in the
                  // directory with a related list of images.
  
  float  weight;  // probability density, path cost, etc
  uchar  status;  // Status of the sample: TEST, TRAIN, ERROR, OUTLIER.
} iftSample;

/* structure used for feature space transformations based on PCA */

typedef struct _featspaceparam {
  float     *mean, *stdev; // parameters used for feature space normalization
                           // and centralization
  int        nfeats; // size of mean, stdev
  char      *w; // binary vector that indicates selected components for SupPCA
  int        ncomps; // size of w (number of PCA components before
                     // feature space reduction by SupPCA)
  iftMatrix *R; // Projection/rotation matrix (the columns are the eigenvectors)
  iftMatrix *W; // Whitening Transformation
} iftFeatSpaceParam;

typedef struct ift_dataset {
  iftSample *sample;   // sample
  int        nfeats;   // number of features
  int        nsamples; // number of samples
  int        nlabels;  // number of clusters 
  int        nclasses; // number of classes
  int        ntrainsamples; // number of training samples
  iftArcWeightFun iftArcWeight; // distance function
  float     *alpha;   // coefficients used for arc weight computation
  void      *ref_data; // it migh be an image, for voxel datasets, a text file with image filenames, for image datasets, a region graph, for supervoxel datasets, etc.
  iftFeatSpaceParam fsp; // parameters of the feature scape transformation
} iftDataSet;

char        iftIsSupervisedDataSet(iftDataSet *Z); 
char        iftIsTrainingDataSet(iftDataSet *Z);
char        iftIsTestingDataSet(iftDataSet *Z);
char        iftIsNormalizedDataSet(iftDataSet *Z);
char        iftIsCentralizedDataSet(iftDataSet *Z);
char        iftIsWhitenedDataSet(iftDataSet *Z);
char        iftIsTransformedDataSetByPCA(iftDataSet *Z);
iftDataSet *iftCreateDataSet(int nsamples, int nfeats);
iftDataSet *iftCreateDataSet2(int nsamples, int nfeats);
void        iftDestroyDataSet(iftDataSet **Z);
void 		iftDestroyDataSet2(iftDataSet **Z);
iftDataSet *iftCopyDataSet(iftDataSet *Z);
iftDataSet *iftCopyDataSet2(iftDataSet *Z);
iftDataSet *iftMergeDataSetsWithDifferentClasses(iftDataSet *Z1, iftDataSet *Z2);
void		iftValidateDataSets(iftDataSet **Zs, int num_Z);
iftDataSet *iftImageToDataSet(iftImage *img);
void        iftImageGTToDataSet(iftImage* imgGT,iftDataSet* Zpixels);
iftDataSet *iftImageToDataSetUsingAdjacency(iftImage *img, iftAdjRel *A);
iftDataSet *iftObjectToDataSet(iftImage *bin);
iftDataSet *iftReadXYDataSet(char *filename);
iftDataSet *iftImageSeedsToDataSet(iftImage *img, iftLabeledSet *S);
iftDataSet *iftMImageSeedsToDataSet(iftMImage *img, iftLabeledSet *S);
iftDataSet *iftImageRegionToDataSet(iftImage *img, iftImage *bin);
iftDataSet *iftSupervoxelsToDataSet(iftImage *img, iftImage *label);
iftDataSet *iftSupervoxelsToAvgMinMaxDataSet(iftImage *img, iftImage *label);
iftDataSet *iftSupervoxelsToHistogramDataSet(iftImage *img, iftImage *label, int nbins, int bpp);
iftDataSet *iftSupervoxelsToMeanSizeDataSet(iftImage* image, iftImage* label_image, char colorspace);
iftDataSet *iftSupervoxelsToSelectiveSearchDataset(iftImage *image, iftImage* label_image, int bins_per_band, char colorspace);
iftDataSet *iftRegionMergingDataSet(iftMImage *image, iftImage* label_image);

iftDataSet *iftMSupervoxelsToDataSet(iftMImage *mimg, iftImage *label);

iftDataSet *iftImageBorderDataSet(iftDataSet *Z1, int size, int nsamples);
iftDataSet *iftReadOPFDataSet(char *filename);
void        iftWriteOPFDataSet(iftDataSet *Z, char *filename);
void        iftResetDataSet(iftDataSet *Z);
int         iftSelectSupTrainSamples(iftDataSet *Z, float perc);
int         iftSelectUnsupTrainSamples(iftDataSet *Z, float perc);
int         iftSelectUnsupTrainSamplesByWeight(iftDataSet *Z, float train_perc);
void        iftCopyClassifResult(iftDataSet *Z1, iftDataSet *Z2);
iftFeatSpaceParam iftComputeOverallMeanAndStdevFromDatasets(iftDataSet **Zs, int num_Z);
iftDataSet *iftSetClassAsPositiveIntoDataSet(iftDataSet *Z, int positive_class, int *num_pos);
iftDataSet *iftNormalizeDataSet(iftDataSet *Z);
iftDataSet *iftNormalizeDataSet2(iftDataSet *Z, iftFeatSpaceParam fsp);
void        iftNormalizeSamples(iftDataSet *Z);
iftDataSet *iftNormalizeContrastDataSet(iftDataSet *Z);
iftDataSet *iftCentralizeDataSet(iftDataSet *Z);
iftDataSet *iftUnitNormDataSet(iftDataSet* Z);
iftDataSet *iftRotateDataSet(iftDataSet *Z, iftMatrix *U);
iftDataSet *iftEliminateAmbiguousSamples(iftDataSet *Z);
iftDataSet *iftExtractClass(iftDataSet *Z, int truelabel);
iftDataSet *iftExtractObjectClasses(iftDataSet *Z);
iftDataSet *iftExtractSamples(iftDataSet *Z, uchar status);
iftDataSet *iftNormalizeTestDataSet(iftDataSet *Z1, iftDataSet *Z2);
iftDataSet *iftNormalizeTestDataSet2(iftDataSet *Z, iftFeatSpaceParam fsp);
iftDataSet *iftCentralizeTestDataSet(iftDataSet *Z1, iftDataSet *Z2);
void        iftMultDataSetByScalar(iftDataSet *Z, float scalar);
iftDataSet *iftMMKernelToDataSet(iftMMKernel *K);
iftMMKernel *iftDataSetToMMKernel(iftDataSet *Z);

iftDataSet *iftTransFeatSpaceByPCA(iftDataSet *Z, int num_of_comps);
iftFeatSpaceParam iftTransFeatSpaceByPCA2(iftDataSet *Z, int num_of_comps);
iftDataSet *iftWhiteningTransform(iftDataSet *Z);
iftDataSet *iftTransFeatSpaceBySupPCA(iftDataSet *Z, int num_of_comps);
iftDataSet *iftTransformTestDataSetByPCA(iftDataSet *Z1, iftDataSet *Z2);
iftDataSet *iftFastTransformTestDataSetByPCA(iftDataSet *Z1, iftDataSet *Z2);
iftDataSet *iftTransformTestDataSetByPCAWhitening(iftDataSet *Z1, iftDataSet *Z2, iftMatrix *S);
iftDataSet *iftInverseTransformTestDataSetByPCA(iftDataSet *Z1, iftDataSet *Z2);
iftDataSet *iftInverseTransformTestDataSetByPCA2(iftFeatSpaceParam fsp, iftDataSet *Z2);

iftImage   *iftDataSetLabel(iftDataSet *Z, char *ref_data_type);
iftImage   *iftDataSetWeight(iftDataSet *Z, char *ref_data_type);
void        iftConfusionMatrix(iftDataSet *Z);

void        iftSetStatus(iftDataSet *Z,uchar status);
void 	    iftSetStatusForSet(iftDataSet *Z, iftSet *set, uchar status);
iftMatrix  *iftCovarianceMatrix(iftDataSet *Z);
iftMatrix  *iftRotationMatrixByPCA(iftDataSet *Z);
iftMatrix  *iftRotationMatrixAndSingularValuesByPCA(iftDataSet *Z, iftMatrix **S);
iftMatrix  *iftDataSetToFeatureMatrix(iftDataSet *Z);
iftDataSet *iftFeatureMatrixToDataSet(iftMatrix *X);
iftDataSet *iftFeatureMatrixToDataSet2(iftMatrix *X, int nrows, int ncols);
iftVector   iftPrincipalAxis(iftImage *bin);

iftDataSet *iftNormOneDataSet(iftDataSet *Z);

iftFeatSpaceParam iftCreateFeatSpaceParam(void);
iftFeatSpaceParam iftReadPCAMatrix(char *filename);
void 			  iftWritePCAMatrix(iftFeatSpaceParam fsp, char *filename);
iftFeatSpaceParam iftCopyFeatSpaceParam(iftDataSet *Z);
iftFeatSpaceParam iftCopyFeatSpaceParam2(iftFeatSpaceParam fsp);
iftFeatSpaceParam iftTransFeatSpaceByPCA3(iftDataSet *Z, int num_of_comps, iftMatrix **S);

iftDistanceTable *iftCreateDistanceTable(int nsamples);
void              iftDestroyDistanceTable(iftDistanceTable **dt); 
iftDistanceTable *iftReadDistanceTable(char *filename);
void              iftWriteDistanceTable(iftDistanceTable *dt, char *filename);
iftDistanceTable *iftCompDistanceTable(iftDataSet *Z);

FILE **iftBuildDataSetsFromMImages(int nimages, int nclasses, int nfeatures, int nfilters,
		int stride, iftAdjRel *A, char *mimage_directory, char *output_directory, iftImageNames *img_names);
void iftExtractSamplesOfMImages(iftMImage *img, int truelabel, iftAdjRel *A, int sample_stride, FILE **fp);

void iftSwitchSamples(iftSample *s1, iftSample *s2, int nfeats);
void iftCopySample(iftSample *sin, iftSample *sout, int nfeats);
void iftCopySampleWithoutFeats(iftSample *sin, iftSample *sout);
void iftSwitchNotSVsPerErrorSamples(iftDataSet *Ztrain, iftDataSet *Ztest, int *not_SVs,
		int num_not_SVs, int *error_samples, int num_errors);
iftDataSet *iftSelectSamplesOfTheClass(iftDataSet *Z, int truelabel);
iftDataSet *iftSelectNegativeSamples(iftDataSet *Z, int positive_class);
iftDataSet *iftBuildPatchDataSet(iftDataSet *Z, int patch, int nfeats);
iftDataSet *iftBuildDataSetFromPatches(iftDataSet *Z, int *patches, int num_of_patches, int nfeats);
iftDataSet *iftBuildPatchDataSetFromSample(iftSample sample, int num_of_patches, int nfeats);

/* Used by pyift */
void iftSetTrainingSupervoxelsFromSeeds(iftDataSet *dataset, iftImage *label_image, iftLabeledSet* seed);

iftDataSet* iftMImageToDataSet(iftMImage* mimg);
iftDataSet* iftMImageToEdgesDataSet(iftMImage* mimg,iftAdjRel* A);
iftDataSet* iftMImageToLabeledEdgesDataSet(iftMImage* mimg,iftImage* imgGT, iftImage* imglabels,iftAdjRel* A);
iftFImage*  iftEdgesDataSetToFImage(iftDataSet* dataset,iftMImage* mimg,iftAdjRel* A);
iftImage*   iftPixelDataSetToImage(iftDataSet*,iftImage *img);
iftFImage*  iftPixelDataSetToFImage(iftDataSet* dataset,iftImage* img);
void        iftSetDistanceFunction(iftDataSet *Z, int function_number);

void iftPrintNumberOfSamplesPerCluster(iftDataSet *Z);


/*---------------------- Distance functions -----------------------------*/

float iftDistance1(float *f1, float *f2, float *alpha, int n); /* default:
   When alpha is either 0 or 1, it selects features for Euclidean
   distance. When alpha is from 0 to 1, it becomes a sort of weighted
   Euclidean distance. */

float iftDistance2(float *f1, float *f2, float *alpha, int n); /* Log of
								  Distance
								  1 */

float iftDistance3(float *f1, float *f2, float *alpha, int n); /* In
         this function, alpha plays the expoents of the absolute
         differences between each feature. Features must be normalized
         within [0,1] to use this distance function and alpha >= 0. */

float iftDistance4(float *f1, float *f2, float *alpha, int n); /* Log of
								  Distance
								  3 */


float iftDistance5(float *f1, float *f2, float *alpha, int n);


float iftDistance6(float *f1, float *f2, float *alpha, int n); /* inner-product-based
								  distance
								  for
								  centralized
								  datasets. Assumes
								  feature
								  vectors
								  with
								  norm
								  1 */

float iftDistance7(float *f1, float *f2, float *alpha, int n); /* Minkowski
								  distance. Assumes
								  feature
								  vectors
								  with
								  norm
								  1. */

float iftDistance8(float *f1, float *f2, float *alpha, int n);

float iftDistance9(float *f1, float *f2, float *alpha, int n);

void  iftSetMinkowskiTable(int mult_factor, float exp_factor); /* initialize
				       look-up table for Minkowski
				       distance computation. */

void  iftDestroyMinkowskiTable(void);

float iftDistMeanSizeSupervoxel(float *f1, float *f2, float *alpha, int n);
float iftDistSelectiveSearchSupervoxel(float *f1, float *f2, float *alpha, int n);
float iftRegionMergingDist(float *f1, float *f2, float *alpha, int n);

/* --------------------- Merging functions ----------------------------- */

float* iftMergeMeanSizeSupervoxel(float *f1, float*f2, float*alpha, int n);
float* iftMergeSelectiveSearchSupervoxel(float *f1, float *f2, float *alpha, int n);
float* iftMergeVector(float *f1, float*f2, float*alpha, int n);
float* iftRegionMergingFeat(float *f1, float*f2, float*alpha, int n);


#endif

