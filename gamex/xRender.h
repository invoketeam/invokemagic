
#pragma once

#include "xGLCommon.h"
#include "oglExt.h"

#include "xMath.h"
#include "xMdx3.h"




//entity
class xEnt
{
public: 
  int sortCode; 
  
  //note -- sortcode is used as if it was unsigned
  //so a -1 is the highest, 0 is the lowest (afaik)

public:
  xMdx3 * fmesh; //facemesh
  xMdx3 * vmesh; //vertmesh


public:
  unsigned int skin; 
  int skinBlend;

  unsigned int skin2;
  int skin2Blend;

public:
  //color is 4 floats
  gamex::cVec3f color;
  float alpha; 
  
  int useColor; //use color array from vmesh (rename to useVmeshColor)

public:
  int useTransMat; //0 use pos and ori    1 use transmat
  gamex::cVec3f pos; //translate
  gamex::cQuat ori; //orientation
  gamex::cVec3f scale; 

  gamex::cMat transMat;

  gamex::cVec3f sortpos; //sort by this position


public:  
  int blend; //0 solid   1 alpha test   2 transparent   3 additive

  int twoSide; //2 sided (disable culling with 1)

  int decal; 

  int flags; //used for simple render (determine if entity is shadowed or not) 

  
 
public:
  int useTexMat;  //use texture matrix (for effects)
  gamex::cMat texMat;

public:

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



  void simpRender(int flag); 



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

  //todo -- seperated for shadow testing only
  void renderBucket0(void);
  void renderBucket1(void);


  void simpRender(int flag = 0xFFffFFff);

};//xrender

















