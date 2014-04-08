#pragma once


#include "../gamex/gamex.h"
#include "invokeGame.h"
#include "../gamex/xGuiGame.h"
#include "../gamex/xData.h"
#include "../gamex/xFont.h"
#include "../gamex/xFlatRender.h"


class invokeMenu : public xGame
{
public:
  xFlatRender myFlat;
  xGuiGame myGui;  

  xFont myFont;
  xTexture myFontSkin;

  xData myData;

public:
  invokeMenu(void);
  virtual  ~invokeMenu(void);

  virtual void init(void);


  virtual void update(void);
	virtual void render(void);


};//invokemenu