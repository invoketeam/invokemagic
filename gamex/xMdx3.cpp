#pragma warning (disable : 4996) //vc2010 no longer likes fopen
#pragma warning (disable : 4786)


#include "xMdx3.h"

#include "xGLCommon.h"
#include "oglExt.h"


//#include <GL/glut.h>


xMdx3::xMdx3()
  {
    numFace = 0;
    numVert = 0;

    drawFace = 0;
        
    rad = 0;
    
    vecIndex = 0;
    vecVert = 0;


    hasVbo = false;
    indexBuf = 0;
    vertBuf = 0;

  }//ctor
  


xMdx3::~xMdx3() 
  {
    clear();
  }//dtor



void 
xMdx3::clear(void)
  {
    if (vecIndex != 0) { delete [] vecIndex; } vecIndex = 0;
    if (vecVert != 0) { delete [] vecVert; } vecVert = 0;
    numFace = 0;
    numVert = 0;
    drawFace = 0;
    
    clearVbo();
    
  }//clear



//todo -- readfile


  
void 
xMdx3::readFile(FILE * file)
  {
  
  //HEADER
    fread(&numFace, 4, 1, file); drawFace = numFace;  
    fread(&numVert, 4, 1, file);

    fread(&rad, 4, 1, file);

    fread(&min, 12, 1, file);
    fread(&max, 12, 1, file);
    fread(&off, 12, 1, file);

  //reserved -- 32 bytes
    fseek ( file , (16+16) , SEEK_CUR);

  //DATA

    //indices are stored in shorts (2byte)
    //convert them to int (4byte)
    short * temp;
    int i;
    int num;
      num = numFace * 3;
      temp = new short[num];
      fread(&temp[0], 2, num, file);
      vecIndex = new int[num];
      for (i = 0; i < num; i++)    { vecIndex[i] = (int) temp[i]; }
    delete [] temp;

    vecVert = new mVert[numVert];
    fread(&vecVert[0], sizeof(mVert), numVert, file);
  }//readfile

  
void 
xMdx3::loadFile(std::string fname)
  {
    clear();
    
    std::string comp = "MDX003_2013";
    char buf[12];
    FILE * file;
    
    file = fopen(fname.c_str(), "rb");
    if (file == 0) { printf("xMdx3 -- Error -- invalid file %s \n", fname.c_str()); return; } //todo error
    
    //FILE_ID
    fread(buf, 12, 1, file);
    if (comp != buf) { printf("xMdx3 -- Error -- invalid format %s \n", fname.c_str()); fclose(file);  return; } //todo error invalid format

    readFile(file);
   
    fclose(file);
  }//loadfile

  
  

void 
xMdx3::calcMinMax(void)
{
  int i, num;
  gamex::cVec3f * p;

  min = 999999999.0f;
  max = -999999999.0f;


  num = numVert;
  for (i = 0; i < num; i++)
  {
    p = &(vecVert[i].pos);

    if (p->x < min.x) { min.x = p->x; }
    if (p->x > max.x) { max.x = p->x; }
    if (p->y < min.y) { min.y = p->y; }
    if (p->y > max.y) { max.y = p->y; }
    if (p->z < min.z) { min.z = p->z; }
    if (p->z > max.z) { max.z = p->z; }

  }//nexti
  


}//calcminmax



void
xMdx3::copyMesh(xMdx3 * mesh)
{
  // if (mesh == this) { } //throw a warning maybe (for now lets assume this wont happen)

  clear();

  //other mesh is empty? -> we are done
  if (mesh->numFace <= 0) { return; }

  numFace = mesh->numFace;
  numVert = mesh->numVert;

  drawFace = mesh->drawFace;
       
  int numIndex;
  numIndex = numFace * 3;
  vecIndex = new int[numIndex];  
  vecVert = new mVert[numVert];

  memcpy(&(vecIndex[0]),&(mesh->vecIndex[0]), numIndex * 4);
  memcpy(&(vecVert[0]),&(mesh->vecVert[0]), numVert * 36);  


  rad = mesh->rad;

  min = mesh->min;
  max = mesh->max;
  off = mesh->off;

}//copymesh


void 
xMdx3::moveMesh(float mx, float my, float mz)
{
  int i, num;
  gamex::cVec3f m;
  m.set(mx,my,mz);

  num = numVert;
  for (i = 0; i < num; i++) { vecVert[i].pos += m; }
}//movemesh


void 
xMdx3::scaleMesh(float sx, float sy, float sz)
{
  int i, num;
  gamex::cVec3f s;
  s.set(sx,sy,sz);

  num = numVert;
  for (i = 0; i < num; i++) { vecVert[i].pos *= s; }

}//scalemesh






  void 
xMdx3::renderVert(int num)
  {
    if (num < 0) { return; }
    if (num > numVert) { return; }
  
    int i;
    mVert * v;
    
    glBegin(GL_POINTS);
    for (i = 0; i < num ;  i++)
    {
      v= &vecVert[i];
        glVertex3f(v->pos.x, v->pos.y, v->pos.z);
    
    }//nexti
    glEnd();
    
  }//rendervert
  


void 
xMdx3::render(void)
{

  bool bColor;
  bColor = false; //todo -- set from parameter



  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  if (bColor) glEnableClientState(GL_COLOR_ARRAY);

  if (bColor) glColorPointer(4, GL_UNSIGNED_BYTE, 36, &(vecVert[0].rgba) );
  
   glNormalPointer(GL_BYTE, 36, &(vecVert[0].norm));  
  
   glTexCoordPointer(2, GL_FLOAT, 36, &(vecVert[0].u));

   //vertex should be last
     glVertexPointer(3, GL_FLOAT, 36, &(vecVert[0].pos.x));   

 // glDrawElements(GL_TRIANGLES, numFace*3, GL_UNSIGNED_INT, vecIndex); 
   glDrawElements(GL_TRIANGLES, drawFace*3, GL_UNSIGNED_INT, vecIndex); 

  if (bColor) glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

}//render
  







void 
xMdx3::initPlane(float scale, float u0, float v0, float u1, float v1)
{
  clear();

    float neg;
    float pos;

    neg = -0.5f * scale;
    pos = 0.5f * scale;

    numFace = 2; drawFace = numFace;
		numVert = 4;


    vecIndex = new int[numFace * 3];
    vecVert = new mVert[numVert];

    vecIndex[0] = 1;
    vecIndex[1] = 0;
    vecIndex[2] = 2;
    vecIndex[3] = 1;
    vecIndex[4] = 2;
    vecIndex[5] = 3;


    int i;
    i = 0;
    vecVert[i].pos.set(neg, 0.0f, neg);    vecVert[i].u = u1;    vecVert[i].v = v0; ++i;
    vecVert[i].pos.set(pos, 0.0f, neg);    vecVert[i].u = u0;    vecVert[i].v = v0; ++i;
    vecVert[i].pos.set(neg, 0.0f, pos);    vecVert[i].u = u1;    vecVert[i].v = v1; ++i;
    vecVert[i].pos.set(pos, 0.0f, pos);    vecVert[i].u = u0;    vecVert[i].v = v1; ++i;

}//initplane




static int vecFace_cube[] =  
     {	1, 0, 2, 1, 2, 3,
				1 + 4, 2 + 4, 0 + 4,  1 + 4, 3 + 4, 2 + 4,
				1 + 8, 2 + 8, 0 + 8,  1 + 8, 3 + 8, 2 + 8,
				1 + 12, 0 + 12, 2 + 12,  1 + 12, 2 + 12, 3 + 12,
				1 + 16, 2 + 16, 0 + 16,  1 + 16, 3 + 16, 2 + 16,
				1 + 20, 0 + 20, 2 + 20,  1 + 20, 2 + 20, 3 + 20};


void 
xMdx3::initBox(float scale)
{
    clear();

    float neg;
    float pos;

    neg = -0.5f * scale;
    pos = 0.5f * scale;

    numFace = 12;  
		numVert = 24;

    drawFace = numFace;

    vecIndex = new int[numFace * 3];
    vecVert = new mVert[numVert];
    
    memcpy(vecIndex, vecFace_cube, 12*3*4);
    
/*
//template
    vecVert[i].pos.set(neg, neg, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(neg, neg, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(neg, neg, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(neg, neg, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
 */


    int i;
    i = 0;
    vecVert[i].pos.set(neg, neg, neg);    vecVert[i].u = 1.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(pos, neg, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(neg, pos, neg);    vecVert[i].u = 1.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(pos, pos, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
 
    vecVert[i].pos.set(neg, neg, pos);    vecVert[i].u = 0.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(pos, neg, pos);    vecVert[i].u = 1.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(neg, pos, pos);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(pos, pos, pos);    vecVert[i].u = 1.0f;    vecVert[i].v = 0.0f; ++i;

    vecVert[i].pos.set(pos, neg, neg);    vecVert[i].u = 1.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(pos, pos, neg);    vecVert[i].u = 1.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(pos, neg, pos);    vecVert[i].u = 0.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(pos, pos, pos);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
 
    vecVert[i].pos.set(neg, neg, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(neg, pos, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(neg, neg, pos);    vecVert[i].u = 1.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(neg, pos, pos);    vecVert[i].u = 1.0f;    vecVert[i].v = 0.0f; ++i;

    vecVert[i].pos.set(neg, neg, neg);    vecVert[i].u = 1.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(pos, neg, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(neg, neg, pos);    vecVert[i].u = 1.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(pos, neg, pos);    vecVert[i].u = 0.0f;    vecVert[i].v = 1.0f; ++i;

    vecVert[i].pos.set(neg, pos, neg);    vecVert[i].u = 1.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(pos, pos, neg);    vecVert[i].u = 0.0f;    vecVert[i].v = 1.0f; ++i;
    vecVert[i].pos.set(neg, pos, pos);    vecVert[i].u = 1.0f;    vecVert[i].v = 0.0f; ++i;
    vecVert[i].pos.set(pos, pos, pos);    vecVert[i].u = 0.0f;    vecVert[i].v = 0.0f; ++i;


}//initbox






void 
xMdx3::clearVbo(void)
{
  if (hasVbo == false) { return; }

  glDeleteBuffersARB(1, &indexBuf);
  glDeleteBuffersARB(1, &vertBuf);


  hasVbo = false;
  indexBuf = 0;
  vertBuf = 0;

}//clear

void 
xMdx3::makeVbo(void)
{
  clearVbo();

  //http://www.songho.ca/opengl/gl_vbo.html
  //http://nehe.gamedev.net/tutorial/vertex_buffer_objects/22002/
  //http://www.opengl.org/wiki/Vertex_Specification_Best_Practices
  //http://onesadcookie.com/svn/Kiloplane/naive_vbo_renderer.c
  //http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml

  glGenBuffersARB(1, &indexBuf);
	 glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuf);
	 glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, numFace*3*4, vecIndex, GL_STATIC_DRAW_ARB);

  glGenBuffersARB(1, &vertBuf);
	 glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertBuf);
	 glBufferDataARB(GL_ARRAY_BUFFER_ARB, numVert*36, vecVert, GL_DYNAMIC_DRAW); 

   hasVbo = true;

   glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
   glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}//makevbo



void 
xMdx3::updateVboVertex(void)
{
  if (hasVbo == false) { return; }

	 glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertBuf);
	 //glBufferDataARB(GL_ARRAY_BUFFER_ARB, numVert*36, vecVert, GL_DYNAMIC_DRAW); 

   //for update need to use sub so it doesnt reallocate it
   //http://www.opengl.org/sdk/docs/man/xhtml/glBufferSubData.xml
   glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, numVert*36, vecVert);

}//update

