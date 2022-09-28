#ifndef IFT_HISTOGRAM_H_
#define IFT_HISTOGRAM_H_

#include "iftCommon.h"
#include "iftImage.h"

typedef struct ift_histogram {
  double *val;
  int nbins;
} iftHistogram;

iftHistogram *iftCreateHistogram(int nbins);
void          iftDestroyHistogram(iftHistogram **hist);
iftHistogram *iftReadHistogram(char *filename);
void          iftWriteHistogram(iftHistogram *hist, char *filename);
iftHistogram *iftGrayHistogram(iftImage *img, int nbins, char normalize);
iftHistogram *iftColorHistogram(iftImage *img, int nbins, char normalize);
iftHistogram *iftAccHistogram(iftHistogram *hist);
int           iftHistogramMode(iftHistogram *hist, char exclude_zero);
float         iftHistogramStdevAroundValue(iftHistogram *hist, float value, char exclude_zero);

#endif
