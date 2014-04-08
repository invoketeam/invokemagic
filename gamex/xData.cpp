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
  myMan.clear();
  storeMesh.clear();
}//clear





xMdx3 *
xData::addMdx(std::string fname, std::string altName)
{
  xMdx3 * a;
  a = new xMdx3();
  a->loadFile(fname);
  if (altName != "0") { fname = altName;  }
  else { fname = stripName(fname); }
  printf("addmdx %s \n" ,fname.c_str());

  storeMesh.addData( fname , a);
  return a;
}//addanim


xTexture * 
xData::addSkin(std::string fname, std::string altName, bool mip)
{
  xTexture * a;
  a = new xTexture();
  a->loadTex(fname, mip);
  a->setTexEnv_Replace();
  if (altName != "0") { fname = altName;  }
  else { fname = stripName(fname); }

  //storeSkin.addData(fname, a);
  myMan.addSkin(a, fname);

   //printf("addSkin %s \n" ,fname.c_str());

  return a;
}//addskin




unsigned int 
xData::getSkin(std::string wname) 
{
 xTexture * a; 
 a = myMan.getTex(wname);
 if (a == 0) { printf("xData::getSkin not found %s \n", wname.c_str());  return 0;}   
 return a->handle;
}//getskin


xTexture *
xData::getTex(std::string wname) 
{ 
 xTexture * a; 
 a = myMan.getTex(wname);
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











