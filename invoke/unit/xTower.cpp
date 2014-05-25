#pragma warning (disable : 4786) 

#include "xTower.h"


#include "../../gamex/xMultiGrid.h"
#include "../../gamex/xGame.h"
#include "../../gamex/xRender.h"
#include "../../gamex/xBigPart.h"

#include "../xInvokeCommon.h"





xTower::xTower(void)
{
  team = 0;
}//ctor

void
xTower::init(void)
{ 

 hp = 5; //very low hp for testing 

 mesh = game->getMdx("wraith_foepulet");
 skin = game->getSkin("wraith_foepulet");


  xrad = 256;
  yrad = 128;
  zrad = 256;

  flags = FR_SELECTABLE | FR_BLOCKMOVE;

  pos.y = game->getHeight(pos.x, pos.z) + yrad;

  putInGridXZ(game->mgrid);
}//init


void 
xTower::onKilled(void)
{

}//onkilled






  
void 
xTower::update(void)
{

   xActor * a;
   float dx, dz;

    a = 0;
    if (targid > 0)  {  a = game->getActor(targid);  if (a == 0) { targid = 0; } }
    else
    {
      if (workb < game->gameTime ) { workb = game->gameTime + 10; 
        a = getTarget(game->mgrid, pos.x-2048, pos.z-2048, 4096, 4096);
        if (a != 0) {targid = a->id; }  else { targid = 0; }
      }
      //todo -- alert others nearby when found a target(?)
    } //endif   

 
    if (a == 0) {   targid = 0; }

 
    if (a != 0)
    {
      dx = a->pos.x - pos.x; if (dx < 0) { dx = -dx; }
      dz = a->pos.z - pos.z; if (dz < 0) { dz = -dz; }

      //only start attacking when got closer than range
      //start moving again when out of range

      if (dx < 600.0f && dz < 600.0f) { worka = 1; }
      if (dx > 628.0f && dz > 628.0f) { worka = 0; }
    }//endif

    if (worka == 1)
    {
      if (a == 0) { worka = 0; targid = 0; return;}
      
      //attack enemy (meele)  
      if (reload < game->gameTime)  { a->gotHit(16, 1, 0,0,0); reload = game->gameTime + 20; }
      
    }//endif


 // putInGridXZ(game->mgrid);
}//update

void 
xTower::render2(xRender * r)
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


