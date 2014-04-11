#pragma once


#include "xMdx3.h"

class xTile
{
public:
	int tex; 
	int wall; 
	int mdx;  //no longer used
  //int zt; //bitflag -- 0 means not walkable //nope, make zonetypes independent to allow dynamic maps
  int height; 
  int spec; //303 connects two heights (any height)
	gamex::cVec3f uv[4];

public:
  xTile()
  {
    tex = 0; wall = 0;
    mdx = 0;
    // zt = 1;
    height = 0;
    spec = 0;
  }//ctor

};//xtile



//todo -- problem: walking speed on terrain should be based on tile(texture)(?)
// -- don't even know if we should have this feature at all

class xRect
{
public:
  gamex::cVec3f vert[4];
  int skin; //skin tile index (maybe it should just store the uv as is?)
  //gamex::cVec3f uv[4]; 
  //todo -- store these in different place -- for animation?
  //.. not sure, problem is terrain will be stored in big chunks
  //for easier access and stuff
  //which means changing the uv for the tiles will be problematic
  //as in opengl each vertex has its own uv and cannot share
  //(unless with crazy shadering, which will be optional
  // so we cannot rely on that)
public:
  xRect() { skin = 0; }

};//xrect



///todo 
//tilemap is probably not the right name anymore
//as its a mix of heightmap and tilemap in 3D


#define XTILEMAP_MAX_TILE 1024

class xTileMap
{
public:
	int mwidth;
	int mheight;
	int * vecGrid;
  xRect * vecRect; //heightmap rectangle (mwidth*mheight)
  int * vecZtype; //zonetype (mwidth*mheight)


	float cw;
	float ch;
	float icw;
	float ich;

  float crise;

	bool bVoidWall; //out of bounds is considered solid or empty

	float rayEndCx;
	float rayEndCy;

	xTile tileSet[XTILEMAP_MAX_TILE];

public:
	xTileMap(void);
	~xTileMap(void);


	void clear(void);

  void initEmpty(int mw, int mh);
  void initFromLayer(int * vec, int mw, int mh);

	
	bool isWall(int tx, int ty);
	bool isWallWorld(float wx, float wy);

  void setWall(int tx, int ty, int t);
  void setWallWorld(float wx, float wy, int t);

  xTile * getXTile(int tx, int ty);
  int getZoneType(int tx, int ty);
  void setZoneType(int tx, int ty, int zt);
  //  int getZoneType2(int tile);

//	void genDebug(void);

  void genHeightRect(void);

  void debApplyHeightMap(void);

	//void renderDebug();
//	void renderDebug2();
  
  void render(void);

  void setSkinFromLayer(int * vec, int mw, int mh);
  

  void makeMesh(xMdx3 * mesh, int sx, int sy, int sw, int sh);
  int getNumTri(int sx, int sy, int sw, int sh);


	void debHeightRender(void);

//todo -- add support for height 
	bool rayCast(int x0, int y0, int x1, int y1);
	bool rayCast2(float ax, float ay, float bx, float by);


	void copyTmap(xTileMap * a);


  float getHeight(float wx, float wy);

  xTile * getTileWorld(float wx, float wy);
  xTile * getTile(int tx, int ty);

  bool canMove(float wx, float wy, float gx, float gy);


	void setRect(int ax, int ay, int aw, int ah, int c);
	void setRectWorld(float wx, float wy, float ww, float wh, int c);





};//xtilemap
