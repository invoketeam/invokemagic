
#pragma once

#include "xMath.h"
#include "xGLCommon.h"


class xCam
{
public:

  float fov;
  float aspect;

//cant call them near and far thanks to windows.h
//what the fuck
//http://stackoverflow.com/questions/118774/is-there-a-clean-way-to-prevent-windows-h-from-creating-a-near-far-macro
  float neard;
  float fard;


	gamex::cVec3f pos;
	gamex::cQuat ori;
	
	gamex::cVec3f camFront; //set by user

  
  
  
public:
	xCam() 
  {
    fov = 90.0f;
    aspect = 640.0f/480.0f;
    neard = 1.0f;
    fard = 300.0f;
    
  }//ctor
	
  
 gamex::cMat getProjView()  
  {
    //gamex::cVec3f at;
		//gamex::cVec3f up;
    gamex::cMat mat;
    gamex::cMat proj;
    gamex::cMat ret;
	
	//	ori.setVecFront(at);
	//	at += pos;

//		ori.setVecUp(up);
    
    gamex::setModelView2(mat, pos, ori);
/*
		mat.setModelView(
			pos.x, pos.y, pos.z,
			at.x, at.y, at.z,
			up.x, up.y, up.z);
*/
    
    proj.setPerspective(fov, aspect, neard, fard);  

//    gamex::cMat::multMatrix(mat, proj, ret);
    gamex::cMat::multMatrix(proj, mat, ret);

    return ret;
  }//getprojview

  

	void getCamMat(gamex::cMat &mat)
	{
	/*	gamex::cVec3f at;
		gamex::cVec3f up;
	
		ori.setVecFront(at);
		at += pos;

		ori.setVecUp(up);

		mat.setModelView(
			pos.x, pos.y, pos.z,
			at.x, at.y, at.z,
			up.x, up.y, up.z);*/

     gamex::setModelView2(mat, pos, ori);
	}//getcammat

	//warning
	//youll probably need 
	//before calling it (all it does its calls glulookat with its own values)
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	void camLookAt()
	{
/*
		//gamex::cVec3f pos;
		gamex::cVec3f at;
		gamex::cVec3f up;
	
		ori.setVecFront(at);
		at += pos;

		ori.setVecUp(up);

		gluLookAt(
			pos.x, pos.y, pos.z,
			at.x, at.y, at.z,
			up.x, up.y, up.z);
*/

    gamex::cMat temp;
    getCamMat(temp);
    glLoadMatrixf(temp.m);

	}//camlookat

	//todo:
	//gles doesnt have glu
	//so need to look up at the lookat implementation
	//someone made to get it to work
	//and implement it to the class


	void lookAtPoint(gamex::cVec3f point, gamex::cVec3f up)
	{
		//now
		//why it jumps behind the camera??
		//ok note // normalise is the unprecise one
		//normPrec is the precise one
		//(should have normFast as well thats same as the normalise so less confusion?)
		//anyway now it seems to be ok

		gamex::cVec3f front;
		gamex::cVec3f side;

		front = point - pos;

		//front.y = pos.y;

		//ori.setVecUp(up);

	
		front.normPrec();
		up.normPrec();

		//side = gamex::cVec3f::cross(front, up);
			side = gamex::cVec3f::cross(up, front);
				side.normPrec();

			up = gamex::cVec3f::cross(front, side);
				up.normPrec();
				

		gamex::cQuat lookat;
		gamex::cMat mx;
		
			mx.m[0] = side.x; 
			mx.m[1] = side.y; 
			mx.m[2] = side.z;
			
			mx.m[4] = up.x; 		
			mx.m[5] = up.y; 	
			mx.m[6] = up.z;

			mx.m[8] = front.x;
			mx.m[9] = front.y; 
			mx.m[10] = front.z;

				lookat.fromMatrix(mx.m);
				lookat.normalise();

					ori = lookat;

	}//lookpoint

/*
	void debugRender()
	{
		gamex::cVec3f fv;
		gamex::cMat mtx;

		ori.setMatrix(mtx.m);

		glPushMatrix();
			glTranslatef(pos.x, pos.y, pos.z);
			//does glrotatef works with quats?
			//or use the multmatrix dunno
			//	glRotatef(ori.w*(180.0f/3.1415f),ori.x, ori.y, ori.z); //doesnt work right

				glMultMatrixf(mtx.m); //rotation



			glColor3f(1,0,0);
//				glutWireCube(0.5);

		glPopMatrix();

	
			glBegin(GL_LINES);

				ori.setVecFront(fv);
				fv *= 2;
				fv += pos;
				glColor3f(0,0,1);
					glVertex3f(pos.x, pos.y, pos.z);
					glVertex3f(fv.x, fv.y, fv.z);

				ori.setVecUp(fv);
				fv += pos;
				glColor3f(0,1,0);
					glVertex3f(pos.x, pos.y, pos.z);
					glVertex3f(fv.x, fv.y, fv.z);

				ori.setVecSide(fv);
				fv += pos;
				glColor3f(1,0,0);
					glVertex3f(pos.x, pos.y, pos.z);
					glVertex3f(fv.x, fv.y, fv.z);
			glEnd();


	}//debrend

*/


	void moveForward(float speed = 1.0f) 
	{
		gamex::cVec3f move;
		ori.setVecFront(move);
		move *= speed;
		
		pos += move;

	}//forward

	void strafeUp(float speed = 1.0f) 
	{
		gamex::cVec3f move;
		ori.setVecUp(move);
		move *= speed;

		pos += move;
	}//strleft

//change to strafeRight
	void strafeRight(float speed = 1.0f) 
	{
		gamex::cVec3f move;
		ori.setVecSide(move);
		move *= speed;

		pos += move;
	}//strleft









	void rotatePitch(float ang)
	{
		ori.rotPitch(ang);
	}//rotpitch

	void rotateYaw(float ang)
	{
		ori.rotYaw(ang);
	}//rotpitch

	void rotateRoll(float ang)
	{
		ori.rotRoll(ang);
	}//rotpitch


	void setPos(float x, float y, float z)
	{
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}//setpos

	//default order is PYR (Pitch->Yaw->Roll)
	//might need other functions for other orders 
	//(or just do it by hand)
	void setRot(float pitch, float yaw, float roll)
	{
		ori.reset();

		rotatePitch(pitch);
		rotateYaw(yaw);
		rotateRoll(roll);
	}//setrot
	//todo: definetly need a setRotDeg


	void setRotDeg(float pitch, float yaw, float roll)
	{
	
	//deg-to-rad:
	//pi/180
		//3.1415/180
			//0.0174527f

		pitch *= 0.0174527f;
		yaw *= 0.0174527f;
		roll *= 0.0174527f;

		ori.reset();

		rotatePitch(pitch);
		rotateYaw(yaw);
		rotateRoll(roll);
	}//setrot


	void alignUpVector(gamex::cVec3f &newup)
	{
			 //found this code in this forum
			 //the guy doesnt write where he found it
			 //but ohmygod it works O_O (or at least it seem to be)
					 //http://forums.create.msdn.com/forums/t/31468.aspx

			gamex::cQuat q;

			gamex::cVec3f c;
 		  gamex::cVec3f up;



 			ori.setVecUp(up); //get old up vector from orientation

				c = up.cross(newup);

			float d = up.dot(newup);
			float s = sqrtf((1.0f+d)*2.0f);
			float rs = 1.0f / s;
				q.x = c.x * rs;	
				q.y = c.y * rs; 
				q.z = c.z * rs; 
				q.w = s * 0.5f;
				
				//q = q * ori;
					q.mul(ori);

						ori = q;  //no slerp yet

						ori.normalise();
				
				//Rotation = Quaternion.Slerp(Rotation, q, 0.1f * distanceScalar); 
	}//alignup


};//ccam