#pragma once


#include "xGameExt.h"



class xGuiCursor : public xActor
{
public:
  int target;

public:
  xGuiCursor(void);
  virtual void update(void);
  virtual bool handCol(xActor * a);

  virtual void render(void);

};//xguicursor




class xButton : public xActor
{
public:

public:
  xButton(void);
  virtual void init(void);
  virtual void update(void);

  virtual void trigger(std::string &str);

  virtual void render(void);
 

};//xbutton




class xGuiGame : public xGameExt
{
public:
  xGame * parentGame;
  xGuiCursor cursor;

public:
  xGuiGame(void);
  ~xGuiGame(void);

  void init(void);
 

  void childUpdate(xGame * parent);
  void childRender(xGame * parent);


 //public function addButton(wname:String, ax:Number, ay:Number, az:Number, str:String, cmd:String, tag:int=1):void

  xActor * addButton(std::string wname, float ax, float ay, float az, std::string str, std::string cmd, int tag);
  void removeButton(std::string wname);
  void hideButton(std::string wname);
  void showButton(std::string wname);

};//classend






