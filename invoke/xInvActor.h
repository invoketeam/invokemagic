#pragma once


#include "../gamex/gamex.h"


extern float getAng(float dy, float dx);

class xInvActor : public xActor
{
public:

public:
   virtual xActor * getTarget(xMultiGrid * mgrid, float ax, float az, float aw, float ah);

   virtual void getClosePointXZ(xActor * a, float * retx, float * retz);

};//xTree