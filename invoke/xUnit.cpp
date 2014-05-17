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

  anim = 0;

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



  xMdx3 * m;
  m = game->getMdx("knight_mesh");
  mesh.copyMesh(m); // (in theory we only need to copy the vertices, but the rest doesnt share that much data)


 mesh.makeVbo();



  xrad = 32;
  yrad = 64;
  zrad = 32;



  yaw = game->getRand() * 6.28f;

  spectype = 100;

  flags = FR_SELECTABLE;

 // shadowMesh = game->getMdx("shadow");
 // shadowSkin = game->getSkin("shadow");

  //color = game->getRand() * 32768;

//  teamColor.set(game->getRand(), game->getRand(), game->getRand());

  if (team == 1) //red
  { teamColor.set(1, 0, 0);  }
  else { teamColor.set(0, 0, 1); }  //blue


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
    //b->vel.set(4*(getRand()-0.5f),4*(getRand()-0.5f),4*(getRand()-0.5f));

  
}//onkilled


//find closest target to attack
xActor * 
xUnit::getTarget(xMultiGrid * m, float ax, float az, float aw, float ah)
{
	xCell * c;
	xActor * a;
  float x1, z1;
  float dist;
  float mdist;
  xActor * ret;

	c = m->doQuery(ax, az, aw, ah);
			
  x1 = ax + aw;
  z1 = az + ah;
			
  mdist = 99999999.0f; //todo -- use float max or something
  ret = 0;

	for (c; c != 0; c= c->next )
	{
		for (a = c->first; a != 0; a = a->next)
		{
			if (a == this) { continue; }
			if (a->dead) { continue; }
      if (a->team == team) { continue; }  //ally
      if (a->team == 0) { continue; }  //neutral
					
				if (a->pos.x + a->xrad < ax) { continue; } 
				if (a->pos.x - a->xrad > x1) { continue; }
				if (a->pos.z + a->zrad < az) { continue; }
				if (a->pos.z - a->zrad > z1) { continue; }


      dist = abs(a->pos.x - pos.x) + abs(a->pos.z - pos.z);
      if (mdist < dist) { continue;}
        mdist = dist;
        ret = a;
					
		}//nexta
	}//nextc

return ret;
}//gettarget




void 
xUnit::update(void)
{
  float fr;
    fr = 1.0f;

    curFrame += 0.5f * fr;
    if (curFrame >= anim->numFrame) { curFrame = 0.0f; } 




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




    


    xActor * a;
    float dx, dz;
    a = 0;
    if (targid > 0)  {  a = game->getActor(targid);  }
    else
    {
      if (workb < game->gameTime ) { workb = game->gameTime + 10; 
        a = getTarget(game->mgrid, pos.x-1024, pos.z-1024, 2048, 2048);
        if (a != 0) {targid = a->id; }
        else { targid = 0; }
      }
    } //endif   

    if (cmd == 1) {  a = 0; targid = 0; workb = game->gameTime + 5;   } //move -- ignore targets (retreat)

    if (cmd <= 0)
    if (a == 0) {   vel.x = 0; vel.z = 0; }

    if (a != 0)
    {
      dx = a->pos.x - pos.x; if (dx < 0) { dx = -dx; }
      dz = a->pos.z - pos.z; if (dz < 0) { dz = -dz; }

      //only start attacking when got closer than range
      //start moving again when out of range

      if (dx < 100.0f && dz < 100.0f) { worka = 1;}
      if (dx > 128.0f && dz > 128.0f) { worka = 0;  }
    }//endif

    if (a != 0) 
    if (worka == 0)
    {

      vel.x = a->pos.x - pos.x;
      vel.z = a->pos.z - pos.z;

      if (vel.x != 0 && vel.z != 0)      {    yaw = getAng(vel.z, vel.x);  }

      float ms;
      ms = 4.0f;
      if (vel.x > ms) { vel.x = ms; }      if (vel.x < -ms) { vel.x = -ms; }
      if (vel.z > ms) { vel.z = ms; }      if (vel.z < -ms) { vel.z = -ms; }

    }//endif


    if (worka == 1)
    {
      if (a == 0) { worka = 0; vel.x = 0; vel.y = 0; targid = 0; return;}
      
      vel.x = a->pos.x - pos.x;
      vel.z = a->pos.z - pos.z;
      if (vel.x != 0 && vel.z != 0)      {    yaw = getAng(vel.z, vel.x);  }

      vel.x = 0; vel.z = 0;

      
      if (reload < game->gameTime) { a->gotHit(16, 1, 0,0,0); reload = game->gameTime + 20; }
      
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
