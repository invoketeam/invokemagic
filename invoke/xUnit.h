#pragma once


#include "../gamex/gamex.h"


class xUnit : public xActor
{
public:
  xMdx3 mesh;
  xBoneAnim anim;
  xSkel skel; 
  unsigned int skin;

  float yaw;
  float curFrame;


  xMdx3 * shadowMesh;
  unsigned int shadowSkin;


public:
  xUnit(void);
  virtual ~xUnit(void);

  virtual void init(void);

  virtual void update(void);
	virtual void render(void);
  virtual void render2(xRender * r);


  virtual unsigned int getSkin(std::string wname);

};//xunit