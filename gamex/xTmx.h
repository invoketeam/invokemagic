
#pragma once

#include <string>
#include <vector>
#include <map>

class xTmxRect
{
public:
  std::string name;
  std::string type;
  float rx, ry, rw, rh;
  float cx, cy;

  

public:
  //xTmxRect(void) {}

  //todo -- properties

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
  xTmxLayer(void);
  ~xTmxLayer(void);

  void clear(void);

  //change all tile values by n
  void addNum(int n);

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
  xTmx(void);
  ~xTmx(void);

  void clear(void);

  bool loadFile(std::string fname);


  xTmxLayer * getLayer(std::string name);
  xTmxRect * getRect(std::string name);



};//classend

