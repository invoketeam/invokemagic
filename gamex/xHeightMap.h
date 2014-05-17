#pragma once


#include "xMdx3.h"
#include "xImage.h"




class xHeightTile
{
public:
   float u0, v0, u1, v1;
   //todo -- store same values as the xTile?
public:
  xHeightTile(void);
};//heighttile



#define XHEIGHTMAP_MAX_TILE 1024

//todo -- add tileskin data (skin uv on rectangle)

class xHeightMap
{
public:
  xHeightTile tileSet[XHEIGHTMAP_MAX_TILE];
  
  float * vecHeight; //height of vertex
  unsigned int * vecColor; //color of vertex
  int * vecGrid; //tile to use
  int * vecTop; //top layer

  int mw;
  int mh;
  
  float cx, cz;
  float cw, ch;

  xMdx3 mesh;

public:
  xHeightMap(void);

  void clear(void);

  void render(void);

  void initEmpty(int w, int h);

  void loadHeight(xImage * img, float yscale = 1.0f);

  void loadColor(xImage * img);
  
  void loadTileSet(std::string fname);
  void loadLayer(int * vec, int w, int h);
  void loadTopLayer(int * vec, int w, int h);


  float getHeight(float wx, float wy);

  void setBufferMesh(int maxQuad = (64*64));
  void updateMesh(float x0, float y0, float w0, float h0);

  void setDecal(xMdx3 * m, float x0, float y0, float w0, float h0);
  void setMesh(xMdx3 * m, float x0, float y0, float w0, float h0);
  void initMesh(xMdx3 * m, int maxQuad);


};//classend
