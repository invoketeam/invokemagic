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
  xRender myRender;
  xCursor myCursor;
  
  
  xTileMap myMap;
  xColMesh myCol;
  xDrawMesh myDraw;

  xMdx3 debmesh;

  xMiniMap myMini;
  
public:
  invokeGame(void);
  virtual ~invokeGame(void);

  virtual void init(void);


  virtual void update(void);
	virtual void render(void);



  virtual xMdx3 * getMdx(std::string wname);
  virtual xTexture * getTex(std::string wname); 
  virtual unsigned int getSkin(std::string wname);
  virtual xSprite * getSprite(std::string wname);



  virtual float getHeight(float wx, float wz);


  virtual xCam * getCamPtr(void);


};//invokemenu



