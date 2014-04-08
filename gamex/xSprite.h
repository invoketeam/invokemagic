
#pragma once

#include <vector>
#include <string>

#include "xTexture.h"
#include "xStore.h"


class xSprite
{
public:
  std::string wname;
  std::string skin;
  float x, y, w, h;
  float u0,v0, u1,v1;
  unsigned int handle;
public:
  xSprite();

};//xsprite

typedef std::vector  <xSprite * > tdVecSprite;




class xSpriteMan
{
public:
  xStore <xSprite> storeSprite;
  xStore <xTexture> storeSkin;

public:
  xSpriteMan(void);
  ~xSpriteMan(void);

  void clear(void);


  void addSkin(xTexture * skin, std::string wname);
  void addSkin(std::string fname);
  void addSprite(std::string fname);

  xTexture * getTex(std::string wname);
  unsigned int getSkin(std::string wname);
  xSprite * getSprite(std::string wname);

  void drawSprite(xSprite * a, float ax, float ay, float ang = 0.0f);
  void drawImage(xSprite * a, float ax, float ay);


};//xspriteman