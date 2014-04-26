#pragma once

#include "xSelect.h"
#include "xInvActor.h"
#include "../gamex/xCursor.h"

class xHand : public xInvActor
{
public:

  int selStart;
  float selx;
  float sely;

  xSelect mySelect;
  xViewBox * viewBoxPtr;

public:

  xHand(void);

  virtual void init(void);

  virtual void update(void);




};//hand


