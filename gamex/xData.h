#pragma once

#include <string>

#include "xTexture.h"
#include "xSprite.h"
#include "xMdx3.h"
#include "xStore.h"



class xData 
{
public:
 xSpriteMan myMan;
 xStore <xMdx3> storeMesh;
 xStore <xTexture> storeSkin;

public:
  xData(void);
  ~xData(void);
  void clear(void);
 
public:
  void addMdx(std::string fname, std::string altName);
  void addSkin(std::string fname, std::string altName, bool mip);
  
public:
  xMdx3 * getMdx(std::string wname);
  xTexture * getTex(std::string wname);  
  unsigned int getSkin(std::string wname);
  xSprite * getSprite(std::string wname);


};//xdata