#pragma once


#include "../gamex/gamex.h"


class xGroundDecal : public xActor
{
public:
  xMdx3 mesh;
  unsigned int skin;

public:

  virtual void init(void);
  virtual void render2(xRender * r);

};//xTree