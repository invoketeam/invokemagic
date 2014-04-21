#pragma warning (disable : 4786)

#include "xGLCommon.h"

#include "xGameExt.h"

#include "xMultiGrid.h"






xGameExt::xGameExt()
{
  curId = 0;
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
  mgrid->init(w,h,7,7,3);  

   curId = 0;
 
  myRand.setSeed(1);
}//resetworld


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





static void drawCube(float cx, float cy, float cz, float cw, float ch, float cd)
{
  glBegin(GL_LINES);
    glVertex3f(cx,cy,cz);       glVertex3f(cx+cw,cy,cz);
    glVertex3f(cx,cy+ch,cz);    glVertex3f(cx+cw,cy+ch,cz);
    glVertex3f(cx,cy,cz+cd);    glVertex3f(cx+cw,cy,cz+cd);
    glVertex3f(cx,cy+ch,cz+cd); glVertex3f(cx+cw,cy+ch,cz+cd);

    glVertex3f(cx,cy,cz);       glVertex3f(cx,cy+ch,cz);
    glVertex3f(cx+cw,cy,cz);    glVertex3f(cx+cw,cy+ch,cz);
    glVertex3f(cx,cy,cz+cd);    glVertex3f(cx,cy+ch,cz+cd);
    glVertex3f(cx+cw,cy,cz+cd); glVertex3f(cx+cw,cy+ch,cz+cd);

    glVertex3f(cx,cy,cz);       glVertex3f(cx,cy,cz+cd);
    glVertex3f(cx+cw,cy,cz);    glVertex3f(cx+cw,cy,cz+cd);
    glVertex3f(cx,cy+ch,cz);    glVertex3f(cx,cy+ch,cz+cd);
    glVertex3f(cx+cw,cy+ch,cz); glVertex3f(cx+cw,cy+ch,cz+cd);
  glEnd();
}//drawcube


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







