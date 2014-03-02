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

  memset(bKeyDown, 0, 512);

  mDownLeft = false;
  mDownRight = false;


  memset(bKeyPress, 0, 512);
  mClickLeft = false;
  mClickRight = false;


  gameTime = 0;

}//ctor

bool 
xGame::isKeyPress(int k)
{
  if (k < 0) { return false; }  if (k > 511) { return false; }
  return (bKeyPress[k] == gameTime);
}//iskeypress



bool 
xGame::isKeyDown(int k)
{

  if (k < 0) { return false; }  if (k > 511) { return false; }
  return bKeyDown[k];
}//iskeydown


void 
xGame::keyDown(short k)
{
 if (k < 0) { return ; } if (k > 511) { return ; }
 
 //hack for handling keypress 
 if (bKeyDown[k] == false) { bKeyPress[k] = gameTime; }

 bKeyDown[k] = true;
}//kdown
  
void 
xGame::keyUp(short k)
{ 
 if (k < 0) { return ; } if (k > 511) { return ; }
 bKeyDown[k] = false;
}//kup


void 
xGame::mouseDown(int btn)
{
  if (btn == 0) { mDownLeft = true; mClickLeft = true; }
  else if (btn == 1) { mDownRight = true; mClickRight = true; }
}//mdown

void 
xGame::mouseUp(int btn)
{
  if (btn == 0) { mDownLeft = false; }
  else if (btn == 1) { mDownRight = false; }
}//mup



void xGame::setMousePos(float ax, float ay)
{
  mx = ax;
  my = ay;

}//setmousepos

