#ifndef IFT_CLUSTERING_H_
#define IFT_CLUSTERING_H_

#include "iftCommon.h"
#include "iftDataSet.h"
#include "iftAdjSet.h"
#include "iftSet.h"
#include "iftFHeap.h"
#include "iftSet.h"
#include "iftBMap.h"
#include "iftMSPS.h"
#include "iftVPS.h"
#include "iftMST.h"
#include "iftSort.h"

typedef struct ift_knnnode {
  float maxarcw;  // maximum arc weight from the node to its adjacent nodes
  int   root;     // root node
  int   sample;   /* training sample in the original dataset Z where
                   each element will be assigned to density and label
                   values after training */
  iftAdjSet *adj;  // adjacent nodes
  iftSet    *adjplat; // adjacent nodes on plateaus of density
} iftKnnNode;

typedef struct ift_knngraph {
  iftKnnNode *node;          // node
  float      *pathval;       // path value for each node
  int        *ordered_nodes; // list of nodes ordered by path value
  int         nnodes;        // number of nodes
  float      *maxarcw;       // maximum arc weight for each value of k
  int         kmax;          // maximum number of arcs
  int         k;             // best number of arcs
  iftFHeap   *Q;             // priority queue
  iftDataSet *Z;             // Each graph node is a training sample in Z
} iftKnnGraph;

typedef float (*iftKnnGraphCutFun)(iftKnnGraph *graph);

/* Public methods */

iftKnnGraph *iftCreateKnnGraph(iftDataSet *Z, int kmax);
void         iftDestroyKnnGraph(iftKnnGraph **graph);
int          iftUnsupTrain(iftKnnGraph *graph,iftKnnGraphCutFun iftGraphCutFun);
int          iftUnsupTrainWithCClusters(iftKnnGraph *graph, int c);//with c clusters
iftKnnGraph *iftUnsupLearn(iftDataSet *Z, float kmax_perc, iftKnnGraphCutFun iftGraphCutFun, int niterations);
int          iftClusterByDist(iftKnnGraph *graph, iftDataSet *Z);

int          iftCluster(iftKnnGraph *graph, iftDataSet *Z);
int          iftSemiSupClassify(iftKnnGraph *graph, iftDataSet *Z);
void         iftUnsupFeatSelecByVPS(iftDataSet *Z, int kmax);
void         iftUnsupFeatSelecByMSPS(iftDataSet *Z, int kmax);
float        iftKnnGraphCutFun1(iftKnnGraph *graph);
float        iftKnnGraphCutFun2(iftKnnGraph *graph);
float        iftKnnGraphCutFun3(iftKnnGraph *graph);
float        iftKnnGraphCutFun4(iftKnnGraph *graph);
float        iftKnnGraphCutFun5(iftKnnGraph *graph);

iftDataSet  *iftGraphBoundaryReduction(iftKnnGraph *graph, iftDataSet *Z);

iftSet      *iftGetKnnBoundarySamples(iftKnnGraph *graph);
iftSet      *iftGetKnnRootSamples(iftKnnGraph *graph);

int        **iftGetNodesPerClusterOrderedByDistanceToRoot(iftKnnGraph *graph, int **nNodes);

void         iftPropagateClusterTrueLabels(iftKnnGraph *graph);

#endif
