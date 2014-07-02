#pragma warning (disable : 4786) 

#include "xBuilding.h"
#include "xTower.h"

#include "../../gamex/xMultiGrid.h"
#include "../../gamex/xGame.h"
#include "../../gamex/xRender.h"
#include "../../gamex/xBigPart.h"

#include "../bullet/xHoming.h"

#include "../xGroundDecal.h"


#include "../xInvokeCommon.h"







xBuilding::xBuilding(void)
{
  team = 0;

  decalId = 0;

  color = 0xFFffFFff;

}//ctor

void
xBuilding::init(void)
{ 

 hp = 250; //very low hp for testing 

 mesh = game->getMdx("wraith_foepulet");
 skin = game->getSkin("wraith_foepulet");


  worka = 0;   //building points gathered
  workb = 100; //building points left
 
 
  xrad = 256;
  yrad = 128;
  zrad = 256;

  flags = FR_SELECTABLE | FR_BLOCKMOVE | FR_ATTACKABLE;
  flags |= FR_BUILDABLE;

  pos.y = game->getHeight(pos.x, pos.z) + yrad;

  //todo -- zonetype values (2 is for buildings .. or walls)
  game->setZoneTypeRect(pos.x-xrad,pos.z-zrad,xrad+xrad,zrad+zrad, 2);


  xActor * a;
   a = new xGroundDecal();
   a->wstr = "crater";
   a->pos = pos; a->pos.y = 0;
   a->xrad = 256;
   a->zrad = 256;
   a->ang = game->getRand()*6.28;
  game->addActor(a);
  decalId = a->id;


  putInGridXZ(game->mgrid);
}//init


void 
xBuilding::onKilled(void)
{
  xBigPart * b;
    b = game->addPart("spark");
    b->pos = pos;
    b->roll = game->getRand()*6.28f;
    b->scale = 32;
    b->rgb.set(0,1,0);
    //b->vel.set(4*(getRand()-0.5f),4*(getRand()-0.5f),4*(getRand()-0.5f));

  //todo -- make decal fade out
  //remove decal
  xActor * a;
  a = game->getActor(decalId);
  if (a != 0)
  {
    a->dead = true;
  }//endif

  //remove self from zones
   game->setZoneTypeRect(pos.x-xrad,pos.z-zrad,xrad+xrad,zrad+zrad, 0);

}//onkilled



void 
xBuilding::gotHit(float dmg, int dtype, float hx, float hy, float hz)
{
  if (dtype == 999)
  {
    xBigPart * b;
      b = game->addPart("spark");
      b->pos = pos;
      b->roll = game->getRand()*6.28f;
      b->scale = 2;
      b->rgb.set(1,1,1);

    worka += 1; 
    if (worka >= workb) { gotBuilt(); }
    return;
  }
  
  xInvActor::gotHit(dmg, dtype, hx, hy, hz);

}//gothit


void 
xBuilding::gotBuilt(void)
{
  if (dead) { return; }  //already got built

  dead = true;
 
 xActor * a;

  //get rid of decal
  a = game->getActor(decalId);
  if (a != 0)
  {
    a->dead = true;
  }//endif

  //remove self from zones
   game->setZoneTypeRect(pos.x-xrad,pos.z-zrad,xrad+xrad,zrad+zrad, 0);

  
  //create building

  a = new xTower();
  a->pos = pos;
  a->team = team;
   game->addActor(a);
  


}//gotbuilt



  
void 
xBuilding::update(void)
{

  
   
}//update


void 
xBuilding::render(void) 
{

}//render


void 
xBuilding::render2(xRender * r)
{
    xEnt * e;

    e = r->addFrame(0);
      e->fmesh = mesh;
      e->vmesh = mesh;
      e->skin = skin;
      e->skinBlend = 3;
      e->pos = pos;

        //until we got animation raise buildings from ground
  float u;
    u =((float) 800 /(float) workb);
        e->pos.y -=  u*workb ;
        e->pos.y += u*(worka);
      e->ori = ori;
      e->color = 1;
      e->flags = 2 + 4 ; //2 cast shadow   4 receive shadow
      e->ori.rotYaw(3.1415f*-0.2f);

}//render2 


