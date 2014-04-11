#pragma once



#include "../gamex/gamex.h"

#include "../gamex/xGuiGame.h"
#include "../gamex/xGameExt.h"
#include "../gamex/xData.h"


class invokeGame : public xGameExt
{
public:
  xData myData;

//  xSpriteMan myMan;

  xCam myCam;

  xRender myRender;
  xCursor myCursor;

  xTileMap myMap;
  xColMesh myCol;
  xDrawMesh myDraw;



  int curId;

  xMdx3 debmesh;


  xGuiGame myGui;


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

/*
public:
//todo -- seperate class to store game data (?) (or at least temporary game data)
 // xStore <xMdx3> storeMesh; //for now all meshes will be stored here
//  xStore <xTexture> storeSkin;

private:
 // void addMdx(std::string fname, std::string altName ="0");
 // void addSkin(std::string fname, std::string altName ="0", bool mip = true);
*/

};//invokemenu



