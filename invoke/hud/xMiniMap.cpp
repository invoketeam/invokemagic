#pragma warning (disable : 4786)

#include "../../gamex/xActor.h"
#include "../../gamex/xWorld.h"

#include "xMiniMap.h"

void 
xMiniMap::init(void)
{
  int size;
  size = 128;

  //todo -- set it to map size (unless map is bigger than 256)
  //that is due the minimap will have a limited size on screen

  img.init(size, size);
  skin.initEmpty(size);

  //img.fillImage(0x000000FF);
  //skin.updateTex(&img);

}//init

void 
xMiniMap::updateImage(xWorld * world)
{
  tdVecActor ::iterator it;
  xActor * a;

  float ax, az;
  float ix, iz;

  //todo -- set these from parameters or members
//  ix = 1.0f/64.0f;
//  iz = 1.0f/64.0f;
  ix = 1.0f/128.0f;
  iz = 1.0f/128.0f;


  ax = 0.0f;
  az = 0.0f;



  img.fillImage(0x000000FF);

  for (it = world->vecActor.begin(); it != world->vecActor.end(); it++)
  {
    a = (*it);
    if (a->visible == false) { continue; }

   //// if (a->spectype == 100)
   // { printf("minimap %0.2f %0.2f \n", ax+a->pos.x*ix, az+a->pos.z*iz); }

    //todo -- set factor
    img.setPixel((int) ax+a->pos.x*ix, az+a->pos.z*iz, a->color);  
  }//nextit

  skin.updateTex(&img);
}//upimage



