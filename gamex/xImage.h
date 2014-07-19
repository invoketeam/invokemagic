
#pragma once

#include <string>

#include <stdio.h>
#include <cstring>




class xImage
{
public:
  int mw, mh;
  unsigned int * dat;
  
  //ext
  int x, y;
  std::string wname;
public:
 

  // int width, height, bit;
 //   unsigned char * data;
//    unsigned int data;

public:
   xImage(void);
   virtual ~xImage(void);


   void clear(void);
   void init(int w, int h);
   int loadImage(std::string  fname, void * mem=0, int memsize=0);
   int loadMem(void * mem);

   void mirVert(void); 
   void endianSwap(void);


public:
   void setPixel(int x, int y, unsigned int c);
   unsigned int getPixel32(int x, int y);
   int getAlpha(int x, int y);
   void fillImage(unsigned int c);       //clear the image to color c
   void drawRect(int x, int y, int w, int h, unsigned int c); 
   void fillRect(int dx, int dy, int dw, int dh, unsigned int c);
   void xorFill(void);  //clear image to a xor texture 
   void blendImage(xImage * src, int dx, int dy, unsigned char op=255);
   void drawImage(xImage * src, int dx, int dy);

   void makeOpaqueABGR(void); //set all alpha value to 255 (fix for screenshots)


public:
  //template -- does nothing
  void extTemp(void);
  
  void extReduceColor(int f = 32);
  void extRemoveRGB(void);

  //src needs to be the exact same size 
  void extReplaceAlpha(xImage * src);

public:
  void resize(xImage * src, int w, int h);
  //todo -- resample (?)

};//classend


