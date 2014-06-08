#pragma once

#include "../../gamex/xActor.h"
#include "../../gamex/xMdx3.h"

#include "../xInvActor.h"


class xMine : public xInvActor
{
public:
    xMdx3 * mesh;
    unsigned int skin;

    int decalId;


public:
  xMine(void);
  
  virtual void onKilled(void);
  
  virtual void init(void);
  virtual void update(void);
  virtual void render2(xRender * r);

  virtual void render(void);

};//xtarget