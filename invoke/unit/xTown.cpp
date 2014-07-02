#pragma warning (disable : 4786) 

#include "xTown.h"


#include "../../gamex/xMultiGrid.h"
#include "../../gamex/xGame.h"
#include "../../gamex/xRender.h"
#include "../../gamex/xBigPart.h"

#include "../xGroundDecal.h"

#include "../xInvokeCommon.h"







xTown::xTown(void)
{
  team = 0;
  color = 0xFFffFFff;
  decalId = 0;
}//ctor

void
xTown::init(void)
{ 

 hp = 250; //very low hp for testing 

 mesh = game->getMdx("box_256");
 skin = game->getSkin("temp_town");


  xrad = 256;
  yrad = 128;
  zrad = 256;

  flags = FR_SELECTABLE | FR_BLOCKMOVE ;
  flags |= FR_RETURNGOODS;
  flags |= FR_ATTACKABLE;



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
 

  //todo -- zonetype values (2 is for buildings .. or walls)
  game->setZoneTypeRect(pos.x-xrad,pos.z-zrad,xrad+xrad,zrad+zrad, 2);

  asleep = true;


  //temporary team selection
    if (team == 1)    { teamColor.set(1, 0, 0);  } //red
    else if (team == 2) { teamColor.set(0, 0, 1); }  //blue
    else if (team == 0) { teamColor.set(0.5, 0.5, 0.5); } //grey (neutral?) 
    else { teamColor.set(0,1,0); } //unknown team


 putInGridXZ(game->mgrid);

}//init


void 
xTown::onKilled(void)
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
xTown::update(void)
{


  putInGridXZ(game->mgrid);
}//update


void 
xTown::render(void) 
{

}//render


void 
xTown::render2(xRender * r)
{
    xEnt * e;

    e = r->addFrame(0);
      e->fmesh = mesh;
      e->vmesh = mesh;
      e->skin = skin;

      e->skinBlend = 2; //use decal blending for using color as teamcolor
    
      e->pos = pos;
      e->ori = ori;
      e->color = teamColor;
      e->useColor = 0;
    
      e->flags = 2 + 4 ; //2 cast shadow   4 receive shadow
      e->ori.rotYaw(3.1415f*-0.2f);

}//render2 


