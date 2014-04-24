#pragma once


//todo -- make part of multigrid.h

class xActor;

class xCell
{
public:
  xActor * first;
  xActor * last;
  
  xCell * next;
  static xCell * lastc;

  int num;

public:
  xCell()
  {
    first = 0;
    last = 0;
    next = 0;
    num = 0;
  }//ctor

	void addToLast(); 
  void at2();
  void setAsLast();
	void addActor(xActor * a);
	void remActor(xActor * a);

};//classend

