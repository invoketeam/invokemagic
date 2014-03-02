#pragma warning (disable : 4786)
#include "xCell.h"

#include "xActor.h"



 xCell* xCell::lastc  = 0;



	void 
  xCell::addToLast() {	if (lastc != 0) lastc->next = this;lastc = this;	}

	void 
  xCell::at2() 	{	if (num <= 0) return;	if (lastc != 0) lastc->next = this;	lastc = this;	}
		
	void 
  xCell::setAsLast() {	lastc = this;	next = 0;	}
		
	void 
  xCell::addActor(xActor * a)
		{
			if (a->cell == this) return; //already in this cell
			if (a->cell != 0) a->cell->remActor(a); //was in another cell, remove it
			if (num == 0) //empty cell -- (same as check for first == 0 i guess)
			{  //add as only element
				first = a;	last = a;	a->next = 0;	a->prev = 0;	}
			else 
			{	//add as last element
				a->prev = last;	a->next = 0;	last->next = a;	last = a;
			}//endif
			a->cell = this; 	num += 1;
		}//addactor
		
	void 
  xCell::remActor(xActor * a)
		{
			if (a->cell != this) {return;} //not in this cell
			if (a->next != 0) {a->next->prev = a->prev;}	if (a->prev != 0) {a->prev->next = a->next;}
			if (first == a) {first = a->next;}	if (last == a) {last = a->prev;}
			a->cell = 0; //not in any cells
			num -= 1; //one less resident in cell
		}//remactor

