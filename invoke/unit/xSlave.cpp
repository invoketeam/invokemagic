#pragma warning (disable : 4786) 

#include "xSlave.h"

#include "../../gamex/xMultiGrid.h"
#include "../../gamex/xGame.h"
#include "../../gamex/xRender.h"
#include "../../gamex/xBigPart.h"

#include "../xInvokeCommon.h"



xSlave::xSlave(void)
{
  hrad = 512.0f;
  cmd = 0; 
  
  load = 0.0f;
  maxLoad = 5.0f;

  yaw = 0.0f;
  curFrame = 0.0f;

  skin = 0;
  
}//ctor


xSlave::~xSlave(void)
{
  
}//dtor


  
void 
xSlave::init(void)
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



   hp = 100.0f;
  
  xrad = 32;  yrad = 64;  zrad = 32;
  spectype = 100;
  flags = FR_SELECTABLE | FR_BLOCKMOVE | FR_ATTACKABLE;

 pos.y = game->getHeight(pos.x, pos.z) + yrad;

    putInGridXZ(game->mgrid);
}//init


//todo -- make this function part of common
static float getAng(float dy, float dx)
{

  float a;
  a = dx / ((dy < 0.0f ? -dy:dy)+(dx < 0.0f ? -dx:dx));
  if (dy < 0.0f) { return (a - 1) * 1.57075f; }
  return (1.0f - a) * 1.57075f;

}//getang




void 
xSlave::update(void)
{

   float fr;
    fr = 1.0f;

    //animate (temporary)
    curFrame += 0.5f * fr;
    if (curFrame >= anim->numFrame) { curFrame = 0.0f; } 

  //for now dont follow commands, go to the closest harvestable
  //and.. harvest it

  xActor * a;
  a = 0;
  
  if (targid > 0)
  {
    a = game->getActor(targid);
    if (a == 0) { targid = 0; cmd = -1;  return; }
  }   
  else if (workb < game->gameTime ) 
  {  workb = game->gameTime + 10; 
      a = getHarvest(game->mgrid, pos.x-2048, pos.z-2048, 4096, 4096);
      if (a != 0) {targid = a->id;  cmd = 10;}  else { targid = 0; }
  }//endif
 
 
  if (cmd == 10)  //goto resource
  {
      dest = a->pos;
      vel.x = dest.x - pos.x;      vel.z = dest.z - pos.z;

      if (vel.x != 0 && vel.z != 0)
      {   yaw = getAng(vel.z, vel.x);      }

      float ms;      ms = 7.0f;
      if (abs(vel.x) <= 32 && abs(vel.z) <= 32) { cmd = 11; vel = 0;   }
      else    {   vel.x = cos(yaw) * ms;        vel.z = sin(yaw) * ms;      }
      
  }//endif
  
  if (cmd == 11) //harvest
  {
    //attack harvestable (harvest it)
    if (reload < game->gameTime) 
    {
      a->gotHit(10, 1, 0,0,0);
      load += 1;
     reload = game->gameTime + 20; }
  }//endif  
  

   pos.x += vel.x;
   pos.z += vel.z;
   pos.y = game->getHeight(pos.x, pos.z) + yrad;
 
  putInGridXZ(game->mgrid);

}//update




void 
xSlave::render(void) 
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



void 
xSlave::render2(xRender * r)
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
xSlave::onKilled(void)
{

}//onkilled

 
void 
xSlave::gotMsg(int msg, int arg0, int arg1, int arg2)
{
 

}//gotmsg



//almost the same as gettarget but doesnt check if you are allies with anything
xActor * 
xSlave::getHarvest(xMultiGrid * m, float ax, float az, float aw, float ah)
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
			
  mdist = 99999999.0f;
  ret = 0;

	for (c; c != 0; c= c->next )
	{
		for (a = c->first; a != 0; a = a->next)
		{
			if (a == this) { continue; }
			if (a->dead) { continue; }
      if ((a->flags & FR_HARVESTABLE) == 0) { continue; } //actor not attackable
     					
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
}//getharvest


bool
xSlave::handCol(xActor * a)
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










  
  