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

class xGame
{
public:
	xMultiGrid * mgrid;

public:
  float mx, my;     //mouse pos 
  bool mDownLeft;
  bool mDownRight;

  bool bKeyDown[512];

  bool mClickLeft; //click needs to be manually reset at end of update
  bool mClickRight;
  int bKeyPress[512]; //needs gameTime to be udated every frame for keypress to work
  //these are hacks so key detection doesn't need to be handled async

public:
  int gameTime;

  //notes:
  //an int is good for 2 147 483 648 frames (at 60 fps)
  //one year is approx 1 892 160 000 frames
  //if a match lasts that long we advise declaring a draw


public:
	xGame();
	virtual ~xGame() {}

  virtual void update() { /*gamelogic here*/ gameTime += 1; mClickLeft=false; mClickRight=false; }
	virtual void render() {}



public:
  virtual void keyDown(short k);
  virtual void keyUp(short k);
  virtual void mouseDown(int btn); //0 left    1 right
  virtual void mouseUp(int btn);

  //needs to be between 0.0 and 1.0 
  virtual void setMousePos(float ax, float ay);


  virtual bool isKeyDown(int k);
  virtual bool isKeyPress(int k);



public:
	virtual void addActor(xActor * a) {}
  virtual xActor * getActor(int id) { return 0; }



public:

  virtual xMdx3 * getMdx(std::string wname) { return 0; }
  virtual xTexture * getTex(std::string wname) { return 0; }  
  virtual unsigned int getSkin(std::string wname) { return 0; }
  virtual xSprite * getSprite(std::string wname) { return 0; }

public:

	virtual float getRand() { return 0.0f; }
	virtual float getRand2() { return 0.0f; }


public:
//  virtual void drawRect(float ax, float ay, float aw, float ah) {}
//  virtual void drawImage(float ax, float ay, float aw, float ah, std::string wname) {}


  virtual xCam * getCamPtr(void) { return 0;}

public:
  //rem -- update xtilemap AND xpathfind (!!!)
  virtual void setZoneType(float wx, float wz, int zt) {}
  virtual void setZoneTypeRect(float wx, float wz, float ww, float wh, int zt) {}


  virtual float getHeight(float wx, float wz) { return 0.0f; }
 


};//xgame
