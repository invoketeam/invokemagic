#pragma warning (disable : 4786)

#include "xInvActor.h"

#include "xInvokeCommon.h"

//find closest target to attack
xActor * 
xInvActor::getTarget(xMultiGrid * m, float ax, float az, float aw, float ah)
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
      if ((a->flags & FR_ATTACKABLE) == 0) { continue; } //actor not attackable
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
xInvActor::getClosePointXZ(xActor * a, float * retx, float * retz)
{
  gamex::cVec3f ret;

  float ix;
  float iz;

  if (a->pos.x - a->xrad > pos.x) { ix = a->pos.x - a->xrad; }
  else if (a->pos.x + a->xrad < pos.x) { ix = a->pos.x + a->xrad; }
  else { ix = pos.x; }   

  if (a->pos.z - a->zrad > pos.z) { iz = a->pos.z - a->zrad; }
  else if (a->pos.z + a->zrad < pos.z) { iz = a->pos.z + a->zrad; }
  else { iz = pos.z; }   

  *retx = ix;
  *retz = iz;
  
}//getclose



