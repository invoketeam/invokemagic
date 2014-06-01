#pragma warning (disable : 4786) 

#include "xHoming.h"


#include "../../gamex/xMultiGrid.h"
#include "../../gamex/xGame.h"
#include "../../gamex/xRender.h"
#include "../../gamex/xBigPart.h"
#include "../../gamex/xKey.h"


xHoming::xHoming(void)
{

}//ctor

void
xHoming::init(void)
{ 
 mesh = game->getMdx("tree");
  //putInGridXZ(game->mgrid);
}//init


void 
xHoming::onKilled(void)
{

}//onkilled

  
void 
xHoming::update(void)
{



  xActor * a;
  
  a = game->getActor(targid);
  if ( a == 0) { dead = true; return; } //if target is dead remove bullet

  vel = (a->pos - pos) * 0.2f;

  if (vel.getMagSquared() < 256) 
  {
    a->gotHit(4, 1, 0,0,0);
    kill();
    return;
  }

  pos += vel;

  putInGridXZ(game->mgrid);

}//update

void 
xHoming::render2(xRender * r)
{

  //printf("homing render \n");

   xBigPart * b;
    b = game->addPart("spark");
    b->pos = pos;
    b->roll = game->getRand()*6.28f;
    b->scale = 3;
    b->rgb = teamColor;
    b->vel.set(4*(game->getRand()-0.5f),4*(game->getRand()-0.5f),4*(game->getRand()-0.5f));
    b->fade = 0.02f;

/*
    xEnt * e;

    e = r->addFrame(0);
      e->fmesh = mesh;
      e->vmesh = mesh;
      e->skin = 0;
      e->skinBlend = 3;
      e->pos = pos;
      e->ori = ori;
      e->color = 1;
  */
}//render2 


