#pragma once

#include <string>

#include "xTexture.h"
#include "xSprite.h"
#include "xMdx3.h"
#include "xStore.h"



class xData 
{
public:
 xSpriteMan myMan;  //also store skins
 xStore <xMdx3> storeMesh;

  //todo -- store fonts, store skeletons and animations

public:
  xData(void);
  ~xData(void);
  void clear(void);
 
public:
  xMdx3 * addMdx(std::string fname, std::string altName);
  xTexture * addSkin(std::string fname, std::string altName, bool mip);
  
public:
  xMdx3 * getMdx(std::string wname);
  xTexture * getTex(std::string wname);  
  unsigned int getSkin(std::string wname);
  xSprite * getSprite(std::string wname);


};//xdata