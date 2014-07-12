#pragma once


#include "../gamex/gamex.h"


class xPartTest : public xActor
{
public:
  unsigned int skin;

  xPartSys myPart;
  xPartSprite myAnim;

public:
  xPartTest(void);
  virtual ~xPartTest(void);

  virtual void init(void);

  virtual void update(void);
	virtual void render(void);
  virtual void render2(xRender * r);

};//xunit