#pragma warning (disable : 4786)

#include "xTileMap.h"

#include <math.h>
#include "xGLCommon.h"


xTile::xTile(void)
{
 maskx = 0;    masky = 0;
 u0 = 0.0f;     v0 = 0.0f;    u1 = 1.0f;    v1 = 1.0f;
 wall = 0;    spec = 0;    disp = 0;
 color = 0;
}//ctor







xTileMap::xTileMap(void)
{
  mw = 0; mh = 0; cw = 16.0f; ch = 16.0f;
  vecGrid = 0;
  vecZone = 0;
  vecHeight = 0;
}//ctor



xTileMap::~xTileMap(void)
{
  clear();
}//dtor


  
void 
xTileMap::clear(void)
{
  if (vecGrid != 0) { delete [] vecGrid; } vecGrid = 0; 
	if (vecZone != 0) { delete [] vecZone; } vecZone = 0; 
	if (vecHeight != 0) { delete [] vecHeight; } vecHeight = 0; 
	
  mw = 0; mh = 0; 

}//clear


void 
xTileMap::initEmpty(int w, int h)
{
  clear();

  int num;

  mw = w;
  mh = h;

  num = mw * mh;

  vecGrid = new int[num]; 	memset(vecGrid, 0, num * 4);
  vecZone = new int[num]; 	memset(vecZone, 0, num * 4);
  vecHeight = new int[num];	memset(vecHeight, 0, num * 4);

}//initempty



void 
xTileMap::initFromHeightmap(xHeightMap * hmap)
{
  clear();

  cw = 32.0f;
  ch = 32.0f;

  mw = (hmap->mw * hmap->cw) / cw;
  mh = (hmap->mh * hmap->ch) / ch;

  initEmpty(mw, mh);

  int i, k, yt;

  //todo -- for now the idea is to generate a voxelised represantion of the heightmap
  //and only allow climbing for 1-2 height difference (?)
  //so this will needs some adjusting

  float h;

  //(not optimised yet as i dont even know if im doing the right thing
  for (i = 0; i < mh; i++)
  {
    yt = i * mw;
    for (k = 0; k < mw; k++)
    {

      h = hmap->getHeight((float)k*cw+(cw*0.5f),(float)i*ch+(cw*0.5f) );

      vecHeight[yt+k] = floorf(h / 32.0f);

    }//nextk
  }//nexti

}//initfromhmap


void 
xTileMap::setGridFromLayer(int * vec, int w, int h)
{
  //todo -- warning if mw mh not match

  int num;
  num = mw * mh;
   memcpy(vecGrid, vec, num*4); 
}//setgrid



void 
xTileMap::setZoneRect(int ax, int ay, int aw, int ah, int zt) 
{
  int ex, ey;	int i;	int k;	int yt;

	ex = ax + aw;   	ey = ay + ah;
           
	if (ax < 0) { ax = 0;  }     	if (ay < 0) { ay = 0;  }  
	if (ex > mw) { ex = mw;   }    if (ey > mh) { ey = mh;    }   
	if (ax >= mw) { return;  }    	if (ay >= mh){ return;  }   
	if (ex < 0) { return;  }  if (ey < 0) { return;  }
            
	for (i = ay; i < ey; i++) 
	{ 
		yt = i * mw;	
		for (k = ax; k < ex; k++) {	vecZone[yt+k] = zt; }
	} 
}//setrect
 

void
xTileMap::setZoneRectWorld(float wx, float wy, float ww, float wh, int zt) 
{
  int ax, ay, aw, ah;
  ax = (int) floorf(wx/cw);
  ay = (int) floorf(wy/cw);
  aw = (int) ceilf(ww/cw);
  ah = (int) ceilf(wh/cw);

  setZoneRect(ax, ay, aw, ah, zt);

}//setrectworld


xTile *
xTileMap::getTileAt(int ax, int ay)
{
  if (ax < 0) { return 0; }   	if (ay < 0) { return 0; }
  if (ax >= mw) { return 0; } 	if (ay >= mh) { return 0; }

  int t;
  t = vecGrid[ax + (ay*mw)];

  //check if in range (?) (for now just crash)

	return &(tileSet[t]); 
}//gettileat


int 
xTileMap::getHeightAt(int ax, int ay)
{
  if (ax < 0) { return 0; }   	if (ay < 0) { return 0; }
  if (ax >= mw) { return 0; } 	if (ay >= mh) { return 0; }

	return vecHeight[ax + (ay*mw)]; 
}//getheight

int
xTileMap::getZoneAt(int ax, int ay)
{
  if (ax < 0) { return 0; }   	if (ay < 0) { return 0; }
  if (ax >= mw) { return 0; } 	if (ay >= mh) { return 0; }

	return vecZone[ax + (ay*mw)]; 
}//getzone


int
xTileMap::getZoneWorld(float wx, float wy)
{
  int ax, ay;
  ax = (int) floorf(wx/cw);
  ay = (int) floorf(wy/cw);
  return getZoneAt(ax,ay);
}//getzoneworld



void 
xTileMap::render(float ax, float ay, float aw, float ah)
{
  int i, k, yt, t;
  float dx, dy;
  int sx, sy, ex, ey;
  xTile * a;


  sx = (int) floorf(ax / cw);
  sy = (int) floorf(ay / ch);
  ex = sx + ((int) ceilf(aw/cw)) + 1;
  ey = sy + ((int) ceilf(ah/ch)) + 1;

  if (sy < 0) { sy = 0;}  if (sy >= mh) { return; }
  if (sx < 0) { sx = 0;}  if (sx >= mw) { return;}
  
  if (ex < 0) { return; }  if (ex > mw) { ex = mw;}
  if (ey < 0) { return; }  if (ey > mh) { ey = mh; } 
  

  for ( i = sy; i < ey; i++)
  {
    yt = i * mw;
    
    for (k = sx; k < ex; k++)
    {
      t = vecGrid[yt+k];
      if (t < 0) { continue; }
      if (t >= XTILEMAP_MAX_TILE) { continue; }

      a = &tileSet[t];
      if (a->disp < 0) { continue; }
      

        //use displayable tile
        t = a->disp;
        if (t < 0) { continue; }
        if (t >= XTILEMAP_MAX_TILE) { continue; }
        a = &tileSet[t];
   
       dx = (k * cw) - ax;
       dy = (i * ch) - ay;
       
       //according to this one, this is better (so the glbegin is inside the loop)
       ///http://www.mesa3d.org/brianp/sig97/perfopt.htm

         glBegin(GL_TRIANGLE_STRIP);
          glTexCoord2f(a->u0, a->v0); //0
          glVertex2f(dx, dy);
      
          glTexCoord2f(a->u0, a->v1); //1 
          glVertex2f(dx, dy+ch);
      
          glTexCoord2f(a->u1, a->v0); //2 
          glVertex2f(dx+cw, dy);
      
          glTexCoord2f(a->u1, a->v1); //3 
          glVertex2f(dx+cw, dy+ch);
         glEnd();
    }//nextk
  }//nexti


}//render