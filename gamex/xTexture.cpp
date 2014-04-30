#pragma warning (disable : 4786)

#include "xGLCommon.h"

#include "xImage.h"
#include "xTexture.h"



//todo -- rename it (its a private class)
struct xRgb
{
  unsigned char r,g,b;
};//xrgb



  xTexture::xTexture()
    {
        handle = 0;
        mw = 0;
        mh = 0;

    }//ctor
    



  xTexture::~xTexture()
   { clear(); }



    
void
xTexture::clear() 
    { 
        if (handle != 0)   {  glDeleteTextures( 1, &handle );   handle = 0;  }

        mw = 0;
        mh = 0;

    }//clear
    

//todo
    //http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    

//todo -- make sure textures are power of 2?
    
    
void 
xTexture::loadTex(std::string fname, bool mip,  bool mirv, bool clamp )
  {
    xImage img;
    img.loadImage(fname);
    if (img.mw == 0 || img.mh == 0) { img.init(32,32); img.fillImage(0xFF0000FF);}
    filename = fname;
    if (mirv) { img.mirVert(); }
    makeTex(&img, mip, clamp);
  }//loadtex
    


void 
xTexture::initEmpty(int size)
  {
    xImage temp;
    temp.init(size, size);
    //temp.xorFill();
    
    filename = "dynamic";
   
    //empty textures dont use mipmaps (todo -- set clamping from parameter)
    makeTex(&temp, false, false);
    setTexClamp(true);
  }//initempty



void 
xTexture::updateTex(xImage * img)
{

  //todo -- check if image is same size
  //and warning or something

  //or just regenerate the whole texture based on the parameter image if its different size, dunno

  if (handle == 0) { return;}

  //i only ever use it here
  #define GL_UNSIGNED_INT_8_8_8_8           0x8035

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, img->mw, img->mh, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, img->dat); 


}//updatetex




void
xTexture::setTexClamp(bool clamp)
{
  glBindTexture(GL_TEXTURE_2D, handle);
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP : GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP : GL_REPEAT );
}//clamp


void
xTexture::setMaxLod(int lod)
{
  glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, lod);
}//maxlod


//private:
    //based on
    //http://www.nullterminator.net/gltexture.html
    
    
    //make opengl texture
void 
xTexture::makeTex(xImage  * img, bool mip, bool clamp)
    {
        if (img->dat == 0) { return; }//image has no data
      
        clear(); //get rid of prev image if there is any
        
        mw = img->mw;
        mh = img->mh;
        
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        
     
   //trilinear
     
        if (mip)
        { 
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        }
        else
        {
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        }
                
        /*
        //nearest
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        */
  
        /* 
        //bilinear
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
        */    
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp ? GL_CLAMP : GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp ? GL_CLAMP : GL_REPEAT );

        if (mip)
        { buildMipMap(img->mw, img->mh,  img->dat);  }
        else
        { glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->mw, img->mh, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, img->dat);  }

       //default blending
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 


        //todo -- set mip map max level

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3)

    }//maketex
    


void 
xTexture::buildMipMap(int w, int h, unsigned int * dat)
{
      unsigned int * buf;
      int wk, hk,  x,y,yt,kt;
      int m;
      int s;
      int num;

      m = 0; 
      s = 0;
      num = w * h;
      wk = w;
      hk = h;
      buf = new unsigned int[ num ];

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, dat);

      while (wk != 1)
      {
        m += 1;
        //s += 2;
        wk /= 2;
        hk /= 2;
        if (wk <= 1) { wk = 1; hk = 1; }
        s = w / wk;
        for (y = 0; y < hk; y++)
        {
          yt = y * wk;
          kt = y * s * w;
          for (x = 0; x < wk; x++)
          {
            buf[yt+x] = dat[(kt)+(x*s)];
            //buf[yt+x] = 0xAfAfAfAf;
          }//nextx
        }//nexty
        
        glTexImage2D(GL_TEXTURE_2D, m, GL_RGBA, wk, hk, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buf);

      }//wend
      
      delete [] buf;

;
}//buildmipmap




/*
    //self clobbed together mipmap generator
    //scary part is that it actually works
void 
xTexture::xBuildMipMap_Tex2D_4Bit(int w_, int h_, unsigned char * data_)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_, h_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);

      unsigned int * buf;
      unsigned int * dat;
      int wk, hk,  x,y,yt,kt;
      int m = 0;
      int s = 0;
      wk = w_;
      hk = h_;
      // printf("m %d  wk %d  hk %d  \n", m, wk, hk);
      int total= w_ * h_;
      dat = (unsigned int *) data_;
      buf = new unsigned int[ total ];
      while (wk != 1)
      {
        m += 1;
        //s += 2;
        wk /= 2;
        hk /= 2;
        if (wk <= 1) { wk = 1; hk = 1; }
        s = w_ / wk;
        for (y = 0; y < hk; y++)
        {
          yt = y * wk;
          kt = y * s * w_;
          for (x = 0; x < wk; x++)
          {
            buf[yt+x] = dat[(kt)+(x*s)];
            //buf[yt+x] = 0xAfAfAfAf;
          }//nextx
        }//nexty
      //  printf("m %d  wk %d  hk %d  \n", m, wk, hk);
         glTexImage2D(GL_TEXTURE_2D, m, GL_RGBA, wk, hk, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);

      }//wend
      
      delete [] buf;
    }//xbuildmipmap

  */

  
    /*
    //clobbing overload iminent
void 
xTexture::xBuildMipMap_Tex2D_3Bit(int w_, int h_, unsigned char * data_)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w_, h_, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);

      xRgb * buf;
      xRgb * dat;
      int wk, hk,x,y,yt,kt;
      int m = 0;
      int s = 0;
      wk = w_;
      hk = h_;
      // printf("m %d  wk %d  hk %d  \n", m, wk, hk);
      int total= w_ * h_;
      dat = (xRgb *) data_;
      buf = new xRgb[ total ];
      while (wk != 1)
      {
        m += 1;
        //s += 2;
        wk /= 2;
        hk /= 2;
        if (wk <= 1) { wk = 1; hk = 1; }
        s = w_ / wk;
                
        for (y = 0; y < hk; y++)
        {
          yt = y * wk;
          kt = y * s * w_;
          for (x = 0; x < wk; x++)
          {
            buf[yt+x] = dat[(kt)+(x*s)];
            //buf[yt+x].r = 255;
            //buf[yt+x] = 0xAfAfAfAf;
          }//nextx
        }//nexty
       // printf("m %d  wk %d  hk %d  \n", m, wk, hk);
         glTexImage2D(GL_TEXTURE_2D, m, GL_RGB, wk, hk, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);

      }//wend
      
      delete [] buf;
    }//xbuildmipmap3bit
*/