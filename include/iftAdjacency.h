#ifndef IFT_ADJACENCY_H_
#define IFT_ADJACENCY_H_

#include "iftCommon.h"

/*  Adjacency relation in 3D */

typedef struct ift_adjrel {
  int *dx,*dy,*dz; /* displacements to achieve the n adjacent voxels. */
  int  n; /* number of adjacent voxels. */
} iftAdjRel;

/* Adjacency relation to be used in faster implementations that
   disconsider the image's border */

typedef struct ift_fastadjrel {
  int  n;          /* number of adjacent voxels */
  int *dq;         /* displacements to reach adjacent voxels for a given image */
  int  bx, by, bz; /* sizes of the image's border to be disconsidered */
} iftFastAdjRel;



iftAdjRel  *iftCreateAdjRel(int n); /* Allocates memory for a 3D
				      adjacency relation */

void        iftDestroyAdjRel(iftAdjRel **A); /* Deallocates memory for a
					     3D adjacency relation */

iftAdjRel  *iftSpheric(float r); /* Creates a 3D ball of radius r as
				 adjacency relation */
iftAdjRel  *iftHemispheric(float r, char axis, int direction); /* \brief Creates a 3D half-ball of radius r as
                                                                adjacency relation, in the corresponding axis and direction.
                                                                This adjacency is useful for segmenting a volume in a single
                                                                direction, e.g., a video-volume where z is time. */
iftAdjRel  *iftSphericEdges(float r);

iftAdjRel  *iftCircular(float r); /* Creates a 2D ball of radius r on
				 the xy plane as adjacency relation */

iftAdjRel  *iftCircularEdges(float r);

iftAdjRel  *iftClockCircular(float r); /* Creates a 2D ball of radius
				 r on the xy plane as adjacency
				 relation for contour pixel
				 labeling */

iftAdjRel *iftRightSide(iftAdjRel *A, float r);

iftAdjRel *iftLeftSide(iftAdjRel *A, float r);

iftAdjRel *iftRectangular(int xsize, int ysize);

iftAdjRel *iftCuboid(int xsize, int ysize, int zsize);

iftAdjRel *iftCopyAdjacency(iftAdjRel *A);

iftFastAdjRel *iftCreateFastAdjRel(iftAdjRel *A, int *tby, int *tbz); /* create an adjacency relation to speed up implementations for a given image by computing the displacements to the adjaceny voxels based on the look-up tables tby and tbz of the image. The fast implementation must disconsider the image's border */

void           iftDestroyFastAdjRel(iftFastAdjRel **F);

void iftMaxAdjShifts(iftAdjRel *A, int *dx, int *dy, int *dz);

void iftWriteAdjRel(iftAdjRel *A, char* filename);
iftAdjRel* iftReadAdjRel(char* filename);


iftVoxel  iftGetAdjacentVoxel(iftAdjRel *A, iftVoxel u, int adj);

int iftIsAdjRel3D(iftAdjRel *A);

#endif
