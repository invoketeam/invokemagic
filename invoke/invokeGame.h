#pragma once



#include "../gamex/gamex.h"

#include "../gamex/xGuiGame.h"
#include "../gamex/xGameExt.h"
#include "../gamex/xHeightMap.h"

#include "../gamex/xShadowTex.h"


#include "../gamex/xBigPart.h"

#include "hud/invokeHud.h"



#define NUM_HUDBUTTON 12


class invokeGame : public xGameExt
{
public:
  invokeHud myHud;

  xFont myFont;

  xRender myRender;

  xCursor myCursor;

  xSpriteMan mySprite;
  
  xBigPartSys myPart;

public:
  xCam myCam;
  gamex::cVec3f camPos;

  xViewBox viewBox;
  gamex::cVec3f lastPos;


public:
  int shadowMode; //0 no   1 simple   2 projection  
  xShadowTex shadTex;
  float lightProj[16];
  float lightView[16];

  xTexture tileSkin;
  xHeightMap myHeight;

  xMdx3 waterDeb;

  xMdx3 debmesh;

public:
  invokeGame(void);
  virtual ~invokeGame(void);

  virtual void init(void);


  virtual void update(void);
	virtual void render(void);

  virtual void makeShadow(void);
  virtual void drawShadow(void);


public:

  virtual float getHeight(float wx, float wz);


  virtual xCam * getCamPtr(void);

public:
  virtual void gotCmd(int cmd, int arg0, int arg1);

public:
  virtual xBigPart * addPart(std::string wname);
  virtual void getDecal(xMdx3 * m, float x0, float y0, float w0, float h0);


public:
  virtual xMdx3 * getMdx(std::string wname);
  virtual xTexture * getTex(std::string wname); 
  virtual unsigned int getSkin(std::string wname);
  virtual xSprite * getSprite(std::string wname);



};//invokemenu



