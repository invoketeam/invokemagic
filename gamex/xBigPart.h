#pragma once

#include "xMath.h"
#include "xMdx3.h"
#include "xSprite.h"
#include "xRender.h"



class xBigPart
{
public:
  xMdx3 mesh;
  xAnim * anim;
  
  float hp;

  unsigned int skin;
  
  gamex::cVec3f pos;
  gamex::cVec3f vel;
  
 
  float curFrame;
  float endFrame;
    float startFrame;
  float animSpeed;
  int lastFrame;

  int repeat;
  
  gamex::cVec3f rgb;
  float alpha;
  float fade;

  float scale;
  float grow;

  float roll;
  float angSpeed;
    
public:
  xBigPart(void);
    
};//bigpart







#define XBIGPART_MAXPART 256


class xBigPartSys
{
public:
  xSpriteMan spriteMan;
  xAssetMan * assetMan; 

  xBigPart vecPart[XBIGPART_MAXPART];
  int it;
  
public:
  xBigPartSys(void);
  ~xBigPartSys(void);
 
  void clear(void);
  void init(void);
  void killAllPart(void);
 
  void loadSprite(std::string wname);
  void loadAnim(std::string wname);
  

  xBigPart * addPart(std::string wanim);

  void update(void);
  void render(xRender * rend, gamex::cQuat camori);

  void debRender(gamex::cQuat camori);

};//xbigpart



