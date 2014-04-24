#pragma once

#include <set>

class xCam;
class xMultiGrid;
class xGame;

class xSelect
{
public:
  int * vecOver;
  int maxSel;
  int numOver;
  std::set <int> setId; //selected units id

  int unitId; //-1 no selection  -- any other: this is a single unit selected
  int overId;
public:
  xSelect(void);
  ~xSelect(void);
  
  void init(void);
  void clear(void);

  //todo -- check for single select (click)
  void selectOver(xCam * cam, xMultiGrid * mgrid, 
    float ax, float ay, float aw, float ah,
    float bx, float by, float bw, float bh   );

  void resetSelect(void);
  void appendOverToSelect(void);

  void sendMsg(xGame * game, int msg, int arg0, int arg1, int arg2);

  void debRender(xGame * game);

};//select