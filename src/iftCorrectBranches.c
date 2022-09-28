#include "iftPANorama.h"


/* 
   Author: Alexandre Falcao.  

   Description: This program adds/deletes branches from a .inf file,
   also updating the colored skeleton image.

*/



int main(int argc, char *argv[]) 
{
  iftImage       *orig, *panimg;
  iftPanicle     *pan;
  char            filename[400], command[600];
  iftVoxel        pt;
  timer          *t1=NULL,*t2=NULL;

  /*--------------------------------------------------------*/

  /* void *trash = malloc(1);                  */
  /* struct mallinfo info;    */
  /* int MemDinInicial, MemDinFinal; */
  /* free(trash);  */
  /* info = mallinfo(); */
  /* MemDinInicial = info.uordblks; */

  /*--------------------------------------------------------*/


  if (argc!=6)
    iftError("Usage: iftCorrectBranches <dirname> <basename> <x> <y> <sign>","main");

  // Read panicle information with its skeleton

  sprintf(filename,"%sskeletons/%s.inf",argv[1],argv[2]);
  pan = iftReadPanicleInfo(filename);
  if (pan->nbranches==0)
    iftError("First execute iftPanicleInfo","main");

  t1     = iftTic();

  pt.x = atoi(argv[3]); pt.y = atoi(argv[4]); pt.z = 0; 
  if (atoi(argv[5])>0)
    iftAddBranch(pan,pt);
  else
    iftDeleteBranch(pan,pt);

  iftWritePanicleInfo(pan,filename);
  sprintf(filename,"%soriginals/%s.jpg",argv[1],argv[2]);
  sprintf(command,"convert %s temp.pgm",filename);
  system(command);
  orig    = iftReadImageP5("temp.pgm");
  sprintf(command,"rm -f temp.pgm");
  system(command);

  panimg = iftDrawPanicleInfo(orig, pan);
  iftWriteImageP6(panimg,"temp.ppm");
  sprintf(filename,"%sskeletons/%s_skel.jpg",argv[1],argv[2]);
  sprintf(command,"convert temp.ppm %s",filename);
  system(command);
  sprintf(command,"rm -f temp.ppm");
  system(command);

  t2     = iftToc();

  fprintf(stdout,"Branch corrected in %f ms\n",iftCompTime(t1,t2));


  iftDestroyPanicleInfo(&pan);  
  iftDestroyImage(&panimg);
  iftDestroyImage(&orig);

  /* ---------------------------------------------------------- */

  /* info = mallinfo(); */
  /* MemDinFinal = info.uordblks; */
  
  /* if (MemDinInicial!=MemDinFinal) */
  /*   printf("\n\nDynamic memory was not completely deallocated (%d, %d)\n", */
  /* 	   MemDinInicial,MemDinFinal);    */
  
  return(0);
}



