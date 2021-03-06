#pragma warning (disable : 4786)

#include "xPartTest.h"

#include "xInvokeCommon.h"

xPartTest::xPartTest(void) 
{
 
  skin = 0;

}//ctor


xPartTest::~xPartTest(void) 
{

}//dtor


void 
xPartTest::init(void)
{

  myPart.initPart(256);
  myPart.sprAnim = game->getPartSprite("smoke");
  myPart.skin = myPart.sprAnim->skin;


  flags = FR_SELECTABLE;
  xrad = 256+128;
  yrad = 128;
  zrad = 256+128;


}//init



void 
xPartTest::update(void)
{
 

 pos.y = game->getHeight(pos.x, pos.z) + 64;


 xPart * a;
  a = myPart.addPart(0,0,0);
  a->size = 36+game->getRand()*128;
  a->hp = 150;
  a->vel.set(game->getRand()*4-2,game->getRand()*4,game->getRand()*4-2);
  a->pos = pos;
  a->pos.x += game->getRand()*512-256;
  a->pos.z += game->getRand()*512-256;

  //a->vel *= 0.02f;
  a->ang_speed = game->getRand() - 0.5f;
  a->ang_speed *= 0.05f;
  a->startFrame = 0;
  a->endFrame = 12; 
  a->image = 0;
  a->curFrame = game->getRand() * a->endFrame;
  a->animSpeed = 0.05f;

 myPart.update();

  putInGrid(game->mgrid);
}//update


void 
xPartTest::render(void) 
{

}//render

  

void 
xPartTest::render2(xRender * r)
{
  gamex::cMat mat;
  xCam * cam;
  cam = game->getCamPtr();

  mat.setView(&(cam->pos), &(cam->ori));

//  game->getCamPtr()->getCamMat(mat);
   myPart.sortPart(cam->pos);
   myPart.updateMesh(mat.m);
//    myPart.render();

    //new step -- upload the vertex data to the gpu
//    mesh.updateVboVertex();

    xEnt * e;

    e = r->addFrame(1);
 
    //myTest.pos.y = myMap.getHeight(myTest.pos.x, myTest.pos.z);

      //a = myRender.addFrame(0);
      e->pos = 0; //pos;
      e->sortpos = pos;
      e->blend = 2; //0 solid   1 alpha test   2 transparent   3 additive
      e->fmesh = &myPart.mesh;
      e->vmesh = &myPart.mesh;
      e->alpha = 1;
      e->color = 1;
      e->twoSide = 1;
      e->skin = myPart.skin;
      //needs 3 (modulate) for per/particle (per vertex) alpha
      e->skinBlend = 3; //1; //looks strange with 1, could be used for horror theme
      e->useColor = 1;

}//render2
