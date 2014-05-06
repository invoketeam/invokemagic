#pragma warning (disable : 4786)

#include "xUnit.h"

#include "xInvokeCommon.h"


xUnit::xUnit(void) 
{
  yaw = 0.0f;
  curFrame = 0.0f;
  skin = 0;
  color = 0xFFffFFff;

//  shadowMesh = 0;
//  shadowSkin = 0;

  cmd = 0;

}//ctor


xUnit::~xUnit(void) 
{

}//dtor


void 
xUnit::init(void)
{
  //each unit will need its own skeleton and model
  //animation can be shared

  mesh.loadFile("data/knight_mesh.mdx3");
  skel.loadFile("data/knight_skel.xskb");
  anim.loadFile("data/knight_walk.banm");
  
  mesh.makeVbo();

  //vel.x = 3;
  //vel.z = 2;

  xrad = 32;
  yrad = 64;
  zrad = 32;

  //vel.x = game->getRand2() * 3.0f;
  //vel.z = game->getRand2() * 3.0f;

  yaw = game->getRand() * 6.28f;

  spectype = 100;

  flags = FR_SELECTABLE;

 // shadowMesh = game->getMdx("shadow");
 // shadowSkin = game->getSkin("shadow");

  //color = game->getRand() * 32768;

  teamColor.set(game->getRand(), game->getRand(), game->getRand());

}//init


  //based on
  //http://stackoverflow.com/questions/16542042/fastest-way-to-sort-vectors-by-angle-without-actually-computing-that-angle
static float getAng(float dy, float dx)
{

  float a;
  a = dx / ((dy < 0.0f ? -dy:dy)+(dx < 0.0f ? -dx:dx));
  if (dy < 0.0f) { return (a - 1) * 1.57075f; }
  return (1.0f - a) * 1.57075f;

}//getang



void 
xUnit::update(void)
{
  float fr;
    fr = 1.0f;

    curFrame += 0.5f * fr;
    if (curFrame >= anim.numFrame) { curFrame = 0.0f; } 


    if (cmd > 0)
    {
      vel.x = dest.x - pos.x;
      vel.z = dest.z - pos.z;

      if (vel.x != 0 && vel.z != 0)
      {
        yaw = getAng(vel.z, vel.x);
      }

      float ms;
      ms = 4.0f;
      if (vel.x > ms) { vel.x = ms; }
      if (vel.x < -ms) { vel.x = -ms; }
      if (vel.z > ms) { vel.z = ms; }
      if (vel.z < -ms) { vel.z = -ms; }

      if (abs(vel.x) < ms && abs(vel.z) < ms) { cmd = -1; vel = 0;}
    }//endif

    pos.x += vel.x;
    pos.z += vel.z;

    pos.y = game->getHeight(pos.x, pos.z) + yrad;

   // yaw += 0.1f;

  /*
    if (vel.x < 0 && pos.x < 0) { vel.x *= -1.0f; }
    else if (vel.x > 0 && pos.x > (32*64)) { vel.x *= -1.0f; }
    if (vel.z < 0 && pos.z < 0) { vel.z *= -1.0f; }
    else if (vel.z > 0 && pos.z > (32*64)) { vel.z *= -1.0f; }
*/

   checkColXZ(game->mgrid);

  putInGrid(game->mgrid);
}//update












void 
xUnit::gotMsg(int msg, int arg0, int arg1, int arg2)
{

  printf("gotmsg %d  --  %d %d %d %d \n", id, msg, arg0, arg1, arg2);

  cmd = msg;
  dest.set(arg0, 0, arg1);



}//gotmsg




void 
xUnit::render(void) 
{

}//render



  
unsigned int xUnit::getSkin(std::string wname)
{
  return game->getSkin(wname);
}//getskin



void 
xUnit::render2(xRender * r)
{
  anim.applyFrame2(curFrame, &skel);
    skel.ori.reset();
    skel.ori.rotYaw(-(yaw-1.57f)); //note: i screwed up the models direction for this one so i need to rotate by 180 as well
    //  skel.ori.rotPitch(yaw*0.5); 

    skel.update();
    skel.applySkin(&mesh, mesh.numVert);

    //new step -- upload the vertex data to the gpu
    mesh.updateVboVertex();


    xEnt * e;

    e = r->addFrame(0);
 
    //myTest.pos.y = myMap.getHeight(myTest.pos.x, myTest.pos.z);

      //a = myRender.addFrame(0);
      e->pos = pos; e->pos.y -= (yrad); 
      e->blend = 0;
      e->fmesh = &mesh;
      e->vmesh = &mesh;
      e->alpha = 1;
      //e->color = 1;
      //e->color.set(1,0,0);
      e->twoSide = 1;
      e->skin = getSkin("knight_skin2") ;
      e->useColor = 0;
      //e->skinBlend = 1;
      //e->skinBlend = 4;
      e->color = teamColor;
      e->skinBlend = 2; //use decal blending for using color as teamcolor
      e->flags = 2 + 4; //cast shadow

    //mesh.render();

    /*
    float d;
    d =  (rend->camPos - pos).getMagSquared();

    if (d > 65384) { lodMesh = gameGetMdx("knight_lod01"); } 
    if (d > (65384+65384)) { lodMesh = gameGetMdx("knight_lod02"); } 
    if (d > (65384+65384+65384)) { lodMesh = gameGetMdx("knight_lod03"); } 


     skel.update();
////     skel.applySkin(&mesh, mesh.numVert);

     skel.applySkin(&mesh, lodMesh->numVert);
    */



  //simple shadow (circle under unit)
  //todo -- make this as an alternative to the projective shadow
  /*

      e = r->addFrame(1);
 
        e->pos = pos;   
        e->pos.y = game->getHeight(pos.x, pos.z) + 3;
        e->blend = 2;  //0 solid   1 alpha test   2 transparent   3 additive
        e->fmesh = shadowMesh;
        e->vmesh = shadowMesh;
        e->alpha = 1;
        e->color = 1;
        e->twoSide = 1;
        e->skin = shadowSkin;
        e->useColor = 0; ///1 use color data of mesh or 0 for the e->color of xEnt
        e->flags = 1;
  */
}//render2



bool
xUnit::handCol(xActor * a)
{

  //so simple and yet it works
  //(a further improvement will be to figure out positions before sending units)
  //so they dont try to arrive at the same exact spot

  if (a->pos.x < pos.x) { pos.x +=0.5f; } else { pos.x -=0.5f; }
  if (a->pos.z < pos.z) { pos.z +=0.5f; }  else { pos.z -=0.5f; }



return true;
}//handcol
