#pragma once

#include "../../gamex/xActor.h"
#include "../../gamex/xMdx3.h"

#include "../xInvActor.h"



// slaves  only harvest resources for now
// later harvesters, builders and fighters should/will be merged into a single class that can do anything


class xBuilder : public xInvActor
{
public:
  xMdx3 mesh;
  xSkel skel; 
  xBoneAnim * anim;
  unsigned int skin;

 
  float yaw;
  float curFrame;

  int cmd;
  
   gamex::cVec3f dest;


public:
  xBuilder(void);
  virtual ~xBuilder(void);
  
  virtual void init(void);
  virtual void update(void);

  virtual void render(void);
  virtual void render2(xRender * r);

  virtual void onKilled(void);
  virtual void gotMsg(int msg, int arg0, int arg1, int arg2);

public:
  virtual bool handCol(xActor * a);
  
};//xBuilder