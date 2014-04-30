#pragma warning (disable : 4786)

#include "oglExt.h"

#include <string>


static bool IsExtensionSupported( char* szTargetExtension );


namespace extOgl
{

bool extVbo = false;
bool extMultiTex = false;

bool extShadow = false;
bool extDepthTex = false;
bool extFbo = false;


//todo -- 
//  store max texture size
//  store max texture units



void getOglExtensions()
{
    std::string strExt;
    //http://www.cplusplus.com/reference/string/string/find_first_of/

    strExt = (char const *) glGetString( GL_EXTENSIONS );

    ///printf("Extensions: %s \n", strExt.c_str() );






    extVbo = strExt.find_first_of("GL_ARB_vertex_buffer_object") != std::string::npos;

  if( extVbo )
	{
        printf("Extension: got GL_ARB_vertex_buffer_object \n");

		// Get Pointers To The GL Functions
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
    glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
					
	}//endif







    extMultiTex = strExt.find_first_of("GL_ARB_multitexture") != std::string::npos;

    if (extMultiTex)
    {   
        printf("Extension: got GL_ARB_multitexture \n");

        //glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits); // todo -- store this somewhere
		  glMultiTexCoord1fARB	= (PFNGLMULTITEXCOORD1FARBPROC)		wglGetProcAddress("glMultiTexCoord1fARB");
		  glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");
		  glMultiTexCoord3fARB	= (PFNGLMULTITEXCOORD3FARBPROC)		wglGetProcAddress("glMultiTexCoord3fARB");
		  glMultiTexCoord4fARB	= (PFNGLMULTITEXCOORD4FARBPROC)		wglGetProcAddress("glMultiTexCoord4fARB");
		  glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
		  glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)	wglGetProcAddress("glClientActiveTextureARB");		

    }//endif



    extShadow = strExt.find_first_of("GL_ARB_shadow") != std::string::npos;

    if (extShadow)
    {
        printf("Extension: got GL_ARB_shadow \n");

    }//endif

  
    extDepthTex = strExt.find_first_of("GL_ARB_depth_texture") != std::string::npos;

    if (extDepthTex)
    {
        printf("Extension: got GL_ARB_depth_texture \n");

    }//endif


    
// https://github.com/core-code/Core3D/blob/master/Utilities/opengl_win32.cpp   

// http://opennero.googlecode.com/svn-history/r6/trunk/external/source/Irrlicht/COpenGLExtensionHandler.cpp
// FrameBufferObjects

    extFbo = strExt.find_first_of(" GL_EXT_framebuffer_object") != std::string::npos;

    if (extFbo)
    {
       printf("Extension: got GL_EXT_framebuffer_object \n");

      glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC) wglGetProcAddress("glBindFramebufferEXT");
	    glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
	    glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) wglGetProcAddress("glGenFramebuffersEXT");
	    glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
	    glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
	    glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC) wglGetProcAddress("glBindRenderbufferEXT");
	    glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
	    glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC) wglGetProcAddress("glGenRenderbuffersEXT");
	    glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC) wglGetProcAddress("glRenderbufferStorageEXT");
	    glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
	    glDrawBuffersARB = (PFNGLDRAWBUFFERSARBPROC) wglGetProcAddress("glDrawBuffersARB");
	    

    }//endif

  
      
}//getogl





};//extogl




// VBO Extension Function Pointers
PFNGLGENBUFFERSARBPROC glGenBuffersARB = 0;					// VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBufferARB = 0;					// VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferDataARB = 0;					// VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = 0;	
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = 0;	



//multitexture extension
PFNGLMULTITEXCOORD1FARBPROC		glMultiTexCoord1fARB	= 0;
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	= 0;
PFNGLMULTITEXCOORD3FARBPROC		glMultiTexCoord3fARB	= 0;
PFNGLMULTITEXCOORD4FARBPROC		glMultiTexCoord4fARB	= 0;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		= 0;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB= 0;	




//FBO 

PFNGLBINDFRAMEBUFFEREXTPROC   glBindFramebufferEXT = 0;
PFNGLDELETEFRAMEBUFFERSEXTPROC	    glDeleteFramebuffersEXT = 0;
PFNGLGENFRAMEBUFFERSEXTPROC	    glGenFramebuffersEXT = 0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	    glCheckFramebufferStatusEXT =0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	    glFramebufferTexture2DEXT = 0;
PFNGLBINDRENDERBUFFEREXTPROC	    glBindRenderbufferEXT = 0;
PFNGLDELETERENDERBUFFERSEXTPROC	    glDeleteRenderbuffersEXT = 0;
PFNGLGENRENDERBUFFERSEXTPROC	    glGenRenderbuffersEXT = 0;
PFNGLRENDERBUFFERSTORAGEEXTPROC	    glRenderbufferStorageEXT = 0;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	    glFramebufferRenderbufferEXT = 0;
PFNGLDRAWBUFFERSARBPROC	    glDrawBuffersARB = 0;
	    

