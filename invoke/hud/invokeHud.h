#pragma once


#include "../../gamex/xGuiGame.h"
#include "../../gamex/xCursor.h"

#include "xMiniMap.h"
#include "xSelect.h"


#define MAXICON 64

class invokeHud : public xGuiGame
{
public:
  xSpriteMan mySprite;


  xSelect mySelect;
  int selStart;
  float selx;
  float sely;
  int curMode;


  //aabb of view
    gamex::cVec3f viewMin;
    gamex::cVec3f viewMax;

  xMiniMap myMini;


  xButton * vecIcon[MAXICON];



public:
  invokeHud(void);
  ~invokeHud(void);

  virtual void init(void);

  virtual void update(void);
	virtual void render(void);


  void renderSelection3D(void);


  void gotCmd(int cmd, int arg0, int arg1);

public:
  virtual xSprite * getSprite(std::string wname);

};//xTree