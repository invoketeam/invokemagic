#pragma warning (disable : 4786)

#include "xData.h"


xData::xData(void)
{

}//ctor

xData::~xData(void)
{
  clear();
}//dtor

  
void 
xData::clear(void)
{

}//clear





void
xData::addMdx(std::string fname, std::string altName)
{
  xMdx3 * a;
  a = new xMdx3();
  a->loadFile(fname);
  if (altName != "0") { fname = altName;  }
  else { fname = stripName(fname); }
  printf("addmdx %s \n" ,fname.c_str());

  storeMesh.addData( fname , a);
}//addanim


void
xData::addSkin(std::string fname, std::string altName, bool mip)
{
  xTexture * a;
  a = new xTexture();
  a->loadTex(fname, mip);
  a->setTexEnv_Replace();
  if (altName != "0") { fname = altName;  }
  else { fname = stripName(fname); }

  storeSkin.addData(fname, a);

   printf("addSkin %s \n" ,fname.c_str());

  //todo -- for each texture make a sprite with its own name
}//addskin




unsigned int 
xData::getSkin(std::string wname) 
{
 xTexture * a; 
 a = storeSkin.getData(wname);
 if (a == 0) { printf("xData::getSkin not found %s \n", wname.c_str());  return 0;}
 return a->handle;
}//getskin


xTexture *
xData::getTex(std::string wname) 
{ 
 xTexture * a; 
 a = storeSkin.getData(wname);
 if (a == 0) { printf("xData::getTex not found %s \n", wname.c_str());  return 0;}
 return a;
}  


xSprite * 
xData::getSprite(std::string wname) 
{
 return myMan.getSprite(wname);
}//getsprite


xMdx3 *
xData::getMdx(std::string wname)
{
  return storeMesh.getData(wname);
}//getmdx











