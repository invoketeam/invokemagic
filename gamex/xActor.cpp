#pragma warning (disable : 4786)

#include <math.h>

#include "xActor.h"
#include "xGame.h"
#include "xCell.h"
#include "xMultiGrid.h"
#include "xRender.h"

 xActor::xActor(void) 
 {
  game = 0;

  
  cell = 0;
  next = 0;
  prev = 0;

  hp = 100;
  ang = 0;
  
  id = -1;
  team = -1;
  
  flags = 0;
  spectype = 0;
  
  reload = 0;

  
  visible = true;
  dead = false;
  asleep = false;

  xrad = 8.0f;
  yrad = 8.0f;
  zrad = 8.0f;
  rad = 8.0f;

  tag = 0;

  color = 0;

  targid = 0;
  ownerid = 0;

  worka = 0;  workb = 0;  workc = 0;
  workx = 0.0f;  worky = 0.0f;  workz = 0.0f;


 }//ctor


 xActor::~xActor(void)
 {
 
	takeOutOfGrid();
 }//dtor


void 
xActor::putInGrid(xMultiGrid * mgrid) 
{
  mgrid->putInXZ(this);
}//putin

void 
xActor::putInGridXY(xMultiGrid * mgrid) 
{
  mgrid->putInXZ(this);
}//putin

void 
xActor::putInGridXZ(xMultiGrid * mgrid) 
{
  mgrid->putInXZ(this);
}//putin


void 
xActor::takeOutOfGrid(void) 
{
 if (cell != 0) { cell->remActor(this); }
}//takeout


void 
xActor::kill(void)
{
  if (dead) { return;} 
  dead = true;
  onKilled();
}//kill

  

void 
xActor::gotHit(float dmg, int dtype, float hx, float hy, float hz)
{
	hp -= dmg;
	if (hp <= 0) { kill(); }

}//gothit





bool 
xActor::overlap(xActor * a)
{
  if (pos.x+xrad <  a->pos.x - a->xrad) { return false; }
  if (pos.x-xrad > a->pos.x + a->xrad) { return false; }
  if (pos.y+yrad <  a->pos.y - a->yrad) { return false; }
  if (pos.y-yrad > a->pos.y + a->yrad) { return false; }
  if (pos.z+zrad <  a->pos.z - a->zrad) { return false; }
  if (pos.z-zrad > a->pos.z + a->zrad) { return false; }
  
  return true;
}//overlap



bool 
xActor::overlapXY(xActor * a)
{
  if (pos.x+xrad <  a->pos.x - a->xrad) { return false; }
  if (pos.x-xrad > a->pos.x + a->xrad) { return false; }
  if (pos.y+yrad <  a->pos.y - a->yrad) { return false; }
  if (pos.y-yrad > a->pos.y + a->yrad) { return false; }
 
  return true;
}//overxy



bool 
xActor::overlapXZ(xActor * a)
{
  if (pos.x+xrad <  a->pos.x - a->xrad) { return false; }
  if (pos.x-xrad > a->pos.x + a->xrad) { return false; }
  if (pos.z+zrad <  a->pos.z - a->zrad) { return false; }
  if (pos.z-zrad > a->pos.z + a->zrad) { return false; }
  
  return true;
}//overxz





void 
xActor::checkColXZ(xMultiGrid * m)
  {
	xCell * c;
	xActor * a;
	float x0, y0, x1, y1, z0, z1;

	x0 = pos.x - xrad;	y0 = pos.y - yrad;	z0 = pos.z - zrad;
	x1 = pos.x + xrad;	y1 = pos.y + yrad;	z1 = pos.z + zrad;

	c = m->doQuery(x0, z0, xrad+xrad, zrad+zrad);
			
			
	for (c; c != 0; c= c->next )
	{
		for (a = c->first; a != 0; a = a->next)
		{
			if (a == this) { continue;  }
			if (a->dead) { continue; }
					
			//check if the two aabb overlaps
				if (a->pos.x + a->xrad < x0) { continue; } 
				if (a->pos.y + a->yrad < y0) { continue; }
				if (a->pos.x - a->xrad > x1) { continue; }
				if (a->pos.y - a->yrad > y1) { continue; }
				if (a->pos.z + a->zrad < z0) { continue; }
				if (a->pos.z - a->zrad > z1) { continue; }

					if (!handCol(a)) { return; } //if it returns false then no more checks are needed
					
		}//nexta
	}//nextc

 }//checkol
  
  

void 
xActor::checkColXY(xMultiGrid * m)
  {
	xCell * c;
	xActor * a;
	float x0, y0, x1, y1, z0, z1;

	x0 = pos.x - xrad;	y0 = pos.y - yrad;	z0 = pos.z - zrad;
	x1 = pos.x + xrad;	y1 = pos.y + yrad;	z1 = pos.z + zrad;

	c = m->doQuery(x0, y0, xrad+xrad, yrad+yrad);
			
			
	for (c; c != 0; c= c->next )
	{
		for (a = c->first; a != 0; a = a->next)
		{
			if (a == this) { continue;  }
			if (a->dead) { continue; }
					
			//check if the two aabb overlaps
				if (a->pos.x + a->xrad < x0) { continue; } 
				if (a->pos.y + a->yrad < y0) { continue; }
				if (a->pos.x - a->xrad > x1) { continue; }
				if (a->pos.y - a->yrad > y1) { continue; }
				if (a->pos.z + a->zrad < z0) { continue; }
				if (a->pos.z - a->zrad > z1) { continue; }

					if (!handCol(a)) { return; } //if it returns false then no more checks are needed
					
		}//nexta
	}//nextc

 }//checkol

