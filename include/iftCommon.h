#ifndef IFT_COMMON_H_
#define IFT_COMMON_H_

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#if !defined(__APPLE__)
	#include <malloc.h>
#endif
#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <cblas.h>
#include <dirent.h>
#include <regex.h>
#include <mm_malloc.h>

/*
 * Common data types
 */


#define INFINITY_INT  INT_MAX
#define INFINITY_INT_NEG  INT_MIN
#define INFINITY_FLT  FLT_MAX
#define INFINITY_FLT_NEG  -FLT_MAX
#define INFINITY_DBL  DBL_MAX
#define INFINITY_DBL_NEG  -DBL_MAX
#define INFINITY_LDBL LDBL_MAX
#define INFINITY_LDBL_NEG -LDBL_MAX

typedef struct timeval timer;

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long long ullong;

typedef struct ift_band {
  float *val;
} iftBand;


typedef struct ift_vector {
  float x,y,z;
} iftVector, iftPoint;

typedef struct ift_voxel {
  int x,y,z;
} iftVoxel;

typedef struct ift_dcomplex
{
  double r;
  double i;
} iftComplex;


typedef struct file_list {
  char **filesRoutes;
  char **filesNames;
  int    n;
} fileList;

typedef struct ift_name_metric_pair {
  char *name;
  float metric;
} iftNameMetricPair;

/**
 * Common definitions
 */

#define IFT_RANDOM_SEED (unsigned int) 213344
#define MAXWEIGHT     4095.0
#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define PI          3.1415926536
#define INTERIOR    0
#define EXTERIOR    1
#define BOTH        2
#define WHITE       0
#define GRAY        1
#define BLACK       2
#define NIL        -1
#define INCREASING  1
#define DECREASING  0
#define Epsilon     1E-05

/**
 * Common operations
 */

#ifndef MAX
#define MAX(x,y) (((x) > (y))?(x):(y))
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y))?(x):(y))
#endif

#define ROUND(x) ((x < 0)?(int)(x-0.5):(int)(x+0.5))
#define SIGN(x) ((x >= 0)?1:-1)


/**
 * Common functions to allocate memory
 */
#define IFT_MEMORY_ALIGNMENT 16

char     *iftAllocCharArray(int n);
uchar    *iftAllocUCharArray(int n);
short    *iftAllocShortArray(int n);
ushort   *iftAllocUShortArray(int n);
uint     *iftAllocUIntArray(int n);
ullong   *iftAllocULLongArray(int n);
int      *iftAllocIntArray(int n);
float    *iftAllocFloatArray(int n);
double   *iftAllocDoubleArray(int n);
iftComplex  *iftAllocComplexArray(int n);
long double *iftAllocLongDoubleArray(int n);

// Aligned memory allocation
int *iftAllocAlignedIntArray(int n, int alignment);
float *iftAllocAlignedFloatArray(int n, int alignment);
uchar *iftAllocAlignedUCharArray(int n, int alignment);
double *iftAllocAlignedDoubleArray(int n, int alignment);


void      iftPrintFloatArray(float* v, int n);
/**
 * Error messages
 */

#define MSG1  "Cannot allocate memory space"
#define MSG2  "Cannot open file"

/**
 *  Error message msg is printed in function func and the program
 *  exits abnormally.
 */

void iftError(char *msg,char *func);

/**
 *  Warning message msg is printed in function func and the program
 *  continues.
 */

void iftWarning(char *msg,char *func);

/**
 *  The contents of a and b are interchanged.
 */

void iftSwitchValues(int *a, int *b);
void iftSSwitchValues(char *a, char *b, int size);
void iftFSwitchValues(float *a, float *b);
void iftDSwitchValues(double *a, double *b);
void iftSwitchVoxels(iftVoxel *u, iftVoxel *v);



/**
 * Returns a random integer number between low and high.
 */

int iftRandomInteger (int low, int high);

/*
 * Randomly selects nelems of the set [low, high]
 */
int *iftRandomIntegers (int low, int high, int nelems);

/**
 * Randomly selects a normal distributed (N(0,1)) float number
 */

float iftRandomNormalFloat(void);

/**
 * Returns the distance between P0 and the line from P1 to P2, whose
 * size is P1P2
 */

float iftVoxelLineDist2D(iftVoxel P0, iftVoxel P1, iftVoxel P2, float P1P2);

iftVoxel iftClosestVoxelOnLine2D(iftVoxel P0, iftVoxel P1, iftVoxel P2);


/**
 * Returns the position of P0 with respect to the line from P1 to
 * P2. Negative values indicate left side, 0 indicates on the line,
 * and positive values indicate right side.
 */

int iftVoxelLinePosition2D (iftVoxel P0, iftVoxel P1, iftVoxel P2);

/**
 * Returns initial time
 */

timer *iftTic(void);

/**
 * Returns final time
 */

timer *iftToc(void);

/**
 * Computes the difference in ms from the initial time to the final time
 */

float iftCompTime(timer *tic, timer *toc);

/**
 * Generates seed for rand(), used in iftRandomInteger.
 */

void iftRandomSeed(unsigned int);

/**
 * Returns the factorial of a number or NIL in case of overflow
 */

long double iftFactorial(int n);

/**
 * Returns the limit to avoid overflow in factorial computation
 */

int iftFactorialLimit(void);

float iftVoxelDistance(iftVoxel u, iftVoxel v);
int   iftSquaredVoxelDistance(iftVoxel u, iftVoxel v);

float iftPointDistance(iftPoint u, iftPoint v);


float iftInnerProduct(iftVector a, iftVector b);

iftVector iftCrossProduct(iftVector a, iftVector b);

char iftCollinearPoints(iftPoint P1, iftPoint P2, iftPoint P3);

char iftCollinearVoxels(iftVoxel P1, iftVoxel P2, iftVoxel P3);

iftVector iftNormalizeVector(iftVector v);

void iftNormalizeFloatArray(float *array, int nelems);

float iftVectorMagnitude(iftVector v);

iftVoxel iftVoxelSum(iftVoxel v, iftVoxel v1);
iftVoxel iftVoxelDivByScalar(iftVoxel v, int scalar);

iftVector iftVectorFromVoxels(iftVoxel v, iftVoxel center);
iftVoxel iftVectorToVoxel(iftVector vec, iftVoxel center);
iftVector iftVectorScalarProduct(iftVector vec, float s);
iftVector iftVectorScalarSum(iftVector vec, float s);
iftVector iftVectorScalarDivision(iftVector vec, float s);
iftVector iftVectorSum(iftVector vec1, iftVector vec2);
iftVector iftVectorSubtraction(iftVector vec1, iftVector vec2);
iftVector iftProjectVectorUontoV(iftVector U, iftVector V);

void iftRemoveCarriageReturn(char *token); /* useful to get rid of the
					      carriage return and the
					      line feed characteres
					      introduced by DOS
					      systems when reading
					      strings from ASCII
					      files */


void   iftWriteFloatArray(float *v, int size, char *filename);
float *iftReadFloatArray(char *filename, int *size);


/**
 * Evaluates the sigmoid function, with x = value.
 * Alfa controls the decay of the function.
 */

float iftSigmoidalValue(float value, float alfa);

void iftVerifyToken(FILE *fp, char *token, char *function);
void iftReadIntValue(FILE *fp, int *value, char *token, char *function);
void iftReadIntValues(FILE *fp, int **value, int nvalues, char *token, char *function);
void iftWriteIntValue(FILE *fp, int value, char *token);
void iftWriteIntValues(FILE *fp, int *value, int nvalues, char *token);
void iftReadFloatValue(FILE *fp, float *value, char *token, char *function);
void iftReadFloatValues(FILE *fp, float **value, int nvalues, char *token, char *function);
void iftWriteFloatValue(FILE *fp, float value, char *token);
void iftWriteFloatValues(FILE *fp, float *value, int nvalues, char *token);
void iftReadDoubleValue(FILE *fp, double *value, char *token, char *function);
void iftReadDoubleValues(FILE *fp, double **value, int nvalues, char *token, char *function);
void iftWriteDoubleValue(FILE *fp, double value, char *token);
void iftWriteDoubleValues(FILE *fp, double *value, int nvalues, char *token);
void iftSkipComments(FILE *fp);
char iftVoxelsAreEqual(iftVoxel u1, iftVoxel u2);
char iftPointsAreEqual(iftPoint u1, iftPoint u2);

/**
 * Common function to handle arrays
 */

void iftCopyFloatArray(float *array1, float *array2, int nelems);
void iftCopyIntArray(int *array1, int *array2, int nelems);
int *iftMergeIntArray(int *array1, int n1, int *array2, int n2, int *nelems);
int *iftIntArrayOfUniqueElemsTransform(int *array, int *n);
fileList             *iftCreateFileList(void);
void                 iftDestroyFileList(fileList **list);
fileList             *iftGetFiles(char *dirname, char *type);

/* These functions are currently used to communicate with numpy */
void iftWriteRawIntArray(char *filename, int *array, int n);
int* iftReadRawIntArray(char *filename, int n);

float iftMean(float *x, int n);
float iftVar(float *x, int n);
float iftCov(float *x, float *y, int n);

int iftAlmostZero(float x);
int iftSafeMod(int a, int n);

#endif
