#pragma warning (disable : 4786)

#include "xGameExt.h"





#include "xMultiGrid.h"


xGameExt::xGameExt()
{
  curId = 0;
}//ctor

xGameExt::~xGameExt()
{
 if (mgrid != 0) { delete mgrid; mgrid = 0;}

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
  
}//resetworld


