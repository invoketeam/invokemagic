#pragma once




class xShadowTex
{
public:
  int mw, mh;
  unsigned int handle; //handle of color texture
  unsigned int depth; //handle of depth texture
  unsigned int fbo; //frambuffer

public:
  xShadowTex(void);
  ~xShadowTex(void);

  void clear(void);
  void init(int size);


};//shadowtex