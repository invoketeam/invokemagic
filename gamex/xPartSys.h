
#pragma once

#include <vector>

#include "xMath.h"
#include "xMdx3.h"
#include "xSprite.h"
#include "xStore.h"


#define XPART_MAXPART 4096



class xPart
{
public:
  int sortCode;
  gamex::cVec3f pos;
  gamex::cVec3f vel;
  float size;
  float hp;
  float ang;

  int image;
  float startFrame;
  float endFrame;
  float animSpeed;
  float curFrame;

  float fade_size;
  float ang_speed;

public:
  xPart(void);

};//xpart


class xPartSprite
{
public:
  tdVecSprite vecSprite;
  unsigned int skin;
  xStore <xPart> storeAnim;

public:
  xPartSprite(void);
  void addSprite(xSprite * a); //todo -- no longer needed/used
  //todo -- anims 
  //just an array of xPart? name lookup?

  //wname: name of anim  frames: "frame1, frame2, frame3, frame4"
  xPart * addAnim(xSpriteMan * sman, std::string wname, float speed, std::string frames);
  xPart * getAnim(std::string wname);

};//xpartspr



class xPartSys
{
public:
  xMdx3 mesh;
  xPartSprite * sprAnim;
  unsigned int skin;
public:
  xPart * vecPart; //only used to keep particles in a single connected chunk of memory (not sure if it helps much)
  xPart ** pindex; 

  int numPart; //used as number of alive parts
  int maxPart;
  int it; 

public:
  xPartSys(void);

  ~xPartSys(void);

  void clear(void);

  void initPart(int maxp = 64);

  xPart * addPart(float cx, float cy, float cz);
  xPart * addPart2(std::string wanim, float cx, float cy, float cz);

  //update particles
  void update(void); 

  //update mesh for rendering
  //mat is expected to be the  view matrix
  void updateMesh(float * mat);

  //p  camera pos, n  camera norm
  void sortPart(gamex::cVec3f p);

  //debug render
  void render(void);

};//xpartsys

