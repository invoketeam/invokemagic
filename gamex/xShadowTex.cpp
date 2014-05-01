#pragma warning (disable : 4786)


#include "xShadowTex.h"


#include "xGLCommon.h"
#include "oglExt.h"




xShadowTex::xShadowTex(void)
{
  mw = 0; 
  mh = 0; 
  handle = 0;
  fbo = 0;
  depth = 0;

}//ctor





xShadowTex::~xShadowTex(void)
{
  clear();
}//dtor






void 
xShadowTex::clear(void)
{
  if (handle != 0) { glDeleteTextures(1, &handle); handle = 0; }
  if (depth != 0) { glDeleteTextures(1, &depth); depth = 0; }
  if (fbo != 0) { glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); glDeleteFramebuffersEXT(1, &fbo); fbo=0;}
  mw = 0; mh = 0;
}//clear








void 
xShadowTex::init(int size)
{
  clear();

  float clampColor[] = { 0, 0, 0, 0 };

//  size = 512;
    size = 1024;

  //todo: check if size is power of 2
  mw = size;
  mh = size;


//COLOR TEXTURE
  glGenTextures(1, &handle);
	  glBindTexture(GL_TEXTURE_2D, handle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);	   
	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

 //DEPTH TEXTURE
   glGenTextures(1, &depth);
	  glBindTexture(GL_TEXTURE_2D, depth);

    
    
      //note -- this is not classic shadowing   
      //as instead of drawing the scene twice the shadows are just drawn in black afterwards everythings drawn 
       glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_ALPHA);
     // glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
      //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_GEQUAL);
       //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);

   
	   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    //depth component is default depth of the scene
      glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

   glGenFramebuffersEXT(1, &fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
     glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, handle, 0);
     glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depth, 0);

//todo
//https://www.opengl.org/wiki/Framebuffer_Object_Examples
// glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT   

//remember to unbind after rendering too
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}//init


