#pragma warning (disable : 4786)


#include "xSelect.h"

#include "xInvokeCommon.h"


#include "../gamex/xDebug.h"

#include "../gamex/xGLCommon.h"

#include "../gamex/xGame.h"
#include "../gamex/xMultiGrid.h"
#include "../gamex/xCell.h"

#include "../gamex/xActor.h"
#include "../gamex/xCam.h"

#include "../gamex/xFrustum.h"







static int get2DCoord(float * view, float * proj, gamex::cVec3f * pos, float * retx, float * rety)
{
	float ta[4];
	float tb[4];

	ta[0] = pos->x;
	ta[1] = pos->y;
	ta[2] = pos->z;
	ta[3] = 1.0f;

	tb[0] = ta[0] * view[0] + ta[1] * view[4] + ta[2] * view[8] + ta[3] * view[12];
	tb[1] = ta[0] * view[1] + ta[1] * view[5] + ta[2] * view[9] + ta[3] * view[13];
	tb[2] = ta[0] * view[2] + ta[1] * view[6] + ta[2] * view[10] + ta[3] * view[14];
	tb[3] = ta[0] * view[3] + ta[1] * view[7] + ta[2] * view[11] + ta[3] * view[15];

	ta[0] = tb[0] * proj[0] + tb[1] * proj[4] + tb[2] * proj[8] + tb[3] * proj[12];
	ta[1] = tb[0] * proj[1] + tb[1] * proj[5] + tb[2] * proj[9] + tb[3] * proj[13];
	ta[2] = tb[0] * proj[2] + tb[1] * proj[6] + tb[2] * proj[10] + tb[3] * proj[14];
	ta[3] = tb[0] * proj[3] + tb[1] * proj[7] + tb[2] * proj[11] + tb[3] * proj[15];

	if (ta[3] == 0.0f) return(0);
	ta[0] /= ta[3];
	ta[1] /= ta[3];
	//    ta[2] /= ta[3];
	/* Map x, y and z to range 0-1 */
	ta[0] = ta[0] * 0.5 + 0.5;
	ta[1] = ta[1] * 0.5 + 0.5;
	//    ta[2] = in[2] * 0.5 + 0.5;

	ta[0] = ((ta[0]) *2) -1;
	ta[1] = ((ta[1]) *2) -1;

	*retx = ta[0];
	*rety = ta[1];


	return (1);
}//get2d















xSelect::xSelect(void)
 {  vecOver = 0; maxSel = 0;  
    numOver = 0;
    unitId = 0;
    overId = 0;
 }//ctor

xSelect::~xSelect(void) 
{ clear(); }//dtor
  
void 
xSelect::init(void) 
{
  clear();

  maxSel = 4096;
  vecOver = new int[maxSel];
  numOver = 0;
  unitId = 0;
  overId = 0;
}//init

void 
xSelect::clear(void) 
{
  if (vecOver != 0) { delete [] vecOver; vecOver = 0; }
  maxSel = 0;
  numOver = 0;
}//clear



void 
xSelect::resetSelect(void)
{
  setId.clear();
}//reset




void 
xSelect::appendOverToSelect(void)
{
  int i;
 
  //no need to check if unit is already selected (hooray for std::set)


  for (i = 0; i < numOver; i++)
  {
    
    setId.insert(vecOver[i]);
  }//nexti



    printf("appendselect %d %d \n",numOver, setId.size());


}//append



void 
xSelect::appendSingleToSelect(void)
{

  if (overId != 0) { setId.insert(overId); }

  if (overId != 0)  printf("appendselectSingle %d  \n",overId);


}//appendsingle








void
xSelect::eraseOver(void)
{

  int i;
  for (i = 0; i < numOver; i++)
  {
    setId.erase(vecOver[i]);
  }//nexti

 printf("eraseselect %d %d \n",numOver, setId.size());

}//eraseover

  
void 
xSelect::eraseSingle(void)
{

 if (overId != 0) { setId.erase(overId); }
 if (overId != 0)  printf("eraseselect Single %d  \n",overId);

}//erasesingle












//idea
//instead of setting the units rectangle size
//use the selection mouse size bigger (?)

//also units should project more points to make this easier
//e.g. a building will need a bigger selection

//(maybe for single selections, raycast to the scene instead?)
//todo -- this is for later, if its noticeably hard to use
//so far its seems to work ok

//another approach is to see which one overlaps the aabb at the terrain at the mouse position
//but this wont work for flying units, but would work amazingly on buildings and ground units ( i guess)

//so the raycast will need the most work but would be the best approach
//for single unit selections that is

//for rectangle selection only moving units are allowed anyway (no buildings)


void 
xSelect::selectOver(xCam * cam, xMultiGrid * mgrid, 
 float ax, float ay, float aw, float ah,
 float bx, float by, float bw, float bh  )
{
	xCell * c;	xActor * a;	
  float x0, x1, y0, y1; //rectangle on grid
  int it;


  //for now use bigger rectangle
  float rad;
  rad = 16.0f;
  bx-=rad;
  by-=rad;
  bw+=rad+rad;
  bh+=rad+rad;

  //float bx, float by, float bw, float bh  //rectangle on screen

  //  printf("bx by %0.2f %0.2f  %0.2f %0.2f \n", bx,by, bw,bh);

  gamex::cMat proj;
  gamex::cMat view;
  float sx, sy; //projected point

  proj.setPerspective(cam->fov, cam->aspect, cam->neard, cam->fard);
  view.setView(&cam->pos, &cam->ori);


  //get2DCoord(view.m, proj.m, &(myCursor.coord), &sx, &sy);

  //todo -- this will need some work
  //drawRect(sx*320+320, 480-(sy*240+240), 4,4);
 

  x0 = ax;
  x1 = ax + aw;
  y0 = ay;
  y1 = ay + ah;

	c = mgrid->doQuery(x0, y0, aw, ah);
			
  it = 0;

	for (c; c != 0; c= c->next )
	{
		for (a = c->first; a != 0; a = a->next)
		{
			if (a->dead) { continue; }

      //check if actor is selectable
      if ((a->flags & FR_SELECTABLE) == 0) { continue; }

      //project unit to screen
      get2DCoord(view.m, proj.m, &(a->pos), &sx, &sy);
        
      drawRect(sx*320+320-2, 480-(sy*240+240)-2, 4,4);

      sx = sx*320+320;
      sy =  480-(sy*240+240);

      if (sx < bx) { continue; }
      if (sy < by) { continue; }
      if (sx > bx + bw) { continue; }
      if (sy > by + bh) { continue; }

      drawRect(sx-4, sy-4, 8,8);




        vecOver[it] = a->id;
        it += 1;
        if (it >= maxSel) { numOver = it; return; } //reached maximum selection
		}//nexta
	}//nextc

  numOver = it;

}//selectunit


void 
xSelect::sendMsg(xGame * game, int msg, int arg0, int arg1, int arg2)
{
  std::set <int> ::iterator it;
  xActor * a;

  for (it = setId.begin(); it != setId.end(); it++)
  {
    a = game->getActor(*it);
    if (a == 0) { continue; }

    a->gotMsg(msg, arg0, arg1, arg2);
  }//nextit
}//sendmsg


void 
xSelect::debRender(xGame * game)
{
  std::set <int> ::iterator it;
  xActor * a;

  for (it = setId.begin(); it != setId.end(); it++)
  {
    a = game->getActor(*it);
    if (a == 0) { continue; }
    drawCube(a->pos.x-a->xrad, a->pos.y-a->yrad, a->pos.z-a->zrad,a->xrad+a->xrad,a->yrad+a->yrad,a->zrad+a->zrad);
    
  }//nextit


  a = game->getActor(overId);
  if (a != 0)
  {
    drawCube(a->pos.x-a->xrad, a->pos.y-a->yrad, a->pos.z-a->zrad,a->xrad+a->xrad,a->yrad+a->yrad,a->zrad+a->zrad);
  }

}//debrender











// source: Christer Ericson (2005) 'Real-Time Collision Detection' p183-184
// bmin should be smaller value than bmax
// p0 and p1 is the segment

inline static bool testSegmentAABB(gamex::cVec3f &p0, gamex::cVec3f &p1, gamex::cVec3f &bmin, gamex::cVec3f &bmax)
{

//strangely enough the code only seems
//to work right with the suggested optimization (what the heck)
	gamex::cVec3f e = bmax - bmin;
	gamex::cVec3f d = p1 - p0;
	gamex::cVec3f m = p0 + p1 - bmin - bmax;

float adx = abs(d.x);
	if (abs(m.x) > e.x + adx) return false; //0;

float ady = abs(d.y);
	if (abs(m.y) > e.y + ady) return false; //0;

float adz = abs(d.z);
	if (abs(m.z) > e.z + adz) return false; //0;


// Add in an epsilon term to counteract arithmetic errors when segment is
// (near) parallel to a coordinate axis (see text for detail)
	adx += 0.000001f; //EPSILON; 
	ady += 0.000001f; //EPSILON; 
	adz += 0.000001f; //EPSILON;

// Try cross products of segment direction vector with coordinate axes
	if (abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) return false;//0;
	if (abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) return false;//0;
	if (abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return false;//0;

// No separating axis found; segment must be overlapping AABB
	return true; //1;

}//testsegaab



void 
xSelect::updateOver(xCam * cam, xMultiGrid * mgrid,   float ax, float ay, float aw, float ah, float umx, float umy)
{



	xCell * c;	xActor * a;	
  float x0, x1, y0, y1; //rectangle on grid
  int it;

//  int last;
//  float t;

  gamex::cVec3f bmin;
  gamex::cVec3f bmax;

  gamex::cVec3f p0;
  gamex::cVec3f p1;


  float kx, ky;
  kx =  ((umx*2)-1)*-1;
  ky =  ((umy*2)-1)*-1;

  xFrustum frust; 
  
      frust.setPerspective(cam->fov, cam->aspect, cam->neard, cam->fard);
	    frust.setPoints(cam->pos, cam->ori, 0, 0);

	    p0 = frust.nc;
	    p0 += frust.nearUp * ky;
	    p0 += frust.nearSide * kx;

	    p1 = frust.fc;
	    p1 += frust.farUp * ky;
	    p1 += frust.farSide * kx;




  overId = 0;

  x0 = ax;  x1 = ax + aw;  y0 = ay;  y1 = ay + ah;

	c = mgrid->doQuery(x0, y0, aw, ah);
			
  it = 0;

	for (c; c != 0; c= c->next )
	{
		for (a = c->first; a != 0; a = a->next)
		{
			if (a->dead) { continue; }

      //check if actor is selectable
      if ((a->flags & FR_SELECTABLE) == 0) { continue; }


      bmin.x = a->pos.x - a->xrad;
      bmin.y = a->pos.y - a->yrad;
      bmin.z = a->pos.z - a->zrad;

      bmax.x = a->pos.x + a->xrad;
      bmax.y = a->pos.y + a->yrad;
      bmax.z = a->pos.z + a->zrad;

      if (testSegmentAABB(p0,p1, bmin, bmax)) { overId = a->id; }      



       // vecOver[it] = a->id;
       // it += 1;
       // if (it >= maxSel) { numOver = it; return; } //reached maximum selection
		}//nexta
	}//nextc

  //numOver = it;



}//updateover





















