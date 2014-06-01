#pragma once


#include "../gamex/gamex.h"
#include "xInvActor.h"



class xUnit : public xInvActor
{
public:
  xMdx3 mesh;
  xSkel skel; 
  xBoneAnim * anim;

  unsigned int skin;

  float yaw;
  float curFrame;


//  xMdx3 * shadowMesh;
//  unsigned int shadowSkin;

  
public:
  gamex::cVec3f teamColor;

  gamex::cVec3f dest;
  int cmd;
  int dontMove; //0 move  1 dontmove (maybe should be called movemode)
  int cmdTarg;

public:
  xUnit(void);
  virtual ~xUnit(void);

  virtual void init(void);

  virtual void update(void);
	virtual void render(void);
  virtual void render2(xRender * r);


  virtual unsigned int getSkin(std::string wname);

  virtual void onKilled(void);
  virtual void gotHit(float dmg, int dtype, float hx, float hy, float hz);

public:
  virtual bool handCol(xActor * a);

public:
  virtual void gotMsg(int msg, int arg0, int arg1, int arg2);


//public:
//  virtual xActor * getTarget(xMultiGrid * mgrid, float ax, float az, float aw, float ah);

  
};//xunit