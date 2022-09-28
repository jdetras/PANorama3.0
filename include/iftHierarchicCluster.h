#ifndef _IFT_HIERARCHIC_CLUSTER_H_
#define _IFT_HIERARCHIC_CLUSTER_H_

#include "iftDataSet.h"
#include "iftClustering.h"
#include "iftKmeans.h"
#include "iftGraphics.h"

typedef struct ift_cluster_hierarchy iftClusterHierarchy; 
typedef struct ift_cluster_hierarchy_level iftClusterHierarchyLevel;
typedef struct ift_cluster iftClusterHierarchyCluster;
typedef struct ift_cluster_sample iftClusterSample;

typedef enum {
  CLUSTER_HIERARCHY_IN_MEMORY,
  CLUSTER_HIERARCHY_IN_DISK,
  CLUSTER_HIERARCHY_INVALID_STATE
} iftClusterLoadStatus;

typedef enum {
  CLUSTER_HIERARCHY_DEEP_COPY,
  CLUSTER_HIERARCHY_SHALLOW_COPY
} iftClusterHierarchyCopyType;

struct ift_cluster_hierarchy {
  char *path; // Where clusters will be stored in disk
  int totalSamples;
  int featureSize;
  int levelCount;
  iftClusterHierarchyLevel **level;
};

struct ift_cluster_hierarchy_level {
  int size;
  int clusterCount;
  iftClusterHierarchyCluster **cluster;
};

struct ift_cluster {
  int sampleCount;
  iftClusterLoadStatus status;
  iftClusterSample *sample; // Samples are ordered by relevance
};

struct ift_cluster_sample {
  float *feat;
  int isPromoted;
  int truelabel;
  int parentCluster;
};

iftClusterHierarchy *iftCreateClusterHierarchy(int featureSize, char *path);
void                 iftAddDataSetToClusterHierarchy(iftClusterHierarchy *H, int level, iftDataSet *Z, iftClusterHierarchyCopyType copyType);
iftDataSet          *iftConvertClusterToDataSet(iftClusterHierarchy *H, int level, int cluster);
void                 iftApplyKNNOnCluster(iftClusterHierarchy *H, int level, int cluster, int K, int maxIterations, float minImprovement);
void                 iftApplyOPFOnCluster(iftClusterHierarchy *H, int level, int cluster, int kmax);
void                 iftApplyRandomSplitOnCluster(iftClusterHierarchy *H, int level, int cluster);
void                 iftApplyClassSplitOnCluster(iftClusterHierarchy *H, int level, int cluster);
void                 iftPromoteClusterSample(iftClusterHierarchy *H, int level, int cluster, int sample);
void                 iftUnloadCluster(iftClusterHierarchy *H, int level, int cluster);
void                 iftLoadCluster(iftClusterHierarchy *H, int level, int cluster);
void                 iftUnloadHierarchyLevel(iftClusterHierarchy *H, int level);
void                 iftLoadHierarchyLevel(iftClusterHierarchy *H, int level);
iftDataSet          *iftChooseSamplesFromHierarchyTop(iftClusterHierarchy *H, int sampleCount);
void                 iftDestroyHierarchy(iftClusterHierarchy **H);
void                 iftDestroyShallowDataSet(iftDataSet **Z);

// Standard hierarchy builders
// 1 - Kmeans, class, 3 level TODO fix memory leak
iftClusterHierarchy *iftBuildHierarchy1(iftDataSet *Z, int K, int maxIterations, int minImprovement, char *path, iftClusterHierarchyCopyType copyType);

/* Debug only */
void DumpHierarchyMetadata(iftClusterHierarchy *H, int showClusters, int showSamples, int showFeatures);
iftDataSet *GetLabelDataSetByLevel(iftClusterHierarchy *H);
#endif /*_IFT_HIERARCHIC_CLUSTER_H_*/
