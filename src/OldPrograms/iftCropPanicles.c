#include "ift.h"

int main(int argc, char *argv[]) 
{
  iftImage *img1=NULL,*img2=NULL;
  iftVoxel  uo,uf;
  char filename[100], command[150];

  /*--------------------------------------------------------*/

  void *trash = malloc(1);                 
  struct mallinfo info;   
  int MemDinInicial, MemDinFinal;
  free(trash); 
  info = mallinfo();
  MemDinInicial = info.uordblks;

  /*--------------------------------------------------------*/

  if (argc!=2)
    iftError("Usage: iftCropPanicles <basename>","main");

  // Read image 

  sprintf(filename,"%s.gif",argv[1]);
  sprintf(command,"convert %s temp.ppm",filename);
  system(command);
  img1  = iftReadImageP6("temp.ppm");
  sprintf(command,"rm -f temp.ppm");
  system(command);

  // Crop image

  uo.x   = 50; uf.x = 2500;
  uo.y   =  0; uf.y = 3700;
  uo.z   =  0; uf.z =    0;  
  img2   = iftCropImage(img1,uo,uf);
    
  // Overwrite old image

  iftWriteImageP6(img2,"temp.ppm");
  sprintf(filename,"%s.gif",argv[1]);
  sprintf(command,"convert temp.ppm %s",filename);
  system(command);
  sprintf(command,"rm -f temp.ppm");
  system(command);

  // Deallocate memory

  iftDestroyImage(&img1);
  iftDestroyImage(&img2);

  /* ---------------------------------------------------------- */

  info = mallinfo();
  MemDinFinal = info.uordblks;
  if (MemDinInicial!=MemDinFinal)
    printf("\n\nDinamic memory was not completely deallocated (%d, %d)\n",
	   MemDinInicial,MemDinFinal);   

  return(0);
}



