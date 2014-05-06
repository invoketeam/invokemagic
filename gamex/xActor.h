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
  
  float hp;

  float xrad, yrad, zrad;
  float rad;  
  
  float ang; //usually used for yaw(3D) or roll(2D) 

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
  bool res; //reserved for byte alignment (todo -- figure out something to use it for)


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

  virtual void init(void) {}

  virtual void update(void) {}
  virtual void frameRender(xFlatRender * render) { }

  virtual void render(void) {} //debugrender
  virtual void render2(xRender * render) {} //3D render

  virtual void putInGrid(xMultiGrid * mgrid); //default is X Z
  virtual void putInGridXY(xMultiGrid * mgrid);
  virtual void putInGridXZ(xMultiGrid * mgrid);

  virtual void takeOutOfGrid(void);


  virtual void checkColXZ(xMultiGrid * m);
  virtual void checkColXY(xMultiGrid * m);

  virtual bool handCol(xActor * a) { return false; }


  virtual void gotHit(float dmg, int dtype, float hx, float hy, float hz);
  
  virtual void kill(void); //no need to override for fx, just use onKilled
  
  virtual void onKilled(void) {} //called by kill

  
public:
  virtual bool overlap(xActor * a);
  virtual bool overlapXY(xActor * a);
  virtual bool overlapXZ(xActor * a);


public:
  virtual void trigger(std::string &str) {}


public:
  virtual void gotMsg(int msg, int arg0, int arg1, int arg2) {}
  
};//classend