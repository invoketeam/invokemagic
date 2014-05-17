#pragma once


#include "xMdx3.h"

class xMultiMdx3
{
public:
  int numMesh;
  xMdx3 * vecMesh;
  
public:
   xMultiMdx3(void);
  ~xMultiMdx3(void);
 
 void clear(void);
 void loadFile(std::string fname); 
 
 xMdx3 * getMesh(int i);
 
};//classend