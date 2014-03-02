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
    
    numTarg = 0;
    numLod = 0;
    
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
    numTarg = 0;
    
    clearVbo();
    
  }//clear



//todo -- readfile

void 
xMdx3::loadFile(std::string fname)
  {
    clear();
    
    std::string comp = "MDX003_2013";
    char buf[12];
    FILE * file;
    
    file = fopen(fname.c_str(), "rb");
    if (file == 0) { printf("xMdx3 -- Error -- invalid file %s \n", fname.c_str()); return; } //todo error
    
    //HEADER
      fread(buf, 12, 1, file);
      
    //  printf("Header: [%s] \n", buf);
      
      
      if (comp != buf) { printf("xMdx3 -- Error -- invalid format %s \n", fname.c_str()); return; } //todo error invalid format

      fread(&numFace, 4, 1, file);  
      fread(&numVert, 4, 1, file);
      
       

     
      fread(&rad, 4, 1, file);
    
      fread(&min, 12, 1, file);
      fread(&max, 12, 1, file);
      fread(&off, 12, 1, file);
     

     
    //extra data -- 32 bytes
      //fread(&numTarg, 2, 1, file);
     // fread(&numLod, 2, 1, file);
     // fseek ( file , (12+16) , SEEK_CUR);
    fseek ( file , (16+16) , SEEK_CUR);

    //DATA
      vecIndex = new short[numFace * 3];
      fread(&vecIndex[0], 2, numFace*3, file);
      
      vecVert = new mVert[numVert];
      fread(&vecVert[0], sizeof(mVert), numVert, file);
     fclose(file);
/*    
    //EXTRA DATA 
      vecTarg = new mTarg[numTarg];
      fread(&vecTarg[0], sizeof(mTarg), numTarg, file);
      

      mLodIndex * k;
      num = numLod;
        
      for (i = 0; i < num; i++)
      {
        k = new mLodIndex();

         fread(&(k->numFace), 4, 1, file); 
         fread(&(k->numVert), 4, 1, file); //new

        k->vecIndex = new short[k->numFace * 3];
                
         fread(&(k->vecIndex[0]), 2, k->numFace * 3, file);
        
        vecLod.push_back(k);
      }//nexti
     
  */  
   
  }//loadfile
  
  

void 
xMdx3::calcMinMax(void)
{
  int i, num;
  gamex::cVec3f * p;

  min = 999999999999.0f;
  max = -999999999999.0f;


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




  
  /*
  
void 
xMdx3::saveFile(std::string fname)
  {
    if (numFace <= 0) { return; }
    
    char head[] = "MDX002_2013\0";
    int izero = 0;
//    int i;
//    int num;
    mLodIndex * k;
    //short szero = 0;
    FILE * file;
    
    file = fopen(fname.c_str(), "wb");
    if (file == 0) { return; } //todo error
    
    //HEADER
    
    fwrite(head, 12, 1, file); //identify file 
    
    fwrite(&numFace, 4, 1, file);
    fwrite(&numVert, 4, 1, file);
    
    fwrite(&rad, 4, 1, file);
    
    fwrite(&min, 12, 1, file);
    fwrite(&max, 12, 1, file);
    fwrite(&off, 12, 1, file);
    
    //extra data -- 32 bytes
      fwrite(&numTarg, 2, 1, file);
      
      numLod = vecLod.size();
       fwrite(&numLod, 2, 1, file);

      fwrite(&izero, 4, 1, file);
      fwrite(&izero, 4, 1, file);
      fwrite(&izero, 4, 1, file);

      fwrite(&izero, 4, 1, file);
      fwrite(&izero, 4, 1, file);
      fwrite(&izero, 4, 1, file);
      fwrite(&izero, 4, 1, file);
    
    //DATA
      
      fwrite(&vecIndex[0], 2, numFace*3, file);
      fwrite(&vecVert[0], sizeof(mVert), numVert, file);
      
    //EXTRA DATA
      fwrite(&vecTarg[0], sizeof(mTarg), numTarg, file);
      
      std::vector <mLodIndex *> ::iterator it;
      
      for (it = vecLod.begin(); it != vecLod.end(); it++)
      {
        k = (*it);
        fwrite(&(k->numFace), 4, 1, file);
        fwrite(&(k->numVert), 4, 1, file); //new
        fwrite(&(k->vecIndex[0]), 2, k->numFace * 3, file);

      }//nextit
      
     
     
    
    
    fclose(file);
  }//savefile+
  
*/













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
  
  /*
  void
xMdx3::renderLod(int i)
  {
    if (i < 0) { return; }
    if (i >= vecLod.size()) {return; }
    
    mLodIndex * m;
    
    m = vecLod[i];
    
    renderVec(m->vecIndex, m->numFace*3);
  }//renderlod
  */
  //vec is vector of indexes
  // num is the number of indexes (not tris)
/*  
void 
xMdx3::renderVec(short * vec, int num)
  {
    int i;
    mVert * v;

    glBegin(GL_TRIANGLES);
    
    for (i = 0; i < num; i++)
    {
      v = &(vecVert[ vec[i] ]);
      
      glTexCoord2f(v->u, v->v);
      glVertex3f(v->pos.x, v->pos.y, v->pos.z);
    
    }//nexti
    
    glEnd();
  
  }//rendervec
  */

/*
void 
xMdx3::render2(gamex::cVec3f * altVert)
{


  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    
 glTexCoordPointer(2, GL_FLOAT, 36, &(vecVert[0].u));

 glVertexPointer(3, GL_FLOAT, 12, altVert);   

  	glDrawElements(GL_TRIANGLES, numFace*3,  GL_UNSIGNED_SHORT, vecIndex); 
  //	glDrawElements(GL_TRIANGLES, numFace,  GL_SHORT, vecIndex); 

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}//render2
*/

void 
xMdx3::render(void)
{

//todo multitex:  set active texture

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
//todo -- add normal rendering to renderer
//but that is only needed for shaders and whatever i guess


 glNormalPointer(GL_BYTE, 36, &(vecVert[0].norm));  
  
 glTexCoordPointer(2, GL_FLOAT, 36, &(vecVert[0].u));

 glVertexPointer(3, GL_FLOAT, 36, &(vecVert[0].pos.x));   


  	glDrawElements(GL_TRIANGLES, numFace*3,  GL_UNSIGNED_SHORT, vecIndex); 
  //	glDrawElements(GL_TRIANGLES, numFace,  GL_SHORT, vecIndex); 

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

}//render
  
  /*
void 
xMdx3::render(void)
  {
    int i;
    int num;
    mVert * v;
    
    num = numFace * 3;
    
    glBegin(GL_TRIANGLES);
    
    for (i = 0; i < num; i++)
    {
      v = &(vecVert[ vecIndex[i] ]);
      
      glTexCoord2f(v->u, v->v);
      glVertex3f(v->pos.x, v->pos.y, v->pos.z);
    
    }//nexti
    
    glEnd();
  }//render
  */











static short vecFace_cube[] =  
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


    vecIndex = new short[numFace * 3];
    vecVert = new mVert[numVert];
    
    memcpy(vecIndex, vecFace_cube, 12*3*2);
    
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
	 glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, numFace*3*2, vecIndex, GL_STATIC_DRAW_ARB);

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





/*
static float vecVert_cube[] =
      {
			 -0.5f,  -0.5f,  -0.5f,
			  0,5f,  -0.5f,  -0.5f,
			 -0.5f, 0.5f,  -0.5f,
			  0.5f, 0.5f,  -0.5f,
			 
			  -0.5f,  -0.5f,  0.5f,
			   0.5f,  -0.5f,  0.5f,
			  -0.5f, 0.5f,  0.5f,
			   0.5f, 0.5f,  0.5f,
			 
			  0.5f,  -0.5f,  -0.5f,
			  0.5f, 0.5f,  -0.5f,
			  0.5f,  -0.5f,  0.5f,
			  0.5f, 0.5f,  0.5f,
			 
			  -0.5f,  -0.5f,  -0.5f,
			  -0.5f, 0.5f,  -0.5f,
			  -0.5f,  -0.5f,  0.5f,
			  -0.5f, 0.5f,  0.5f,
			  
			  -0.5f,   -0.5f,  -0.5f,
			   0.5f,  -0.5f,  -0.5f,
			  -0.5f,   -0.5f,  0.5f,
			   0.5f,  -0.5f,  0.5f,
			  
			  -0.5f,  0.5f,  -0.5f,
			   0.5f, 0.5f,  -0.5f,
			  -0.5f,  0.5f,  0.5f,
			   0.5f, 0.5f,  0.5f
			};


vecUv = Vector.<Number>([
			1.0, 1.0,
			0.0, 1.0,
			1.0, 0.0,
			0.0, 0.0,
			
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
			
			1.0, 1.0,
			1.0, 0.0,
			0.0, 1.0,
			0.0, 0.0,
			
			
			0.0, 1.0,
			0.0, 0.0,
			1.0, 1.0,
			1.0, 0.0,
						
			1.0, 0.0,
			0.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			
			1.0, 1.0,
			0.0, 1.0,
			1.0, 0.0,
			0.0, 0.0
			]);

*/


  
