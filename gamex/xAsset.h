#pragma once


#include "xStore.h"




//asset is a pointer object 
//it is always in memory
//but the data it holds may be not


class xMdx3;
class xTexture;
class xSkel;
class xBoneAnim;
class xSprite;
class xColMesh;

class xAsset
{
public:
  std::string fname;
  std::string altName; //name referred to in map
  int inZip; //is inside a zip file
  int id; //index in zip file
  int rtype; //resource type (todo make this same as assetgroup) (or have seperate?)
  int loaded;  //0 or less -- not loaded
  int scope; //local, global, whatever 
  int bytes; //bytes used for storage (unused)


public:
  xAsset(void);
  ~xAsset(void);  
  void clear(void);

  void loadData(void);

  void loadTex(bool mip, bool mirv, bool clamp);

    //get uncompressed data from zipfile
  //its your task to free it up
  void * getZipData(std::string zname, int i, int * size=0);

public:
  xMdx3 * mesh;
  xTexture * texture;
  xSkel * skel;
  xBoneAnim * boneanim;  
  xColMesh * cmesh;

};//xasset





#define MAX_ASSET_GROUP 16

#define ASSETGRP_INVALID -1
#define ASSETGRP_TEXTURE 0
#define ASSETGRP_MESH 1
#define ASSETGRP_XSKEL 2
#define ASSETGRP_XBONEANIM 3
#define ASSETGRP_TMX 4      //only storing filename for now
#define ASSETGRP_SOUND 5    //not handled by xasset yet
#define ASSETGRP_XFONT 6    //not handled by xasset yet
#define ASSETGRP_XSPRITE 7  
#define ASSETGRP_XANIM 8    //only storing filename
#define ASSETGRP_COLMESH 9  //cmx3 (same as mdx3)

//problem is keeping track of xsprite as it has texture associated with it
//also same for xfont



class xAssetMan
{
public:
  xStore <xAsset> vecAsset[MAX_ASSET_GROUP];

public:
  xAssetMan(void);
  ~xAssetMan(void);

  void clear(void);

  //unload all assets with scope from memory
  void clearScope(int scope);

  //add directory as assets
  void addDir(std::string dname, int scope = 0);
  void addZip(std::string zname, int scope = 0);

  void addAsset(xAsset * a, std::string wname, int grp);

  xAsset * getAsset(std::string wname, int grp);

  int getRtype(std::string ext);

  //you cannot remove assets as they are merely pointer objects
  //(you can overwrite them though)


public:
  std::string getFilename(std::string wname, int grp);

public:
  xTexture *  initTexture(std::string wname, bool mip, bool mirv, bool clamp);

public:
  xMdx3 * getMesh(std::string wname);
  xTexture * getTexture(std::string wname);
  unsigned int getTexHandle(std::string wname);
  xSkel * getSkel(std::string wname);
  xBoneAnim * getBoneAnim(std::string wname);
  xColMesh * getColMesh(std::string wname);

};//assetman




