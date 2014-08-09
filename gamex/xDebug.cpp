#pragma warning (disable : 4786)



#include "xGLCommon.h"

#include "xDebug.h"

#include <math.h>


void drawRect(float cx, float cy, float cw, float ch)
{
  glBegin(GL_LINES);
    glVertex2f(cx,cy);       glVertex2f(cx+cw,cy);
    glVertex2f(cx,cy+ch);    glVertex2f(cx+cw,cy+ch);
    glVertex2f(cx,cy);       glVertex2f(cx,cy+ch);
    glVertex2f(cx+cw,cy);    glVertex2f(cx+cw,cy+ch);
  glEnd();
}//drawcube




void fillRect(float ax, float ay, float aw, float ah,float u0, float v0, float u1, float v1)
  {
   glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(u0, v0);
    glVertex2f(ax, ay); //0

    glTexCoord2f(u0, v1);
    glVertex2f(ax, ay+ah); //1

    glTexCoord2f(u1, v0);
    glVertex2f(ax+aw, ay); //2

    glTexCoord2f(u1, v1);
    glVertex2f(ax+aw, ay+ah);//3
   glEnd();
    
  }//drawrect


void fillSprite(float cx, float cy, float cw, float ch, float roll)
{

  float xw, yw;

  xw = cosf(roll) * (cw * 0.5f);
  yw = sinf(roll) * (ch * 0.5f);

//		  rx = -ca+sa;	ry = -sa-ca;
	//		rx = ca +sa;	ry = sa-ca;
//			rx = -ca-sa;	ry = -sa+ca;	  
//			rx = ca-sa;		ry = sa+ca;


   glBegin(GL_TRIANGLE_STRIP); 
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(cx+(xw+yw), cy+(yw-xw)); //0

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(cx+(-xw+yw), cy+(-yw-xw)); //1

     glTexCoord2f(1.0f, 1.0f);
    glVertex2f(cx+(xw-yw), cy+(yw+xw));//2

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(cx+(-xw-yw), cy+(-yw+xw)); //3
  
   glEnd();


}//fillrect2






void drawCube(float cx, float cy, float cz, float cw, float ch, float cd)
{
  glBegin(GL_LINES);
    glVertex3f(cx,cy,cz);       glVertex3f(cx+cw,cy,cz);
    glVertex3f(cx,cy+ch,cz);    glVertex3f(cx+cw,cy+ch,cz);
    glVertex3f(cx,cy,cz+cd);    glVertex3f(cx+cw,cy,cz+cd);
    glVertex3f(cx,cy+ch,cz+cd); glVertex3f(cx+cw,cy+ch,cz+cd);

    glVertex3f(cx,cy,cz);       glVertex3f(cx,cy+ch,cz);
    glVertex3f(cx+cw,cy,cz);    glVertex3f(cx+cw,cy+ch,cz);
    glVertex3f(cx,cy,cz+cd);    glVertex3f(cx,cy+ch,cz+cd);
    glVertex3f(cx+cw,cy,cz+cd); glVertex3f(cx+cw,cy+ch,cz+cd);

    glVertex3f(cx,cy,cz);       glVertex3f(cx,cy,cz+cd);
    glVertex3f(cx+cw,cy,cz);    glVertex3f(cx+cw,cy,cz+cd);
    glVertex3f(cx,cy+ch,cz);    glVertex3f(cx,cy+ch,cz+cd);
    glVertex3f(cx+cw,cy+ch,cz); glVertex3f(cx+cw,cy+ch,cz+cd);
  glEnd();
}//drawcube

//2*6
static int vecFace_cube[] =  
     {	1, 0, 2, 1, 2, 3,
				1 + 4, 2 + 4, 0 + 4,  1 + 4, 3 + 4, 2 + 4,
				1 + 8, 2 + 8, 0 + 8,  1 + 8, 3 + 8, 2 + 8,
				1 + 12, 0 + 12, 2 + 12,  1 + 12, 2 + 12, 3 + 12,
				1 + 16, 2 + 16, 0 + 16,  1 + 16, 3 + 16, 2 + 16,
				1 + 20, 0 + 20, 2 + 20,  1 + 20, 2 + 20, 3 + 20 };

static float vecVert_cube[] =
{
  -0.5f, -0.5f, -0.5f,
   0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
   0.5f, 0.5f, -0.5f,

   -0.5f, -0.5f, 0.5f,
   0.5f, -0.5f, 0.5f,
   -0.5f, 0.5f, 0.5f,
   0.5f, 0.5f, 0.5f,

   0.5f, -0.5f, -0.5f,
   0.5f, 0.5f, -0.5f,
   0.5f, -0.5f, 0.5f,
   0.5f, 0.5f, 0.5f,

   -0.5f, -0.5f, -0.5f,
   -0.5f, 0.5f, -0.5f,
   -0.5f, -0.5f, 0.5f,
   -0.5f, 0.5f, 0.5f,

   -0.5f, -0.5f, -0.5f,
   0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, 0.5f,
   0.5f, -0.5f, 0.5f,

   -0.5f, 0.5f, -0.5f,
   0.5f, 0.5f, -0.5f,
   -0.5f, 0.5f, 0.5f,
   0.5f, 0.5f, 0.5f

};//vecvert

static float vecUv_cube[] =
{
  1.0f, 1.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f,

  0.0f, 1.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 0.0f,

  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  0.0f, 0.0f,

  0.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 1.0f,
  1.0f, 0.0f,

  1.0f, 0.0f,
  0.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 1.0f,

  1.0f, 1.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f

};//vecuv



void fillCube(float px, float  py, float pz,  float  rx, float  ry, float rz)
{
 // float ax,ay,az;  float bx,by,bz;
 // ax = ay = az = -1;  bx = by = by = 1;

//todo fix order (original was gl_triangles)

  int *vf;
  float *vv;
  float *st;

  vf = vecFace_cube;
  vv = vecVert_cube;
  st = vecUv_cube;

  int i;
  int k;
  int m;
  glBegin(GL_TRIANGLES);
  for (i = 0; i < 12; i++)
  {
    k = vf[i*3+0]; m=k*2; k*= 3;
    glTexCoord2f(st[m],st[m+1]);
    glVertex3f(px+vv[k]*rx,py+vv[k+1]*ry,pz+vv[k+2]*rz);
    k = vf[i*3+1]; m=k*2; k*= 3;
    glTexCoord2f(st[m],st[m+1]);
    glVertex3f(px+vv[k]*rx,py+vv[k+1]*ry,pz+vv[k+2]*rz);
    k = vf[i*3+2]; m=k*2; k*= 3;
    glTexCoord2f(st[m],st[m+1]);
    glVertex3f(px+vv[k]*rx,py+vv[k+1]*ry,pz+vv[k+2]*rz);

  }//nexti
  glEnd();


}//fillcube

