#pragma warning (disable : 4786)


#include "xGuiGame.h"

#include "xGLCommon.h"




xGuiCursor::xGuiCursor(void)
{
  xrad = 2; yrad = 2;
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
  { if (worka == 1) { worka = 2; } }

  if (game->mClickRight == game->gameTime) { worka = 3;}

  if (worka == 2 && target == -1) { worka = 0; }
  if (worka == 3 && target == -1) { worka = 0; }

  

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
    
  xButton * b;
  b = (xButton *) a; //todo -- not safe but cannot think of a better solution for now


  if (game->mDownLeft)
  if (b->btnMode == 1) //instant mode
  {
    game->gotCmd(b->cmd, pos.x-b->pos.x + b->xrad, pos.y-b->pos.y + b->yrad);
  }//endif


  if (worka == 2)
  {
     // b = dynamic_cast<xButton*>(a);

      //if (b != 0) { game->gotCmd(b->cmd, b->arg0, b->arg1); }

      //for now give back the cursor position relative to the button pos
      //(based on the top left corner of the button)

      if (b != 0)
      { 
       if (b->btnMode == 1)
       {
         game->gotCmd(b->cmd, pos.x-b->pos.x + b->xrad, pos.y-b->pos.y + b->yrad); 
       }
       else
       {
         game->gotCmd(b->cmd, b->arg0, b->arg1);
       }

      }//endif


      //todo -- allow non buttons to have command?
      // game->gotCmd(a->reload, 0, 0);

      worka = 0;
  }//endif


  if (worka == 3)
  {
    
    if (b != 0) { game->gotCmd(b->rightCmd, pos.x-b->pos.x + b->xrad, pos.y-b->pos.y + b->yrad); }
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



void 
xGuiCursor::frameRender(xFlatRender * render)
{

  if (curSpr != 0)   {  render->addSprite2(curSpr, pos.x+7.0f, pos.y+7.0f, pos.z, 16.0f, 16.0f);  }

}//framerender





xButton::xButton(void) 
{
  xrad = 32;
  yrad = 16;
  curSpr = 0;
  arg0 = 0; arg1 = 0;
  cmd = 0;
  rightCmd = 0;
  spectype = 100;
  drawMode = 0;
  btnMode = 0;

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
xButton::frameRender(xFlatRender * render)
{
  if (drawMode == -1) { return; } //invisible button (because   bool visible  is used for enabled/disabled as well)

  if (drawMode == 1)
  {
    //todo -- use wx wy for scale
    if (workc == game->gameTime)
    {  if (curSpr != 0)  {  render->addSprite(curSpr, pos.x+3, pos.y+3, pos.z, 1.0f, 1.0f);  }  }
    else if (worka == game->gameTime)
    {  if (curSpr != 0)  {  render->addSprite(curSpr, pos.x-1, pos.y-1, pos.z, 1.0f, 1.0f);    }  }
    else
    {  if (curSpr != 0)  {  render->addSprite(curSpr, pos.x, pos.y, pos.z, 1.0f, 1.0f);    } }
  } 
  else
  {
    if (workc == game->gameTime)
    {  if (curSpr != 0)  {  render->addSprite2(curSpr, pos.x+3, pos.y+3, pos.z,xrad+xrad,yrad+yrad);  }  }
    else if (worka == game->gameTime)
    {  if (curSpr != 0)  {  render->addSprite2(curSpr, pos.x-1, pos.y-1, pos.z,xrad+xrad,yrad+yrad);  }  }
    else
    {  if (curSpr != 0)  {  render->addSprite2(curSpr, pos.x, pos.y, pos.z,xrad+xrad,yrad+yrad);  } }
  }//endif
 
 
  game->drawStr(1, 16, pos.x, pos.y-8, pos.z+2, this->wstr.c_str());
}//framerender


void 
xButton::setSprite(xSprite * s)
{
  curSpr = s;
}//setsprite








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

  update();
  
}//childupdate


void 
xGuiGame::childRender(xGame * parent)
{
    
   //  myWorld.render();
   // cursor.render();

   glMatrixMode(GL_PROJECTION);  	glLoadIdentity();       glOrtho(0,640,480,0,-3000,3000); 
	 glMatrixMode(GL_MODELVIEW);	  glLoadIdentity();

      myFlat.resetFrame();

   
      render();
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




xButton * 
xGuiGame::addButton(std::string wname, std::string disp, int cmd, float ax, float ay, float az, float aw, float ah, xSprite * spr,  int tag)
{
  xButton * a;
  a = new xButton();
  a->xrad = aw * 0.5f;
  a->yrad = ah * 0.5f;
  a->pos.set(ax,ay,az);
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


xButton * 
xGuiGame::getButtonId(int id)
{
  xActor * a;
  xButton * b;  
  

  a = myWorld.getActor(id);

  if (a == 0) { return 0;}
  if (a->spectype != 100) { return 0; } //not button
  b = (xButton *) a;  //todo -- so far no better idea how to do this

  return b;
}//getbuttonid



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








