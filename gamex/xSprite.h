
#pragma once

#include <vector>
#include <string>

#include "xTexture.h"
#include "xStore.h"

#include "xAsset.h"


class xSprite
{
public:
  std::string wname;
  std::string skin;
  float x, y, w, h;
  float u0,v0, u1,v1;
  unsigned int handle;
public:
  xSprite(void);

};//xsprite



typedef std::vector  <xSprite * > tdVecSprite;


class xSpriteMan
{
public:
  xStore <xSprite> storeSprite;
  //xStore <xTexture> storeSkin;
  xAssetMan * assetMan;

public:
  xSpriteMan(void);
  ~xSpriteMan(void);

  void clear(void);


  //add xms file as sprite
  void addSpriteXms(std::string wname);
  
  //add single texture as sprite
  void addSpriteTex(std::string wname);


  xSprite * getSprite(std::string wname);

  void drawSprite(xSprite * a, float ax, float ay, float ang = 0.0f);
  void drawImage(xSprite * a, float ax, float ay);


};//xspriteman