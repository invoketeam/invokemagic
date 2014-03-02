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

	//tdVecQuad vecQuad;

  xQuad * vecQuad; //qwidth*qheight
  
	int qcell; //a quadrant is qcell*qcell in size
	int qwidth;
	int qheight;

	float cw; //tilemap cell size
	float ch; 
  int mw; //tilemap size
  int mh;

		
public:
	xPathFind();
	~xPathFind();

	void clear();


	void findPath(int numstep = 1024, int mode=0);
	int startPath(float sx, float sy, float ex, float ey, int zflag=0xFFffFFff);
	void startPath2(xPathZone * startzone, xPathZone * goalzone);
	xPathZone * getZone(float wx, float wy);
	void setPath(std::vector <gamex::cVec3f> &vec);

	void buildZone(xTileMap * tmap);
	void conQuad(xQuad * qa);
	void conQuad2(xQuad * qa, xQuad * qb);
	void genGroup();

	void setDirty(float wx, float wy);
  void setDirtyRect(float wx, float wy, float ww, float wh);
  void rebuildQuad(xTileMap * tmap);
  void makeZoneQuad(xTileMap * tmap, xQuad * qa);


  void debRender();


	/*
    public function buildZone(vecGrid:Vector.<int>, mw:int, mh:int,  cellw:Number=1.0, cellh:Number=1.0):int
          public function conQuad(qa:xQuad):void
      public function conQuad2(qa:xQuad, qb:xQuad):void
            public var mr:xMegaRand = new xMegaRand();
        public function genGroup():void
     public function getClosex(vec:Vector.<int>, col:Vector.<int>, mw:int, ax:int, ay:int):int
       public function copyVec(vec:Vector.<int>):Vector.<int>
     public function setRect(vec:Vector.<int>,mw:int,mh:int,  ax:int, ay:int, aw:int, ah:int, c:int):void

	//rebuild dirty quads (and connections with their neighboring quads)
		public var tempQuad:Vector.<xQuad> = new Vector.<xQuad>(1024, false);
		public function rebuildQuad(vecGrid:Vector.<int>, mw:int, mh:int):void
			public function makeZoneQuad(qa:xQuad, vec:Vector.<int>, mw:int, mh:int,  cellw:Number=1.0, cellh:Number=1.0):void
    */

};//classend
