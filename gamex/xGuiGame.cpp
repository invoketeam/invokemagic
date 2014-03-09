#pragma warning (disable : 4786)

#include "xGuiGame.h"







void 
xGuiCursor::update(void)
{

/*

 cx = game.mx;
    cy = game.my;
    
    color = game.mbutton ? 0 : 0xFF;
    
    
    if (game.mbutton)
    if (worka == 0) { worka = 1; }   
    
    if (game.mbutton == false)
    if (worka == 1) { worka = 2; }
    if (worka == 2 && target == -1) { worka = 0; }
    
    if (worka == 0)
    { checkCol(game.mgrid); }
    
    //trace("worka ", worka);
    
    var a:xActor;
    a = game.getActor(target);
    if (a == null) { return; }
    if (a.dead) { target = -1; return; }
    if (a.visible == false) { target = -1; return; }
    if (a.overlap(this) == false) { a.color = 0; target = -1; return; }
    
    a.worka = game.gameTime;
    if (game.mbutton) { a.workc = game.gameTime; }
    
    if (worka == 2)
    {
      //todo -- need args as well -- and i alread used worka workb workc argh
     
      //trace("got command ", a.id, a.wstr2); 
      game.gotCmd(a.wstr2, 0, 0);
      worka = 0;
    }//endif

*/
}//update


bool 
xGuiCursor::handCol(xActor * a)
{
/*
  if (a.visible == false) { return true; }
       
    //todo -- use the one with the highest z
    target = a.id;
    a.color = 0x0000FF;
    
    return false;

*/


 return false; 
}//handcol







xButton::xButton(void) 
{

}//ctor

void 
xButton::init(void)
{

}//init

void 
xButton::update(void)
{

}//update

void 
xButton::trigger(std::string &str)
{

}//trigger





xGuiGame::xGuiGame(void) 
{
  parentGame = 0;
}//ctor


xGuiGame::~xGuiGame(void) 
{

}//dtor


void 
xGuiGame::childUpdate(xGame * parent)
{
/*
  parentGame = game;
    //rem: these are just pointers  so its safe to do this every frame
     vecKey = game.vecKey;
     vecPress = game.vecPress;
     gameTime = game.gameTime;
     mbutton = game.mbutton;    
     mx = game.mx;
     my = game.my;
     wmx = game.wmx;
     wmy = game.wmy;
     wmz = game.wmz;
     
     world.update();
     cursor.update();
*/
}//childupdate


void 
xGuiGame::childRender(xGame * parent)
{

}//childrender


