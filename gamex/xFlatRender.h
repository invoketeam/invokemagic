#pragma once


class xSprite;


class xFrame
{
public:
  float cx, cy, cz;
  float cw, ch;
  float ang;
  float alpha;

  int sortCode;
  int spec;

  unsigned int skin; //gl handle
  float u0, v0, u1, v1; //texture coords

public:
  xFrame(void);

};//xframe


class xFlatRender
{
public:
  xFrame ** vecFrame;
  int numFrame;
  int it;

public:
  xFlatRender(void);
  ~xFlatRender(void);

  void clear(void);

  void init(void);

  //note: position (ax,ay) is expected to be the center (az is only used for sorting, -32768 lowest you can use (without bugs))
  xFrame * addFrame(float ax, float ay, float az, float aw, float ah, unsigned int skin);
  
  xFrame * addSprite(xSprite * spr, float ax, float ay, float az, float scalex = 1.0f, float scaley = 1.0f, bool xmir = false, bool ymir = true); 
  xFrame * addSprite2(xSprite * spr, float ax, float ay, float az, float aw, float ah, bool xmir = false, bool ymir = true); 
    

  void resetFrame(void);

  void render(bool bSort = true);

  void radixSort(xFrame ** vec, int num);


};//xflatrender