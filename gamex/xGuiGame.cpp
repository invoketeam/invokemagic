#pragma warning (disable : 4786)


#include "xGuiGame.h"

#include "xGLCommon.h"



/*
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
*/




xGuiCursor::xGuiCursor(void)
{
  xrad = 4; yrad = 4;
  target = -1;
  curSpr = 0;
}//ctor





void 
xGuiCursor::update(void)
{
  xActor * a;


  pos.x = game->mx;
  pos.y = game->my;

  color = game->mDownLeft ? 0 : 0xFF;

  if (game->mDownLeft)
  {if (worka == 0) { worka = 1; } }


  if (game->mDownLeft == false)
  {if (worka == 1) { worka = 2; } }
  if (worka == 2 && target == -1) { worka = 0; }
  
  if (worka == 0)
  {
    workz = -9999999; //reset z (choose button with highest z -- aka the one on top)
    checkColXY(game->mgrid); 
  }//endif
  
  a = game->getActor(target);
  if (a == 0) { target = -1; return;}
  if (a->dead) { target = -1; return;}
  if (a->visible == false) {  target = -1; return; }
  if (a->overlapXY(this) == false) {  target = -1; return; }


  a->worka = game->gameTime;
  if (game->mDownLeft) { a->workc = game->gameTime; }
    
  if (worka == 2)
  {
    xButton * b;
     // b = dynamic_cast<xButton*>(a);
      b = (xButton *) a; //todo -- not safe but cannot think of a better solution for now
      if (b != 0)
      { game->gotCmd(b->cmd, b->arg0, b->arg1); }
      //todo -- allow non buttons to have command?
      // game->gotCmd(a->reload, 0, 0);
      worka = 0;
  }//endif

}//update


bool 
xGuiCursor::handCol(xActor * a)
{
  if (a->visible == false) { return true;}

  //todo -- choose the one with the highest z

  if (a->spectype != 100) { return true; } //only consider buttons

  if (a->pos.z < workz) { return true; }
     workz = a->pos.z;
     
  target = a->id;
   a->worka = game->gameTime;

 return false; 
}//handcol

/*
void 
xGuiCursor::render(void)
{
  glColor3f(1,1,1);
  drawRect(pos.x-xrad,pos.y-yrad, xrad+xrad, yrad+yrad);

}//render
*/

void 
xGuiCursor::frameRender(xFlatRender * render)
{

  if (curSpr != 0)   {  render->addSprite2(curSpr, pos.x, pos.y, pos.z, 16.0f, 16.0f);  }

}//framerender







xButton::xButton(void) 
{
  xrad = 32;
  yrad = 16;
  curSpr = 0;
  arg0 = 0; arg1 = 0;
  cmd = 0;
  spectype = 100;
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

/*
void 
xButton::render(void)
{

  if (worka == game->gameTime)
  {  glColor3f(1,0,0); } else { glColor3f(0,0,0); }

  drawRect(pos.x-xrad,pos.y-yrad, xrad+xrad, yrad+yrad);
}//render

*/

void 
xButton::frameRender(xFlatRender * render)
{

  if (workc == game->gameTime)
  {  if (curSpr != 0)  {  render->addSprite2(curSpr, pos.x+3, pos.y+3, pos.z,xrad+xrad,yrad+yrad);  }  }
  else if (worka == game->gameTime)
  {  if (curSpr != 0)  {  render->addSprite2(curSpr, pos.x-1, pos.y-1, pos.z,xrad+xrad,yrad+yrad);  }  }
  else
  {  if (curSpr != 0)  {  render->addSprite2(curSpr, pos.x, pos.y, pos.z,xrad+xrad,yrad+yrad);  } }
 
  game->drawStr(1, 16, pos.x, pos.y-8, pos.z+2, this->wstr.c_str());
}//framerender








xGuiGame::xGuiGame(void) 
{
  parentGame = 0;
  pfont = 0;
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

  myFlat.init();

}//init

void 
xGuiGame::gotCmd(int cmd, int arg0, int arg1)
{
  if (parentGame == 0) { return; } //todo -- warning
  
  parentGame->gotCmd(cmd, arg0, arg1);

}//gotcmd


void 
xGuiGame::childUpdate(xGame * parent)
{

  copyControl(parent);
  parentGame = parent;  

  myWorld.update();
  cursor.update();

}//childupdate


void 
xGuiGame::childRender(xGame * parent)
{
    
   //  myWorld.render();
   // cursor.render();

   glMatrixMode(GL_PROJECTION);  	glLoadIdentity();       glOrtho(0,640,480,0,-3000,3000); 
	 glMatrixMode(GL_MODELVIEW);	  glLoadIdentity();

      myFlat.resetFrame();


      myWorld.frameRender(&myFlat);
      cursor.frameRender(&myFlat);    

     
      myFlat.render(true);

    glDisable(GL_TEXTURE_2D);

}//childrender





void 
xGuiGame::drawStr(int font, float size, float cx, float cy, float cz, const char* str, ...)
{
    if (size == 0.0f) { return; }
    if (pfont == 0) { return; } 

	  static char buffer[1024];

	  va_list params;
	  va_start(params, str);
		  _vsnprintf(buffer, sizeof(buffer), str, params);
	  va_end(params);

    size = size / pfont->fontsize;

    static std::string tmp;
    tmp = buffer;
    if (font == 1) { cx -= (pfont->getWidth(tmp)) *0.5f ; } //experimental -- align to middle
  
    pfont->writeStrFrame(&myFlat, pfont->handle, cx, cy, cz, tmp,  size );
}//drawstr



//xButton *  
//xGuiGame::addButton(std::string wname, float ax, float ay, float az, std::string str, std::string cmd, int tag)

xButton * 
xGuiGame::addButton(std::string wname, std::string disp, int cmd, float ax, float ay, float az, float aw, float ah, xSprite * spr,  int tag)
{
  xButton * a;
  a = new xButton();
  a->xrad = aw * 0.5f;
  a->yrad = ah * 0.5f;
  a->pos.set(ax+a->xrad,ay+a->yrad,az);
  a->wstr = disp;
  //a->wstr2 = cmd; //not using string for commands anymore
  //a->reload = cmd;
  a->tag = tag;
  
  a->arg0 = 0;
  a->arg1 = 0;
  a->cmd = cmd;
  
  //todo -- draw something if sprite is null (default sprite in gui?)
  //if (spr == 0) { spr = defSpritePtr; }
  a->curSpr = spr;
  
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










