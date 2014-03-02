
#pragma once

#include <string.h>


class xImage;


class xTexture
{
public:
 
   unsigned int handle;
  
    int mw;
    int mh;
 
    std::string filename;
     
public:
  xTexture();
  ~xTexture(); 

  void clear(); 



  void loadTex(std::string fname, bool mip = true, bool mirv = true, bool clamp = false);

  void initEmpty(int size);
  
  void updateTex(xImage * img);

  void setTexEnv_Modulate(void);
  void setTexEnv_Replace(void);
  void setTexEnv_Blend(void);
  void setTexEnv_Decal(void);
  void setTexClamp(bool clamp);

private:
  void makeTex(xImage * img, bool mip, bool clamp);
  //void xBuildMipMap_Tex2D_4Bit(int w_, int h_, unsigned char * data_);
//  void xBuildMipMap_Tex2D_3Bit(int w_, int h_, unsigned char * data_);
  void  buildMipMap(int w, int h, unsigned int * data);

};//classend

