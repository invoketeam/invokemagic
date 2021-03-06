#pragma once

#include <string>

#include "xMath.h"
#include "xMdx3.h"

//todo -- sometime in the future
//replace all int float and whatsoever
//with types whose byte size is guaranteed


struct xSkVert
{
  gamex::cVec3f pos;
  int numbone;
  int bone[4];
  float weight[4];
};//skvert


class xBone
{
public:
  //local
  gamex::cVec3f pos;
  gamex::cQuat ori;

  //world (use for attachment)
  gamex::cVec3f wpos;
  gamex::cQuat wori;

  gamex::cVec3f scale; 

  gamex::cMat transMat;

  //used to transform matrices
  gamex::cMat skinMat;
  gamex::cMat vertMat;

  int parent;
  int id;
  int depth;
  int spec;

public:
  xBone();
  
  void makeSkinMat(gamex::cVec3f &p, gamex::cQuat &o);

};//xbone


class xSkel
{
public:
  gamex::cVec3f pos;
  gamex::cQuat ori;
  gamex::cVec3f scale;
  gamex::cMat baseMat;

  int numBone;
  int numVert;

  xBone * vecBone;
  xSkVert * vecVert;

public:
  xSkel(void);
  ~xSkel(void);

  void clear(void);


  void update(void);

  //note -- so far the animation is rigid (only 1 bone per vertex)
  void applySkin(xMdx3 * mesh, int num);

  void copySkel(xSkel * skel);

  void loadFile(std::string fname);

  void readSkelV1(FILE * file);
  void readSkelV2(FILE * file);

  //void debRender(void);


  //only needed in tools (probably)
  void saveFile(std::string fname);
  
  //only needed for lod generation
  void reIndexSkin(short * vec, int num);

};//xskel



struct xBoneFrame
{
  gamex::cVec3f pos;
  gamex::cQuat ori;
  gamex::cVec3f res; //reserved for size, not used for now
};//xboneframe


//todo -- store name in anim(?)
class xBoneAnim
{
public:
  std::string name;
  int numBone;
  int numFrame;
  xBoneFrame * vecFrame;

public:
  xBoneAnim(void);
  ~xBoneAnim(void);

  void clear(void);

  
  void applyFrame(int frame, xSkel * skel);



  //use this for smooth animation
  void applyFrame2(float frame, xSkel * skel);

  void loadFile(std::string fname);

  void readAnim(FILE * file);

};//xboneanim


