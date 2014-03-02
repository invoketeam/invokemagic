
#pragma once


class xFlat
{
public:
  float u0, v0, u1, v1;
  int maskx, masky;
  int wall;
  int spec;
  int disp; //which tile u0 v0 u1 v1 to use (used for animation)
  //todo -- same as disp but one used for mask and wall (?)
public:
  xFlat() 
  { 
    maskx = 0;
    masky = 0;
    u0 = 0.0f; 
    v0 = 0.0f;
    u1 = 1.0f;
    v1 = 1.0f;
    wall = 0;
    spec = 0;
    disp = 0;
  }//ctor

};//xflat

//todo -- collision mask will be an ximage

//enough for an 1024x1024 texture of 32x32 tiles
#define XTILEFLAT_MAX_TILE 1024

class xTileFlat
{
public:
  int mwidth;
	int mheight;
	int * vecGrid;
  
  float cw, ch;

  xFlat tileSet[XTILEFLAT_MAX_TILE]; 

public:
  xTileFlat(void);
  ~xTileFlat(void);
  
  void clear(void); 

  //init for a 512x512 tilemap of 32x32 tiles
  void initDefaultSet(void);
  
  void initEmpty(int mw, int mh);
  void initFromLayer(int * vec, int mw, int mh); 

  
  void render(float cx, float cy, int w=20, int h=15);
  
 /* 
  bool isWall(int tx, int ty);
	bool isWallWorld(float wx, float wy);

  void setTile(int tx, int ty, int t);
  void setTileWorld(float wx, float wy, int t);
 
  xFlat * getTileWorld(float wx, float wy);
  xFlat * getTile(int tx, int ty);
  
  void setRect(int ax, int ay, int aw, int ah, int c);
	void setRectWorld(float wx, float wy, float ww, float wh, int c);
 
  */
  
};//classend












