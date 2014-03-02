
#pragma once

#include "xGLCommon.h"
#include "oglExt.h"

#include "xMath.h"
#include "xMdx3.h"




//entity
class xEnt
{
public:
  xMdx3 * fmesh; //facemesh
  xMdx3 * vmesh; //vertmesh
  unsigned int skin;
  unsigned int skin2;
  int sortCode; 
  int blend; //0 solid   1 alpha test   2 transparent   3 additive
  
  //note -- sortcode is used as if it was unsigned
  //so a -1 is the highest, 0 is the lowest (afaik)

  int twoSide; //2 sided
  int useColor; //use color array from vmesh

  //color is 4 floats -- probably not the best idea
  gamex::cVec3f color;
  float alpha; 

  gamex::cVec3f pos; //translate

  gamex::cVec3f sortpos; //sort by this position

public:
  xEnt()
  {
    fmesh = 0;
    vmesh = 0;
    skin = 0;
    skin2 = 0;
    sortCode = 0;
    alpha = 1;
    color = 1;
    blend = 0;
    useColor = 0;
    twoSide = 0;
  }//ctor

};//xentity


//renderbucket 
//using different buckets
//for different kind of sorting codes 
//(solid is sorted by texture, transparent by distance to camera)

typedef xEnt * tdEntPtr;
class xBucket
{
public:
  tdEntPtr * vecEnt;
  int numEnt;
  int it;

public:
  xBucket(void)
  { 
    vecEnt = 0;
    numEnt = 0;
    it = 0;
  }//ctor


  ~xBucket(void) { clear(); }



  void clear(void)
  {
    if (vecEnt != 0)
    { 
      int i;
      for (i = 0; i < numEnt; i++)
      {   delete vecEnt[i];  }
      delete [] vecEnt;
    }
    vecEnt = 0;
    numEnt = 0;
    it = 0;
  }//clear



  void init(int maxEnt = 16384)
  {
    clear();
    numEnt = maxEnt;
    vecEnt = new tdEntPtr[numEnt];
    int i;
    for (i = 0; i < numEnt; i++) { vecEnt[i] = new xEnt(); }
  }//init



  inline xEnt * addFrame(void)
  { 
    xEnt * a;
    a = vecEnt[it];
    //todo -- reset entity values? (dont need to reset all)
      a->skin = 0;
      a->skin2 = 0;
      a->useColor = 0;
      a->twoSide = 0;

    it += 1;
    if (it >= numEnt) { it = numEnt - 1; } //keep repeating last ent if out of range
    return a;
  }//addframe





  void render(void)
  {
    int i;
    xEnt * a;
    xMdx3 * fmesh;
    xMdx3 * vmesh;
    //float ang;
    int blend;
    unsigned int skin;
    unsigned int skin2;
    int useColor;
    int twoSide;

    //note -- also need to set
    //http://www.opengl.org/wiki/Texture_Combiners

    //http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml

    //http://www.khronos.org/message_boards/showthread.php/6435-BindTexture-vs-ActiveTexture
  
    //http://www.informit.com/articles/article.aspx?p=770639&seqNum=5

    //http://stackoverflow.com/questions/9116494/opengl-gl-modulate-and-multitexturing

    //so the trick is that 
    //there is a seperate activetexture and clientactivetexture
    //function to make things harder

    glEnableClientState(GL_VERTEX_ARRAY);
 
    glActiveTextureARB(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE1);
     glDisable(GL_TEXTURE_2D);

    glClientActiveTextureARB(GL_TEXTURE0);
     glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   
    glClientActiveTextureARB(GL_TEXTURE1);
     glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   
   // glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    //todo -- 2 sided
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);
  
    blend = -1;
    skin = -1;
    skin2 = -1;
    useColor = -1;
    twoSide = -1;

/*
    glActiveTextureARB(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

*/

    int stat_t1 = 0;
    int stat_t2 = 0;

    



    //for testing, everythings two-sided
   // glDisable(GL_CULL_FACE);

    for (i = 0; i < it; i++)
    {
      a = vecEnt[i];
      fmesh = a->fmesh;
      vmesh = a->vmesh;

      if (a->twoSide != twoSide)
      {
        twoSide = a->twoSide;
        if (twoSide == 1)
        { glDisable(GL_CULL_FACE); }
        else { glEnable(GL_CULL_FACE); }
      }//endif

      if (a->skin != skin)
      {
        stat_t1 += 1;

        skin = a->skin;
        glActiveTextureARB(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skin);
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
      }//endif


      if (a->skin2 != skin2)
      {
        stat_t2 += 1;

        skin2 = a->skin2;
        glActiveTextureARB(GL_TEXTURE1);
        if (skin2 == 0) { glDisable(GL_TEXTURE_2D); }
        else {
           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, skin2);
           //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //todo -- set per texture
          //printf("skin2 %d \n", skin2);
        } 

      }//endif




      if (a->blend != blend)
      { 
       // printf("blend switch %d %d \n", blend, a->blend);
        blend = a->blend; 
        
        //blend = 2; //debug
       // if (blend > 0) { return; }
       // blend = 0;
        if (blend < 2) 
        {
          //turn on zbuffer write
          //turn off blending
         // glDepthMask(true);
          glDepthMask(GL_TRUE);
          glDisable(GL_BLEND);
          //is this part not reached??

          if (blend == 1) { glEnable(GL_ALPHA_TEST); } //turn on alpha test
          else { glDisable(GL_ALPHA_TEST); } //turn off alpha test
        }
        else
        {
         /// glDepthMask(GL_FALSE);
         //// glEnable(GL_BLEND);
         // glBlendFunc(GL_ONE, GL_ZERO);

          //turn off write to zbuffer
          //turn off alpha test
          glDepthMask(GL_FALSE);
          glEnable(GL_BLEND);
          if (blend == 2) { glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); } //set blending func to alpha blending
          else { glBlendFunc(GL_SRC_ALPHA, GL_ONE); } //set blending to additive
        } 
      }//endif


      if (a->useColor != useColor)
      {
        useColor = a->useColor;
      
        if (useColor == 1)
        {  glEnableClientState(GL_COLOR_ARRAY); }
        else   { glDisableClientState(GL_COLOR_ARRAY);  }

      }//endif

      //todo -- bind texture, check for changes etc

      if (useColor != 1)
      { glColor4f(a->color.x, a->color.y, a->color.z, a->alpha); }

      
      glPushMatrix();
        glTranslatef(a->pos.x, a->pos.y, a->pos.z);
        //glRotatef(ang*(180.0 / 3.1415), 0, 1, 0);
        //todo -- rotation matrix -- or rotation values


       //todo -- 36 is the vertex struct size -- it will be probably better to use a constant or something for this

       if (vmesh->hasVbo == false || fmesh->hasVbo == false)
       {
       	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
      	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);


        if (useColor == 1)
        { glColorPointer(4, GL_UNSIGNED_BYTE, 36, &(vmesh->vecVert[0].rgba) );  }
     
        glClientActiveTextureARB(GL_TEXTURE0);
          glTexCoordPointer(2, GL_FLOAT, 36, &(vmesh->vecVert[0].u));

        glClientActiveTextureARB(GL_TEXTURE1);
          glTexCoordPointer(2, GL_FLOAT, 36, &(vmesh->vecVert[0].u));

         glVertexPointer(3, GL_FLOAT, 36, &(vmesh->vecVert[0].pos.x));   

  	    glDrawElements(GL_TRIANGLES, fmesh->numFace*3,  GL_UNSIGNED_SHORT, fmesh->vecIndex); 

       }
       else
       {
         //printf("using vbo for drawing \n");

         glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, fmesh->indexBuf); 
         glBindBufferARB(GL_ARRAY_BUFFER_ARB, vmesh->vertBuf);  
       
//vbo stride is tricky..
//should store these in a constant as well
/*
  gamex::cVec3f pos;
  float u, v;
  float u2, v2;
  int norm; //compressed normal
  int rgba; //color
*/

         if (useColor == 1)
         { glColorPointer(4, GL_UNSIGNED_BYTE, 36, (char*)12+8+8+4 );  }
         

          glClientActiveTextureARB(GL_TEXTURE0);
            glTexCoordPointer(2, GL_FLOAT, 36, (char*) 12);

          glClientActiveTextureARB(GL_TEXTURE1);
            glTexCoordPointer(2, GL_FLOAT, 36, (char*)12+8);
            
          glVertexPointer(3, GL_FLOAT, 36, 0); 

          glDrawElements(GL_TRIANGLES, fmesh->numFace*3,  GL_UNSIGNED_SHORT, 0); 
       }//endif



      glPopMatrix();
    }//nexti


    //reset vertex array/pointer settings to default
    
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);


    glActiveTextureARB(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);

    glClientActiveTextureARB(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glActiveTextureARB(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D); 

    glClientActiveTextureARB(GL_TEXTURE1);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glActiveTextureARB(GL_TEXTURE0);

  
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_BLEND);

    //printf("Texture switches:  %d  %d \n", stat_t1, stat_t2);   
  }//render











public:
 void calcSort_Skin(void)
 {
  int i;
  xEnt * a;
  for (i = 0; i < it; i++)
  {
    a = vecEnt[i];
    a->sortCode = ((a->skin & 0xFFFF) << 16) | (a->skin2 &0xFFFF);
  }//nexti

 }//skin


 void calcSort_CamDist(gamex::cVec3f cpos)
 {
  int i;
  xEnt * a;
  gamex::cVec3f d;
  for (i = 0; i < it; i++)
  {
    a = vecEnt[i];
    d = cpos;
    d -= a->sortpos;
    a->sortCode = (int) d.getMagSquared();    
  }//nexti

 }//camdist



public:
 xEnt * tempVec[32768]; //todo -- set max from variable
 int bucket[256];

  void sortEnt(void)
  {
    //radix sort entities
      radixSort(vecEnt, it);
  }//sortent

  void radixSort(xEnt ** vec, int num)
  {
    int i;
    int shift;
    shift = 0;
     
    while (shift < 32)
    {      
      //sometimes for is faster than memset, not really matters this case imho
      memset(bucket, 0, 1024);

      for (i = 0; i < num; i++) { bucket[(vec[i]->sortCode >> shift) & 0xFF]++; }
      for (i = 1; i < 256; i++) {  bucket[i] += bucket[i - 1];  }     
      for (i = num - 1; i >= 0; i--)   { tempVec[--bucket[(vec[i]->sortCode >> shift) & 0xFF]] = vec[i];   }
      for (i = 0; i < num; i++) {  vec[i] = tempVec[i]; }

      shift +=  8;     

    }//wend

  }//radixsort 


};//xbucket





class xRender
{
public:
  xBucket vecBucket[4];
  int numBucket;

public:
  gamex::cQuat camOri;
  gamex::cVec3f camPos;
  gamex::cMat camMat;

public:
  xRender(void)
  {
    numBucket = 4;
  }//ctor
 
  ~xRender(void)
  { clear(); }

  void clear()
  {
    int i;
    for (i = 0; i < numBucket; i++) { vecBucket[i].clear(); }
  }//clear


  void setCam(gamex::cVec3f &pos, gamex::cQuat &ori)
  {
    camPos = pos;
    camOri = ori;
/*
    camOri.setMatrix(camMat.m);
    camMat.m[12] = pos.x;
    camMat.m[13] = pos.y;
    camMat.m[14] = pos.z;
*/
    gamex::setModelView2(camMat, pos, ori); 

  }//setcam


  //maxent is per bucket
  void init(int maxEnt = 16384)
  {
    int i;
    for (i = 0; i < numBucket; i++) { vecBucket[i].init(maxEnt); }
  }//init

  inline xEnt * addFrame(int bucket = 0)
  {
    //note -- no error check, supposed to fail if we are out of range
    //todo -- consider putting in an asset or something 
    return vecBucket[bucket].addFrame();
  }//addframe


  void resetFrame(void)
  {
    int i;
    for (i = 0; i < numBucket; i++) { vecBucket[i].it = 0; }
  }//resetframe

  void render(bool bSort = true)
  {
    int i;

    if (bSort) 
    { 
      vecBucket[0].calcSort_Skin();
      vecBucket[1].calcSort_CamDist(camPos);
      for (i = 0; i < numBucket; i++) { vecBucket[i].sortEnt(); }
    }

    for (i = 0; i < numBucket; i++) { vecBucket[i].render(); }
  }//render

};//xrender

