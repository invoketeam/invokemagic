#pragma warning (disable : 4786)

#include "xUnit.h"

#include "xInvokeCommon.h"

#include "../gamex/xGLCommon.h"




xUnit::xUnit(void) 
{
  yaw = 0.0f;
  curFrame = 0.0f;
  skin = 0;
  color = 0xFFffFFff;



  cmd = 0;
  cmdTarg = 0;

  anim = 0;

  dontMove = 0;

}//ctor


xUnit::~xUnit(void) 
{

}//dtor


void 
xUnit::init(void)
{
  //each unit will need its own skeleton and model
  //animation can be shared

  xSkel * ks;
  ks = game->assetMan->getSkel("knight_skel");
  skel.copySkel(ks, false); //make a shallow copy of the skeleton

  anim = game->assetMan->getBoneAnim("knight_walk");

  skin = getSkin("knight_skin2");

  xMdx3 * m;
  m = game->getMdx("knight_mesh");
  mesh.copyMesh(m); // (in theory we only need to copy the vertices, but the rest doesnt share that much data)


 mesh.makeVbo();

   hp = 100.0f;



  xrad = 32;
  yrad = 64;
  zrad = 32;



  yaw = game->getRand() * 6.28f;

  spectype = 100;

  flags = FR_SELECTABLE | FR_BLOCKMOVE | FR_ATTACKABLE;

 // shadowMesh = game->getMdx("shadow");
 // shadowSkin = game->getSkin("shadow");

  //color = game->getRand() * 32768;

//  teamColor.set(game->getRand(), game->getRand(), game->getRand());

  if (team == 1) //red
  { teamColor.set(1, 0, 0);  }
  else if (team == 2) { teamColor.set(0, 0, 1); }  //blue
  else if (team == 0) { teamColor.set(0.5, 0.5, 0.5); } //grey (neutral?) 
  else { teamColor.set(0,1,0); } //unknown team

}//init




/*
static gamex::cVec3f getVel(gamex::cVec3f * start, gamex::cVec3f * end, float maxLength)
{
  gamex::cVec3f ret;
  ret = *end;
  ret -= *start;
  if (ret.getMagSquared() > maxLength*maxLength)
  {
    ret.normPrec();
    ret *= maxLength;
  }
  return ret;
}//getvel
*/


//todo -- add attackerid parameter (check if its the same as targid, check if its on same team etc)
void 
xUnit::gotHit(float dmg, int dtype, float hx, float hy, float hz)
{
	hp -= dmg;

  //todo -- make spark texture full white so it can be used as any color
  int i;
  xBigPart * b;
  for (i = 0; i < 4; i++)
  {
    b = game->addPart("spark");
    b->pos = pos;
    b->roll = game->getRand()*6.28f;
    b->scale = 3;
    b->rgb = teamColor;
    b->vel.set(4*(game->getRand()-0.5f),4*(game->getRand()-0.5f),4*(game->getRand()-0.5f));
  }//nexti

  //printf("gothit %p %0.2f %d  -- %0.2f \n",this,  dmg, dtype, hp);

	if (hp <= 0) { kill(); }

}//gothit

void 
xUnit::onKilled(void)
{

  xBigPart * b;
    b = game->addPart("spark");
    b->pos = pos;
    b->roll = game->getRand()*6.28f;
    b->scale = 7;
    b->rgb = teamColor;
    //b->vel.set(4*(getRand()-0.5f),4*(getRand()-0.5f),4*(getRand()-0.5f));

  
}//onkilled



void 
xUnit::update(void)
{
  float fr;
    fr = 1.0f;

    //animate (temporary)
    curFrame += 0.5f * fr;
    if (curFrame >= anim->numFrame) { curFrame = 0.0f; } 



    //move command (or attack and no target)
    if (cmd == 1 || (cmd == 2 && targid == 0) )
    {
      vel.x = dest.x - pos.x;
      vel.z = dest.z - pos.z;

       vel.z += 0.1f; //slight correction (todo -- fix up getang)

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

       // if (vel.x > ms) { vel.x = ms; }
      //  if (vel.x < -ms) { vel.x = -ms; }
      //  if (vel.z > ms) { vel.z = ms; }
      //  if (vel.z < -ms) { vel.z = -ms; }
      }
  
    }//endif




    


    xActor * a;
    float dx, dz;
    a = 0;


    if (cmd == 3)  //attack (hunt down) target
    {
      targid = cmdTarg;
      a = game->getActor(targid);  if (a == 0) { targid = 0; cmdTarg = 0; cmd = 0; } 
    }


    //turn targetid to an actor pointer or 
    //if targetid doesnt refer to anyone (zero)
    //try to find a nearby target (but not every frame)
      if (targid > 0)  {  a = game->getActor(targid);  if (a == 0) { targid = 0; } }
      else
      {
        if (workb < game->gameTime ) { workb = game->gameTime + 10; 
          a = getTarget(game->mgrid, pos.x-1024, pos.z-1024, 2048, 2048);
          if (a != 0) {targid = a->id; }  else { targid = 0; }
        }
        //todo -- alert others nearby when found a target(?)
      } //endif   

    //command to move -- ignore nearby targets (move is same as retreat)
    if (cmd == 1) {  a = 0; targid = 0; workb = game->gameTime + 5;   } 


    //no command and no enemy in sight -> stay put, reset targetid
    if (cmd <= 0)
    if (a == 0) {   vel.x = 0; vel.z = 0; targid = 0; }


    float apx, apz;

    //todo -- dont use the targets center position
    //calculate the closest point to its rectangle on the XZ plane
    if (a != 0)
    {
      getClosePointXZ(a, &apx, &apz);

      dx = apx - pos.x; if (dx < 0) { dx = -dx; }
      dz = apz - pos.z; if (dz < 0) { dz = -dz; }

      //only start attacking when got closer than range
      //start moving again when out of range

      if (dx < 100.0f && dz < 100.0f) { worka = 1;}
      if (dx > 128.0f && dz > 128.0f) { worka = 0;  }
    }//endif

    
    //follow target
    if (a != 0) 
    if (worka == 0)
    {
     
      vel.x = apx - pos.x;
      vel.z = apz - pos.z;

      if ( !(vel.x == 0 && vel.z == 0))      {   yaw = getAng(vel.z, vel.x);      }


      float ms;
        ms = 4.0f;
     
      vel.x = cos(yaw) * ms;
      vel.z = sin(yaw) * ms;

      //if (vel.x > ms) { vel.x = ms; }      if (vel.x < -ms) { vel.x = -ms; }
      //if (vel.z > ms) { vel.z = ms; }      if (vel.z < -ms) { vel.z = -ms; }

      //hold position
      if (dontMove == 1) { vel.x = 0; vel.z = 0;}

    }//endif


    //attack target
    if (worka == 1)
    {
      if (a == 0) { worka = 0; vel.x = 0; vel.y = 0; targid = 0; return;}
      
      vel.x = apx - pos.x;
      vel.z = apz - pos.z;
      if (vel.x != 0 && vel.z != 0)      {    yaw = getAng(vel.z, vel.x);  }

      vel.x = 0; vel.z = 0;

      //attack enemy (meele)  
      if (reload < game->gameTime) 
      { a->gotHit(16, 1, 0,0,0);
        // printf("en targ [%d] [%d] \n", id, a->targid);
       reload = game->gameTime + 20; }
      
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

  printf("gotmsg  id %d msg %d  -- %d %d %d \n", id, msg, arg0, arg1, arg2);

  //stop moving and attack (todo -- also stop self issued commands?)
  if (msg == MSG_STOP)
  { cmd = -1; vel.x = 0; vel.z = 0; targid = 0; return; }

  //hold position
  if (msg == MSG_HOLD)
  { cmd = -1; vel.x = 0; vel.z = 0; targid = 0; dontMove = 1; return; }


  //any command for moving should let the unit move again
  dontMove = 0;

  if (msg == MSG_MOVE)
  {  cmd = 1;    dest.set(arg0, 0, arg1); return; }


  if (msg == MSG_ATTACK_MOVE)
  { cmd = 2; dest.set(arg0, 0, arg1);  return; }


  if (msg == MSG_ATTACK_TARG)
  { 
    xActor * a;
     cmdTarg = arg0;
     a = game->getActor(cmdTarg);
     if (a == 0) {  } //todo -- error message because this shouldnt happen
     if (a == this) { cmd = -1; vel = 0;  targid = 0; return; } //dont attack self (it looks too funny)
     cmd = 3;
    return;
  }//endif

  if (msg == MSG_PROTECT_TARG)
  {
    printf("[actor %d ]: i dont know how to protect yet :( \n", id);
  }//endif
  
  //all unimplemented commands are ignored for now
 
}//gotmsg




void 
xUnit::render(void) 
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
      }
    }//endif

}//render



  
unsigned int xUnit::getSkin(std::string wname)
{
  return game->getSkin(wname);
}//getskin



void 
xUnit::render2(xRender * r)
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
      e->skin = skin;  
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

  if ( (a->flags & FR_BLOCKMOVE) > 0)
  {
   if (a->pos.x < pos.x) { pos.x +=0.5f; } else { pos.x -=0.5f; }
   if (a->pos.z < pos.z) { pos.z +=0.5f; }  else { pos.z -=0.5f; }
  }//endif


return true;
}//handcol
