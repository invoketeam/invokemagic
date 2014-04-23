#pragma once

#include <string>
#include <stdio.h>
#include <vector>
//#include <map>
#include <algorithm>


#include "xMath.h"



//size 20 +16+ -- 36  bytes
struct mVert
{
  gamex::cVec3f pos;
  float u, v;
  float u2, v2;
  int norm; //compressed normal
  int rgba; //color
};//mvert



class xMdx3
{
public:
  int numFace;
  int numVert;
  
  gamex::cVec3f min;
  gamex::cVec3f max;
  gamex::cVec3f off;
  
  float rad;
  
  int * vecIndex;
  mVert * vecVert;

  int drawFace; //number of faces to draw (used by renderer)

  unsigned int indexBuf;
  unsigned int vertBuf;
  
  bool hasVbo; //todo -- test if indexBuf != 0 instead?

public:
   xMdx3();
  ~xMdx3();
 
 void clear(void);

 void calcMinMax(void);

 void renderVert(int num);
 void render(void); //debug render


 void initBox(float scale = 1.0f);
 void initPlane(float scale = 1.0f, float u0=0.0f, float v0=0.0f, float u1=1.0f, float v1=1.0f);
  
 void loadFile(std::string fname); 
 void readFile(FILE * file);

 void moveMesh(float mx, float my, float mz);
 void scaleMesh(float sx, float sy, float sz);
 


 void clearVbo(void);
 void makeVbo(void);
 void updateVboVertex(void);

 
};//classend