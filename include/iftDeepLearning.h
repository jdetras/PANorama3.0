#ifndef IFT_DEEPLEARNING_H_
#define IFT_DEEPLEARNING_H_

#include "iftCommon.h"
#include "iftMImage.h"
#include "iftAdjacency.h"
#include "iftGraphics.h"
#include "iftColor.h"
#include "iftFiltering.h"
#include "iftInterpolation.h"
#include "iftClustering.h"
#include "iftSVM.h"
#include "iftKmeans.h"

#define ACTIV_THRES_LIMIT  1000000
#define ALPHA_LIMIT        100.

typedef struct ift_convnetwork{
  int            nlayers; // number of layers
  int            nstages; // number of stages is 3*layers + 2
  iftAdjRel     *input_norm_adj; // normalization adjacency of the input image
  int            input_norm_adj_param; // parameter to create the normalization adjacency of the input image
  int            input_xsize, input_ysize, input_zsize, input_nbands; // input image dimensions
  iftMMKernel  **k_bank; // one kernel bank per layer
  int           *k_bank_adj_param; // parameter to create adjacency of each kernel bank
  int           *nkernels; // number of kernels per layer 
  iftAdjRel    **pooling_adj; // one pooling adjacency per layer
  int           *pooling_adj_param; // parameters to create one pooling adjacency per layer 
  int           *stride; // one pooling stride per layer 
  float         *alpha; // one parameter of the pooling metric per layer
  iftAdjRel    **norm_adj; // one normalization adjacency for the end of each layer
  int           *norm_adj_param; // parameter to create one normalization adjacency for the end of each layer 
  iftMatrix    **img_ind;  // one image index matrix per layer for fast filtering 
  int            rescale;  // Output the image of the last layer with its 0- actual resolution or 1- with the resolution of the input image
  int            with_weights; // Write 1-with or 0-without kernel weights */  
  float			*activ_thres; // one activation threshold per layer
  int			*activ_option; // one option per layer
} iftConvNetwork;

typedef struct ift_msconvnetwork{
  int              nscales; // number of scales
  float           *reduction_factor; // one reduction factor per scale
  iftAdjRel       *output_norm_adj;  // normalization adjacency of the output image
  float            output_norm_adj_param; // parameter to create the normalization adjacency of the output image
  iftConvNetwork **convnet; // one convolution network per scale
} iftMSConvNetwork;

typedef struct ift_svmhyperplane {
	int n;		  // size of feature vector
	float *feat; // feature values
	float bias;
} iftSVMHyperplane;

iftConvNetwork   *iftCreateConvNetwork(int nlayers);
void              iftDestroyConvNetwork(iftConvNetwork **convnet);
iftMSConvNetwork *iftCreateMSConvNetwork(int nscales);
void              iftDestroyMSConvNetwork(iftMSConvNetwork **msconvnet);
iftConvNetwork   *iftReadConvNetwork(char *filename);
void              iftWriteConvNetwork(iftConvNetwork *convnet, char *filename);
iftConvNetwork   *iftMergeConvNetwork(iftConvNetwork *convnet1, iftConvNetwork *convnet2);
iftMSConvNetwork *iftReadMSConvNetwork(char *filename);
void              iftWriteMSConvNetwork(iftMSConvNetwork *msconvnet, char *filename);
iftMImage         *iftApplyConvNetwork(iftMImage *img, iftConvNetwork *convnet);
iftMImage         *iftApplyMSConvNetwork(iftMImage *img, iftMSConvNetwork *convnet);
void              iftPrintConvNetwork(iftConvNetwork* convnet);
void 		  iftImageDimensionsAlongNetwork(iftConvNetwork *convnet, int *xsize, int *ysize, int *zsize, int *nbands);

void              iftUnsupLearnKernels(iftMImage *img, iftConvNetwork *convnet, int nsamples, float kmax_perc, char whitening);
void              iftUnsupLearnKernelsByKmeans(iftMImage *img, iftConvNetwork *convnet, int nsamples, int k, char whitening);
void              iftUnsupLearnKernelsByKmedoids(iftMImage *img, iftConvNetwork *convnet, int nsamples, int k, char whitening);
void              iftUnsupLearnKernelsBySpKmeans(iftMImage *img, iftConvNetwork *convnet, int nsamples, int k, char whitening);

void       	  iftUnsupLearnKernelsFromImages(char *directory_path, iftConvNetwork *convnet, int target_layer, int nsamples, float kmax_perc, char whitening);
void       	  iftUnsupLearnKernelsByKmeansFromImages(char *directory_path, iftConvNetwork *convnet, int target_layer, int nsamples, int k, char whitening);
void       	  iftUnsupLearnKernelsByKmedoidsFromImages(char *directory_path, iftConvNetwork *convnet, int target_layer, int nsamples, int k, char whitening);
void       	  iftUnsupLearnKernelsBySpKmeansFromImages(char *directory_path, iftConvNetwork *convnet, int target_layer, int nsamples, int k, char whitening);

iftMMKernel       *iftUnsupLearnKernelsFromDataSet(iftDataSet* Z,iftAdjRel* A, float kmax_perc, char whitening);
iftMMKernel       *iftUnsupLearnKernelsByKmedoidsFromDataSet(iftDataSet* Z,iftAdjRel* A, int k, char whitening);
iftMMKernel       *iftUnsupLearnKernelsByKmeansFromDataSet(iftDataSet* Z,iftAdjRel* A, int k, char whitening);
iftMMKernel       *iftUnsupLearnKernelsBySpKmeansFromDataSet(iftDataSet* Z,iftAdjRel* A, int k, char whitening);

iftMMKernel	  **iftSupLearnKernelBanksFromDataSets(iftImageNames *dataset_names, int nclasses,
					int nkernels, iftAdjRel *A, int nbands, int nsamples, float *predict_mean);
iftSVMHyperplane **iftSupLearnKernelsFromDataSet(iftDataSet* Z, float *predict_sum);

void               iftSelectRandomKernelsFromImages(char *directory_path, iftConvNetwork *convnet, int target_layer, char whitening,int npatches);
void               iftWhiteningFromImages(char *directory_path, iftConvNetwork *convnet, int target_layer, int npatches);
void               iftSelectRandomKernels(iftMImage *img, iftConvNetwork *convnet, char whitening);

void            iftCreateAdjRelAlongNetwork(iftConvNetwork *convnet);
void            iftMImageIndexMatrices(iftConvNetwork *convnet);
void            iftCreateRandomKernelBanks(iftConvNetwork *convnet); 
iftConvNetwork *iftCopyFirstLayersConvNetwork(iftConvNetwork* convnet, int nlayers);
void            iftLoadKernelBanks(iftConvNetwork *convnet, FILE *fp);

iftSVMHyperplane *iftCreateSVMHyperplane(int n);
void			iftDestroySVMHyperplane(iftSVMHyperplane **h);
void 			iftConvertHyperplaneInKernelBand(iftSVMHyperplane *h, iftBand *kernel, int band_size);

iftDataSet *iftCNNFeatReductionByOPF(iftDataSet *Z, int nfeats, int total_of_patches, int num_rep_patches, float kmax);
int *iftCNNFeatSelectionByOPF(iftDataSet *Z, int nfeats, int total_of_patches, int num_rep_patches, float kmax);

#endif
