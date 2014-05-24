#pragma once


#include "xInvActor.h"


class xTree : public xInvActor
{
public:
  xMdx3 * mesh;
  unsigned int skin;


public:
  xTree(void);
  virtual ~xTree(void);

  virtual void init(void);

  virtual void update(void);
	virtual void render(void);
  virtual void render2(xRender * r);


  virtual unsigned int getSkin(std::string wname);

};//xTree