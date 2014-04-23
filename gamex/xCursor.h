#pragma once

#include "xCam.h"
#include "xFrustum.h"
#include "xColMesh.h"
#include "xHeightMap.h"







class xCursor
{
public:
  gamex::cVec3f coord;
  
public:

  //umx and umy are the mouse coordinates 
  //need to be in  the range of [-1.0, 1.0]

  
  void updateHmap(xCam * cam, float umx, float umy, xHeightMap * hmap, float planey = 0.0f );

  void updateCmesh(xCam * cam, float umx, float umy, xColMesh * cmesh, float planey = 0.0f );




};//classend



//generate an aabb based on where a frustum intersects a (infinte) plane
class xViewBox
{
public:
  gamex::cVec3f min;
  gamex::cVec3f max;

  gamex::cVec3f v0[4];
  gamex::cVec3f v1[4];

public:

  void genBox(xFrustum * frust, 
    float px=0.0f, float py=0.0f, float pz=0.0f,
    float nx=0.0f, float ny=1.0f, float nz=0.0f);


};//viewbox







