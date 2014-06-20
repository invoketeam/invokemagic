#pragma warning (disable : 4786)

#include "xGLCommon.h"

#include "xGameExt.h"

#include "xMultiGrid.h"

#include "xDebug.h"




xGameExt::xGameExt()
{
  curId = 1; //start at 1
}//ctor

xGameExt::~xGameExt()
{
  clear();
}//dtor



void 
xGameExt::init(void)
{
  
}//init

  
void 
xGameExt::clear(void)
{
  myWorld.clear();
  if (mgrid != 0) { delete mgrid; mgrid = 0;}
  clearNameZone();

}//clear


void
xGameExt::addActor(xActor * a) 
{
  //if a doesnt exist -> crash

  a->game = this; //important
  curId += 1;
  a->id = curId;
  myWorld.addActor(a);
  a->init();
}//addactor


xActor * 
xGameExt::getActor(int id) 
{ 
  return myWorld.getActor(id);
}//getactor


void 
xGameExt::resetWorld(float w, float h)
{
  clear();
 
  mgrid = new xMultiGrid();
//  mgrid->init(w,h,7,7,3);  
//  mgrid->init(w,h,9,9,3);  

//note -- top cell size needs to be adjusted per game for good optimisation
// 2 << 7  was too small in this case and too many actors ended up in the top cell
//the top cell is for actors that fall on lines between grid lines
//and are checked in every collision test
  mgrid->init(w,h,10,10,3);  

   curId = 1; //start at 1
 
  myRand.setSeed(1);
}//resetworld


xWorld * 
xGameExt::getWorldPtr(void) { return &myWorld; }

float 
xGameExt::getRand(void)
{
  return myRand.getRand();
}//getrand

float 
xGameExt::getRand2(void)
{
  return myRand.getRand() - 0.5f;
}//getrand2



void 
xGameExt::addNameZone(xActor * a, std::string wname) 
{
  mapZone[wname] = a->id;
}//addnamezone



xActor * 
xGameExt::getNameZone(std::string wname) 
{
 int i;
 xActor * a;
  
 i = mapZone[wname];
 if (i == 0) { return 0; }
 
 a = getActor(i);
 if (a == 0) { mapZone.erase(wname); return 0; }  

 return a;
}//getnamezone



void 
xGameExt::remNameZone(std::string wname) 
{
  mapZone.erase(wname);
}//remnamezone




void 
xGameExt::clearNameZone(void) 
{
  mapZone.clear();
}//clearnamezone







void 
xGameExt::debugDraw(void)
  {
   
    tdVecActor ::iterator it;
    xActor * a;
    
    glColor4f(0,1,0,1);

    for (it = myWorld.vecActor.begin(); it != myWorld.vecActor.end(); it++)
    { 
       a = (*it);
       drawCube(a->pos.x-a->xrad, a->pos.y-a->yrad, a->pos.z-a->zrad,a->xrad+a->xrad,a->yrad+a->yrad,a->zrad+a->zrad);
     
       //printf("debdraw %0.2f %0.2f \n", a->pos.x, a->pos.y);
      //note: for 2d a drawrect will be better because opengl will use the z to check if the sprite is on screen
    }
  }//debugdraw







