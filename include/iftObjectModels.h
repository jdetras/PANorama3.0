#ifndef IFT_OBJECTMODELS_H_
#define IFT_OBJECTMODELS_H_

#include <dirent.h>
#include "iftCommon.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftRepresentation.h"
#include "iftRegistrationMSPS.h"

typedef struct ift_fuzzy_model {
  iftFImage *model;
  iftAdjRel *AIn;
  iftAdjRel *AOut;
  iftAdjRel *AUncertain;
  iftVoxel  start;
  iftVoxel  end;
} iftFuzzyModel;

typedef struct ift_fuzzy_model_extended {
  iftFImage *model;
  iftAdjRel *AIn;
  iftAdjRel *AOut;
  iftAdjRel *AUncertain;
  iftVoxel  start;
  iftVoxel  end;
  float     mean;
  float     deviation;
  int       mode;
} iftFuzzyModelExtended;

typedef struct ift_fuzzy_model_aligned {
  iftFImage *model;
  iftAdjRel *AIn;
  iftAdjRel *AOut;
  iftAdjRel *AUncertain;
  iftVoxel  start;
  iftVoxel  end;
  float     *parametersRange; //Max and min rotation x, y, z; Max and min isometric scale
} iftFuzzyModelAligned;

typedef struct ift_statistic_model {
  iftFImage *model;
  iftAdjRel *AIn;
  iftAdjRel *AOut;
  iftAdjRel *AUncertain;
  iftVoxel  start;
  iftVoxel  end;
  float     *parametersRange; //Max and min rotation x, y, z; Max and min isometric scale
  iftImage  *reference;
  iftImage  *referenceAligned;
  char      *referenceName;
} iftStatisticModel;

typedef struct ift_statistic_model_simple {
  iftFImage *model;
  float     mean;
  float     deviation;
  int       mode;
} iftStatisticModelSimple;

typedef struct ift_statistic_model_complex {
  iftFImage *model;
  iftAdjRel *AIn;
  iftAdjRel *AOut;
  iftAdjRel *AUncertain;
  iftVoxel  start;
  iftVoxel  end;
  float     mean;
  float     deviation;
  int       mode;
} iftStatisticModelComplex;

iftImage             *iftCropObjectLoose(iftImage *obj, int xLSize, int yLSize, int zLSize);
void                 iftModelDomain(char *dirname, int *xsize, int *ysize, int *zsize);
int                  iftHasUncertain6Adjacent(iftFImage *model, int i);
void                 iftModelAdjacencies(iftFImage *model, iftAdjRel **AIn, iftAdjRel **AOut, iftAdjRel **AUncertain);
void                 iftFuzzyByAveraging(iftFImage *model, fileList *imageFiles);
void                 iftFuzzyByDistanceTransform(iftFImage *model, fileList *imageFiles);
void                 iftComputeFuzzyModel(char *dirname, iftFImage *model, char option);
void                 iftStatisticByAveraging(iftStatisticModel * statisticModel, fileList *imageFiles);
void                 iftComputeStatisticModel(char *dirname, iftStatisticModel *model, char option);
void                 iftDestroyFuzzyModel(iftFuzzyModel **fuzzyModel);
void                 iftDestroyFuzzyModelAligned(iftFuzzyModelAligned **fuzzyModel);
void                 iftWriteFuzzyModel(iftFuzzyModel *fuzzyModel, char *filename);
void                 iftWriteFuzzyModelAligned(iftFuzzyModelAligned *fuzzyModel, char *filename);
void                 iftWriteFuzzyModelExtended(iftFuzzyModelExtended *fuzzyModel, char *filename);
iftFuzzyModel        *iftReadFuzzyModel(char *filename);
iftFuzzyModelExtended *iftReadFuzzyModelExtended(char *filename);
iftFuzzyModelAligned *iftReadFuzzyModelAligned(char *filename);
void                 iftModelPositions(char *dirImages, char *dirInstances, iftFuzzyModel *fuzzyModel);
void                 iftModelPositionsFuzzyExtended(char *dirImages, char *dirInstances, iftFuzzyModelExtended *fuzzyModel);
void                 iftModelPositionsStatistic(char *dirImages, char *dirInstances, iftStatisticModel *statisticModel);
void                 iftModelPositionsStatisticComplex(char *dirImages, char *dirInstances, iftStatisticModelComplex *statisticModel);
void	             iftWriteStatisticModel(iftStatisticModel *statisticModel, char *filename);
void                 iftDestroyStatisticModel(iftStatisticModel **statisticModel);
iftStatisticModel    *iftReadStatisticModel(char *filename);
float                *iftComputeFuzzyModelAligned(char *binsRoute, iftFImage *model);
void                 iftModelPositionsAligned(char *dirImages, char *dirInstances, iftFuzzyModelAligned *fuzzyModel);
void	             iftWriteStatisticModelSimple(iftStatisticModelSimple *statisticModel, char *filename);
void                 iftDestroyStatisticModelSimple(iftStatisticModelSimple **statisticModel);
iftStatisticModelSimple *iftReadStatisticModelSimple(char *filename);
void                 iftWriteStatisticModelComplex(iftStatisticModelComplex *statisticModel, char *filename);
void                 iftDestroyStatisticModelComplex(iftStatisticModelComplex **statisticModel);
void                 iftDestroyFuzzyModelExtended(iftFuzzyModelExtended **fuzzyModel);
void                 iftComputeHistogramParameters(char *dirImages, char *dirInstances, float *meanP, int *modeP, float *sdP);
iftStatisticModelComplex *iftReadStatisticModelComplex(char *filename);
void                 iftFuzzyByAveragingNoGCCentering(iftFImage *model, fileList *imageFiles);

#endif


