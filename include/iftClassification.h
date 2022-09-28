#ifndef IFT_CLASSIFICATION_H_
#define IFT_CLASSIFICATION_H_

#include "iftCommon.h"
#include "iftDataSet.h"
#include "iftSet.h"
#include "iftFHeap.h"
#include "iftVPS.h"
#include "iftMST.h"
#include "iftMetrics.h"

typedef struct ift_cplnode {
  int   pred;     // predecessor node
  int   sample;   // corresponding training sample in Z
} iftCplNode;

typedef struct ift_cplgraph {
  iftCplNode *node;          // node
  float      *pathval;       // path value for each node
  int        *ordered_nodes; // list of nodes ordered by path value 
  int         nnodes;        // number of nodes
  iftFHeap   *Q;             // priority queue
  iftDataSet *Z;             // Each graph node is a training sample in Z
} iftCplGraph;

iftCplGraph *iftCreateCplGraph(iftDataSet *Z);
void         iftDestroyCplGraph(iftCplGraph **graph);
void         iftSupTrain(iftCplGraph *graph);
iftCplGraph *iftSupLearn(iftDataSet *Z);
int          iftClassify(iftCplGraph *graph, iftDataSet *Z);
void         iftBinClassify(iftCplGraph *graph, iftDataSet *Z);
void         iftBorderClassify(iftCplGraph *graph, iftDataSet *Z,int truelabel);
iftSet      *iftPrototypesByMST(iftCplGraph *graph);
void         iftSupFeatSelection(iftDataSet *Z, float train_perc, int nteams);
int          iftClassifyByKNN(iftDataSet *Z1,iftDataSet *Z, int k);
iftSet      *iftGetMSTBoundarySamples(iftCplGraph *graph, iftMST *mst, int nsamples);
float        iftGetKFoldCrossValidationMeanOPFAccuracy(iftDataSet *Z, const int k);
iftSet      *iftGetRootDistanceBasedSamples(iftCplGraph *trainGraph, iftDataSet *Z,
							int **sampleLists, int *nSamples, uchar status, int statusCond,
							int nSelect);
#endif
