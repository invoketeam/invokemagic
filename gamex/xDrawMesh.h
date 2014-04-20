
#pragma once


#include "xMath.h"
#include "xMdx3.h"
#include "xTileMap.h"
#include "xFrustum.h"

#include <vector>


class xRender;

class xDrawRect
{
public:
  gamex::cVec3f pos;
  float rad;
  xMdx3 mesh;
  
public:
  xDrawRect(void) { rad = 1.0f; }

};//drawrect




typedef std::vector <xMdx3 * > tdVecMesh;



//its an octree 

class xDrawTree
{
public:
  gamex::cVec3f pos; //top corner pos
  gamex::cVec3f size;
  gamex::cVec3f mid;
  float rad;
  int depth;

  tdVecMesh vecMesh; //only ptr dont delete


  //not using array to scare away people from touching this thing

  xDrawTree * child0;
  xDrawTree * child1;
  xDrawTree * child2;
  xDrawTree * child3;

  xDrawTree * child4;
  xDrawTree * child5;
  xDrawTree * child6;
  xDrawTree * child7;


public:
  xDrawTree(void);
  ~xDrawTree(void);

  void clear(void);

  void genChild(void);
  bool addMesh(xMdx3 * a, int maxd=4);
  void render(xFrustum * frust, xRender * rend, unsigned int skin);


};//drawtree







class xDrawMesh
{
public:
  xDrawRect * vecRect;
  int numRect;

  xDrawTree tree;

public:
  xDrawMesh(void);
  ~xDrawMesh(void);


  void clear(void);


  void render(xFrustum * frust, xRender * rend, unsigned int skin)
  {
    tree.render(frust, rend, skin);

  }//render

  

  void initRect(xTileMap * tmap);


  /*
  void debRender(xFrustum * frust)
  {
    
    int i;
    xDrawRect * a;
    // a 256x256 map will have 1024 zones .. 
    // thats not much, but might be better to use a tree for this thing as well
    for (i = 0; i < numRect; i++)
    {
      a = &(vecRect[i]);
      if (frust->isPointInside(a->pos, a->rad) == false) { continue; }
      a->mesh.render();
    }//nexti
    
  }//debrender
   */
};//drawmesh
