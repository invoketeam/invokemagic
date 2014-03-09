#pragma once


#include "xGameExt.h"



class xGuiCursor : public xActor
{
public:

public:
  virtual void update(void);
  virtual bool handCol(xActor * a);

};//xguicursor




class xButton : public xActor
{
public:

public:
  xButton(void);
  virtual void init(void);
  virtual void update(void);
  virtual void trigger(std::string &str);
 
 

};//xbutton




class xGuiGame : public xGameExt
{
public:
  xGame * parentGame;


public:
  xGuiGame(void);
  ~xGuiGame(void);
 

  void childUpdate(xGame * parent);
  void childRender(xGame * parent);



};//classend