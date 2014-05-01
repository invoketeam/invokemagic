#pragma once



#include "../gamex/gamex.h"

#include "../gamex/xGuiGame.h"
#include "../gamex/xGameExt.h"
#include "../gamex/xData.h"
#include "../gamex/xHeightMap.h"

#include "../gamex/xShadowTex.h"


#include "xMiniMap.h"

#include "xSelect.h"
#include "xHand.h"



class invokeGame : public xGameExt
{
public:
  xData myData;

  xGuiGame myGui;

  xFont myFont;
  xTexture myFontSkin;


  xRender myRender;
  xCursor myCursor;
  
public:
  xCam myCam;
  gamex::cVec3f camPos;

  xViewBox viewBox;
  gamex::cVec3f lastPos;


  xHand myHand; //cursor used for commands 
  //xSelect testSelect;

public:
  xShadowTex shadTex;
  float lightProj[16];
  float lightView[16];

  xTexture tileSkin;
  xHeightMap myHeight;

  xMiniMap myMini;




  xTexture waterSkin;
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
  virtual void upCursor(void);




public:

  virtual float getHeight(float wx, float wz);


  virtual xCam * getCamPtr(void);

public:
  virtual void gotCmd(int cmd, int arg0, int arg1);


public:
  virtual xMdx3 * getMdx(std::string wname);
  virtual xTexture * getTex(std::string wname); 
  virtual unsigned int getSkin(std::string wname);
  virtual xSprite * getSprite(std::string wname);



};//invokemenu



