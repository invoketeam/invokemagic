#pragma once


#include "../gamex/gamex.h"




class xUnit : public xActor
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

public:
  xUnit(void);
  virtual ~xUnit(void);

  virtual void init(void);

  virtual void update(void);
	virtual void render(void);
  virtual void render2(xRender * r);


  virtual unsigned int getSkin(std::string wname);


public:
  virtual bool handCol(xActor * a);

public:
  virtual void gotMsg(int msg, int arg0, int arg1, int arg2);
  
};//xunit