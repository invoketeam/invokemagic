
#pragma once

#include "xMath.h"
#include "xMdx3.h"
#include "xTileMap.h"

#include <vector>








class xTri
{
public:
//cant be bothered to replace these with vec3f for now
  float x0, y0, z0;  float x1, y1, z1;  float x2, y2, z2;  float nx, ny, nz;
public:
  inline void setValue(gamex::cVec3f * a, gamex::cVec3f * b, gamex::cVec3f * c);
  inline void calcNormal(void);

  //0-1 point is on line
  //negative (-999) -- tri was not hit
  inline  float lineTest(float ax, float ay, float az,    float bx, float by, float bz, float rad);

  //returns squared distance from point to triangle
  float getDist(float wx, float wy, float wz, gamex::cVec3f * ret=0);
};//xtri





typedef std::vector <xTri * > tdVecTri;




//a quadtree designed to store the triangles
//(as the gameplay is 2.5D it will do nicely for now)
//and its only really needed to check where the cursor is on the terrain for now
//(but might be used for other things, so i didnt use the zbuffer reading method for that)
class xColRect
{
public:
  float rx, ry, rw, rh; //actually rx rz (for now)
  int d;
  xColRect * child0;  xColRect * child1;  xColRect * child2;  xColRect * child3;
  tdVecTri vecTri; //only ptr, dont delete members
  xTri * colTri;
public:
  xColRect(void);
  ~xColRect(void);
  void clear(void);
  void render(float yc);
  void genChild(int md=-1);
  bool isTriInside(xTri * a);
  bool addTri(xTri * a, int maxd = 3);
  float lineTest(gamex::cVec3f * start, gamex::cVec3f * end, float rad=1.0f);
  bool isOverLine(float ax, float az, float bx, float bz, float rad);

};//classend



class xColMesh
{
public:
  tdVecTri vecTri; //real storage -- delete on clear
  xColRect rect; //make sure starting rect is big enough
  xTri * colTri; //last triangle linetest hit
public:
  xColMesh(void);
  ~xColMesh(void);
  
  void initMesh(xMdx3 * mdx, int maxd = 4);
  void initRect(float ax, float ay, float aw, float ah);

  void addMesh(xMdx3 * mdx, int maxd = 4);
  void addTileMap(xTileMap * tmap);

  void clear(void);
  float lineTest(gamex::cVec3f * start, gamex::cVec3f  * end, float rad = 1.0f);
  void render(void);
 
};//xcolmesh




class xColOb
{
public:
  gamex::cMat transMat;
  xColMesh * colMesh; //only ptr, can be shared among colob
  gamex::cVec3f lastNorm;
  gamex::cVec3f colPos;
  float t;
  bool bHit;
public:
  xColOb(void);

  void updateMatrix(gamex::cVec3f * pos, gamex::cQuat * ori);
  float lineTest(gamex::cVec3f * start, gamex::cVec3f * end, float rad);
   
};//classend












