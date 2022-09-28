#ifndef IFT_SIMILARITY_H_
#define IFT_SIMILARITY_H_

#include "iftCommon.h"
#include "iftAdjacency.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftRepresentation.h"
#include "iftObjectModels.h"
#include "iftSegmentation.h"
#include "iftRegistrationMSPS.h"

typedef float 	(*iftImageSimilarityFunction) (iftImage *baseImage, iftImage *auxImage); 

float 		iftSimilarityByRegistration(iftImage *baseImage, iftImage *auxImage);
float 		iftSimilarityByOverlapping(iftImage *baseImage, iftImage *auxImage);
float 		iftGetBordersDistance(iftImage *baseImage, iftImage *auxImage);
float 		iftSimilarityByDistanceTransform(iftImage *baseImage, iftImage *auxImage);
float 		iftSimilarityByRegistrationAndOverlapping(iftImage *baseImage, iftImage *auxImage);
iftMatrix 	*iftMakeSimilarityMatrix(fileList *imageFiles, iftImageSimilarityFunction similarityFunction);
iftMatrix 	*iftMakeSimilarityMatrixNoRepeat(fileList *imageFiles, iftImageSimilarityFunction similarityFunction);
void 		iftPrintImagesSimilarityMatrix(iftMatrix *result, fileList *imageFiles, char *out, int maximize);
double          iftShannonEntropy(iftImage *image);
double          iftJointEntropy(iftImage *image1, iftImage *image2);
double          iftNormalizedMutualInformation(iftImage *image1, iftImage *image2);

#endif


