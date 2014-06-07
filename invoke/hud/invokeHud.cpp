#pragma warning (disable : 4786)

#include "invokeHud.h"

#include "../xInvokeCommon.h"

#include "../../gamex/xKey.h"
#include "../../gamex/xDebug.h"
#include "../../gamex/xGLCommon.h"

#include "../unit/xTower.h"


#define CURMODE_NONE -1
#define CURMODE_DEFAULT 0
#define CURMODE_MOVE 1
#define CURMODE_ATTACK 2
#define CURMODE_BUILD 3



invokeHud::invokeHud(void)
{
   selStart = 0;
   selx = 0;
   sely = 0;
   curMode = 0;

}//ctor


invokeHud::~invokeHud(void)
{

}//dtor


void 
invokeHud::init(void)
{
  xGuiGame::init();
  
  mySelect.init();

  myMini.init();


  mySprite.assetMan = assetMan;
   assetMan->initTexture("button", true,false,false); 
   mySprite.addSpriteXms("button");
   mySprite.addSpriteTex("btn_hold");
   mySprite.addSpriteTex("btn_build");


 xButton * b;
  int bi;
  bi = 0;

  b = addButton("btn1", "", 200, 610-36-36-36,380, 16, 32,32, getSprite("btn_move"), 1);
  b = addButton("btn2", "", 201, 610-36-36, 380, 16, 32,32, getSprite("btn_stop"), 1);
  b = addButton("btn3", "", 202, 610-36, 380, 16, 32,32, getSprite("btn_attack"), 0);
  b = addButton("btn4", "", 203, 610, 380, 16, 32,32, getSprite("btn_hold"), 0);
  b = addButton("btn5", "", 204, 610-36*3, 380+36, 16, 32,32, getSprite("btn_build"), 0);

  //rest of buttons are just for alignment for now
  b = addButton("btn6", "", 1, 610-36*2, 380+36, 16, 32,32, getSprite("button64x64"), 0);
  b = addButton("btn7", "", 1, 610-36, 380+36, 16, 32,32, getSprite("button64x64"), 0);
  b = addButton("btn8", "", 1, 610, 380+36, 16, 32,32, getSprite("button64x64"), 0);
  b = addButton("btn9", "", 1, 610-36*3, 380+36+36, 16, 32,32, getSprite("button64x64"), 0);
  b = addButton("btn10", "", 1, 610-36*2, 380+36+36, 16, 32,32, getSprite("button64x64"), 0);
  b = addButton("btn11", "", 1, 610-36, 380+36+36, 16, 32,32, getSprite("button64x64"), 0);
  b = addButton("btn12", "", 1, 610, 380+36+36, 16, 32,32, getSprite("button64x64"), 0);

 // setBtnLayout("hidden");


//testing -- a button for the minimap (probably not the best way to go about this)
   b = addButton("btn_mini", "minimap", 500, 64+4, 480-64-8, 16, 128,128, getSprite("button64x64"), 1);
   b->drawMode = -1; //make button hidden but usable
   b->btnMode = 1; //make button react when you hold down the button on it
   b->rightCmd = 501;


}//init



void 
invokeHud::update(void)
{

 gamex::cVec3f viewSize;

  viewSize = viewMax - viewMin;

//check single actor the mouse is over
 mySelect.updateOver(parentGame->getCamPtr(), parentGame->mgrid, 
  viewMin.x,  viewMin.z,  viewSize.x,viewSize.z, umx, umy);


  //selection rectangle
  float sx;
  float sy;
  float sw;
  float sh;


  sx = selx < mx ? selx:mx;
  sy = sely < my ? sely:my;

  sw = abs(mx - selx);
  sh = abs(my - sely);



float hudHeight;
hudHeight = 340.0f;


  if (mDownLeft)
  {  

    
    if (selStart <= 0)
    {
      if (my < hudHeight)
      {
        printf("selstart 1 %0.2f \n", my);
        selStart = 1;
        selx = mx;
        sely = my;
      }
    }
    else
    {
      selStart += 1;
       if (my > hudHeight) { my = hudHeight; }
      //glColor3f(0,1,0);
      //drawRect(selx,sely, mx - selx, my - sely);
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

      if (isKeyDown(KEY_SHIFT) == false)
      if (isKeyDown(KEY_CONTROL) == false)    
      { mySelect.resetSelect(); }
      
     
      if (isKeyDown(KEY_CONTROL))
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


//cancel
  if (curMode > 0)  //== CURMODE_BUILD)
  if (mClickRight >= (gameTime-2) )
  {
    curMode = 0;
  }//endif


  if (mClickLeft >= (gameTime-2) )
  {
    //place building
    if (curMode == CURMODE_BUILD)
    {
      //todo -- ax az should be member variables as well (always calc the current grid position of wmx wmz)
     float ax, az;
        ax = floor(wmx / 128.0f) * 128.0f;
        az = floor(wmz / 128.0f) * 128.0f;
      //todo -- test a larger rectangle than the actual building needs (add 1 tile wide border)
      if (parentGame->canBuildRect(ax,az, 64,64) == true) //only if you are allowed to build here
      {
        //todo -- definetly dont do this directly (like it is now)
        //send the parentgame a makebuilding command 
        xActor * a;
          a = new xTower();
          a->team = 1;
          a->pos.set(ax, wmy, az);    
        parentGame->addActor(a);
      }
      else { return; } //todo -- put click test in a function
    }//endif 

    if (curMode == CURMODE_MOVE)
    {
      mySelect.sendMsg(parentGame, MSG_MOVE, wmx, wmz, 0);
    }//move


    if (curMode == CURMODE_ATTACK)
    {
      if (mySelect.overId != 0) //cursor is over something
      { mySelect.sendMsg(parentGame, MSG_ATTACK_TARG, mySelect.overId, 0, 0); }
      else 
      { mySelect.sendMsg(parentGame, MSG_ATTACK_MOVE, wmx, wmz, 0); }

    }//move

  //reset cursor mode
    curMode = 0;
  }//endif


 //todo -- check if mouse is over minimap
  if (my < hudHeight)
  if (mClickRight >= (gameTime-2) )
  {
  
    printf("clickright %d \n ", gameTime);
    //check which unit the cursor is over
    //to figure out what command to send

    //todo -- build formation out of the units

    //(still need to test if they attempt to reach something where another unit is standing)

    if (mySelect.overId != 0) //cursor is over something
    {
      //this is tricky as we dont have a player class yet (maybe a player team variable would be enough hough)
      
      int playerTeam;  //temp value (make it a member or something)
      playerTeam = 1;
      xActor * a;
      a = parentGame->getActor(mySelect.overId);
     // if (a->team != playerTeam)
     // {   mySelect.sendMsg(parentGame, MSG_ATTACK_TARG, mySelect.overId, 0, 0);  }
     // else
     // {   mySelect.sendMsg(parentGame, MSG_PROTECT_TARG, mySelect.overId, 0, 0);   }
      //for testing attack everything you right click on
      mySelect.sendMsg(parentGame, MSG_ATTACK_TARG, mySelect.overId, 0, 0);
    }//endif
    else
    {
      mySelect.sendMsg(parentGame, MSG_MOVE, wmx, wmz, 0);
    }

  }//endif





   if (selStart > 0)
   if (sw > 16 || sh > 16)
   {
    mySelect.selectOver(parentGame->getCamPtr(), parentGame->mgrid, 
    viewMin.x,  viewMin.z,  viewSize.x,  viewSize.z, sx, sy, sw, sh);
   }//endif


  if ((gameTime % 30) == 0)  { myMini.updateImage(parentGame->getWorldPtr()); }

}///update


void 
invokeHud::renderSelection3D(void)
{


 if (curMode == CURMODE_BUILD)
  {
   xMdx3 * mesh;
   mesh = assetMan->getMesh("wraith_foepulet"); 

   glColor3f(1,0,0);  


   glPushMatrix();
    float ax, az;
    ax = floor(wmx / 128.0f) * 128.0f;
    az = floor(wmz / 128.0f) * 128.0f;

      //todo -- store size of building to be built
    if (parentGame->canBuildRect(ax,az, 64,64) == false)
    {  glColor3f(1,0,0);     }
    else {   glColor3f(0,1,0);  } 


    glTranslatef(ax, wmy, az);
    mesh->render();
   glPopMatrix(); 

  }//endif



 mySelect.debRender(parentGame);

}//rendersel




void 
invokeHud::render(void)
{
 
  

  glColor3f(0,1,0);
   if (selStart > 0) { drawRect(selx,sely, mx - selx, my - sely); }

  
  xFrame * f;

  myFlat.addFrame(64+4, 480-64-8, 300, 128,128, myMini.skin.handle);

  unsigned int hudBack;
  hudBack = assetMan->getTexHandle("hudback");
  f = myFlat.addFrame(0+320, 480-140 + 70, 1, 640,140, hudBack);
  f->alpha = 0.5f;

  //drawStr(0, 16, 16, 16, 32, "Testing ");

  if (curMode == CURMODE_BUILD)
  {
    drawStr(0, 12, 16, 16, 32, "Left click: place building ");
    drawStr(0, 12, 16, 16+16, 32, "Right click: cancel ");
  }

  if (curMode == CURMODE_ATTACK)
  {
    drawStr(0, 12, 16, 16, 32, "Left click: attack location or unit ");
    drawStr(0, 12, 16, 16+16, 32, "Right click: cancel ");
  }

  if (curMode == CURMODE_MOVE)
  {
    drawStr(0, 12, 16, 16, 32, "Left click: move or retreat to location ");
    drawStr(0, 12, 16, 16+16, 32, "Right click: cancel ");
  }

}//render



xSprite * 
invokeHud::getSprite(std::string wname) 
{
  return mySprite.getSprite(wname);
}//getsprite




void 
invokeHud::gotCmd(int cmd, int arg0, int arg1)
{ 

 if (parentGame == 0) { return; } //todo -- warning 

 // printf("invokehud gotcmd %d   %d %d \n", cmd, arg0, arg1);


   if (cmd == 200 ) //move
   {
      curMode = CURMODE_MOVE;
      return;
   }

   if (cmd == 202 ) //attack
   {
      curMode = CURMODE_ATTACK;
      return;
   }


   if (cmd == 201) //stop
   {
     mySelect.sendMsg(parentGame, MSG_STOP, 0, 0, 0);
     return;
   }//endif


   if (cmd == 203) //hold position
   {
     mySelect.sendMsg(parentGame, MSG_HOLD, 0, 0, 0);
     return;
   }//endif

   if (cmd == 204) //make building (debug)
   {
     curMode = CURMODE_BUILD;
     return;
   }//endif


   if (cmd == 501) //right click on minimap
   {
    //todo -- map is 128x128  and a cell is 128x128
    //but this will change so a better handling for this is needed
    
      mySelect.sendMsg(parentGame, MSG_MOVE, arg0*128, arg1*128, 0);
   }//endif
  


  //minimap movement 
   if (cmd == 500) {  parentGame->gotCmd(cmd, arg0, arg1); }
  
  //todo -- send relevant commands to the parentGame (e.g. pause game)

  
  

}//gotcmd













