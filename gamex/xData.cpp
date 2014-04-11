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
xData::addSkin(std::string fname, std::string altName, bool mip, bool ymir, bool clamp)
{
  xTexture * a;
  a = new xTexture();
  a->loadTex(fname, mip, ymir, clamp);
  a->setTexEnv_Replace();
  if (altName != "0") { fname = altName;  }
  else { fname = stripName(fname); }

  //storeSkin.addData(fname, a);
  myMan.addSkin(a, fname);

   //printf("addSkin %s \n" ,fname.c_str());

  return a;
}//addskin


void 
xData::addSprite(std::string fname, std::string texfname)
{
  addSkin(texfname, stripName(texfname), true, false, true);

  myMan.addSprite(fname);

}//addsprite




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



xFont * 
xData::addFont(std::string fname, std::string altName, std::string texfname)
{

  xTexture * tex;
  xFont * a;

  tex = addSkin(texfname, stripName(texfname) ,true,false, false);

  a = new xFont();
  a->loadCharDef(fname);
  a->handle = tex->handle;

  storeFont.addData(altName, a);


  return a;
}//addfont

xFont * 
xData::getFont(std::string wname)
{
  return storeFont.getData(wname);

}//getfont



xSkel * 
xData::addSkel(std::string fname, std::string altName)
{
  xSkel * a;
  a = new xSkel();
  a->loadFile(fname);

  storeSkel.addData(altName, a);

  return a;
}//addskel

xSkel * 
xData::getSkel(std::string wname)
{
  return storeSkel.getData(wname);
}//getskel



xBoneAnim * 
xData::addBoneAnim(std::string fname, std::string altName)
{
  xBoneAnim * a;
  a = new xBoneAnim();
  a->loadFile(fname);

  storeBanim.addData(altName, a);

  return 0;
}//addbone


xBoneAnim * 
xData::getBoneAnim(std::string wname)
{
  return storeBanim.getData(wname);
}//getbone









