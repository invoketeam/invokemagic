#pragma once

#include "xCam.h"
#include "xFrustum.h"
#include "xColMesh.h"

class xCursor
{
public:
  gamex::cVec3f coord;
  
public:

//umx and umy needs to be in  the range of [-1.0, 1.0]
  void update(xCam * cam, float umx, float umy, xColMesh * cmesh = 0, float planey = 0.0f )
  {
    gamex::cVec3f ppos;
	  gamex::cVec3f pnorm;
	  gamex::cVec3f p0, p1;
	  gamex::cVec3f ret;
	  float t;
    xFrustum frust; 
  
      frust.setPerspective(cam->fov, cam->aspect, cam->neard, cam->fard);
	    frust.setPoints(cam->pos, cam->ori, 0, 0);

	    p0 = frust.nc;
	    p0 += frust.nearUp * umy;
	    p0 += frust.nearSide * umx;

	    p1 = frust.fc;
	    p1 += frust.farUp * umy;
	    p1 += frust.farSide * umx;


      if (cmesh == 0) { t = 999.0f; }
      else { t = cmesh->lineTest(&p0, &p1, 1); }

      if (t == 999.0f)
      {
        //if didnt hit terrain -- use the infinite plane
        ppos.set(0,planey,0);
      	pnorm.set(0,1,0);
        t = frust.getPlaneInter(p0, p1, ppos, pnorm);
      }//endif
 
	    coord = p0 +(p1 -p0) * t;

  }//update

};//classend


