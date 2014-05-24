
#pragma once

#include <vector>

#include "xMath.h"
#include "xMdx3.h"
#include "xSprite.h"



class xPartSprite
{
public:
  tdVecSprite vecSprite;

public:
  void addSprite(xSprite * a);

};//xpartspr



class xPart
{
public:
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



class xPartSys
{
public:
  xMdx3 mesh;
  xPartSprite * sprAnim;
public:
//  int numFace;
//  int numVert;
// short * vecIndex; 
//  mVert * vecVert;
  xPart * vecPart;

  int numPart; //used as number of alive parts
  int maxPart;
  int it; 

public:
  xPartSys(void);

  ~xPartSys(void);

  void clear(void);

  void initPart(int maxp = 64);

  xPart * addPart(float cx, float cy, float cz);

  //update particles
  void update(void); 

  //update mesh for rendering
  //mat is expected to be the  view matrix
  void updateMesh(float * mat);


  //debug render
  void render(void);

};//xpartsys

