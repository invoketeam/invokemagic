
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
   xImage();
   virtual ~xImage();


   void clear();
   void init(int w, int h);
   int loadImage(std::string  fname);

   void mirVert(); 
   void endianSwap();


public:
   void setPixel(int x, int y, unsigned int c); 
   void fill(unsigned int c);
   void drawRect(int x, int y, int w, int h, unsigned int c); 
   void xorFill();
   void blendImage(xImage * src, int dx, int dy, unsigned char op=255);
   void drawImage(xImage * src, int dx, int dy);

};//classend


