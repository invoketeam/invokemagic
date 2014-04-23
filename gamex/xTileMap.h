
#pragma once

#include "xHeightMap.h"



class xTile
{
public:
  float u0, v0, u1, v1;
  int maskx, masky;
  int wall;
  int spec;
  int disp; 
  
  unsigned int color;

public:
  xTile(void);

};//xtile2



#define XTILEMAP_MAX_TILE 1024


//maybe the tilemap should be part of the heightmap?
//aka the heightmap related functions should be in heightmap
//and the heightmap should have a copy of the tilemap


class xTileMap
{
public:
  xTile tileSet[XTILEMAP_MAX_TILE]; 

  int mw, mh;
  float cw, ch;


  //each tilemap has 3 layers
   int * vecGrid;
   int * vecZone;
   int * vecHeight;

public:
  xTileMap(void);
  ~xTileMap(void);
  
  void clear(void);

  void initEmpty(int w, int h);
  void setGridFromLayer(int * vec, int w, int h);

  void setZoneRect(int ax, int ay, int aw, int ah, int zt);
  void setZoneRectWorld(float wx, float wy, float ww, float wh, int zt);

  int getZoneAt(int ax, int ay);
  int getZoneWorld(float wx, float wy);

  xTile * getTileAt(int ax, int ay);
  int getHeightAt(int ax, int ay);

  void initFromHeightmap(xHeightMap * hmap);
  
  void render(float ax, float ay, float aw, float ah);

};//tilemap







