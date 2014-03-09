#pragma once


#include "xGame.h"
#include "xActor.h"
#include "xWorld.h"



class xGameExt : public xGame
{
public:
  int curId;
  xWorld myWorld;

public:
  xGameExt();
  virtual ~xGameExt();

  void init(void);

  void clear(void);

  virtual void addActor(xActor * a);
  virtual xActor * getActor(int id);

  virtual void resetWorld(float w = 640.0f, float h =480.0f);

};//classend



