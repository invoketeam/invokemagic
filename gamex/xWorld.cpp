#pragma warning (disable : 4786)


#include "xWorld.h"
#include "xActor.h"
//#include "xRender.h"


xWorld::xWorld(void)
{

}//ctor


xWorld::~xWorld(void)
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {    delete *it;  }
  
  vecActor.clear();
  mapActor.clear();
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
xWorld::clear(void)
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {  (*it)->remove();   delete *it;  }
  
  vecActor.clear();
  mapActor.clear();
}//clear

  
void 
xWorld::render(void)
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {
    if ((*it)->visible == false) { continue; }
    (*it)->render();
  }//nexti
}//render


  
void 
xWorld::render2(xRender * r)
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {
    if ((*it)->visible == false) { continue; }
    (*it)->render2(r);
  }//nexti
}//render2

  
void 
xWorld::frameRender(xFlatRender * r)
{
  tdVecActor ::iterator it;
  
  for (it = vecActor.begin(); it != vecActor.end(); it++)
  {
    if ((*it)->visible == false) { continue; }
    (*it)->frameRender(r);
  }//nexti
}//render2

  

  
void 
xWorld::preUpdate(int gt) 
{
  int i;  int num;  xActor * a;
  int tk;

  tk = gt % 60;

  num = (int) vecActor.size();

   for (i = 0; i < num; i++)
    {
      a = vecActor[i];
      if (a->dead)   { continue; }
			if (a->asleep) { continue; }       

      if ((a->id % 60) == tk) { a->think(); }

      if (a->parentid > 0) { continue; } //updated by parent

      a->preUpdate();             
    }//nexti

}//update



void 
xWorld::update(void)
{
  int i;  int num;  xActor * a;
  bool bFoundDead;

  
  bFoundDead = false;
  
  num = (int) vecActor.size();

   for (i = 0; i < num; i++)
      {
        a = vecActor[i];
        if (a->dead) { bFoundDead = true; continue; }
				if (a->asleep) { continue; }

        


        if (a->parentid > 0) { continue; } //updated by parent       
          a->update();

      }//nexti
 
   
      if (bFoundDead)
            {
                //need to be in seperate loops
                //because actor creation and deletion can clash
                //resulting in multi update
                num = (int) vecActor.size();
                for (i = 0; i < num; i++)
                {
                    a = vecActor[i];
                    if (a->dead == false) { continue; }
               
                        a->remove();

                        vecActor[i] = vecActor[num - 1];
                        vecActor.pop_back();
                        //mapActor[a->id] = 0; 
                        mapActor.erase(a->id);
                        delete a;
                        //a.remove(); 
                        num = (int) vecActor.size();
                        i -= 1;

                }//nexti
            }//endif
 

}//update

