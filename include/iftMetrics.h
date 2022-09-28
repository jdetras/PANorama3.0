#ifndef _IFT_METRICS_H_
#define _IFT_METRICS_H_

#include "iftCommon.h"
#include "iftDataSet.h"


//True positives, false positives, false negatives, true negatives.
typedef struct ift_error_classification {
	int tp;
	int fp;
	int fn;
	int tn;
} iftErrorClassification;

float iftPrecisionGivenErrors(iftErrorClassification* error);
float iftRecallGivenErrors(iftErrorClassification* error);

float iftAccuracyGivenErrors(iftErrorClassification* error);
float iftFScoreGivenErrors(iftErrorClassification* error);
float iftProbabilityRatioGivenErrors(iftErrorClassification* error);

float iftSumFloatArray (float *array, int length);
float iftMeanFloatArray (float *array, int length);
float iftMeanFloatArrayDiffZero(float *array, int length);
float iftVarianceFloatArray (float *array, int length);
float iftStddevFloatArray (float *array, int length);

float iftFscore(iftDataSet *Z, int PositiveClass);
float iftTruePositives(iftDataSet *Z);
float iftSkewedTruePositives(iftDataSet *Z);
float iftClassifAccuracy(iftDataSet *Z);

float iftOPFAccuracy(iftDataSet *Z);

float iftMeasureERR(float *negatives, int total_neg, float *positives, int total_pos, float threshold);
void  iftMeasureFarFrr(iftDataSet *Z, float *FAR, float *FRR);
iftErrorClassification iftMeasureErrorClassification(iftDataSet *Ztest);
float iftWeightedAccuracyBinaryClasses(iftErrorClassification cm);
float iftMeasureMCC(iftErrorClassification cm);

float iftCosineDistance(float *f1, float *f2, int n);
float iftCosineDistance2(float *f1, float *f2, int n);

float iftCorrCoef(float *x, float *y, int n);
float iftCorrCoefDist(float *x, float *y, int n);
float iftMICI(float *x, float *y, int n);


#endif
