
#pragma once

#include "xMath.h"
#include "xMdx3.h"
#include "xTileMap.h"

#include <vector>






//todo -- store uv (lightmap uv lookup)

class xTri
{
public:
//cant be bothered to replace these with vec3f for now
  float x0, y0, z0;  float x1, y1, z1;  float x2, y2, z2;  float nx, ny, nz;
  float u0, v0, u1, v1, u2, v2;
  //todo -- material id? spec id?
public:
  inline void setValue(gamex::cVec3f * a, gamex::cVec3f * b, gamex::cVec3f * c);
  inline void calcNormal(void);

  //0-1 point is on line
  //negative (-999) -- tri was not hit
  inline  float lineTest(float ax, float ay, float az,    float bx, float by, float bz, float rad);

  //returns squared distance from point to triangle
  float getDist(float wx, float wy, float wz, gamex::cVec3f * ret=0);

  //p is a 3D point on the triangle -- writes the resulting uv coordinats into the values pointed at
  void getUv(gamex::cVec3f p, float * ret_u, float * ret_v);

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


  //cut tris from the aabb [smin, smax] with triangles facing norm, and put the tris in mesh  
  //notes: set mesh drawface to 0  (stops when it reaches numface)
  // also mesh vertices needs to be 3x the number of faces
  // (and you need to set the indices yourself to be 0 1 2 .. etc)
  void getDecalMesh(gamex::cVec3f smin, gamex::cVec3f smax, gamex::cVec3f norm, xMdx3 * mesh); 

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
//  void addTileMap(xTileMap * tmap);

  void clear(void);
  float lineTest(gamex::cVec3f * start, gamex::cVec3f  * end, float rad = 1.0f);

  //used for debugging
  void render(void);
  void uvRender(void);
 
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












