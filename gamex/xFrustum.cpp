#pragma warning (disable : 4786)


#include <stdio.h>
#include "xGLCommon.h"

#include "xFrustum.h"



void 
xFrustum::setPerspective(float fov_, float aspect_, float nearDist_, float farDist_)
	{

		fov = fov_;
		aspect = aspect_;
		nearDist = nearDist_;
		farDist = farDist_;
	
		float tang;
		fov_ *= 3.1415f / 180.0f; //torad
		tang = tanf(fov_ * 0.5f);

		nearHeight = nearDist * tang;
		nearWidth = nearHeight * aspect;

		farHeight = farDist * tang;
		farWidth = farHeight * aspect;
	
	}//setpersp
  

void 
xFrustum::setOrtho(float w, float h, float neard, float fard)
{
	nearDist = neard;
  farDist = fard;

  nearWidth = w;
  nearHeight = h;

  farWidth = w;
  farHeight = h;

}//setortho



static void calcPlane( gamex::cVec3f p0, gamex::cVec3f p1, gamex::cVec3f p2, gamex::cVec3f * p, gamex::cVec3f * n)
{
  gamex::cVec3f e0, e1;
  e0 = p1 - p0;
  e1 = p2 - p0;

  *p = p0;  
  *n = gamex::cVec3f::cross(e0, e1);
  *n *= -1.0f;
  n->normPrec();


}//calcplane



void 
xFrustum::makeFrustum2(gamex::cVec3f pos, gamex::cQuat ori)
{
  gamex::cVec3f front;
  gamex::cVec3f up;
  gamex::cVec3f side;

  ori.setVecFront(front);
  ori.setVecUp(up);
  ori.setVecSide(side); 
  side = -side;

  makeFrustum(pos, front, up, side);

}//makefrust2


void 
xFrustum::makeFrustum(gamex::cVec3f pos, gamex::cVec3f front, gamex::cVec3f up,  gamex::cVec3f side)
{
    farUp = up * farHeight;
    farSide = side * farWidth;

    nearUp = up * nearHeight;
    nearSide = side * nearWidth;

  	farCenter = pos + front * farDist;
		
			farTopLeft = farCenter + farUp - farSide;
			farTopRight = farCenter + farUp + farSide;
			farBotLeft = farCenter - farUp - farSide;
			farBotRight = farCenter - farUp + farSide;

		nearCenter = pos + front * nearDist;

			nearTopLeft = nearCenter + nearUp - nearSide;
			nearTopRight = nearCenter + nearUp + nearSide;
			nearBotLeft = nearCenter - nearUp - nearSide;
			nearBotRight = nearCenter - nearUp + nearSide;
          
    //near
      vecPoint[0] = nearCenter;
      vecNorm[0] = (farCenter - nearCenter);
      vecNorm[0].normPrec();
    //far
      vecPoint[1] = farCenter;
      vecNorm[1] = (nearCenter - farCenter);
      vecNorm[1].normPrec();    
    //left
      calcPlane(nearTopLeft, nearBotLeft, farBotLeft, &vecPoint[2], &vecNorm[2]);
      vecPoint[2] += (farBotLeft - nearTopLeft) * 0.5f; 
    //right
      calcPlane(nearBotRight, nearTopRight, farBotRight, &vecPoint[3], &vecNorm[3]); 
      vecPoint[3] += (farTopRight - nearBotRight) * 0.5f;
    //top
      calcPlane(nearTopRight, nearTopLeft, farTopLeft, &vecPoint[4], &vecNorm[4]); 
      vecPoint[4] += (farTopLeft - nearTopRight) * 0.5f;
    //bottom
      calcPlane(nearBotLeft, nearBotRight, farBotRight, &vecPoint[5], &vecNorm[5]);
      vecPoint[5] += (farBotRight - nearBotLeft) * 0.5f;


      /*
      static bool first = true;
      int i;
              gamex::cVec3f n;


              //normals seem to be fine
      if (first)
      {
      for (i = 0; i < 6; i++)
      {
        n = vecNorm[i];
          printf("n [%0.2f][%0.2f][%0.2f] \n", n.x, n.y, n.z);
      }
      first = false;
      }
      */
}//makefrustum
  


void 
xFrustum::debRender(void)
{
  int i;
  
  for (i = 0; i < 6; i++)
  {
    glBegin(GL_LINES);
      glVertex3f(vecPoint[i].x, vecPoint[i].y, vecPoint[i].z);
      glVertex3f(vecPoint[i].x+vecNorm[i].x, vecPoint[i].y+vecNorm[i].y, vecPoint[i].z+vecNorm[i].z);
    glEnd();
  }//nexti

  
  glBegin(GL_LINE_LOOP);
    glColor3f(1,0,0);
      glVertex3f(farTopLeft.x, farTopLeft.y, farTopLeft.z);
      glVertex3f(farTopRight.x, farTopRight.y, farTopRight.z);
      glVertex3f(farBotRight.x, farBotRight.y, farBotRight.z);
      glVertex3f(farBotLeft.x, farBotLeft.y, farBotLeft.z);
  glEnd();

  glBegin(GL_LINE_LOOP);
    glColor3f(0,0,1);
      glVertex3f(nearTopLeft.x, nearTopLeft.y, nearTopLeft.z);
      glVertex3f(nearTopRight.x, nearTopRight.y, nearTopRight.z);
      glVertex3f(nearBotRight.x, nearBotRight.y, nearBotRight.z);
      glVertex3f(nearBotLeft.x, nearBotLeft.y, nearBotLeft.z);
  glEnd();


  glBegin(GL_LINE_LOOP);
    glColor3f(0,1,0);
      glVertex3f(nearTopLeft.x, nearTopLeft.y, nearTopLeft.z);
      glVertex3f(farTopLeft.x, farTopLeft.y, farTopLeft.z);
      glVertex3f(farBotLeft.x, farBotLeft.y, farBotLeft.z);
      glVertex3f(nearBotLeft.x, nearBotLeft.y, nearBotLeft.z);
  glEnd();

  glBegin(GL_LINE_LOOP);
    glColor3f(0,1,1);
      glVertex3f(nearTopRight.x, nearTopRight.y, nearTopRight.z);
      glVertex3f(farTopRight.x, farTopRight.y, farTopRight.z);
      glVertex3f(farBotRight.x, farBotRight.y, farBotRight.z);
      glVertex3f(nearBotRight.x, nearBotRight.y, nearBotRight.z);
  glEnd();
    

}//debrender


  
  
  
bool 
xFrustum::isPointInside(gamex::cVec3f * p, float rad)
	{
    int i;
	  float dist;

    // check distance to planes
		// distance = (object position - cam position) dot (plane normal)

    rad = -rad; 

    for (i = 0; i < 6; i++)
    {
     	dist = (p->x - vecPoint[i].x) * vecNorm[i].x + 
             (p->y - vecPoint[i].y) * vecNorm[i].y +
             (p->z - vecPoint[i].z) * vecNorm[i].z;
			if (dist < rad) return false;
    }//nexti

    return true;
	}//ispinside

  
bool 
xFrustum::isBoxInside(gamex::cVec3f * bmin, gamex::cVec3f * bmax)
{
  //based on
  //http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/

  int i;
  float dist;


  gamex::cVec3f vp;
  gamex::cVec3f vn;
//  bool bInter;
  //bInter = false;

  for (i = 0; i < 6; i++)
  {
    if (vecNorm[i].x >= 0) { vp.x = bmax->x; vn.x = bmin->x; } else { vp.x = bmin->x; vn.x = bmax->x; }
    if (vecNorm[i].y >= 0) { vp.y = bmax->y; vn.y = bmin->y; } else { vp.y = bmin->y; vn.y = bmax->y; }
    if (vecNorm[i].z >= 0) { vp.z = bmax->z; vn.z = bmin->z; } else { vp.z = bmin->z; vn.z = bmax->z; }


    dist = (vp.x - vecPoint[i].x) * vecNorm[i].x + 
           (vp.y - vecPoint[i].y) * vecNorm[i].y +
           (vp.z - vecPoint[i].z) * vecNorm[i].z;
    if (dist < 0) { return false; }
  
    /*
    dist = (vn.x - vecPoint[i].x) * vecNorm[i].x + 
           (vn.y - vecPoint[i].y) * vecNorm[i].y +
           (vn.z - vecPoint[i].z) * vecNorm[i].z;
    if (dist < 0) { bInter = true; }
    */
  }//nexti

  return true;

}//isboxinside




void 
xFrustum::getMouseLine(float umx, float umy, gamex::cVec3f * retNear, gamex::cVec3f * retFar)
{
  *retNear = nearCenter;
  *retNear += nearUp * umy;
  *retNear += nearSide * umx;

  *retFar = farCenter;
  *retFar += farUp * umy;
  *retFar += farSide * umx;

}//getmouseline

  
  
float 
xFrustum::getPlaneInter(gamex::cVec3f a, gamex::cVec3f b, gamex::cVec3f p, gamex::cVec3f n)
	{
		gamex::cVec3f v, d;
		float ndotv;
		v = b - a;		d = p - a;
		ndotv = (v.x * n.x) + (v.y * n.y) + (v.z * n.z);
		if (ndotv == 0) { return -999.0f; } //line and plane parallel, no inters		
		return ( ( ( n.x * d.x) + (n.y * d.y) + (n.z * d.z) ) / ndotv );
	}//getplaneinter
  
  


  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  