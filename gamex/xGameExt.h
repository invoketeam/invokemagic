#pragma once

#include <map>

#include "xGame.h"
#include "xActor.h"
#include "xWorld.h"



class xGameExt : public xGame
{
public:
  int curId;
  xWorld myWorld;

  std::map <std::string, int> mapZone;

public:
  xGameExt();
  virtual ~xGameExt();

  void init(void);

  void clear(void);

  virtual void addActor(xActor * a);
  virtual xActor * getActor(int id);

  virtual void resetWorld(float w = 640.0f, float h =480.0f);


public:
  virtual void addNameZone(xActor * a, std::string wname);
  virtual xActor * getNameZone(std::string wname);
  virtual void remNameZone(std::string wname);
  virtual void clearNameZone(void);

};//classend



