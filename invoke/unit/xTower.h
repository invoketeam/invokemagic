#pragma once

#include "../../gamex/xActor.h"
#include "../../gamex/xMdx3.h"

#include "../xInvActor.h"


class xTower : public xInvActor
{
public:
    xMdx3 * mesh;
    unsigned int skin;

    int decalId;


public:
  xTower(void);
  
  virtual void onKilled(void);
  
  virtual void think(void);

  virtual void init(void);
  virtual void update(void);
  virtual void render2(xRender * r);

  virtual void render(void);

};//xtarget