#pragma once


#include "../../gamex/xTexture.h"
#include "../../gamex/xImage.h"

class xWorld;

class xMiniMap
{
public:
  xTexture skin;
  xImage img;

  //store --
  //images for houses, castles, etc;
  //teamcolors, etc

public:
  void init(void);

  //todo -- provide   team and flags to consider (?)
  void updateImage(xWorld * world);

  //todo -- render method (?)
  
};//xminimap