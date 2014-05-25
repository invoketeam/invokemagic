#pragma once


#include "../../gamex/xActor.h"
#include "../../gamex/xMdx3.h"




class xHoming : public xActor
{
public:
    xMdx3 * mesh;
  gamex::cVec3f teamColor;

 
public:
  xHoming(void);
  
  virtual void onKilled(void);
  
  virtual void init(void);
  virtual void update(void);
  virtual void render2(xRender * r);

};//xtarget