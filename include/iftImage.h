#ifndef IFT_IMAGE_H_
#define IFT_IMAGE_H_

#include "iftCommon.h"
#include "iftColor.h"
#include "iftFIFO.h"
#include "iftAdjacency.h"

/**
@file
@brief iftImage definition and prototypes.
*/


typedef struct ift_image {
  int    *val;
  ushort *Cb,*Cr;
  int xsize,ysize,zsize;
  float dx,dy,dz;
  int *tby, *tbz;        // speed-up voxel access tables
  int maxval, minval, n; // minimum and maximum values, and number of voxels 
} iftImage;

typedef struct ift_dicom_info {
  int    xsize,ysize,zsize;
  float  dx,dy,dz;
  int    bitsPerVoxel;        
  char   *orientation; 
  int    min, max;
} iftDicomInfo;

#define iftGetXCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) % (s)->xsize)
#define iftGetYCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) / (s)->xsize)
#define iftGetZCoord(s,p) ((p) / (((s)->xsize)*((s)->ysize)))
#define iftGetVoxelIndex(s,v) ((v.x)+(s)->tby[(v.y)]+(s)->tbz[(v.z)])
#define iftDiagonalSize(s) (ROUND(sqrtf(s->xsize*s->xsize + s->ysize*s->ysize + s->zsize*s->zsize)))

void      iftVerifyImageDomains(iftImage *img1, iftImage *img2, char *function);
char      iftIsColorImage(iftImage *img);
char      iftIs3DImage(iftImage *img);
int       iftXSize(iftImage *img);
int       iftYSize(iftImage *img);
int       iftZSize(iftImage *img);
void      iftDestroyPyImage(iftImage *img);
iftVoxel  iftGetVoxelCoord(iftImage *img, int p);

iftImage *iftCreateImage(int xsize,int ysize,int zsize);
iftImage *iftCreateColorImage(int xsize,int ysize,int zsize);
void      iftDestroyImage(iftImage **img);
void      iftUpdateMinMax(iftImage *img);
void      iftCopyVoxelSize(iftImage *img1, iftImage *img2);
void      iftCopyCbCr(iftImage *img1, iftImage *img2);
void      iftSetCbCr(iftImage *img, ushort value);
void      iftSetCb(iftImage *img, ushort value);
void      iftSetCr(iftImage *img, ushort value);
char      iftValidVoxel(iftImage *img, iftVoxel v);
int       iftMaximumValue(iftImage *img);
int       iftMaximumCb(iftImage *img);
int       iftMaximumCr(iftImage *img);
int       iftMinimumValue(iftImage *img);
iftImage *iftReadImage(char *filename);
void      iftWriteImage(iftImage *img, char *filename);
iftImage *iftReadImageP5(char *filename);
void      iftWriteImageP5(iftImage *img, char *filename);
iftImage *iftReadImageAsP5(char *filename);
void      iftWriteImageExtFormat(iftImage *img, char *filename);
iftImage *iftReadImageAsP6(char *filename);
iftImage *iftReadImageP6(char *filename);
void      iftWriteImageP6(iftImage *img, char *filename);
iftImage *iftReadImageP2(char *filename);
void      iftWriteImageP2(iftImage *img, char *filename);
iftImage *iftExtractObject(iftImage *label, int obj_code, iftVoxel *pos);
void      iftInsertObject(iftImage *bin, iftImage *label, int obj_code, iftVoxel  pos);
iftImage  *iftCopyImage(iftImage *img);
iftImage  *iftCreateCuboid(int xsize, int ysize, int zsize);
iftImage  *iftCSVtoImage(char *filename);
char       iftAdjacentVoxels(iftImage *img, iftAdjRel *A, iftVoxel u, iftVoxel v);
iftImage  *iftImageGradientMagnitude(iftImage *img, iftAdjRel *A);
iftImage *iftAddFrame(iftImage *img, int sz, int value);
iftImage *iftRemFrame(iftImage *fimg, int sz);
void      iftSetImage(iftImage *img, int value);

iftImage *iftGetXYSlice(iftImage *img, int zcoord);
iftImage *iftGetZXSlice(iftImage *img, int ycoord);
iftImage *iftGetYZSlice(iftImage *img, int xcoord);
void      iftPutXYSlice(iftImage *img, iftImage *slice, int zcoord);
void      iftPutZXSlice(iftImage *img, iftImage *slice, int ycoord);
void      iftPutYZSlice(iftImage *img, iftImage *slice, int xcoord);

iftVoxel  iftGeometricCenterVoxel(iftImage *obj);
iftPoint  iftGeometricCenter(iftImage *obj);
int       iftObjectDiagonal(iftImage *obj);
iftImage *iftCropImage(iftImage *img, iftVoxel uo, iftVoxel uf);
void      iftGetDisplayRange(iftImage *img, int *lower, int *higher);
iftImage *iftImageCb(iftImage *img);
iftImage *iftImageCr(iftImage *img);
iftImage *iftImageRed(iftImage *img);
iftImage *iftImageGreen(iftImage *img);
iftImage *iftImageBlue(iftImage *img);
iftImage *iftImageGray(iftImage *img);
iftImage *iftCreateGaussian(int xsize, int ysize, int zsize, iftVoxel mean, float stdev, int maxval);
iftImage *iftRegionBorders(iftImage *label, int value);
iftImage *iftExtractROI(iftImage *img, iftVoxel uo, iftVoxel uf);
void      iftInsertROI(iftImage *roi, iftImage *img, iftVoxel pos);
float     iftObjectVolume(iftImage *label, int obj_code);
iftImage *iftCreateImageWithGaussianHistogram(int xsize, int ysize, int zsize, float mean, float stdev, int maxval);
iftImage *iftCreateImageWithTwoGaussiansHistogram(int xsize, int ysize, int zsize, float mean1, float stdev1, float mean2, float stdev2, int maxval);
iftImage *iftReadRawSlices(char *basename, int first, int last, int xsize, int ysize, int bits_per_voxel);
void      iftWriteRawSlices(iftImage *img, char *basename);
iftImage *iftExtractGreyObject(iftImage *image);
iftImage *iftExtractGreyObjectPos(iftImage *image, iftVoxel *pos);
iftImage *iftCrop2DImageByBorder(iftImage *img, int border);
int        iftImageValueAtPoint(iftImage *img, iftPoint P);
int        iftImageValueAtPoint2D(iftImage *img, iftPoint P);


//Converts between color spaces. This function quantizes the LAB cspace between 0 and 255.
iftImage *iftConvertColorSpace(iftImage* image, char origin_cspace, char dest_cspace);
iftDicomInfo *iftGetDicomInfo(iftNameMetricPair *orderedFiles, int size);
iftDicomInfo *iftReadDicomInfo(char *file);
void iftWriteDicomInfo(char *path, char *fileName, iftDicomInfo* dicomInfo);
void  iftDestroyDicomInfo(iftDicomInfo **dicomInfo);
iftImage *iftReadDicom(iftNameMetricPair *orderedFiles, int size, int xsize, int ysize, float dx, float dy, float dz, int bits_per_voxel);
iftImage *iftLooseImage(iftImage *image, int xsize, int ysize, int zsize);
void iftCenterImages(iftImage *image1, iftImage *image2, iftImage **centeredImage1, iftImage **centeredImage2);
iftNameMetricPair *iftSortDicomDistance(char *source, int *size);

iftImage *iftColorTableToImage(iftColorTable *ct, int xsize, int ysize);
void iftTickColorTableImage(iftImage *img, float minval, float maxval, int nticks, char *filename);

#endif


