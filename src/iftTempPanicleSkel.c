#include "iftPANorama.h"


/* 
   Author: Alexandre Falcao.  

   Description: This program computes the skeleton of a panicle image. 

*/

int main(int argc, char *argv[]) 
{
  iftImage       *orig=NULL,*bin=NULL,*panimg=NULL;
  iftPanicle     *pan=NULL;
  char            filename[400],command[500],*basename;
  timer          *t1=NULL,*t2=NULL;

  /*--------------------------------------------------------*/

  /* void *trash = malloc(1);                  */
  /* struct mallinfo info;    */
  /* int MemDinInicial, MemDinFinal; */
  /* free(trash);  */
  /* info = mallinfo(); */
  /* MemDinInicial = info.uordblks; */

  /*--------------------------------------------------------*/

  if (argc!=2)
    iftError("Usage: iftTempPanicleSkel <image.jpg>","main");

  /* Converting jpg image into a grayscale pgm image */

  basename = strtok(argv[1],".");
  sprintf(filename,"%s.jpg",basename);
  sprintf(command,"convert %s temp.pgm",filename);
  system(command);
  orig  = iftReadImageP5("temp.pgm");
  sprintf(command,"rm -f temp.pgm");
  system(command);


  t1     = iftTic();

  /* Segment the panicle */

  bin    = iftSegmentPanicle(orig);


  t2     = iftToc();

  fprintf(stdout,"segmentation in %f ms\n",iftCompTime(t1,t2));


  /* Compute the panicle's skeleton */

  t1     = iftTic();

  pan    = iftExtractPanicleSkeleton(bin);


  t2     = iftToc();

  fprintf(stdout,"skeletonization in %f ms\n",iftCompTime(t1,t2));

  /* Draw panicle information (skeleton only) */

  t1     = iftTic();

  panimg = iftDrawPanicleInfo(orig, pan);


  t2     = iftToc();

  fprintf(stdout,"skeleton drawn in %f ms\n",iftCompTime(t1,t2));
  
  iftWriteImageP6(panimg,"temp.ppm");
  sprintf(command,"convert temp.ppm %s_skel.jpg",basename);
  system(command);
  sprintf(command,"rm -f temp.ppm");
  system(command);
  
  iftDestroyImage(&panimg);
  iftDestroyPanicleInfo(&pan);
  iftDestroyImage(&bin);
  iftDestroyImage(&orig);

  /* ---------------------------------------------------------- */

  /* info = mallinfo(); */
  /* MemDinFinal = info.uordblks; */
  
  /* if (MemDinInicial!=MemDinFinal) */
  /*   printf("\n\nDynamic memory was not completely deallocated (%d, %d)\n", */
  /* 	   MemDinInicial,MemDinFinal);    */
  
  return(0);
}


