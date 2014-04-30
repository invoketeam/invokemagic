#pragma once

#include "xGLCommon.h"

namespace extOgl
{

  extern bool extVbo;
  extern bool extMultiTex;

  extern bool extShadow;
  extern bool extDepthTex;
  extern bool extFbo;

  extern void getOglExtensions();

};//extogl

//todo -- add shaders someday
//http://nehe.gamedev.net/article/glsl_an_introduction/25007/

// VBO Extension Definitions, From glext.h
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);

// VBO Extension Function Pointers
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;					// VBO Name Generation Procedure
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;					// VBO Bind Procedure
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;					// VBO Data Loading Procedure
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;			// VBO Deletion Procedure
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;	

//Multiexture function pointers
extern PFNGLMULTITEXCOORD1FARBPROC		glMultiTexCoord1fARB;
extern PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD3FARBPROC		glMultiTexCoord3fARB;
extern PFNGLMULTITEXCOORD4FARBPROC		glMultiTexCoord4fARB;
extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB;	



extern PFNGLBINDFRAMEBUFFEREXTPROC   glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC	    glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC	    glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	    glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	    glFramebufferTexture2DEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC	    glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC	    glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC	    glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC	    glRenderbufferStorageEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	    glFramebufferRenderbufferEXT;
extern PFNGLDRAWBUFFERSARBPROC	    glDrawBuffersARB;
	    
