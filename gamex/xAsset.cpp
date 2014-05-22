#pragma warning (disable : 4786)

#include "xAsset.h"

#include "stb/dirent.h"

#include "xMdx3.h"
#include "xTexture.h"
#include "xSkel.h"

#include "pugixml/pugixml.hpp"



xAssetMan::xAssetMan(void)
{ }


xAssetMan::~xAssetMan(void) 
{ clear(); }



void
xAssetMan::clear(void) 
{
  int i;
  for (i = 0; i < MAX_ASSET_GROUP; i++)  { vecAsset[i].clear(); }
}//clear



void 
xAssetMan::clearScope(int scope)
{
  
  std::map <std::string, xAsset *> ::iterator it;

  int i;
  xAsset * a;

  for (i = 0; i < MAX_ASSET_GROUP; i++)
  {
    for (it = vecAsset[i].mapData.begin(); it != vecAsset[i].mapData.end(); it++)
    {
       a = it->second;
       if (a->scope == scope) { a->clear(); }
    }//nextit

  }//nexti

}//clearscope


void 
xAssetMan::addAsset(xAsset * a, std::string wname, int grp)
{

    printf("addasset %d  %s   \n", grp, wname.c_str() );

    vecAsset[grp].addData(wname, a); 

}//addasset


xAsset * 
xAssetMan::getAsset(std::string wname, int grp)
{
  return vecAsset[grp].getData(wname);
}//getasset

std::string 
xAssetMan::getFilename(std::string wname, int grp)
{
  xAsset * a;

  a = getAsset(wname, grp);
  if (a == 0) { return "notfound"; }

  return a->fname;
}///getname




//todo -- turn extension to lowercase
static std::string getExt(std::string fname)
{
  std::string ret;
  size_t p;
  p = fname.find_last_of(".");
  if (p == std::string::npos) { ret = "none"; return ret; }
  ret = fname.substr(p+1);
  return ret;
}//getext



void 
xAssetMan::addDir(std::string dname, int scope) 
{

  DIR *dir = 0;
  struct dirent *ent;
  std::string fname;
  std::string ext;

  int rtype;
  xAsset * a;

  dir = opendir (dname.c_str());
	 if (dir == 0) {	printf("Couldn't open directory [%s] \n)", dname.c_str() );	return;	}

   while ((ent = readdir (dir)) != NULL)
   {
     fname = ent->d_name;
     ext = getExt(fname);

     rtype = -1; 

     if (ext == "png") { rtype = ASSETGRP_TEXTURE; }
     else if (ext == "jpg") { rtype = ASSETGRP_TEXTURE; }
     else if (ext == "mdx3") { rtype = ASSETGRP_MESH; }
     else if (ext == "xska") { rtype = ASSETGRP_XBONEANIM; }
     else if (ext == "banm") { rtype = ASSETGRP_XBONEANIM; }
     else if (ext == "xskb") { rtype = ASSETGRP_XSKEL; }
     else if (ext == "tmx") { rtype = ASSETGRP_TMX; }
     else if (ext == "ogg") { rtype = ASSETGRP_SOUND; }
     else if (ext == "xfnt") { rtype = ASSETGRP_XFONT; }
     else if (ext == "xms") { rtype = ASSETGRP_XSPRITE; }
     else if (ext == "xma") { rtype = ASSETGRP_XANIM; }


     if (rtype < 0) { continue; }

     a = new xAsset();
      a->rtype = rtype;
      a->fname = dname; a->fname.append("/"); a->fname.append( ent->d_name );
      a->altName = stripName(fname);
      a->scope = scope;

      printf("asset filename %s \n", a->fname.c_str() );

     addAsset(a, a->altName, a->rtype);

  }//wend

}//adddir





xAsset::xAsset(void)
{
  inZip = 0;
  id = 0;
  rtype = 0;
  loaded = 0;
  scope = 0;

  mesh = 0;
  texture = 0;
  skel = 0;
  boneanim = 0;

}//ctor

xAsset::~xAsset(void)
{
  clear();
}//dtor
 
void 
xAsset::clear(void)
{
  //delete mesh, image, texture, whatever

  if (mesh != 0) { delete mesh; } mesh = 0;
  if (texture != 0) { delete texture; } texture = 0;
  if (skel != 0) { delete skel; } skel = 0;
  if (boneanim != 0) { delete boneanim; } boneanim = 0;

  loaded = 0;
}//clear


void 
xAsset::loadTex(bool mip, bool mirv, bool clamp)
{
  clear();
  //if (rtype != ASSETGRP_TEXTURE) { } //todo -- warning

  texture = new xTexture();
  texture->loadTex(fname, mip, mirv, clamp);

  loaded = 2; //special load
}//loadtex


void
xAsset::loadData(void)
{
  clear();
  
  switch (rtype)
  {
    case ASSETGRP_TEXTURE: 
      texture = new xTexture();
      texture->loadTex(fname, true, false, false); 
    break;

    case ASSETGRP_MESH:
      mesh = new xMdx3();
      mesh->loadFile(fname);
    break;

    case ASSETGRP_XSKEL:
      skel = new xSkel();
      skel->loadFile(fname);
    break;

    case ASSETGRP_XBONEANIM:
      boneanim = new xBoneAnim();
      boneanim->loadFile(fname);
    break;

  };//swend

  loaded = 1;
}//load




xMdx3 * 
xAssetMan::getMesh(std::string wname)
{
  xAsset * a;
  a = vecAsset[ASSETGRP_MESH].getData(wname);
  if (a == 0) { return 0; }
  if (a->loaded <= 0) { a->loadData(); }

  return a->mesh;
}//getmesh


xTexture * 
xAssetMan::initTexture(std::string wname, bool mip, bool mirv, bool clamp)
{
 xAsset * a;
 a = vecAsset[ASSETGRP_TEXTURE].getData(wname);
 if (a == 0) { return 0; }
 if (a->loaded != 2) { a->loadTex(mip, mirv, clamp); }

 return a->texture;
}//inittex


xTexture * 
xAssetMan::getTexture(std::string wname)
{
  xAsset * a;
  a = vecAsset[ASSETGRP_TEXTURE].getData(wname);
  if (a == 0) { return 0; }
  if (a->loaded <= 0) { a->loadData(); }

  return a->texture;
}//gettexture

unsigned int 
xAssetMan::getTexHandle(std::string wname)
{
  xAsset * a;
  a = vecAsset[ASSETGRP_TEXTURE].getData(wname);
  if (a == 0) { return 0; }
  if (a->loaded <= 0) { a->loadData(); }
  if (a->texture == 0) { return 0; }

  return a->texture->handle;
}//gettexhandle


xSkel * 
xAssetMan::getSkel(std::string wname)
{
  xAsset * a;
  a = vecAsset[ASSETGRP_XSKEL].getData(wname);
  if (a == 0) { return 0; }
  if (a->loaded <= 0) { a->loadData(); }

  return a->skel;
}//getskel



xBoneAnim * 
xAssetMan::getBoneAnim(std::string wname)
{
   xAsset * a;
  a = vecAsset[ASSETGRP_XBONEANIM].getData(wname);
  if (a == 0) { return 0; }
  if (a->loaded <= 0) { a->loadData(); }

  return a->boneanim;
}//getboneanim



















