

#pragma once

//based on the http://www.lighthouse3d.com/
//frustum tutorial (geometric approach)
//sources
//http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
//http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-implementation/


#include "xMath.h"

class xFrustum
{
public:
  
	gamex::cVec3f ftl, ftr, fbl, fbr; //front points
	gamex::cVec3f ntl, ntr, nbl, nbr; //near points

	float nearWidth, nearHeight;
	float farWidth, farHeight;

	float fov;
	float aspect;
	float nearDist, farDist;

	//only need these for testing
	//(rest is for show and calculating these)
	gamex::cVec3f pa, pb, pc, pd, pe, pf;
	gamex::cVec3f norma, normb, normc, normd, norme, normf;


//mouse in 3D
  gamex::cVec3f mclose;
  gamex::cVec3f mfar;


//frustum AABB (todo)
  gamex::cVec3f aabb_min;
  gamex::cVec3f aabb_max;


public:
  //setpoints helpers
  gamex::cVec3f farUp, farSide;
	gamex::cVec3f nearUp, nearSide;
	gamex::cVec3f fc, nc;


public:


	//aspect -- e.g. 640/480
	void setPerspective(float fov_= 60, float aspect_=1.33f, float nearDist_=1, float farDist_=300);


//todo -- getplaneinter is more of a static function
//its staying here as its mostly used with frustum related calculations
//(maybe it should be put in xMath sometime)
//p plane position
//n plane normal
//a b  -- points of line you want to test
//you get -99999 if the line is parallel to the plane
//or the position on the line (so the coordinate is  a +(b-a)* ret  )
  float getPlaneInter(gamex::cVec3f a, gamex::cVec3f b, gamex::cVec3f p, gamex::cVec3f n);

//note: umx and umy are mouse coords
//they are between [-1, 1]
//so use the screen (or whatever you are rendering into) size to calculate
//(e.g.  ((mousex/640)*2)-1
	void setPoints(gamex::cVec3f pos, gamex::cQuat ori, float umx=0, float umy=0);

  bool isPointInside(gamex::cVec3f &p, float rad = 0.0f);


protected:
  void setNormals();

/*
public:
  void drawMouse(float umx, float umy);
	void render()//debugrender;
*/
	

};//classend