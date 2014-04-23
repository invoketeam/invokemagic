#pragma once

#include <vector>

#include "xMath.h"



//todo -- something got fucked up at memory handling lane
//its somewhere in rebuildzone or makezonequad
//we do some heap bullshit at dtor thing


class xQuad;
class xPathZone;
class xTileMap;

typedef std::vector <xPathZone *> tdVecZone;
//typedef std::vector <xQuad *> tdVecQuad;

class xPathZone
{
public:
	float cx, cy, cw, ch;
	float mx, my;
	float cx2, cy2; //(cx+cw), (cy+ch)

   int zt; //zonetype
   int height;
   int spec;

	int group;

	int col;
	
	double cost;
	
	int frame; //if same as curframe we already checked this one (eliminating the closed array)

	xPathZone * parent; //used for building final path
	tdVecZone vecNext; //neighbours

	bool dead; //used in cleanup

public:
	xPathZone();
};//classend


typedef xPathZone* tdZonePtr; 


class xPathFind
{
public:
	tdVecZone vecZone;

	tdZonePtr * vecOpen;
	int maxOpen;

	int curFrame;
	int oit; //open list iterator

	float startx, starty, endx, endy;

	xPathZone * start;
	xPathZone * goal;
	xPathZone * lastZone;

  int zoneFlag;

	bool bFinish;

  xQuad * vecQuad; //qwidth*qheight
  
	int qcell; //a quadrant is qcell*qcell in size
	int qwidth;
	int qheight;

	float cw; //tilemap cell size
	float ch; 
  int mw; //tilemap size
  int mh;

		
public:
	xPathFind(void);
	~xPathFind(void);

	void clear(void);


	void findPath(int numstep = 1024, int mode=0);
	int startPath(float sx, float sy, float ex, float ey, int zflag=0xFFffFFff);
	void startPath2(xPathZone * startzone, xPathZone * goalzone);
	xPathZone * getZone(float wx, float wy);
	void setPath(std::vector <gamex::cVec3f> &vec);

	void buildZone(xTileMap * tmap);
	void conQuad(xQuad * qa);
	void conQuad2(xQuad * qa, xQuad * qb);
	void genGroup(void);

	void setDirty(float wx, float wy);
  void setDirtyRect(float wx, float wy, float ww, float wh);
  void rebuildQuad(xTileMap * tmap);
  void makeZoneQuad(xTileMap * tmap, xQuad * qa);


  void debRender(float ay);



};//classend
