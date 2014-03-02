
#pragma once

#include <string>
#include <vector>
#include <map>

class xTmxRect
{
public:
  std::string name;
  std::string type;
  float cx, cy, cw, ch;
public:
  xTmxRect() {}

};//tmxrect



class xTmxLayer
{
public:
  std::string name;

public:
  int mwidth;
  int mheight;
  int * vecGrid;

public:
  xTmxLayer();
  ~xTmxLayer();

  void addNum(int n);
  void clear();

};//tmxlayer



typedef std::vector <xTmxRect *> tdVecTmxRect;
typedef std::map <std::string, xTmxRect * > tdMapTmxRect;

typedef std::vector <xTmxLayer *> tdVecTmxLayer;
typedef std::map <std::string, xTmxLayer *> tdMapTmxLayer;


class xTmx
{
public:
  int mwidth;
  int mheight;

  std::string setName;

public:
  tdVecTmxRect vecRect;
  tdMapTmxRect mapRect;

public:
  tdVecTmxLayer vecLayer;
  tdMapTmxLayer mapLayer;

public:
  xTmx();
  ~xTmx();

  void clear();

  bool loadFile(std::string fname);


  xTmxLayer * getLayer(std::string name);
  xTmxRect * getRect(std::string name);



};//classend

