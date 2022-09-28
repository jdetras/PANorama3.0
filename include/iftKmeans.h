#ifndef IFT_KMEANS_H_
#define IFT_KMEANS_H_

#include "iftDataSet.h"

void        iftKmeansRun(int bKmedoids,iftDataSet* Z,iftDataSet** pZk,int maxIterations,float minImprovement);
void        iftSphericalKmeansRun(iftDataSet* Z,iftDataSet** pZk,int maxIterations);
iftDataSet* iftKmeansInitCentroidsFromSamples(iftDataSet* Z, int k);
iftDataSet* iftKmeansInitCentroidsFromSamples2(iftDataSet* Z, int k);
iftDataSet* iftKmeansInitCentroidsRandomNormal(iftDataSet* Z, int k);

iftDataSet *iftNearestSamplesFromCentroids(iftDataSet* Zorig, iftDataSet *Zc, iftDataSet* Zk, int m, int include_centroids);
iftDataSet *iftBuildDataSetAsNearestSamplesFromCentroids(iftDataSet* Zorig, iftDataSet *Zc,
			iftDataSet* Zk, int include_centroids, char *method);


#endif
