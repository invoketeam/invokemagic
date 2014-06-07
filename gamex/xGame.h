#pragma once



#include <string>
#include "xMath.h"


class xActor;
class xWorld;
class xMultiGrid;
class xPathFind;
class xMdx3;
class xTexture;
class xUnitDef;
class xPlayer;
class xSprite;
class xCam;
class xFrame;
class xBigPart;

class xAssetMan;

#define XGAME_MAXKEY 512


class xGame
{
public:
	xMultiGrid * mgrid;

public:
  xAssetMan * assetMan;

public:
  float umx, umy;   //universal mouse pos ( 0,0 - 1,1)
  float mx, my;     //mouse pos (0,0 - 640,480)
  float wmx, wmy, wmz; //world mouse pos
  bool mDownLeft;
  bool mDownRight;

  bool bKeyDown[XGAME_MAXKEY];

  int mClickLeft; //stores the gametime of the click
  int mClickRight;
  int bKeyPress[XGAME_MAXKEY]; //needs gameTime to be udated every frame for keypress to work
  //these are hacks so key detection doesn't need to be handled async

public:
  int view_x, view_y, view_width, view_height; //viewport (screen size)


public:
  int gameTime;

  //notes:
  //an int is good for 2 147 483 648 frames (at 60 fps)
  //one year is approx 1 892 160 000 frames
  //if a match lasts that long we advise declaring a draw

public:
  int gameState;  

public:
	xGame();
	virtual ~xGame();

  virtual void clear(void) {}

  virtual void init(void) {}

  virtual void update(void) { /*gamelogic here*/ gameTime += 1;}
	virtual void render(void) {}



public:
  virtual void setKeyDown(short k);
  virtual void setKeyUp(short k);
  virtual void setMouseDown(int btn); //0 left    1 right
  virtual void setMouseUp(int btn);

  //needs to be between 0.0 and 1.0 
  virtual void setMousePos(float ax, float ay);


  virtual bool isKeyDown(int k);
  virtual bool isKeyPress(int k);

  virtual void setViewPort(int x, int y, int w, int h);

public:
	virtual void addActor(xActor * a) {}
  virtual xActor * getActor(int id) { return 0; }

public:

	virtual float getRand(void) { return 0.0f; }
	virtual float getRand2(void) { return 0.0f; }


public:
  virtual void addNameZone(xActor * a, std::string wname) { }
  virtual xActor * getNameZone(std::string wname) { return 0;}
  virtual void remNameZone(std::string wname) { }
  virtual void clearNameZone(void) { }



public:
  virtual void gotCmd(int cmd, int arg0, int arg1) {}
  virtual void copyControl(xGame * game);


public:

  virtual xCam * getCamPtr(void) { return 0;}
  virtual xWorld * getWorldPtr(void) { return 0; }

//todo -- figure out a better way to handle fonts and drawing
  virtual void drawStr(int font, float size, float cx, float cy, float cz, const char* str, ...) {}


public:
  //rem -- update xtilemap AND xpathfind (!!!)
  virtual void setZoneType(float wx, float wz, int zt) {}
  virtual void setZoneTypeRect(float wx, float wz, float ww, float wh, int zt) {}

  virtual bool canBuildRect(float wx, float wz, float ww, float wh) { return true; }

  virtual float getHeight(float wx, float wz) { return 0.0f; }


public:
  virtual int playSound(std::string wname) { return 0; }


public:
  virtual xBigPart * addPart(std::string wname) { return 0; }
  virtual void getDecal(xMdx3 * m, float x0, float y0, float w0, float h0) { }

public:

  virtual xMdx3 * getMdx(std::string wname) { return 0; }
  virtual xTexture * getTex(std::string wname) { return 0; }  
  virtual unsigned int getSkin(std::string wname) { return 0; }
  virtual xSprite * getSprite(std::string wname) { return 0; }


};//xgame









