#pragma once


#include "../gamex/gamex.h"
#include "../gamex/xGameExt.h"

#include "../gamex/xGuiGame.h"
#include "../gamex/xFont.h"
#include "../gamex/xFlatRender.h"

#include "../gamex/xSound.h"


#include "invokeGame.h"



#define STATE_MAINMENU 0
#define STATE_GAME 1
#define STATE_LOADMENU 2
#define STATE_SETTINGS 4
#define STATE_PAUSEMENU 5
#define STATE_QUITNOW -999



class invokeMenu : public xGameExt
{
public:
  xFlatRender myFlat;
  xGuiGame myGui;  

  xFont myFont;
//  xTexture myFontSkin;

//  xData myData;

  xSpriteMan mySprite;

  xSoundMan mySnd;

public:
  int mState;

  invokeGame testGame;

public:
  invokeMenu(void);
  virtual  ~invokeMenu(void);

  virtual void init(void);

  virtual void update(void);
	virtual void render(void);

  virtual void gotCmd(int cmd, int arg0, int arg1);
  virtual xSprite * getSprite(std::string wname); 



public:
  virtual void setKeyDown(short k);
  virtual void setKeyUp(short k);
  virtual void setMouseDown(int btn);
  virtual void setMouseUp(int btn);
  virtual void setMousePos(float ax, float ay);

  virtual void setViewPort(int x, int y, int w, int h);


 
};//invokemenu