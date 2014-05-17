#pragma warning (disable : 4786)

#include "xBuildTest.h"

#include "xInvokeCommon.h"

#include "xUnit.h"




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

  //todo -- use an oobb for selection
  //and use the aabb for optimising

  xrad = 256+128;
  yrad = 128;
  zrad = 256+128;

  mesh = game->getMdx("kunyho");
  skin = game->getSkin("kunyho_textura_walpha");

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
      e->flags = 2 + 4; //cast shadow
      e->ori.rotYaw(3.1415f*-0.2f);

  //todo -- render rally point


}//render2



void 
xBuildTest::gotMsg(int msg, int arg0, int arg1, int arg2)
{

  printf("gotmsg %d  --  %d %d %d %d \n", id, msg, arg0, arg1, arg2);


  //testing -- create an unit and send it to direction

    if (msg == MSG_MOVE)
    {
      xActor * a;
      
      a = new xUnit();
      a->pos = pos;

      a->gotMsg(msg, arg0, arg1, arg2);

      game->addActor(a);


    }//endif

  

}//gotmsg







