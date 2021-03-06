#pragma once

#include <string>

#include "xMath.h"
#include "xMdx3.h"
#include "xColMesh.h"

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
  
  //gamex::cVec3f cmin, cmax;
  xColMesh * colMesh; //ptr only
public:
  xBone();
  
  void makeSkinMat(gamex::cVec3f &p, gamex::cQuat &o);
  float lineTest(gamex::cVec3f * va, gamex::cVec3f * vb, float r);

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



  bool bCopied; //for copied xSkel vecVert is a pointer and not freed when skel is deleted
public:
  xSkel(void);
  ~xSkel(void);

  void clear(void);


  void update(void);

  //note -- so far the animation is rigid (only 1 bone per vertex)
  void applySkin(xMdx3 * mesh, int num);

  void copySkel(xSkel * skel, bool deepCopy=true);

  void loadFile(std::string fname);

  void loadMem(void * mem,int i, int memsize);
  void readMemV2(void * mem,int i, int memsize);
  //V1 needs to be retired soon

  void readSkelV1(FILE * file);
  void readSkelV2(FILE * file);

  void debRender(void);
  void debBone(int i); //only render bone at index

  int lineTest(gamex::cVec3f *v0, gamex::cVec3f *v1, float r);

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

  void loadMem(void * mem, int i, int memsize);
  void readMem(void * mem, int i, int memsize);

};//xboneanim


