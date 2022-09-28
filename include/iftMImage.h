#ifndef IFT_MIMAGE_H_
#define IFT_MIMAGE_H_

#include "iftCommon.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftColor.h"
#include "iftAdjacency.h"


#define iftMGetXCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) % (s)->xsize)
#define iftMGetYCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) / (s)->xsize)
#define iftMGetZCoord(s,p) ((p) / (((s)->xsize)*((s)->ysize)))
#define iftMGetVoxelIndex(s,v) ((v.x)+(s)->tby[(v.y)]+(s)->tbz[(v.z)])
#define iftMDiagonalSize(s) (ROUND(sqrtf(s->xsize*s->xsize + s->ysize*s->ysize + s->zsize*s->zsize)))

/* Multiband image: iftMImage *img; img->band[b].val[p]  */

typedef struct ift_mimage {
  iftBand *band; /* a list of images, one for each band */
  int      xsize,ysize,zsize; /* image dimensions of each band */
  float    dx,dy,dz;  /* voxel size */
  int     *tby, *tbz; /* LUT to speed up index to/from coordinate conversions */
  int      n,m; /* number of voxels and number of bands */
} iftMImage;

int         iftMXSize(iftMImage *img);
int         iftMYSize(iftMImage *img);
int         iftMZSize(iftMImage *img);
iftVoxel    iftMGetVoxelCoord(iftMImage *img, int p);
iftMImage  *iftCreateMImage(int xsize,int ysize,int zsize, int nbands);
void        iftDestroyMImage(iftMImage **img);
iftMImage  *iftCopyMImage(iftMImage *img);
char        iftMValidVoxel(iftMImage *img, iftVoxel v);
void        iftMCopyVoxelSize(iftMImage *img1, iftMImage *img2);
iftMImage  *iftImageToMImage(iftImage *img, char color_space); /* See options for color_space in iftColor.h */
iftImage   *iftMImageToImage(iftMImage *img, int Imax, int band);
char        iftIs3DMImage(iftMImage *img);

iftMImage   *iftReadMImage(char *filename);
void  		iftWriteMImage(iftMImage *mimg, char *filename);

void        iftWriteMImageBands(iftMImage *mimg, char *base_filename);

iftMImage  *iftMAddFrame(iftMImage *img, int bx, int by, int bz, float value);
iftMImage  *iftMRemFrame(iftMImage *fimg, int bx, int by, int bz);
void        iftSetMImage(iftMImage *img, float value);
iftImage   *iftEuclMImageBasins(iftMImage *img, iftAdjRel *A);
iftImage   *iftMImageBasins(iftMImage *img, iftAdjRel *A);
iftImage   *iftBorderProbImage(iftMImage *img);
iftImage   *iftRegionProbImage(iftMImage *img);
iftImage   *iftUniformProbImage(iftMImage *img);


void        iftMultMImageByScalar(iftMImage *Z, float scalar);

#endif


