#pragma warning (disable : 4786)

#include "xBuildTest.h"


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

  mesh = game->getMdx("kunyho");
  skin = game->getSkin("kunyho_textura_walpha");

  spectype = 100;

   pos.y = game->getHeight(pos.x, pos.z);

}//init



void 
xBuildTest::update(void)
{

}//update


void 
xBuildTest::render2(xRender * r)
{
    xEnt * e;

    e = r->addFrame(0);
      e->pos = pos;
      e->blend = 0;
      e->fmesh = mesh;
      e->vmesh = mesh;
      e->alpha = 1;
      e->color = 1;
      e->twoSide = 0;
      e->skin = skin ;


}//render2



