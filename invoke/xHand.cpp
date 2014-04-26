#pragma warning (disable : 4786)

#include "xHand.h"

#include "../gamex/xDebug.h"

#include "xInvokeCommon.h"



xHand::xHand(void)
{
   selStart = 0;
   selx = 0;
   sely = 0;
   viewBoxPtr = 0;
}//ctor


void 
xHand::init(void)
{
  mySelect.init();

}//init



void 
xHand::update(void)
{

  float mx;
  float my;
  float wmx;
  float wmz;

  mx = game->mx;
  my = game->my;

  wmx = game->wmx;
  wmz = game->wmz;


//check single actor the mouse is over
 mySelect.updateOver(game->getCamPtr(), game->mgrid, 
  viewBoxPtr->min.x,  viewBoxPtr->min.z,  viewBoxPtr->size.x,viewBoxPtr->size.z, game->umx, game->umy);


  //selection rectangle
  float sx;
  float sy;
  float sw;
  float sh;


  sx = selx < mx ? selx:mx;
  sy = sely < my ? sely:my;

  sw = abs(mx - selx);
  sh = abs(my - sely);







  if (game->mDownLeft)
  {  
    if (selStart <= 0)
    {
      selStart = 1;
      selx = mx;
      sely = my;
    }
    else
    {
      selStart += 1;
      glColor3f(0,1,0);
      drawRect(selx,sely, mx-selx, my-sely);
    }
  }
  else 
  { 
    if (selStart >= 1)
    {
      //shift to append to selection
      //control to remove selected from selection

      //hmm.. control is usually used to make groups..
      //so this will need some tweaking later

      if (game->isKeyDown(KEY_SHIFT) == false)
      if (game->isKeyDown(KEY_CONTROL) == false)    
      { mySelect.resetSelect(); }
      
     
      if (game->isKeyDown(KEY_CONTROL))
      { 

        if (sw > 16 || sh > 16)   {  mySelect.eraseOver(); }
        mySelect.eraseSingle();
        
      }
      else
      {
        
        if (sw > 16 || sh > 16)
        {
          mySelect.appendOverToSelect();
        }

        mySelect.appendSingleToSelect();

      }//endif2

    }//endif

    selx = mx;
    sely = my;  
    selStart = 0; 
  }//mdownleft


  //printf("clickright  %d  %d  \n ", mClickRight, gameTime);
  //because of debugging upcursor is called in render
  //so right click test will need to be altered later (aka fixed)


  if (game->mClickRight >= (game->gameTime-2) )
  {
  
    printf("clickright %d \n ", game->gameTime);
    //check which unit the cursor is over
    //to figure out what command to send

    //todo -- build formation out of the units

    //(still need to test if they attempt to reach something where another unit is standing)

    mySelect.sendMsg(game, MSG_MOVE, wmx, wmz, 0);

  }//endif


 if (selStart > 0)
 if (sw > 16 || sh > 16)
 {
  mySelect.selectOver(game->getCamPtr(), game->mgrid, 
  viewBoxPtr->min.x,  viewBoxPtr->min.z,  viewBoxPtr->max.x-viewBoxPtr->min.x,viewBoxPtr->max.z-viewBoxPtr->min.z,
  sx, sy, sw, sh);
 }//endif


}//update





