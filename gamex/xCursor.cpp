#pragma warning (disable : 4786)

#include "xCursor.h"



//todo -- make this function part of updateheightmap
static float getHmapInter(xHeightMap * hmap, gamex::cVec3f v0, gamex::cVec3f v1)
{
  float t;
  float u;
  float ax, ay, az;

  //todo -- set cell size as parameter and
  //calculate percantage based on that


  //a simple solution -- not the most accurate one, but we dont need accurate as it turns out
  //also doesnt need any triangle data, only the height data

  t = 0.03f;

  for (u = 0.0f; u < 1.0f; u += t)
  {
    ax = v0.x + (v1.x - v0.x) * u;
    ay = v0.y + (v1.y - v0.y) * u;
    az = v0.z + (v1.z - v0.z) * u;

    if ( hmap->getHeight(ax, az) > ay ) { return u; }
        
  }//nextu

  return -999.0f;
}//gethmapinter


  
void 
xCursor::updateHmap(xCam * cam, float umx, float umy, xHeightMap * hmap, float planey)
  {
    gamex::cVec3f ppos;
	  gamex::cVec3f pnorm;
	  gamex::cVec3f p0, p1;
	  gamex::cVec3f ret;
	  float t;
    xFrustum frust; 
  
      frust.setPerspective(cam->fov, cam->aspect, cam->neard, cam->fard);
	    frust.setPoints(cam->pos, cam->ori, 0, 0);

	    p0 = frust.nc;
	    p0 += frust.nearUp * umy;
	    p0 += frust.nearSide * umx;

	    p1 = frust.fc;
	    p1 += frust.farUp * umy;
	    p1 += frust.farSide * umx;

    ppos.set(0,planey,0);
    pnorm.set(0,1,0);
    t = frust.getPlaneInter(p0, p1, ppos, pnorm);
     
    if (t < 0) { coord = p1; return; } //did not hit the plane 

    p1 = p0 + (p1-p0) * t;

    t = getHmapInter(hmap, p0, p1);
  
    coord = p0 +(p1 -p0) * t;


  }//updateheightmap



void
xCursor::updateCmesh(xCam * cam, float umx, float umy, xColMesh * cmesh, float planey)
  {
    gamex::cVec3f ppos;
	  gamex::cVec3f pnorm;
	  gamex::cVec3f p0, p1;
	  gamex::cVec3f ret;
	  float t;
    xFrustum frust; 
  
      frust.setPerspective(cam->fov, cam->aspect, cam->neard, cam->fard);
	    frust.setPoints(cam->pos, cam->ori, 0, 0);

	    p0 = frust.nc;
	    p0 += frust.nearUp * umy;
	    p0 += frust.nearSide * umx;

	    p1 = frust.fc;
	    p1 += frust.farUp * umy;
	    p1 += frust.farSide * umx;


      if (cmesh == 0) { t = 999.0f; }
      else { t = cmesh->lineTest(&p0, &p1, 1); }

      if (t == 999.0f)
      {
        //if didnt hit terrain -- use the infinite plane
        ppos.set(0,planey,0);
      	pnorm.set(0,1,0);
        t = frust.getPlaneInter(p0, p1, ppos, pnorm);
      }//endif
 
	    coord = p0 +(p1 -p0) * t;

  }//update












void 
xViewBox::genBox(xFrustum * frust, float px, float py, float pz, float nx, float ny, float nz)
  {
    //clamping the aabb is not enough

    gamex::cVec3f p0, p1;
    float umx, umy;
    int i;
    gamex::cVec3f ppos, pnorm;
    float t;
    gamex::cVec3f coord;

    min = 999999999.0f;
    max = -999999999.0f;


    //set plane
     ppos.set(px, py, pz);
     pnorm.set(nx, ny, nz);

    for (i = 0; i < 4; i++)
    {
      if (i == 0) { umx = -1.0f;  umy = -1.0f; }
      else if (i == 1) { umx = 1.0f;  umy = -1.0f; }
      else if (i == 2) { umx = -1.0f;  umy = 1.0f; }
      else if (i == 3) { umx = 1.0f;  umy = 1.0f; }

        p0 = frust->nc;
	      p0 += frust->nearUp * umy;
	      p0 += frust->nearSide * umx;

	      p1 = frust->fc;
	      p1 += frust->farUp * umy;
	      p1 += frust->farSide * umx;

      //check where p0 - p1 intersects the plane
        //if didnt hit terrain -- use the infinite plane

        t = frust->getPlaneInter(p0, p1, ppos, pnorm);
        if (t < 0) { coord = p1; }
        else { coord = p0 + (p1 - p0) * t; }
        v0[i] = coord;
        v1[i] = p0;

          if (coord.x < min.x) { min.x = coord.x; }
          if (coord.x > max.x) { max.x = coord.x; }
          if (coord.y < min.y) { min.y = coord.y; }
          if (coord.y > max.y) { max.y = coord.y; }
          if (coord.z < min.z) { min.z = coord.z; }
          if (coord.z > max.z) { max.z = coord.z; }

        coord = p0;
          if (coord.x < min.x) { min.x = coord.x; }
          if (coord.x > max.x) { max.x = coord.x; }
          if (coord.y < min.y) { min.y = coord.y; }
          if (coord.y > max.y) { max.y = coord.y; }
          if (coord.z < min.z) { min.z = coord.z; }
          if (coord.z > max.z) { max.z = coord.z; }

    }//nexti


  }//genbox






















