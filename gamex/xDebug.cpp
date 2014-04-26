#pragma warning (disable : 4786)



#include "xGLCommon.h"

#include "xDebug.h"


void drawRect(float cx, float cy, float cw, float ch)
{
  glBegin(GL_LINES);
    glVertex2f(cx,cy);       glVertex2f(cx+cw,cy);
    glVertex2f(cx,cy+ch);    glVertex2f(cx+cw,cy+ch);
    glVertex2f(cx,cy);       glVertex2f(cx,cy+ch);
    glVertex2f(cx+cw,cy);    glVertex2f(cx+cw,cy+ch);
  glEnd();
}//drawcube



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



void fillRect(float ax, float ay, float aw, float ah)
  {
   glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(ax, ay); //0

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(ax, ay+ah); //1

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(ax+aw, ay); //2

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(ax+aw, ay+ah);//3
   glEnd();
    
  }//drawrect
