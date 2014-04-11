#pragma warning (disable : 4786)

#include <math.h>

#include "xFlatRender.h"

#include "xGLCommon.h"
#include "oglExt.h"

#include "xSprite.h"


xFrame::xFrame(void)
  {
    cx = 0;
    cy = 0;
    cz = 0;
    sortCode = 0;
    skin = 0;
    ang = 0;
    alpha = 0;
    spec = 0;
    u0 = 0.0f;
    v0 = 0.0f;
    u1 = 1.0f;
    v1 = 1.0f;

  }//ctor





xFlatRender::xFlatRender(void)
  { 
    numFrame = 0;
    vecFrame = 0;
    it = 0;
  }//ctor

  
  
xFlatRender::~xFlatRender(void)
  { clear(); }

  
  
void
xFlatRender::clear(void) 
  { 
    int i;

    if (vecFrame != 0)
    {
      for (i = 0; i < numFrame; i++) { delete vecFrame[i]; }
      delete [] vecFrame;
    }
    vecFrame = 0;
    numFrame = 0;
    it = 0;
  }//clear

  
  
  
void 
xFlatRender::init(void)
  {
    clear();

    int i;

    numFrame = 8192;
    vecFrame = new xFrame*[numFrame];
    for (i = 0; i < numFrame; i++) { vecFrame[i] = new xFrame(); }
  }//init



xFrame * 
xFlatRender::addSprite(xSprite * spr, float ax, float ay, float az, float scalex, float scaley, bool xmir, bool ymir)
{
  xFrame * a;

  a = addFrame(ax, ay, az, scalex * spr->w, scaley * spr->h, spr->handle);
  
  if (xmir)  {  a->u0 = spr->u1;    a->u1 = spr->u0;  }
  else  {  a->u0 = spr->u0;    a->u1 = spr->u1;  }

  if (ymir)  {  a->v0 = spr->v1;    a->v1 = spr->v0;  }
  else  {  a->v0 = spr->v0;    a->v1 = spr->v1;  }

  return a;
}//addsprite 


xFrame * 
xFlatRender::addSprite2(xSprite * spr, float ax, float ay, float az, float aw, float ah, bool xmir, bool ymir)
{
  xFrame * a;

  a = addFrame(ax, ay, az, aw, ah, spr->handle);
  
  if (xmir)  {  a->u0 = spr->u1;    a->u1 = spr->u0;  }
  else  {  a->u0 = spr->u0;    a->u1 = spr->u1;  }

  if (ymir)  {  a->v0 = spr->v1;    a->v1 = spr->v0;  }
  else  {  a->v0 = spr->v0;    a->v1 = spr->v1;  }

  return a;
}//addsprite2

  

  //todo -- add mirroring
xFrame * 
xFlatRender::addFrame(float ax, float ay, float az, float aw, float ah, unsigned int skin)
  {
    xFrame * a;

    a = vecFrame[it];

    a->cx = ax;    a->cy = ay;    a->cz = az;
    a->cw = aw;    a->ch = ah;
    a->skin = skin;

    a->sortCode = (32768) + (int)az;

    //set default values
      a->spec = 0;
      a->ang = 0;
      a->alpha = 1;
      a->u0 = 0.0f;
      a->v0 = 0.0f;
      a->u1 = 1.0f;
      a->v1 = 1.0f;

    it += 1; if (it >= numFrame) { it = numFrame - 1; } //reached maximum images to draw

    return a;
  }//addframe

void
xFlatRender::resetFrame(void)
  {
    it = 0;
  }//resetframe



void
xFlatRender::render(bool bSort)
  {
    unsigned int curSkin;

    curSkin = 0;

    //sad thing is for 2D we dont have much choice when it comes to optimisations
    //so its back to front rendering

    //todo -- should this include the let-the-actor-do-the-rendering hack from flash gamex?




    //set up rendering for 2D
    //disable buffers

      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);


       glClientActiveTextureARB(GL_TEXTURE0);     glDisableClientState(GL_TEXTURE_COORD_ARRAY);
       glActiveTextureARB(GL_TEXTURE1);    glDisable(GL_TEXTURE_2D); 

       glClientActiveTextureARB(GL_TEXTURE1);   glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  
 
    //enable first texture layer, enable blending, use alpha blending, disable culling
      glActiveTextureARB(GL_TEXTURE0);       
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);

    int i;
    xFrame * a;
    float wx, wy;

    //sort frames
    if (bSort) { }


    for (i = 0; i < it; i++)
    {
      
      a = vecFrame[i];

      if (curSkin != a->skin)
      {
        curSkin = a->skin; 
        glBindTexture(GL_TEXTURE_2D, curSkin);    
      }//endif


      glColor4f(1,1,1, a->alpha);


    //todo -- not sure how to speed this up (or if it needs speed up at all, probably not)
 
    wx = 0.5f*a->cw;
    wy = 0.5f*a->ch;

    if (a->ang != 0.0f)
    {

      glPushMatrix();
        glTranslatef(a->cx, a->cy, a->cz);
        glRotatef(a->ang*(180.0f/3.1415f),0,0,1);
        
        glBegin(GL_TRIANGLE_STRIP);
          glTexCoord2f(a->u0, a->v0);
          glVertex2f(-wx, -wy); //0
          glTexCoord2f(a->u0, a->v1);
          glVertex2f(-wx, wy); //1
          glTexCoord2f(a->u1, a->v0);
          glVertex2f(wx, -wy); //2
          glTexCoord2f(a->u1, a->v1);
          glVertex2f(wx, wy); //3
         glEnd();
      glPopMatrix();

    }
    else
    {

       glBegin(GL_TRIANGLE_STRIP);
          glTexCoord2f(a->u0, a->v0);
          glVertex2f(-wx+a->cx, -wy+a->cy); //0
          glTexCoord2f(a->u0, a->v1);
          glVertex2f(-wx+a->cx, wy+a->cy); //1
          glTexCoord2f(a->u1, a->v0);
          glVertex2f(wx+a->cx, -wy+a->cy); //2
          glTexCoord2f(a->u1, a->v1);
          glVertex2f(wx+a->cx, wy+a->cy); //3
       glEnd();

    }//endif

/*
       //draw texture
     glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(a->u0, a->v0);
      glVertex2f(a->cx, a->cy); //0
      glTexCoord2f(a->u0, a->v1);
      glVertex2f(a->cx, a->cy+a->ch); //1
      glTexCoord2f(a->u1, a->v0);
      glVertex2f(a->cx+a->cw, a->cy); //2
      glTexCoord2f(a->u1, a->v1);
      glVertex2f(a->cx+a->cw, a->cy+a->ch); //3
     glEnd();
*/

    }//nexti

  }//render


void 
xFlatRender::radixSort(xFrame ** vec, int num)
  {
    static xFrame * tempVec[32768];
    static int bucket[256];

    int i;
    int shift;
    shift = 0;
     
    while (shift < 32)
    {      
      //sometimes for is faster than memset, not really matters this case imho
      memset(bucket, 0, 1024);

      for (i = 0; i < num; i++) { bucket[(vec[i]->sortCode >> shift) & 0xFF]++; }
      for (i = 1; i < 256; i++) {  bucket[i] += bucket[i - 1];  }     
      for (i = num - 1; i >= 0; i--)   { tempVec[--bucket[(vec[i]->sortCode >> shift) & 0xFF]] = vec[i];   }
      for (i = 0; i < num; i++) {  vec[i] = tempVec[i]; }

      shift +=  8;     

    }//wend

  }//radixsort 

