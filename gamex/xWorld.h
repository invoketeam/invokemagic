#pragma once

#include <string>
#include <vector>
#include <map>

class xActor;
class xRender;
class xFlatRender;

typedef std::vector <xActor *> tdVecActor;
typedef std::map <int, xActor *> tdMapActor;

class xWorld
{
public:
    tdVecActor vecActor;
    tdMapActor mapActor;

public:
  xWorld();
  ~xWorld();
  
  void clear();

  
  void addActor(xActor * a);
  xActor * getActor(int id);
 
 
  void render();
  void update();

  void render2(xRender * r);
  void frameRender(xFlatRender * r);
  
};//xworld
