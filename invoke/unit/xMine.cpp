#pragma warning (disable : 4786) 

#include "xMine.h"


#include "../../gamex/xMultiGrid.h"
#include "../../gamex/xGame.h"
#include "../../gamex/xRender.h"
#include "../../gamex/xBigPart.h"

#include "../xGroundDecal.h"

#include "../xInvokeCommon.h"







xMine::xMine(void)
{
  team = 0;

  decalId = 0;
}//ctor

void
xMine::init(void)
{ 

 hp = 250; //very low hp for testing 

 mesh = game->getMdx("box_256");
 skin = game->getSkin("skin_256");


  xrad = 256;
  yrad = 128;
  zrad = 256;

  flags = FR_SELECTABLE | FR_BLOCKMOVE ;
  flags |= FR_HARVESTABLE;
 



  xActor * a;
   a = new xGroundDecal();
   a->wstr = "crater";
   a->pos = pos; a->pos.y = 0;
   a->xrad = 512;
   a->zrad = 512;
   a->ang = game->getRand()*6.28;
  game->addActor(a);
  decalId = a->id;



pos.y = game->getHeight(pos.x, pos.z);
  putInGridXZ(game->mgrid);

  //todo -- zonetype values (2 is for buildings .. or walls)
  game->setZoneTypeRect(pos.x-xrad,pos.z-zrad,xrad+xrad,zrad+zrad, 2);

  asleep = true;
}//init


void 
xMine::onKilled(void)
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
xMine::update(void)
{


  putInGridXZ(game->mgrid);
}//update


void 
xMine::render(void) 
{

}//render


void 
xMine::render2(xRender * r)
{
    xEnt * e;

    e = r->addFrame(0);
      e->fmesh = mesh;
      e->vmesh = mesh;
      e->skin = skin;
      e->skinBlend = 3;
      e->pos = pos;
      e->ori = ori;
      e->color = 1;
      e->flags = 2 + 4 ; //2 cast shadow   4 receive shadow
      e->ori.rotYaw(3.1415f*-0.2f);

}//render2 


