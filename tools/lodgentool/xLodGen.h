
#pragma once

#include <string>
#include <map>

#include "xMdx3.h"


//idea is the same as    http://www.melax.com/polychop
//with added support for UV maps
//(no code copied over though, written from sratch)

class xLodLevel
{
public:
  short * vecIndex;
  int numFace;
public:
  xLodLevel();
  ~xLodLevel();
  void clear();

};//lodlevel


//remember -- also need to sort the vertices in the skeleton file


typedef std::vector <xLodLevel *> tdVecLod;

class xLodGen
{
public:
  //lodgen::xModel * model;

  tdVecLod vecLod;
//  std::map <short, short> mapPos;
  short * vecPos;
  int numPos;
public:
  xLodGen();
  ~xLodGen();
  void clear();

public:
  void addBase(xMdx3 * mdx);
  void addLevel(xMdx3 * mdx, int nface);
  void reIndex(xMdx3 * mdx);

  void reSaveMdx(xMdx3 * mdx, std::string fname);
  void saveLod(xMdx3 * mdx, std::string fname);

  void saveLevelAsMdx(xMdx3 * mdx, int lev, std::string fname);

  void saveVecPos(std::string fname);

};//xlodgen


