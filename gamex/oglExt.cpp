#pragma warning (disable : 4786)

#include "oglExt.h"

#include <string>


static bool IsExtensionSupported( char* szTargetExtension );


namespace extOgl
{

bool extVbo = false;
bool extMultiTex = false;


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

        //glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits); // i only need 2 so whatever
		glMultiTexCoord1fARB	= (PFNGLMULTITEXCOORD1FARBPROC)		wglGetProcAddress("glMultiTexCoord1fARB");
		glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord3fARB	= (PFNGLMULTITEXCOORD3FARBPROC)		wglGetProcAddress("glMultiTexCoord3fARB");
		glMultiTexCoord4fARB	= (PFNGLMULTITEXCOORD4FARBPROC)		wglGetProcAddress("glMultiTexCoord4fARB");
		glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
		glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)	wglGetProcAddress("glClientActiveTextureARB");		

    }//endif


}//getogl


};//extogl


// VBO Extension Function Pointers
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;					// VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;					// VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;					// VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;	
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = NULL;	

//multitexture extension
PFNGLMULTITEXCOORD1FARBPROC		glMultiTexCoord1fARB	= NULL;
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	= NULL;
PFNGLMULTITEXCOORD3FARBPROC		glMultiTexCoord3fARB	= NULL;
PFNGLMULTITEXCOORD4FARBPROC		glMultiTexCoord4fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB= NULL;	
