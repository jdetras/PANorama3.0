#ifndef IFT_PANorama_H_
#define IFT_PANorama_H_

#include "ift.h"

#define VERSION      1.0     /* version 1.0 of PANorama */

#define SCALE      120.0     /* pixels per cm */
#define OTSUTHRES    1.2     /* factor to be multiplied by Otsu's threshold for panicle binarization */
#define MAXDISTSEG 0.005     /* maximum allowed distance in cm to the
				segment in order to compute a segment
				length; higher is the value, smoother
				is the segment. */     
#define BULLETSZ   0.040     /* size of the bullets in cm on the skeleton */
#define ANWSTHICK  0.032     /* thickness of awns in cm for awns removal */
#define MINEXTSZ   0.083     /* minimum extrusion size in cm in order
				to consider extrusion */
#define MAXSKDIST  0.083     /* maximum distance in cm from the
				skeleton in order to find the closest
				skeleton point */

#define MINJCTDIST    0.25     /* minimum inter-nodes distance (in cm)
				in order to consider a new junction */ 

#define MINFSTJCTLEN  0.5     /* minimum length (in cm) between the
				 first point on the main axis and the
				 first junction in order to consider
				 this a valid segment */ 

#define MINBRANCSZ   1.0     /* minimum size in cm to be considered a
				branch */

#define SEEDTHRES    0.11    /* Threshold in cm to erode mask and count
			        seeds as the remaining connected
			        components */

#define SKELTHRES  0.10     /* Threshold to convert multiscale
			       skeletons into a binary skeleton */


#define R_END          0     /* color of the end points */
#define G_END        255
#define B_END          0

#define R_SKL        255     /* color of the skeleton points */
#define G_SKL        255
#define B_SKL          0

#define R_BCH          0     /* color of the primary braches */
#define G_BCH        255
#define B_BCH        255

#define R_JCT          0     /* color of the junction points */
#define G_JCT          0
#define B_JCT        255

#define R_AXS        255     /* color of the main axis and its first
				and last points */
#define G_AXS        100
#define B_AXS          0

#define R_EXT        255     /* color of the extrusion and its initial
				point */
#define G_EXT          0
#define B_EXT        255

#define DEBUG          0     /* Switch from 0 to 1 to Obtain extra
                                information related to the phenotyping
                                process. */

typedef struct ift_branch { 
  iftVoxel *point;       /* list of points along the branch. */
  iftVoxel  first, last; /* its terminal points */  
  int       npoints;     /* number of points along the branch */
} iftBranch;

typedef struct ift_panicle {
  iftBranch *primary;     /* list of primary branches */
  int        nbranches;   /* number of primary branches */
  iftBranch  mainaxis;    /* main axis */
  int       *junction;    /* addresses of the junction points in the list of main axis points */
  int       njunctions;   /* number of junction points along the main axis */
  iftVoxel  *extrusion;   /* list of points between the initial extrusion point and the first point at the main axis */
  int        nextpts;     /* number of extrusion points */
  iftImage  *skel;        /* skeleton binary image */
  int        nskelpts;    /* number of skeleton points */
  float      scale;       /* image scale in pixels per cm */
} iftPanicle;

iftImage     *iftSegmentPanicle(iftImage *orig);
iftPanicle   *iftExtractPanicleSkeleton(iftImage *bin);

iftPanicle   *iftCreatePanicleInfo(void); 
void          iftDestroyPanicleInfo(iftPanicle **pan);
iftPanicle   *iftReadPanicleInfo(char *filename);
void          iftWritePanicleInfo(iftPanicle *pan, char *filename);
void          iftPrintPanicleInfo(iftPanicle *pan);
float         iftBranchLength(iftPanicle *pan, int branch); // -1 for mainaxis
iftImage     *iftDrawPanicleInfo(iftImage *orig, iftPanicle *pan);
float         iftSegmentLength(iftVoxel *point, int first, int last); 
iftPoint      iftSegmentCenter(iftVoxel *point, int npoints);
int           iftClosestPointInSegment(iftVoxel *point, int npoints, iftPoint P);
int           iftFarthestPointInSegment(iftVoxel *point, int npoints, iftPoint P);
int           iftGetBranchJunction(iftPanicle *pan, int branch);
int           iftCenterOfMainAxis(iftPanicle *pan);
void          iftSortBranches(iftPanicle *pan);
iftVoxel      iftGetClosestSkeletonPoint(iftPanicle *pan, iftVoxel u);
void          iftInvertVoxelSequence(iftVoxel *seq, int npoints);
void          iftAddBranch(iftPanicle *pan, iftVoxel pt);
void          iftDeleteBranch(iftPanicle *pan, iftVoxel pt);

iftImage     *iftImageOfEndPoints(iftImage *skel);
iftImage     *iftImageOfExtrusion(iftPanicle *pan);
iftImage     *iftImageOfMainAxis(iftPanicle *pan);
iftImage     *iftSkeletonWithoutMainAxis(iftPanicle *pan);
iftImage     *iftSkeletonWithoutExtrusion(iftPanicle *pan);
iftImage     *iftRemoveAwns(iftImage *bin, int distance_thres);
iftImage     *iftReconnectPanicle(iftImage *orig, int brightness_thres, iftImage *label);
iftImage     *iftPanicleMask(iftImage *orig);
iftImage     *iftPanicleEDT(iftImage *orig);
iftImage     *iftSeedSegmentation(iftPanicle *pan, iftImage *orig);

#endif
