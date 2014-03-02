#pragma once



class xGrid;
class xCell;
class xActor;

class xMultiGrid
{
public:
  float cx, cz;
  xCell * topCell;
  xGrid * vecGrid;
  int numGrid;
public:

  xMultiGrid();
  ~xMultiGrid();

    void clear();
  void init (float width_, float height_, int minxshift_, int  minyshift_, int levels_);
  xCell * doQuery(float x0, float y0, float w_, float h_);
  void putInXY(xActor * a);
  void putInXZ(xActor * a);
	void setPos(float x_, float y_);
    
};//xmultigrid
