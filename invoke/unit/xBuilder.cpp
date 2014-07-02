#pragma warning (disable : 4786) 

#include "xBuilder.h"

#include "../../gamex/xMultiGrid.h"
#include "../../gamex/xGame.h"
#include "../../gamex/xRender.h"
#include "../../gamex/xBigPart.h"

#include "../xInvokeCommon.h"



xBuilder::xBuilder(void)
{

  cmd = 0; 
  

  yaw = 0.0f;
  curFrame = 0.0f;

  skin = 0;
  
  color = 0xFFffFFff;

}//ctor


xBuilder::~xBuilder(void)
{
  
}//dtor


  
void 
xBuilder::init(void)
{
  xMdx3 * m;
   m = game->getMdx("knight_mesh");
   mesh.copyMesh(m);
  mesh.makeVbo();

  xSkel * ks;
    ks = game->assetMan->getSkel("knight_skel");
    skel.copySkel(ks, false); //make a shallow copy of the skeleton

  anim = game->assetMan->getBoneAnim("knight_walk");

  skin = game->getSkin("knight_skin2");


    team = 1; //testing

   hp = 100.0f;
  
  xrad = 32;  yrad = 64;  zrad = 32;
  spectype = 100;
  flags = FR_SELECTABLE | FR_BLOCKMOVE | FR_ATTACKABLE;

 pos.y = game->getHeight(pos.x, pos.z) + yrad;

    putInGridXZ(game->mgrid);
}//init




void 
xBuilder::update(void)
{

   float fr;
    fr = 1.0f;

    //animate (temporary)
    curFrame += 0.5f * fr;
    if (curFrame >= anim->numFrame) { curFrame = 0.0f; } 


  xActor * a;
  a = 0;

  
  if (targid > 0)
  {
    a = game->getActor(targid);
    if (a == 0) { vel = 0; targid = 0; cmd = -1;  return; }
  }//endif   

  if (a != 0)
  if (cmd == 20) //go to building site
  {
    getClosePointXZ(a, &dest.x, &dest.z);
    vel.x = dest.x - pos.x;      vel.z = dest.z - pos.z;

    if ( !(vel.x == 0 && vel.z == 0))      {   yaw = getAng(vel.z, vel.x);      }

    float ms;      ms = 7.0f;
    if (abs(vel.x) <= 32 && abs(vel.z) <= 32) { cmd = 21; vel = 0;   }
    else    {   vel.x = cos(yaw) * ms;        vel.z = sin(yaw) * ms;      }
      
    //printf("builder cmd 20   %d \n ", targid);

  }//endif
  

  if (a != 0)
  if (cmd == 21) //build
  {
    vel = 0;
    
    if ( (a->flags & FR_BUILDABLE) > 0)
    {   
      //todo -- use enum or constant or whatever for 999 (special hit type - build)
      a->gotHit(0, 999, 0,0,0); 
    }
    else { targid = 0; }
    
    //printf("builder cmd 21   %d \n ", targid);
  }//endif  




    //move command (or attack and no target)
    if (cmd == 1 || (cmd == 2 && targid == 0) )
    {
      vel.x = dest.x - pos.x;
      vel.z = dest.z - pos.z;

      vel.z += 0.1f;

      if (vel.x != 0 && vel.z != 0)
      {
        yaw = getAng(vel.z, vel.x);
      }

      float ms;
      ms = 4.0f;

      if (abs(vel.x) <= ms && abs(vel.z) <= ms) { cmd = -1; vel = 0;  printf("arrived \n"); }
      else
      {
        vel.x = cos(yaw) * ms;
        vel.z = sin(yaw) * ms;
      }
  
    }//endif


   pos.x += vel.x;
   pos.z += vel.z;
   pos.y = game->getHeight(pos.x, pos.z) + yrad;

   checkColXZ(game->mgrid);
 
  putInGridXZ(game->mgrid);

}//update




void 
xBuilder::render(void) 
{
    xActor * t;

    glColor3f(0,1,0);

    if (targid != 0)
    {
      t = game->getActor(targid);
      if (t != 0)
      {
        glBegin(GL_LINES);
          glVertex3f(pos.x,pos.y, pos.z);
          glVertex3f(t->pos.x, t->pos.y, t->pos.z);
        glEnd();


      /*
        float apx, apz; 
        getClosePointXZ(t, &apx, &apz);

       glColor3f(0,1,1);
       glBegin(GL_LINES);
          glVertex3f(pos.x,pos.y, pos.z);
          glVertex3f(apx, t->pos.y, apz);
        glEnd();
      */
      }
    }//endif
}//render



void 
xBuilder::render2(xRender * r)
{
  anim->applyFrame2(curFrame, &skel);
    skel.ori.reset();
    skel.ori.rotYaw(-(yaw-1.57f)); //note: i screwed up the models direction for this one so i need to rotate by 180 as well
    //  skel.ori.rotPitch(yaw*0.5); 

    skel.update();
    skel.applySkin(&mesh, mesh.numVert);

    //new step -- upload the vertex data to the gpu
    mesh.updateVboVertex();


  xEnt * e;

    e = r->addFrame(0);
 
      e->pos = pos; 
      e->pos.y -= (yrad); 
      
      e->blend = 0;
      e->fmesh = &mesh;
      e->vmesh = &mesh;
      e->alpha = 1;
      e->twoSide = 1;
      e->skin =  skin;  
      e->useColor = 0;
      e->color = 1; //teamColor;
      e->skinBlend = 2; //use decal blending for using color as teamcolor
      e->flags = 2 + 4; //cast shadow

}//render2


void 
xBuilder::onKilled(void)
{

}//onkilled

 
void 
xBuilder::gotMsg(int msg, int arg0, int arg1, int arg2)
{
 if (msg == MSG_BUILD)
 {
 // printf("builder got build command  %d  %d \n", id, arg0);

  targid = 0;
  xActor * a;
  a = game->getActor(arg0);
  if (a == 0) { return; }
  if (a->team != team) { return; } //not on same team //todo -- transform command to attack? move to?
  if ((a->flags & FR_BUILDABLE) > 0) { cmd = 20; targid = arg0;  return; }
 
 }//endif

 if (msg == MSG_MOVE)
 {  cmd = 1;    dest.set(arg0, 0, arg1); return; }

}//gotmsg





bool
xBuilder::handCol(xActor * a)
{

  //so simple and yet it works
  //(a further improvement will be to figure out positions before sending units)
  //so they dont try to arrive at the same exact spot

  if ( (a->flags & FR_BLOCKMOVE) > 0)
  {
   if (a->pos.x < pos.x) { pos.x +=0.5f; } else { pos.x -=0.5f; }
   if (a->pos.z < pos.z) { pos.z +=0.5f; }  else { pos.z -=0.5f; }
  }//endif


return true;
}//handcol










  
  