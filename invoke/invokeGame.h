#pragma once



#include "../gamex/gamex.h"

#include "../gamex/xGuiGame.h"
#include "../gamex/xGameExt.h"
#include "../gamex/xData.h"

#include "xMiniMap.h"

class invokeGame : public xGameExt
{
public:
  xData myData;
  xGuiGame myGui;

  
  xCam myCam;
  gamex::cVec3f camPos;


  xRender myRender;
  xCursor myCursor;
  
  
  xTexture tileSkin;
  xTileMap myMap;
  xColMesh myCol;
  xDrawMesh myDraw;

  xMdx3 debmesh;


  xTexture waterSkin;
  xMdx3 waterDeb;


  xMiniMap myMini;

  xFont myFont;
  xTexture myFontSkin;

public:
  invokeGame(void);
  virtual ~invokeGame(void);

  virtual void init(void);


  virtual void update(void);
	virtual void render(void);

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



