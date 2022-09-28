#ifndef IFT_DESCRIPTORS_H_
#define IFT_DESCRIPTORS_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftMImage.h"
#include "iftAdjacency.h"
#include "iftRadiometric.h"

typedef struct ift_features {
  float *val;
  int    n;
} iftFeatures;

iftFeatures *iftCreateFeatures(int n);
void         iftDestroyFeatures(iftFeatures **feat);
void         iftWriteFeatures(iftFeatures *feat, char *filename);
void         iftWriteFeatures2(iftFeatures *feat, char *filename);
iftFeatures *iftReadFeatures2(char *filename);
iftImage    *iftLocalBinaryPattern(iftImage *img, iftAdjRel *A);
iftFeatures *iftExtractLBP(iftImage *img, iftAdjRel *A);
iftFeatures *iftExtractBrightness(iftImage *img);
iftFeatures *iftExtractGradient(iftImage *img, iftAdjRel *A);
iftFeatures *iftExtractBIC(iftImage *img, int bins_per_band);
float        iftFeatDistL1(iftFeatures *feat1, iftFeatures *feat2);
iftFeatures *iftMImageToFeatures(iftMImage *img);
void 	     iftWriteFeaturesInFile(iftFeatures *features, int truelabel, FILE *fp);
iftFeatures *iftIntensityProfile(iftImage *img, iftPoint u1, iftPoint u2);

#endif
