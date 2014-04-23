
#pragma once

#include "xGLCommon.h"
#include "oglExt.h"

#include "xMath.h"
#include "xMdx3.h"




//entity
class xEnt
{
public:
  xMdx3 * fmesh; //facemesh
  xMdx3 * vmesh; //vertmesh
  unsigned int skin;
  unsigned int skin2;

  int skinBlend;
  int skin2Blend;

  int sortCode; 
  int blend; //0 solid   1 alpha test   2 transparent   3 additive
  
  //note -- sortcode is used as if it was unsigned
  //so a -1 is the highest, 0 is the lowest (afaik)

  int twoSide; //2 sided
  int useColor; //use color array from vmesh

  //color is 4 floats -- probably not the best idea
  gamex::cVec3f color;
  float alpha; 

  gamex::cVec3f pos; //translate

  gamex::cVec3f sortpos; //sort by this position

  int debid; //debug -- was used to test a bug -- kept as a reminder of it
public:
  xEnt(void);


};//xentity


//renderbucket 
//using different buckets
//for different kind of sorting codes 
//(solid is sorted by texture, transparent by distance to camera)

typedef xEnt * tdEntPtr;
class xBucket
{
public:
  tdEntPtr * vecEnt;
  int numEnt;
  int it;

public:
  xBucket(void);
  ~xBucket(void);


  void clear(void);

  void init(int maxEnt = 16384);

  xEnt * addFrame(void);

  void render(void);





public:
 void calcSort_Skin(void);
 void calcSort_CamDist(gamex::cVec3f cpos);



public:
 xEnt * tempVec[32768]; //todo -- set max from variable
 int bucket[256];

  void sortEnt(void);

  void radixSort(xEnt ** vec, int num);


};//xbucket





class xRender
{
public:
  xBucket vecBucket[4];
  int numBucket;

public:
  gamex::cQuat camOri;
  gamex::cVec3f camPos;
  gamex::cMat camMat;

public:
  xRender(void);
  ~xRender(void);

  void clear();

  void setCam(gamex::cVec3f &pos, gamex::cQuat &ori);


  //maxent is per bucket
  void init(int maxEnt = 16384);

  xEnt * addFrame(int bucket = 0);


  void resetFrame(void);

  void render(bool bSort = true);

};//xrender

