#pragma warning (disable : 4786) 

#include "xTower.h"


#include "../../gamex/xMultiGrid.h"
#include "../../gamex/xGame.h"
#include "../../gamex/xRender.h"
#include "../../gamex/xBigPart.h"

#include "../bullet/xHoming.h"

#include "../xGroundDecal.h"


#include "../xInvokeCommon.h"







xTower::xTower(void)
{
  team = 0;

  decalId = 0;
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

  flags = FR_SELECTABLE | FR_BLOCKMOVE | FR_ATTACKABLE;

  pos.y = game->getHeight(pos.x, pos.z) + yrad;


  xActor * a;
   a = new xGroundDecal();
   a->wstr = "crater";
   a->pos = pos; a->pos.y = 0;
   a->xrad = 512;
   a->zrad = 512;
   a->ang = game->getRand()*6.28;
  game->addActor(a);
  decalId = a->id;


  putInGridXZ(game->mgrid);
}//init


void 
xTower::onKilled(void)
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

}//onkilled






  
void 
xTower::update(void)
{

   xActor * a;
   float dx, dz;


   float range;

   range = 2048;

    a = 0;
    if (targid > 0)  {  a = game->getActor(targid);  if (a == 0) { targid = 0; } }
    else
    {
      if (workb < game->gameTime ) { workb = game->gameTime + 10; 
        a = getTarget(game->mgrid, pos.x-range, pos.z-range, range+range, range+range);
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

      if (dx < range && dz < range) { worka = 1; }
      if (dx > (range+40.0f) && dz > (range+40.0f)) { worka = 0; }
    }//endif

    if (worka == 1)
    {
      if (a == 0) { worka = 0; targid = 0; return;}
      
      //attack enemy (meele)  
     if (reload < game->gameTime) 
     { 
       //printf("tower fire \n");

        xHoming * b;
         b = new xHoming();
          b->teamColor.set(0,1,0);
          b->targid = a->id;
          b->pos = pos;
          b->pos.y += 768;
        game->addActor(b);

      //a->gotHit(16, 1, 0,0,0);

      reload = game->gameTime + 20;

     }
      
    }//endif


 // putInGridXZ(game->mgrid);
}//update


void 
xTower::render(void) 
{
    xActor * t;

    glColor3f(1,1,1);

    if (targid != 0)
    {
      t = game->getActor(targid);
      if (t != 0)
      {
        glBegin(GL_LINES);
          glVertex3f(pos.x,pos.y, pos.z);
          glVertex3f(t->pos.x, t->pos.y, t->pos.z);
        glEnd();
      }
    }//endif

}//render


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


