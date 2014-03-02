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


public:
  xUnit(void);
  virtual ~xUnit(void);

  virtual void init(void);

  virtual void update(void);
	virtual void render(void);
  virtual void render2(xRender * r);


  virtual unsigned int getSkin(std::string wname);

};//xunit