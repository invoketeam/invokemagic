#pragma once

#include <map>

#include "xGame.h"
#include "xActor.h"
#include "xWorld.h"
#include "xRand.h"


class xGameExt : public xGame
{
public:
  int curId;
  xWorld myWorld;
  xRand myRand;

  std::map <std::string, int> mapZone;

public:
  xGameExt();
  virtual ~xGameExt();
  void clear(void);



  void init(void);


  virtual void resetWorld(float w = 640.0f, float h =480.0f);



  virtual void addActor(xActor * a);
  virtual xActor * getActor(int id);



  virtual float getRand(void);
  virtual float getRand2(void);


  virtual xWorld * getWorldPtr(void);

  virtual void debugDraw(void);


public:
  virtual void addNameZone(xActor * a, std::string wname);
  virtual xActor * getNameZone(std::string wname);
  virtual void remNameZone(std::string wname);
  virtual void clearNameZone(void);

};//classend



