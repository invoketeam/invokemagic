#pragma once

#include <string>

#include "xTexture.h"
#include "xSprite.h"
#include "xMdx3.h"
#include "xStore.h"
#include "xFont.h"
#include "xSkel.h"




class xData 
{
public:
 xSpriteMan myMan;  //also stores skins (textures)

 xStore <xFont> storeFont;

  //todo -- store fonts, store skeletons and animations
 xStore <xMdx3> storeMesh;
 xStore <xSkel> storeSkel;
 xStore <xBoneAnim> storeBanim;


public:
  xData(void);
  ~xData(void);
  void clear(void);
 
public:
  xTexture * addSkin(std::string fname, std::string altName, bool mip=true, bool ymir=false, bool clamp=false);

  //todo -- addsprite (also load texture)
  void addSprite(std::string fname, std::string texfname);

public:
  xTexture * getTex(std::string wname);  
  unsigned int getSkin(std::string wname);
  xSprite * getSprite(std::string wname);

public:
  xFont * addFont(std::string fname, std::string altName, std::string texfname);
  xFont * getFont(std::string wname);

public:
  xMdx3 * addMdx(std::string fname, std::string altName);
  xMdx3 * addMdxFromPtr(xMdx3 * mesh, std::string altName); //remember --  deleting will be handled by xData this way

  xMdx3 * getMdx(std::string wname);

public:
  xSkel * addSkel(std::string fname, std::string altName);
  xSkel * getSkel(std::string wname); //todo -- you need to do the copy part yourself

public:
  xBoneAnim * addBoneAnim(std::string fname, std::string altName);
  xBoneAnim * getBoneAnim(std::string wname);


};//xdata