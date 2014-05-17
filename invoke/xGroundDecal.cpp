#pragma warning (disable : 4786)

#include "xGroundDecal.h"
#include "xInvokeCommon.h"




void 
xGroundDecal::init(void)
{

 

  game->getDecal(&mesh, pos.x-xrad, pos.z - zrad, xrad+xrad, zrad+zrad); 
  skin =  game->getSkin("crater");
   
yrad = 128;

 team = 0; //neutral
 pos.y = game->getHeight(pos.x, pos.z) + yrad;

//flags = FR_SELECTABLE; 

  putInGrid(game->mgrid);

}//grounddecal
  

void 
xGroundDecal::render2(xRender * r)
{


    xEnt * e;

    e = r->addFrame(0);
 
      e->pos = 0; //pos;
      e->sortpos = pos;
      e->fmesh = &mesh;
      e->vmesh = &mesh;
      e->twoSide = 1;
      e->skin = skin;
      e->useColor = 0;
      e->blend = 2;
      //e->skinBlend = 2; 
      e->skinBlend = 3; 

      e->flags = 4;  //receive shadow
      e->decal = 1;
      e->alpha = 1.0;
      e->color = 1;
      e->twoSide = 0;



}//render2