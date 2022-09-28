#ifndef IFT_COLOR_H_
#define IFT_COLOR_H_

#include "iftCommon.h"

#define YCbCr_CSPACE 0
#define RGB_CSPACE   1
#define GRAY_CSPACE  2
#define WEIGHTED_YCbCr_CSPACE 3
#define LAB_CSPACE 4

#define WHITEPOINT_X	0.950456
#define WHITEPOINT_Y	1.0
#define WHITEPOINT_Z	1.088754

#define LABF(t)	\
	((t >= 8.85645167903563082e-3) ? \
	pow(t,0.333333333333333) : (841.0/108.0)*(t) + (4.0/29.0))

#define LABINVF(t)	\
	((t >= 0.206896551724137931) ? \
	((t)*(t)*(t)) : (108.0/841.0)*((t) - (4.0/29.0)))


typedef struct ift_fcolor {
	float val[3];
} iftFColor;


typedef struct ift_color {
  int val[3];
} iftColor;

typedef struct ift_colortable {
  iftColor *color;
  int ncolors;
} iftColorTable;

iftColorTable *iftCreateColorTable(int ncolors);
void iftDestroyColorTable(iftColorTable **ctb);
iftColorTable *iftBlueToRedColorTable(int ncolors);

iftColor iftRGBtoYCbCr(iftColor cin);
iftColor iftYCbCrtoRGB(iftColor cin);

iftFColor iftRGBtoLab(iftColor rgb);
iftColor iftLabtoRGB(iftFColor lab);

iftColor iftLabtoQLab(iftFColor lab);
iftFColor iftQLabToLab(iftColor qlab);

#endif
