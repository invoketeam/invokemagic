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
  //short numTarg;
  //short numLod;
  
  gamex::cVec3f min;
  gamex::cVec3f max;
  gamex::cVec3f off;
  
  float rad;
  
  short * vecIndex;
  mVert * vecVert;

  
  bool hasVbo;
  unsigned int indexBuf;
  unsigned int vertBuf;


public:
   xMdx3();
  ~xMdx3();
 
 void clear(void);

 void calcMinMax(void);

// void renderVert(int num);
// void render(void);
 //void render2(gamex::cVec3f * altVert);

 void initBox(float scale = 1.0f);
  
 void loadFile(std::string fname); 

 void moveMesh(float mx, float my, float mz);
 void scaleMesh(float sx, float sy, float sz);


 //void clearVbo(void);
 //void makeVbo(void);
 //void updateVboVertex(void);

 
};//classend