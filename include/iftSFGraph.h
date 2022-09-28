#ifndef IFT_SFGRAPH_H_
#define IFT_SFGRAPH_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftAdjSet.h"
#include "iftFHeap.h"
#include "iftFIFO.h"
#include "iftLIFO.h"
#include "iftFSet.h"


typedef struct ift_gltable {
  float **len;
  int n;
} iftGLTable;

typedef struct ift_sfnode {
  int   voxel;
  int   label;
  int   pred;
  iftAdjSet *adj;
} iftSFNode;

typedef struct ift_sfgraph {
  iftSFNode  *node;
  float      *pathval;
  int         nnodes;
  iftImage   *img;
  iftFHeap   *Q;
  //  iftGLTable *glt; // table to speed up geodesic length computation
} iftSFGraph;

// Only public methods

iftSFGraph *iftCreateSFGraph(int nnodes);
void        iftDestroySFGraph(iftSFGraph **graph);
float       iftMaxGeodesicMeasure(iftSFGraph *graph, int *r, int nroots);
float       iftGeodesicLength(iftSFGraph *graph, int r1, int r2);
float       iftGeodesicArea(iftSFGraph *graph);
iftSFGraph *iftSurfaceGraph(iftImage *root, iftAdjRel *A);
iftImage   *iftSFGraphLabel(iftSFGraph *graph);
char        iftAdjacentSFNodes(iftSFGraph *graph, int p, int q, float *arcw);
iftFImage  *iftSFGraphPathVal(iftSFGraph *graph);

#endif
