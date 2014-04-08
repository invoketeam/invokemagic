#pragma warning (disable : 4786)


#include "xWorld.h"
#include "xActor.h"
//#include "xRender.h"


xWorld::xWorld()
{

}//ctor


xWorld::~xWorld()
{
  clear();
}//dtor
  
  
void 
xWorld::addActor(xActor * a)
{
  vecActor.push_back(a);
  mapActor[a->id] = a;
}//addactor


xActor *
xWorld::getActor(int id)
{
  return mapActor[id];
}//getactor


  
void 
xWorld::clear()
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {    delete *it;  }
  
  vecActor.clear();
  mapActor.clear();
}//clear

  
void 
xWorld::render()
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {
    (*it)->render();
  }//nexti
}//render


  
void 
xWorld::render2(xRender * r)
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {
    (*it)->render2(r);
  }//nexti
}//render2

  
void 
xWorld::frameRender(xFlatRender * r)
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {
    (*it)->frameRender(r);
  }//nexti
}//render2

  

  
void 
xWorld::update()
{
  int i;
  int num;
  xActor * a;
  bool bFoundDead;
  
  bFoundDead = false;
  
  num = vecActor.size();

   for (i = 0; i < num; i++)
      {
        a = vecActor[i];
        if (a->dead) { bFoundDead = true; continue; }
				if (a->asleep) { continue; }
               
        a->update();
      }//nexti
 
   
      if (bFoundDead)
            {
                //need to be in seperate loops
                //because actor creation and deletion can clash
                //resulting in multi update
                num = vecActor.size();
                for (i = 0; i < num; i++)
                {
                    a = vecActor[i];
                    if (a->dead == false) { continue; }
               
                        vecActor[i] = vecActor[num - 1];
                        vecActor.pop_back();
                        //mapActor[a->id] = 0; 
                        mapActor.erase(a->id);
                        delete a;
                        //a.remove(); 
                        num = vecActor.size();
                        i -= 1;

                }//nexti
            }//endif
 

}//update




