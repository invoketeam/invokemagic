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


 void initEmpty(int num); //empty mesh with num faces (3 vert per face)
 void initBox(float scale = 1.0f);
 void initPlaneXZ(float scale = 1.0f, float u0=0.0f, float v0=0.0f, float u1=1.0f, float v1=1.0f);
 void initPlaneXY(float scale = 1.0f, float u0=0.0f, float v0=0.0f, float u1=1.0f, float v1=1.0f);

  
 void loadFile(std::string fname); 
 void readFile(FILE * file);

 void loadMem(void * mem, int i, int memsize);
 int readMem(void * mem, int i, int memsize);

 //warning: a copy is a deepcopy, doesnt have vbo by default (even if the copied has)
 void copyMesh(xMdx3 * mesh); 

 void moveMesh(float mx, float my, float mz);
 void scaleMesh(float sx, float sy, float sz);
 void transMesh(float * mat);
 //todo -- apply matrix to mesh (vertices and/or uv)

 void projectUvOrtho(float * mat, float w, float h, int maxv);
 void planarUvXZ(gamex::cVec3f smin, gamex::cVec3f smax, int maxv, float * mat = 0);
 //void projectBox(gamex::cVec3f p, float r, int maxv);

 void scaleUV(float sx, float sy);
 void moveUV(float mx, float my);
 

 void clearVbo(void);
 void makeVbo(void);
 void updateVboVertex(void);

 
};//classend