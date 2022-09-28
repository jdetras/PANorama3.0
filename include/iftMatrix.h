#ifndef _IFTMATRIX_H_
#define _IFTMATRIX_H_

#include "iftCommon.h"
#include <stdarg.h>


typedef struct ift_lmatrix {
  double *val;                  /* matrix's values */
  int ncols, nrows, *tbrow;     /* number of columns, number of rows,
				   and look-up table to speed up index
				   access */
  int n;                        /* ncols * nrows */
} iftMatrix;

#define iftGetMatrixCol(m,i) ((i) % (m)->ncols)
#define iftGetMatrixRow(m,i) ((i) / (m)->ncols)
#define iftGetMatrixIndex(m,c,r) ((c)+(m)->tbrow[(r)])

iftMatrix *iftCreateMatrix(int ncols, int nrows);
iftMatrix *iftCopyMatrix(iftMatrix *A);
void        iftDestroyMatrix(iftMatrix **M);
iftMatrix *iftReadMatrix(char *filename);
void        iftWriteMatrix(iftMatrix *M, char *filename);
void        iftPrintMatrix(iftMatrix *M);
iftMatrix *iftMultMatrices(iftMatrix *A, iftMatrix *B);
iftMatrix *iftComputeTransformation(int n_args, ...);
void       iftMultMatrixByScalar(iftMatrix *A, double scalar );
iftMatrix *iftInvertMatrix(iftMatrix *A);
iftMatrix *iftTransposeMatrix(iftMatrix *A);
double     iftMatrixDeterminant(iftMatrix *A);
iftMatrix *iftCoFactorMatrix(iftMatrix *A);
iftMatrix *iftRotationMatrix(char axis, float theta); 
iftMatrix *iftTranslationMatrix(iftVector T);
iftMatrix *iftScaleMatrix(float sx, float sy, float sz);
iftVector  iftTransformVector(iftMatrix *A, iftVector u);
iftPoint   iftTransformPoint(iftMatrix *A, iftPoint  u);
iftVoxel   iftTransformVoxel(iftMatrix  *A, iftVoxel  u);
iftMatrix *iftIdentityMatrix(int ncols);
iftMatrix *iftRotationMatrixToAlignVectorWithZ(iftVector v);
iftMatrix *iftRotationMatrixToAlignZWithVector(iftVector v);
iftMatrix *iftExtendMatrix(iftMatrix *M, int ncols, int nrows);
void       iftSingleValueDecomp(iftMatrix *A, iftMatrix **U, iftMatrix **S, iftMatrix **Vt);
void       iftNormalizeMatrix(iftMatrix *M);
double     iftFrobeniusNorm(iftMatrix *M);
iftMatrix  *iftReadMatrixFILE(FILE *fp);
void 	   iftWriteMatrixFILE(iftMatrix *M, FILE *fp);



#endif
