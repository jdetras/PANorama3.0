#ifndef IFT_Curve_H_
#define IFT_Curve_H_

#include <iftCommon.h>

typedef struct ift_curve {
  double *x, *y;
  int     npts;
} iftCurve;

iftCurve *iftCreateCurve(int npts);
void      iftDestroyCurve(iftCurve **curve);
iftCurve *iftCopyCurve(iftCurve *curve);

void      iftMeansOfXandY(iftCurve *curve, double *mean_of_x, double *mean_of_y);

void      iftStdevsOfXandY(iftCurve *curve, double mean_of_x, double mean_of_y, double *stdev_of_x, double *stdev_of_y);

iftCurve *iftCentralizeCurve(iftCurve *curve, double mean_of_x,  double mean_of_y);

double   iftCorrCoefficient(iftCurve *curve, double mean_of_x, double mean_of_y); 

void     iftLinearRegression(iftCurve *curve, float *slope, float *intercept, double *sqr_corr_coef);

void     iftWriteCurve(iftCurve *curve, char *filename);

#endif
