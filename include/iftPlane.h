#ifndef IFT_PLANE_H_
#define IFT_PLANE_H_

#include "iftCommon.h"
#include "iftMatrix.h"

typedef struct ift_plane {
  iftPoint  pos;    // reference position
  iftVector normal; // normal vector which gives its orientation
} iftPlane;

iftPlane *iftCreatePlane(void);
void iftDestroyPlane(iftPlane **pl);
void iftTranslatePlane(iftPlane *pl, float Tx, float Ty, float Tz);
void iftRotatePlane(iftPlane *pl, char axis, float theta); 
void iftSetPlanePos(iftPlane *pl, float x, float y, float z);
void iftSetPlaneOrient(iftPlane *pl, float Nx, float Ny, float Nz);
iftPlane *iftDefinePlaneFromPoints(iftPoint P1, iftPoint P2, iftPoint P3);
iftPlane *iftDefinePlaneFromVoxels(iftVoxel P1, iftVoxel P2, iftVoxel P3);
char iftPlaneSide(iftPlane *pl, iftPoint P);


#endif
