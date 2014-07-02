#pragma once

#include "../../gamex/xActor.h"
#include "../../gamex/xMdx3.h"

#include "../xInvActor.h"


class xBuilding : public xInvActor
{
public:
    xMdx3 * mesh;
    unsigned int skin;

    int decalId;


public:
  xBuilding(void);
  
  virtual void onKilled(void);
  
  virtual void init(void);
  virtual void update(void);
  virtual void render2(xRender * r);

  virtual void render(void);

  virtual void gotHit(float dmg, int dtype, float hx, float hy, float hz);  
  
  void gotBuilt(void);
  
};//xtarget

