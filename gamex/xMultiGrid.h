#pragma once



class xActor;
class xGrid;

class xCell
{
public:
  xActor * first;
  xActor * last;
  
  xCell * next;
  static xCell * lastc;

  int num;

public:
  xCell(void)
  {
    first = 0;
    last = 0;
    next = 0;
    num = 0;
  }//ctor

	void addToLast(void); 
  void at2(void);
  void setAsLast(void);
	void addActor(xActor * a);
	void remActor(xActor * a);

};//classend



class xMultiGrid
{
public:
  float cx, cz;
  xCell * topCell;
  xGrid * vecGrid;
  int numGrid;
public:

  xMultiGrid(void);
  ~xMultiGrid(void);

  void clear(void);
  void init (float width_, float height_, int minxshift_, int  minyshift_, int levels_);
  xCell * doQuery(float x0, float y0, float w_, float h_);
  void putInXY(xActor * a);
  void putInXZ(xActor * a);
	void setPos(float x_, float y_);
    
};//xmultigrid
