#pragma warning (disable : 4786)

#include "xTileFlat.h"

#include <math.h>
#include "xGLCommon.h"


xTileFlat::xTileFlat(void)
{
  mwidth = 0;
	mheight = 0;
	vecGrid = 0; 
  
  cw = 32;
  ch = 32;
  
  initDefaultSet();
  
}//ctor

xTileFlat::~xTileFlat(void)
{
  clear();
}//dtor
  
  
void 
xTileFlat::clear(void)
{
  if (vecGrid != 0) { delete [] vecGrid; } vecGrid = 0; 
	mwidth = 0;
	mheight = 0; 
}//clear 



void 
xTileFlat::initDefaultSet(void)
{
  int i;
  int k; 
  int yt;
  int d;
  float u;
  float v;
  float pix;
  xFlat * a;
  
  u = 1.0f / 512.0f;
  v = 1.0f / 512.0f;
  
  pix = 0.5f * u;
  
  //rem .. add half as texel is supposed to be the center of the pixel
  
  //remember -- opengl uv coordinates are vertically mirrored
  
  d = 0;
  
  for (i = 0; i < 16; i++)
  {
    yt = i * 16;
    for (k = 0; k < 16; k++)
    {
      a = &tileSet[yt+k];
      a->maskx = k * 32;
      a->masky = i * 32;
      
      a->u0 = (k*32)*u + pix;
      a->u1 = a->u0 + (32*u) - pix;
      a->v0 = 1.0f-((i*32)*v) - pix;
      a->v1 = a->v0 - (32*v) + pix;
      
      a->disp = d;
      d += 1;
      
    }//nextk
  }//nexti

}//initset



void 
xTileFlat::initEmpty(int mw, int mh)
{
  int num;

  clear();

	mwidth = mw;
	mheight = mh;

	num = mwidth * mheight;

	vecGrid = new int[num];
 
	memset(vecGrid, 0, num * 4);

}//initempty



void 
xTileFlat::initFromLayer(int * vec, int mw, int mh)
{
  int num;

  clear();

	mwidth = mw;
	mheight = mh;

	num = mwidth * mheight;

	vecGrid = new int[num];

  memcpy(vecGrid, vec, num*4);

}//initlayer  






void 
xTileFlat::render(float cx, float cy, int w, int h) //, float sx, float sy, float ex, float ey)
{
  int i, k, yt, t;
  float dx, dy;
  int sx, sy, ex, ey;
  xFlat * a;
  
  //w = 20;
  //h = 15;
  
  sx = (int) floor(cx / cw);
  sy = (int) floor(cy / ch);
  ex = sx + w + 1;
  ey = sy + h + 1;

  if (sy < 0) { sy = 0;}  if (sy >= mheight) { return; }
  if (sx < 0) { sx = 0;}  if (sx >= mwidth) { return;}
  
  if (ex < 0) { return; }  if (ex > mwidth) { ex = mwidth;}
  if (ey < 0) { return; }  if (ey > mheight) { ey = mheight; } 
  

  for ( i = sy; i < ey; i++)
  {
    yt = i * mwidth;
    
    for (k = sx; k < ex; k++)
    {
      t = vecGrid[yt+k];
      if (t < 0) { continue; }
      if (t >= XTILEFLAT_MAX_TILE) { continue; }

      a = &tileSet[t];
      if (a->disp < 0) { continue; }
      

        //use displayable tile
        t = a->disp;
        if (t < 0) { continue; }
        if (t >= XTILEFLAT_MAX_TILE) { continue; }
        a = &tileSet[t];


      
       dx = (k*cw)-cx;
       dy = (i*ch)-cy;
       
       //todo -- fill a vertex array
       //todo -- not vertex array, fill an xmdx
       //and the xmdx should take care of the vbo

       //according to this one, this is better (so the glbegin is inside the loop)
       ///http://www.mesa3d.org/brianp/sig97/perfopt.htm

         //glBegin(GL_TRIANGLES);
         glBegin(GL_TRIANGLE_STRIP);
          glTexCoord2f(a->u0, a->v0); //0
          glVertex2f(dx, dy);
      
          glTexCoord2f(a->u0, a->v1); //1 
          glVertex2f(dx, dy+ch);
      
          glTexCoord2f(a->u1, a->v0); //2 
          glVertex2f(dx+cw, dy);
      
          glTexCoord2f(a->u1, a->v1); //3 
          glVertex2f(dx+cw, dy+ch);
/*
          glTexCoord2f(a->u1, a->v0); //2 
          glVertex2f(dx+cw, dy);
      
          glTexCoord2f(a->u0, a->v1); //1 
          glVertex2f(dx, dy+ch);
              
          glTexCoord2f(a->u1, a->v1); //3 
          glVertex2f(dx+cw, dy+ch);
          */
         glEnd();
    }//nextk
  }//nexti

  
}//render
