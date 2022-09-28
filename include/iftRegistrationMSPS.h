#ifndef IFT_REGISTRATIONMSPS_H_
#define IFT_REGISTRATIONMSPS_H_

#include "iftCommon.h"
#include "iftAdjacency.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftRepresentation.h"
#include "iftGeometric.h"

typedef struct ift_registerProb {
   iftImage *baseImage;
   iftImage *auxImage;
   iftFImage *baseDistanceImage;
   iftSet *borderPointsAuxImage;
}  iftRegisterProb;


iftRegisterProb *iftCreateRegisterProb(iftImage *baseImage, iftImage *auxImage);
void             iftDestroyRegisterProb(iftRegisterProb **prob);
iftMatrix       *iftTransformationMatrix(float thetaX, float thetaY, float thetaZ, float isoScale);
iftMatrix *iftTransformationMatrixFullCentered(float *parameters, iftVector translationCenter);
float            iftRegisterMSPSFitness(void *problem, float *theta);
//iftImage        *iftTransformImageClipping(iftImage *img, iftMatrix *InvE, int xsize, int ysize, int zsize);
void             iftInitializeRegistrationParam(iftMSPS *msps);
void             iftKeepParams(iftMSPS *msps, float *paramArray);
iftImage        *iftRegisterInstances(iftImage *baseImage, iftImage *auxImage, float *paramArray);

#endif


