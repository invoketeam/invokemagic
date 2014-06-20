#pragma warning (disable : 4786)

#include <math.h>

#include "xMultiGrid.h"
#include "xActor.h"


xCell* xCell::lastc  = 0;


void 
xCell::addToLast(void) {	if (lastc != 0) lastc->next = this;lastc = this;	}

void 
xCell::at2(void) 	{	if (num <= 0) return;	if (lastc != 0) lastc->next = this;	lastc = this;	}
	
void 
xCell::setAsLast(void) {	lastc = this;	next = 0;	}
	
void 
xCell::addActor(xActor * a)
	{
		if (a->cell == this) return; //already in this cell
		if (a->cell != 0) a->cell->remActor(a); //was in another cell, remove it
		if (num == 0) //empty cell -- (same as check for first == 0 i guess)
		{  //add as only element
			first = a;	last = a;	a->next = 0;	a->prev = 0;	}
		else 
		{	//add as last element
			a->prev = last;	a->next = 0;	last->next = a;	last = a;
		}//endif
		a->cell = this; 	num += 1;
	}//addactor
	
void 
xCell::remActor(xActor * a)
	{
		if (a->cell != this) {return;} //not in this cell
		if (a->next != 0) {a->next->prev = a->prev;}	if (a->prev != 0) {a->prev->next = a->next;}
		if (first == a) {first = a->next;}	if (last == a) {last = a->prev;}
		a->cell = 0; //not in any cells
		num -= 1; //one less resident in cell
	}//remactor






class xGrid
{
public:
  xCell * vecCell;
  int numCell;
  
   
  int xcell;
  int ycell;
  
  //cell size stored in shift 
	//5 shift is 32 as the minimum allowed
	int shift_x;
  int shift_y;

  //precalculated (1 << shift_x)
  float cell_size_x;
  float cell_size_y;

  float icellx;
  float icelly;

  //precalculated (cell_size_x * xcell)
  float grid_size_x;
  float grid_size_y;
  
  
  float cx;
  float cy;
  
  


public:
  xGrid(void);
  ~xGrid(void);
  

  void clear(void);
  void init(int xcell_, int ycell_, int shiftx_, int shifty_);
  void doQuery(float x0, float y0, float w_, float h_);
  bool putInXY(xActor * a);
  bool putInXZ(xActor * a);


};//classend_xgrid






  xMultiGrid::xMultiGrid(void)
  {
    cx = 0; cz = 0;
    topCell = 0;
    vecGrid = 0;
    numGrid = 0;
  }//ctor
  
  
xMultiGrid::~xMultiGrid(void)
{
  clear();
}//dtor  


void 
xMultiGrid::clear(void)
{
  if (topCell != 0) { delete topCell; topCell = 0; }
  if (vecGrid != 0) { delete [] vecGrid; vecGrid = 0;}
  numGrid = 0;
  
}//clear
  

 void 
 xMultiGrid::init (float width_, float height_, int minxshift_, int  minyshift_, int levels_)
 {
      clear();
 
    	if (width_ < 512) width_ = 512;
			if (height_ < 512) height_ = 512;
			
			if (minxshift_ < 5) minxshift_ = 5;
			if (minyshift_ < 5) minyshift_ = 5;
			
			if (minxshift_ > 12) minxshift_ = 12;
			if (minyshift_ > 12) minyshift_ = 12;
			
			if (levels_ < 1) levels_ = 1;
			if (levels_ > 6) levels_ = 6;
			
			numGrid = levels_;
			
      vecGrid = new xGrid[numGrid];
      topCell = new xCell();

      int i;
      int perx;
      int pery;

      
      
      for (i = 0; i < numGrid; i++)
      {
        perx = (int) floorf(width_ / (1 << minxshift_)) + 1;
        pery = (int) floorf(height_ / (1 << minyshift_)) + 1;
        
        vecGrid[i].init(perx, pery, minxshift_, minyshift_);
      
        minxshift_++;
        minyshift_++;
      }//nexti
      
 }//init
 
 
 
 xCell * 
 xMultiGrid::doQuery(float x0, float y0, float w_, float h_)
 {
 		//set topcell as the query first cell
			topCell->setAsLast();
			
				x0 -= cx;
				y0 -= cz;
			
			//make sure rect is valid
			if (x0 < 0) { x0 = 0; }
			if (y0 < 0) { y0 = 0; }
			
      float x1;
      float y1;
      int i;
      
			x1 = x0 + w_;
			y1 = y0 + h_;
			
			
	for (i = 0; i < numGrid; i++) {  vecGrid[i].doQuery(x0, y0, x1, y1);	}

    xCell::lastc->next = 0; 
      
	return topCell; //always return this one -- start of the query cell
 }//doquery
 
 
 
 
void
 xMultiGrid::putInXY(xActor * a)
 {
  int i;
  for (i = 0; i < numGrid; i++) {   if (vecGrid[i].putInXY(a)) { return; } }
	topCell->addActor(a);  //couldnt put in grids put it in the topcell
 }//putinxy
  
void
 xMultiGrid::putInXZ(xActor * a)
 {
  int i;
   for (i = 0; i < numGrid; i++) {   if (vecGrid[i].putInXZ(a)) { return; } }
 	 topCell->addActor(a);  //couldnt put in grids put it in the topcell
 }//putinxz
  
 void 
 xMultiGrid::setPos(float x_, float y_)
{
  int i;
  for (i = 0; i < numGrid; i++) {   vecGrid[i].cx = x_;  vecGrid[i].cy = y_;}
  cx = x_; 
  cz = y_;
}//setpos









 xGrid::xGrid(void)
 {
  vecCell = 0;
  numCell = 0;
   
  xcell = 0;
  ycell = 0;

  shift_x = 0;
  shift_y = 0;

  cell_size_x = 0;
  cell_size_y = 0;

  icellx = 0;
  icelly = 0;

  grid_size_x = 0;
  grid_size_y = 0;
   
  cx = 0;
  cy = 0;
 }//ctor
 
 
 
 xGrid::~xGrid(void)
 {
  clear();
 }//dtor
 
  
 void 
 xGrid::clear(void)
 {
  if (vecCell != 0) { delete [] vecCell; vecCell = 0; }
  numCell = 0;
 }//clear
	
 void 
 xGrid::init(int xcell_, int ycell_, int shiftx_, int shifty_)
 {
 
			xcell = xcell_;
			ycell = ycell_;
			
			if (xcell <= 0) xcell = 1;
			if (ycell <= 0) ycell = 1;
			
			numCell = xcell * ycell;
			
		  vecCell = new xCell[numCell];
      
			shift_x = shiftx_;
			shift_y = shifty_;
			
			if (shift_x < 5) shift_x = 5;
			if (shift_y < 5) shift_y = 5;
			
			//maximum size is 2^16 -- 65536 -- should be enough for now
			if (shift_x > 16) shift_x = 16;
			if (shift_y > 16) shift_y = 16;
			
			cell_size_x = (float) ( 1 << shift_x );
			cell_size_y = (float) (1 << shift_y );

			icellx = 1.0f / cell_size_x;
			icelly = 1.0f / cell_size_y;
			
			grid_size_x = cell_size_x * xcell;
			grid_size_y = cell_size_y * ycell;

    printf("grid init numcell %d cell [%0.2f]x[%0.2f] \n", numCell,cell_size_x, cell_size_y  );
 }//init
  
void
 xGrid::doQuery(float x0, float y0, float w_, float h_) 
 {
	int ix0, iy0, ix1, iy1;
	int yt;
	int gy;
	int gx;

	ix0 = (int) floorf(x0 * icellx); 
	iy0 = (int) floorf(y0 * icelly); 
			
	ix1 = (int) floorf((x0+w_) * icellx);
	iy1 = (int) floorf((y0+h_) * icelly);
			
			ix1 += 1;
			iy1 += 1;
			
			//make sure it is in limits
			//not x0 and y0 is checked in the multigrid doQuery(..) already
			if (ix1 > xcell) { ix1 = xcell; }
			if (iy1 > ycell) { iy1 = ycell; }
	
		

			for (gy = iy0; gy < iy1; gy++)
			{
				yt = gy * xcell;

				for (gx = ix0; gx < ix1; gx++)
				{
					vecCell[yt + gx].at2(); //add to end of list if has any actors in it
				}//next gx
			}//next gy
 }//doquery
 

 bool 
 xGrid::putInXY(xActor * a)
 {
  int px;
  int py;
  
 		//x tile to put in
				px = (int) floorf((a->pos.x - (a->xrad)-cx) * icellx);
			
				//is it valid
					if (px < 0 || px >= xcell) return false;
					
				//is it the same as the other corner
					if (px != (int) floorf(((a->pos.x  + (a->xrad)-cx) * icellx)) ) return false;
			
			//y tile to put in
				//var py:Number = (a->cy - (a->yrad)) >> shift_y;
          py = (int) floorf( (a->pos.y - (a->yrad)-cy) * icelly);
			
				//is it valid
					if (py < 0 || py >= ycell) return false;
				
				//is same as other corner
					if (py != (int) floorf(((a->pos.y + (a->yrad)-cy) * icelly)) ) return false;
			
			//put in cell at px, py
			
				vecCell[ ((py * xcell) + px) ].addActor(a);
			
			return true;
 }//putinxy
 
 
 
 bool 
 xGrid::putInXZ(xActor * a)
 {
  int px;
  int py;
 
 	//x tile to put in
				px = (int) floorf( (a->pos.x - (a->xrad) - cx) * icellx);
			
				//is it valid
					if (px < 0 || px >= xcell) return false;
					
				//is it the same as the other corner
					if (px != (int) floorf(((a->pos.x  + (a->xrad) - cx) * icellx)) ) return false;
			
			//y tile to put in
				py = (int) floorf( (a->pos.z - (a->zrad) - cy) * icelly);
			
				//is it valid
					if (py < 0 || py >= ycell) return false;
				
				//is same as other corner
					//if (py != ((a.cy + (a.yrad) - cy) >> shift_y) ) return false;
					if (py != (int) floorf( ((a->pos.z + (a->zrad) - cy) * icelly) ) ) return false;
				
			//put in cell at px, py
			
				vecCell[ ((py * xcell) + px) ].addActor(a);
			
			return true;
 }//putinxz

