#pragma once


#include "xMdx3.h"

class xMultiMdx3
{
public:
  int numMesh;
  xMdx3 * vecMesh;

  gamex::cVec3f min, max, size;
  
public:
   xMultiMdx3(void);
  ~xMultiMdx3(void);
 
 void clear(void);
 void loadFile(std::string fname); 
 void calcMinMax(void);
 
 xMdx3 * getMesh(int i);
 
};//classend