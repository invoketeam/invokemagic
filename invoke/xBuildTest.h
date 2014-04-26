#pragma once


#include "../gamex/gamex.h"


class xBuildTest : public xActor
{
public:
  xMdx3 * mesh;
  unsigned int skin;

  float yaw;

  //todo -- needs seperate model for blending with the ground heightmap
  //and that model needs a seperate skin

  //todo -- seperate class to handle drawing buildings (?)

public:
  xBuildTest(void);
  virtual ~xBuildTest(void);

  virtual void init(void);

  virtual void update(void);
  virtual void render2(xRender * r);


public:
  virtual void gotMsg(int msg, int arg0, int arg1, int arg2);
  


};//xunit