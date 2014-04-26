#pragma warning (disable : 4786)

#include "xBuildTest.h"

#include "xInvokeCommon.h"



xBuildTest::xBuildTest(void) 
{
  yaw = 0.0f;
  skin = 0;
  color = 0xFFffFFff;
}//ctor


xBuildTest::~xBuildTest(void) 
{

}//dtor


void 
xBuildTest::init(void)
{

//todo -- get file from game

  //mesh.loadFile("data/build/kunyho.mdx3");
  //mesh.makeVbo();


  xrad = 256;
  yrad = 128;
  zrad = 256;

  mesh = game->getMdx("kunyho");
  skin = game->getSkin("kunyho");

  spectype = 100;


 flags = FR_SELECTABLE;

 pos.y = game->getHeight(pos.x, pos.z) + yrad;


}//init



void 
xBuildTest::update(void)
{

  putInGrid(game->mgrid);
}//update


void 
xBuildTest::render2(xRender * r)
{
    xEnt * e;

    e = r->addFrame(0);
      e->pos = pos;
      e->pos.y -= yrad;
      e->blend = 0;
      e->fmesh = mesh;
      e->vmesh = mesh;
      e->alpha = 1;
      e->color = 1;
      e->twoSide = 0;
      e->skin = skin ;


}//render2



