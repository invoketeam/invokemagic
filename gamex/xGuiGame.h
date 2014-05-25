#pragma once


#include "xGameExt.h"
#include "xFlatRender.h"
#include "xSprite.h"
#include "xFont.h"

class xGuiCursor : public xActor
{
public:
  int target;
  xSprite * curSpr;

public:
  xGuiCursor(void);
  virtual void update(void);
  virtual bool handCol(xActor * a);

  //virtual void render(void);
  virtual void frameRender(xFlatRender * render);

};//xguicursor




class xButton : public xActor
{
public:
  xSprite * curSpr;
  int cmd;
  int rightCmd;
  int arg0, arg1;
  int drawMode;
  int btnMode; //0 normal   1 instant (repeating)

public:
  xButton(void);
  virtual void init(void);
  virtual void update(void);

  virtual void trigger(std::string &str);

 // virtual void render(void);
  virtual void frameRender(xFlatRender * render);

  virtual void setSprite(xSprite * s);

};//xbutton




class xGuiGame : public xGameExt
{
public:
  xGame * parentGame;

  xFont * pfont;

  xGuiCursor cursor;
  xFlatRender myFlat;
 
public:
  xGuiGame(void);
  virtual ~xGuiGame(void);

  virtual  void init(void);
  
  virtual void gotCmd(int cmd, int arg0, int arg1);
  
  virtual void childUpdate(xGame * parent);
  virtual void childRender(xGame * parent);

  virtual void drawStr(int font, float size, float cx, float cy, float cz, const char* str, ...);


 //command can be an int too, doesnt really matter 
  virtual xButton * addButton(std::string wname, std::string disp, int cmd, float ax, float ay, float az, float aw=128, float ah=32,xSprite * spr = 0,  int tag = -1); 
  
  virtual void removeButton(std::string wname);

  virtual  void hideButton(std::string wname);

  virtual void showButton(std::string wname);


};//classend






