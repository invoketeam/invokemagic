#pragma once

#include "../../gamex/xActor.h"
#include "../../gamex/xMdx3.h"

#include "../xInvActor.h"



// slaves  only harvest resources for now
// later harvesters, builders and fighters should/will be merged into a single class that can do anything


class xSlave : public xInvActor
{
public:
  xMdx3 mesh;
  xSkel skel; 
  xBoneAnim * anim;
  unsigned int skin;

  gamex::cVec3f dest; //destination

  gamex::cVec3f hpoint; //harvest point
  float hrad; //harvest radius 
 
  float yaw;
  float curFrame;

  int cmd;
  
  float load;  //resources held
  float maxLoad;  //max capacity
 
public:
  xSlave(void);
  virtual ~xSlave(void);
  
  virtual void init(void);
  virtual void update(void);

  virtual void render(void);
  virtual void render2(xRender * r);

  virtual void onKilled(void);
  virtual void gotMsg(int msg, int arg0, int arg1, int arg2);

  virtual xActor * getHarvest(xMultiGrid * mgrid, float ax, float az, float aw, float ah);

public:
  virtual bool handCol(xActor * a);
  
};//xslave