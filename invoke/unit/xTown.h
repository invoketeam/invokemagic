#pragma once

#include "../../gamex/xActor.h"
#include "../../gamex/xMdx3.h"

#include "../xInvActor.h"


class xTown : public xInvActor
{
public:
    xMdx3 * mesh;
    unsigned int skin;

    int decalId;

    gamex::cVec3f teamColor;

public:
  xTown(void);
  
  virtual void onKilled(void);
  
  virtual void init(void);
  virtual void update(void);
  virtual void render2(xRender * r);

  virtual void render(void);

};//xtarget