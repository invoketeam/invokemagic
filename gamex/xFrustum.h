

#pragma once

//based on the http://www.lighthouse3d.com/
//frustum tutorial (geometric approach)
//sources
//http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
//http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-implementation/
//http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/

#include "xMath.h"

/*
enum
{
  FRUST_NEAR = 0,
  FRUST_FAR = 1,
  FRUST_LEFT = 2,
  FRUST_RIGHT = 3,
  FRUST_UP = 4,
  FRUST_DOWN = 5
};
*/

class xFrustum
{
public:
  float nearWidth, nearHeight;
	float farWidth, farHeight;

	float fov;
	float aspect;
	float nearDist, farDist;
  
  gamex::cVec3f farTopLeft, farTopRight;
  gamex::cVec3f farBotLeft, farBotRight;
  gamex::cVec3f nearTopLeft, nearTopRight;
  gamex::cVec3f nearBotLeft, nearBotRight;
  
  gamex::cVec3f farUp, farSide;
	gamex::cVec3f nearUp, nearSide;
	gamex::cVec3f farCenter, nearCenter;
  
  gamex::cVec3f vecPoint[6];
  gamex::cVec3f vecNorm[6];
  

public:
	void setPerspective(float fov_= 60, float aspect_=1.33f, float nearDist_=1, float farDist_=300);
  void setOrtho(float w, float h, float neard=1.0f, float fard=300.0f);

  void makeFrustum(gamex::cVec3f pos, gamex::cVec3f front, gamex::cVec3f up,  gamex::cVec3f side);
  void makeFrustum2(gamex::cVec3f pos, gamex::cQuat ori);
  
  bool isPointInside(gamex::cVec3f * p, float rad = 0.0f);
  bool isBoxInside(gamex::cVec3f * bmin, gamex::cVec3f * bmax);
  
  void getMouseLine(float umx, float umy, gamex::cVec3f * retNear, gamex::cVec3f * retFar);


  void debRender(void);
  

	//a b  -- points of line you want to test
//p plane position
//n plane normal
//you get -999.0f if the line is parallel to the plane
//or the position on the line (so the coordinate is  a +(b-a)* ret  )
  float getPlaneInter(gamex::cVec3f a, gamex::cVec3f b, gamex::cVec3f p, gamex::cVec3f n);
  

};//classend



