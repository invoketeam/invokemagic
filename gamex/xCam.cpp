#pragma warning (disable : 4786)

#include "xCam.h"




xCam::xCam(void) 
  {
    fov = 90.0f;
    aspect = 640.0f/480.0f;
    neard = 1.0f;
    fard = 300.0f;
    
  }//ctor
	


void 
xCam::moveForward(float speed) 
	{
		gamex::cVec3f move;
		ori.setVecFront(move);
		move *= speed;
		
		pos += move;

	}//forward

void 
xCam::strafeUp(float speed) 
	{
		gamex::cVec3f move;
		ori.setVecUp(move);
		move *= speed;

		pos += move;
	}//strleft

void 
xCam::strafeRight(float speed) 
	{
		gamex::cVec3f move;
		ori.setVecSide(move);
		move *= speed;

		pos += move;
	}//strleft



void 
xCam::rotatePitch(float ang)
	{
		ori.rotPitch(ang);
	}//rotpitch

void 
xCam::rotateYaw(float ang)
	{
		ori.rotYaw(ang);
	}//rotpitch

void 
xCam::rotateRoll(float ang)
	{
		ori.rotRoll(ang);
	}//rotpitch