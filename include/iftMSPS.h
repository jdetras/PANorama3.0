#ifndef _iftMSPS_H_
#define _iftMSPS_H_

#include "iftCommon.h"
#include "iftMatrix.h"

/* Fitness function used for optimization problems */
typedef float (*iftMSPSFitnessFunc)   (void *problem, float *theta); 

/*Function to generate the perturbation, given a matrix delta in the ith dimension, jth scale*/
typedef void (*iftPerturbationFunc) (iftMatrix* delta, int i, int j, float* perturbation);


/* Data structure for Multiscale Parameter Search */

typedef struct ift_MSPS {
  iftMSPSFitnessFunc iftFitness;   /* fitness function for optimization. */
  void   *problem;             /* the context of the problem */
  int     n;                   /* number of parameters for optimization */
  int     m;                   /* number of scales */
  float     *theta;            /* parameter vector to be optimized */
  iftMatrix *delta;            /* matrix of displacements for each
				  parameter and all scales */
  int     niters;              /* maximum number of iterations */

  float stopcriteria;/*If the gain between iterations is smaller than this stop*/

  iftPerturbationFunc iftPerturbation;/* generator of the perturbation vector */
  int iterstar;/*iteration that found the optimum value*/

} iftMSPS;

iftMSPS  *iftCreateMSPS(int n,int m, iftMSPSFitnessFunc f,void *prob);
void      iftDestroyMSPS(iftMSPS **msps);
float     iftMSPSMax(iftMSPS *msps);
float     iftMSPSMin(iftMSPS *msps);

void iftMSPSLinearRandomPerturbation(iftMatrix* delta, int row, int col, float *out);
void iftMSPSGaussianRandomPerturbation(iftMatrix* delta, int row, int col, float *out);
void iftMSPSDeterministicPerturbation(iftMatrix* delta, int row, int col, float *out);
void iftMSPSRandomDirectionPerturbation(iftMatrix* delta, int row, int col, float *out);

#endif
