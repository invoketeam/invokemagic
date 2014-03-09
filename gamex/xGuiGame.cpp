#pragma warning (disable : 4786)


#include "xGuiGame.h"

#include "xGLCommon.h"




//meant for debug only (so will be probably stay here forever)
static void drawRect(float ax, float ay, float aw, float ah)
  {
   glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(ax, ay); //0

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(ax, ay+ah); //1

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(ax+aw, ay); //2

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(ax+aw, ay+ah);//3
   glEnd();
    
  }//drawrect





xGuiCursor::xGuiCursor(void)
{
  xrad = 4; yrad = 4;
  target = -1;
}//ctor



void 
xGuiCursor::update(void)
{
  xActor * a;


  pos.x = game->mx;
  pos.y = game->my;

  color = game->mDownLeft ? 0 : 0xFF;

  if (game->mDownLeft)
  if (worka == 0) { worka = 1; }


  if (game->mDownLeft == false)
  if (worka == 1) { worka = 2; }
  if (worka == 2 && target == -1) { worka = 0; }
  
  if (worka == 0)
  { checkColXY(game->mgrid); }
  
  a = game->getActor(target);
  if (a == 0) { target = -1; return;}
  if (a->dead) { target = -1; return;}
  if (a->visible == false) {  target = -1; return; }
  if (a->overlapXY(this) == false) {  target = -1; return; }


  a->worka = game->gameTime;
  if (game->mDownLeft) { a->workc = game->gameTime; }
    
  if (worka == 2)
  {
      game->gotCmd(a->wstr2, 0, 0);
      worka = 0;
  }//endif

}//update


bool 
xGuiCursor::handCol(xActor * a)
{
  if (a->visible == false) { return true;}

  //todo -- choose the one with the highest z

   target = a->id;
   a->worka = game->gameTime;

 return false; 
}//handcol


void 
xGuiCursor::render(void)
{
  glColor3f(1,1,1);
  drawRect(pos.x-xrad,pos.y-yrad, xrad+xrad, yrad+yrad);

}//render





xButton::xButton(void) 
{
  xrad = 32;
  yrad = 16;
}//ctor

void 
xButton::init(void)
{

}//init

void 
xButton::update(void)
{
  putInGridXY(game->mgrid);
}//update

void 
xButton::trigger(std::string &str)
{

}//trigger

void 
xButton::render(void)
{

  if (worka == game->gameTime)
  {  glColor3f(1,0,0); } else { glColor3f(0,0,0); }

  drawRect(pos.x-xrad,pos.y-yrad, xrad+xrad, yrad+yrad);
}//render





xGuiGame::xGuiGame(void) 
{
  parentGame = 0;
}//ctor


xGuiGame::~xGuiGame(void) 
{

}//dtor


void 
xGuiGame::init(void)
{
  resetWorld(640, 480);
  cursor.game = this;
  cursor.init();
}//init



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

  copyControl(parent);
  parentGame = parent;  

  myWorld.update();
  cursor.update();

}//childupdate


void 
xGuiGame::childRender(xGame * parent)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
       glOrtho(0,640,480,0,-1,1); 

	  glMatrixMode(GL_MODELVIEW);
	  glLoadIdentity();



       myWorld.render();
       cursor.render();



}//childrender





xActor *  
xGuiGame::addButton(std::string wname, float ax, float ay, float az, std::string str, std::string cmd, int tag)
{
  xActor * a;
  a = new xButton();
  a->pos.set(ax,ay,az);
  a->wstr = str;
  a->wstr2 = cmd;
  a->tag = tag;

  addActor(a);
    
  addNameZone(a, wname);


  return a;
}//addbutton




void xGuiGame::removeButton(std::string wname)
{
  xActor * a;
  a = getNameZone(wname);
  if (a == 0) { return; }

  a->dead = true;

  remNameZone(wname);
}//removebutton



void xGuiGame::hideButton(std::string wname)
{
  xActor * a;

  a = getNameZone(wname);
  if (a == 0) { return; }
  a->visible = false;
}//hidebutton




void xGuiGame::showButton(std::string wname)
{
  xActor * a;

  a = getNameZone(wname);
  if (a == 0) { return; }
  a->visible = true;
}//showbutton










