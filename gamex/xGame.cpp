#pragma warning (disable : 4786)

#include "xGame.h"
#include "xActor.h"
#include "xWorld.h"
#include "xMultiGrid.h"
#include "xTexture.h"
#include "xMdx3.h"


xGame::xGame()
{
	mgrid = 0;

  mx = 0.0f;
  my = 0.0f;

  umx = 0.0f;
  umy = 0.0f;

  wmx = 0.0f;
  wmy = 0.0f;
  wmz = 0.0f;


  memset(bKeyDown, 0, 512);

  mDownLeft = false;
  mDownRight = false;


  memset(bKeyPress, 0, 512);
  mClickLeft = 0;
  mClickRight = 0;


  gameTime = 1; //rem -- gametime should always reset to 1

  gameState = -1;
}//ctor


xGame::~xGame() 
{
 clear(); 
 if (mgrid != 0) { delete mgrid; mgrid = 0;}
}//dtor


bool 
xGame::isKeyPress(int k)
{
  if (k < 0) { return false; }  if (k >= XGAME_MAXKEY) { return false; }
  return (bKeyPress[k] == gameTime);
}//iskeypress



bool 
xGame::isKeyDown(int k)
{

  if (k < 0) { return false; }  if (k >= XGAME_MAXKEY) { return false; }
  return bKeyDown[k];
}//iskeydown


void 
xGame::setKeyDown(short k)
{
 if (k < 0) { return ; } if (k >= XGAME_MAXKEY) { return ; }
 
 //hack for handling keypress 
 if (bKeyDown[k] == false) { bKeyPress[k] = gameTime; }

 bKeyDown[k] = true;
}//kdown
  
void 
xGame::setKeyUp(short k)
{ 
 if (k < 0) { return ; } if (k >= XGAME_MAXKEY) { return ; }
 bKeyDown[k] = false;
}//kup


void 
xGame::setMouseDown(int btn)
{
  if (btn == 0) { mDownLeft = true; mClickLeft = gameTime; }
  else if (btn == 1) { mDownRight = true; mClickRight = gameTime; }
}//mdown

void 
xGame::setMouseUp(int btn)
{
  if (btn == 0) { mDownLeft = false; }
  else if (btn == 1) { mDownRight = false; }
}//mup



void xGame::setMousePos(float ax, float ay)
{
  umx = ax;
  umy = ay;
   
  mx = umx * 640.0f;
  my = umy * 480.0f;

}//setmousepos



void 
xGame::copyControl(xGame * game)
{
  gameTime = game->gameTime; //function only meant to be used in guis

  mDownLeft = game->mDownLeft;
  mDownRight = game->mDownRight;
  mClickLeft = game->mClickLeft;
  mClickRight = game->mClickRight;

  mx = game->mx;
  my = game->my;

  wmx = game->wmx;
  wmy = game->wmy;
  wmz = game->wmz;


  memcpy(bKeyDown, game->bKeyDown, XGAME_MAXKEY);
  memcpy(bKeyPress, game->bKeyPress, XGAME_MAXKEY*4);
  

}//copycontrol









