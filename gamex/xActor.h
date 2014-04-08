#pragma once

#include "xMath.h"
#include <string>


class xGame;
class xCell;
class xMultiGrid;
class xRender;
class xFlatRender;

class xActor
{
public:
  xGame * game;

public:
  xCell * cell;
  xActor * next;
  xActor * prev;

public:
  gamex::cVec3f pos;
  gamex::cQuat ori;
  gamex::cVec3f vel;
  
  float xrad, yrad, zrad;
  float rad;

  
  float hp;
  float yaw;

  int id;
  int team;

  int flags;    
  int spectype;  
  int reload;

  int tag;

  unsigned int color;

public:
  int targid;
  int ownerid;

public:
  bool asleep;
  bool visible;
  bool dead;  
  bool res; //reserved for byte alignment


public:
  int worka, workb, workc; //generic work values
  float workx, worky, workz; 

public:
  //might seem as an overkill to have so many strings per actor
  //but these actually will make our life easier
  std::string skin;
  std::string wstr, wstr2; 


public:
  xActor(void);
  virtual ~xActor(void);

  virtual void update(void) {}
  virtual void render(void) {}
  virtual void render2(xRender * render) {}
  virtual void frameRender(xFlatRender * render) { }

  virtual void putInGrid(xMultiGrid * mgrid); //default (xz)
  virtual void putInGridXY(xMultiGrid * mgrid);
  virtual void putInGridXZ(xMultiGrid * mgrid);

  virtual void takeOutOfGrid(void);

  void kill();
  virtual void onKilled(void) {}

  virtual void init(void) {}

  virtual void checkColXZ(xMultiGrid * m);
  virtual void checkColXY(xMultiGrid * m);

  virtual bool handCol(xActor * a) { return false; }

  virtual void gotHit(float dmg, int dtype, float hx, float hy, float hz);

  
public:
  virtual bool overlap(xActor * a);
  virtual bool overlapXY(xActor * a);
  virtual bool overlapXZ(xActor * a);


public:
  virtual void trigger(std::string &str) {}

 
  
};//classend