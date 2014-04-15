
#pragma once

#include "xMath.h"
#include "xGLCommon.h"


class xCam
{
public:
  float fov;
  float aspect;
  float neard;
  float fard;

	gamex::cVec3f pos;
	gamex::cQuat ori;
	

  
public:
	xCam(void);
	

  //works with opengl coordinates -- negative is forward
	void moveForward(float speed = -1.0f);

  //using the up and right relative to the current orientation
	void strafeUp(float speed = 1.0f);

	void strafeRight(float speed = 1.0f);



	void rotatePitch(float ang);

	void rotateYaw(float ang);

	void rotateRoll(float ang);


};//ccam