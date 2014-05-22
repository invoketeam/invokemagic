#pragma warning (disable : 4786)

#include "xRender.h"




xEnt::xEnt(void)
  {
    debid = 0;
    fmesh = 0;
    vmesh = 0;
    skin = 0;
    skin2 = 0;
    skinBlend = 0;
    skin2Blend = 0;
    sortCode = 0;
    alpha = 1;
    color = 1;
    blend = 0;
    useColor = 0;
    twoSide = 0;
    useTexMat = 0;
    flags = 0;
    decal = 0;
  }//ctor




 xBucket::xBucket(void)
  { 
    vecEnt = 0;
    numEnt = 0;
    it = 0;
  }//ctor


xBucket::~xBucket(void)   
  {
   clear();
  }//dtor



void 
xBucket::clear(void)
  {
    
   // printf("clear bucket this %p %d \n", this, vecEnt);
    //todo -- vc2010 doesnt seem to like this (but only crashes in debug mode when you exit)

    xEnt * a;
    if (vecEnt != 0)
    { 
      int i;
      for (i = 0; i < numEnt; i++)
      {   
        a = vecEnt[i];
        //printf("debid %d \n", a->debid);
        delete a; 
      }

      delete [] vecEnt;
    }

    vecEnt = 0;
    numEnt = 0;
    it = 0;
    
  }//clear





void 
xBucket::init(int maxEnt)
  {
    clear();

    numEnt = maxEnt;
    vecEnt = new tdEntPtr[numEnt];

    int i;
    for (i = 0; i < numEnt; i++) { vecEnt[i] = new xEnt(); vecEnt[i]->debid = i; }
  }//init



xEnt * 
xBucket::addFrame(void)
  { 
    xEnt * a;
    a = vecEnt[it];
    //todo -- reset entity values? (dont need to reset all)
      a->skin = 0; a->skinBlend = 0;
      a->skin2 = 0; a->skin2Blend = 0;
      a->useColor = 0;
      a->twoSide = 0;
      a->useTexMat = 0;
      a->flags = 0;

      //reset orientation and scale
      a->useTransMat = 0;
      a->ori.reset();
      a->scale = 1.0f;
      //position is expected to be always set (for now)

      a->decal = 0;

    it += 1;
    if (it >= numEnt) { it = numEnt - 1; } //keep repeating last ent if out of range
    return a;
  }//addframe





inline static void setSkinBlendMode(int mode)
{

  //a switch of 0,1,2,3 etc.. is supposed to be compiled into a jump table instead of series of ifs
  // aka yay we are saving microseconds maybe

  switch (mode)
  {
    case 0:      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);    break;

    case 1:      glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );    break;

    case 2:      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);    break;

    case 3:      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    break;

    case 4:

     //combiners dont give as much freedom as shaders but certainly less hassle

     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
     
     glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
     glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
     
     glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
     glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA); 

    break;


  };//swenc

}//setskinblendmode



void 
xBucket::render(void)
  {
    int i;
    xEnt * a;
    xMdx3 * fmesh;
    xMdx3 * vmesh;
    //float ang;
    int blend;
    unsigned int skin;
    unsigned int skin2;
    int skinBlend;
    int skin2Blend;
    int useColor;
    int twoSide;
    int texMat;
    int decal;


    //note -- also need to set
    //http://www.opengl.org/wiki/Texture_Combiners

    //http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml

    //http://www.khronos.org/message_boards/showthread.php/6435-BindTexture-vs-ActiveTexture
  
    //http://www.informit.com/articles/article.aspx?p=770639&seqNum=5

    //http://stackoverflow.com/questions/9116494/opengl-gl-modulate-and-multitexturing

    //so the trick is that 
    //there is a seperate activetexture and clientactivetexture
    //function to make things harder

    //default shade
    glShadeModel(GL_SMOOTH);

    glEnableClientState(GL_VERTEX_ARRAY);

 
    glActiveTextureARB(GL_TEXTURE0);    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, 0);   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glActiveTextureARB(GL_TEXTURE1);    glDisable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, 0); 

    glClientActiveTextureARB(GL_TEXTURE0);  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTextureARB(GL_TEXTURE1);  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   
   // glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);   glDepthMask(GL_TRUE);

    //todo -- 2 sided
    glEnable(GL_CULL_FACE);   glCullFace(GL_BACK);

    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);

    blend = -1;
    skin = -1;
    skin2 = -1;
    useColor = -1;
    twoSide = -1;
    skinBlend = -1;
    skin2Blend = -1;
    texMat = -1;
    decal = -1;

/*
    glActiveTextureARB(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

*/

    glMatrixMode(GL_TEXTURE); glLoadIdentity();
  

	  glMatrixMode(GL_MODELVIEW);
	 

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
        if (twoSide == 1) { glDisable(GL_CULL_FACE); }  else { glEnable(GL_CULL_FACE); }
      }//endif


      if (a->decal != decal)
      {
        decal = a->decal;
        if (decal == 0) { glDisable(GL_POLYGON_OFFSET_FILL); } else {  glEnable(GL_POLYGON_OFFSET_FILL);   glPolygonOffset(-1.0, -1.0); }
      }//endif
    


      if (a->skin != skin)
      {
        
        skin = a->skin;
        glActiveTextureARB(GL_TEXTURE0);  glBindTexture(GL_TEXTURE_2D, skin);

      }//endif


      if (a->skin2 != skin2)
      {
       
        skin2 = a->skin2;
        glActiveTextureARB(GL_TEXTURE1);
        if (skin2 == 0) { glDisable(GL_TEXTURE_2D); }
        else { glEnable(GL_TEXTURE_2D);   glBindTexture(GL_TEXTURE_2D, skin2); } 
      }//endif


      if (a->skinBlend != skinBlend)
      {
        skinBlend = a->skinBlend;
        glActiveTextureARB(GL_TEXTURE0);
        setSkinBlendMode(skinBlend);

        //i should probably use a function for this, but whatever
      }//endif


      if (skin2 != 0)
      if (a->skin2Blend != skin2Blend)
      {
        skin2Blend = a->skin2Blend;
        glActiveTextureARB(GL_TEXTURE1);
        setSkinBlendMode(skin2Blend);

      }//endif

      //texture matrix
      if (a->useTexMat != texMat || a->useTexMat == 1)
      {

        glMatrixMode(GL_TEXTURE);
        texMat = a->useTexMat;
        if (texMat == 0)   { glLoadIdentity(); }
         else    { glLoadMatrixf(a->texMat.m); }

         //restore to modelview matrix mode
         glMatrixMode(GL_MODELVIEW);

      }//endif





      if (a->blend != blend)
      { 
       
        blend = a->blend; 
        if (blend < 2) 
        {
          //turn on zbuffer write
          //turn off blending
          glDepthMask(GL_TRUE);
          glDisable(GL_BLEND);

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
        else   {  glDisableClientState(GL_COLOR_ARRAY);  }

      }//endif


   
      if (useColor != 1)
      { glColor4f(a->color.x, a->color.y, a->color.z, a->alpha); }

      

      glPushMatrix();
        if (a->useTransMat)
        {
          glMultMatrixf(a->transMat.m);
        }
        else
        {
          //only use transmat for rotation
          a->ori.setMatrix(a->transMat.m);
            glTranslatef(a->pos.x, a->pos.y, a->pos.z);
             glMultMatrixf(a->transMat.m);
              glScalef(a->scale.x, a->scale.y, a->scale.z);
        }


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
          glTexCoordPointer(2, GL_FLOAT, 36, &(vmesh->vecVert[0].u2));

         glVertexPointer(3, GL_FLOAT, 36, &(vmesh->vecVert[0].pos.x));   

        //if (fmesh->drawFace > fmesh->numFace) { fmesh->drawFace = fmesh->numFace; }

  	    glDrawElements(GL_TRIANGLES, fmesh->drawFace*3,  GL_UNSIGNED_INT, fmesh->vecIndex); 
  	    //glDrawElements(GL_TRIANGLES, fmesh->numFace*3,  GL_UNSIGNED_INT, fmesh->vecIndex); 


       }
       else
       {
         //printf("using vbo for drawing \n");

         glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, fmesh->indexBuf); 
         glBindBufferARB(GL_ARRAY_BUFFER_ARB, vmesh->vertBuf);  
       
                //vbo stride is tricky..
                //should store the size of these in a constant as well
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

        //if (fmesh->drawFace > fmesh->numFace) { fmesh->drawFace = fmesh->numFace; }
          glDrawElements(GL_TRIANGLES, fmesh->drawFace*3,  GL_UNSIGNED_INT, 0); 
       }//endif



      glPopMatrix();
    }//nexti


    glMatrixMode(GL_TEXTURE); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    //reset vertex array/pointer settings to default
    
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);


    glActiveTextureARB(GL_TEXTURE0);    glDisable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, 0);   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glActiveTextureARB(GL_TEXTURE1);    glDisable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, 0);  
    glActiveTextureARB(GL_TEXTURE0); 

    glClientActiveTextureARB(GL_TEXTURE0);      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTextureARB(GL_TEXTURE1);      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTextureARB(GL_TEXTURE0);

 
    glDisable(GL_CULL_FACE);    glCullFace(GL_BACK);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);

    glColor4f(1,1,1,1);
    //printf("Texture switches:  %d  %d \n", stat_t1, stat_t2);   
  }//render






void xBucket::simpRender(int flag)
{

  //glShadeModel(GL_FLAT); //for speed 





 	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
  
  glEnableClientState(GL_VERTEX_ARRAY);   
  
  glMatrixMode(GL_MODELVIEW);

  
  int i;
  xEnt * a;
  xMdx3 * fmesh;
  xMdx3 * vmesh;

    for (i = 0; i < it; i++)
    {
      a = vecEnt[i];
      
        if ( (a->flags & flag) <= 0) { continue; } 
  

      fmesh = a->fmesh;
      vmesh = a->vmesh;

      glPushMatrix();
        if (a->useTransMat)
        {        glMultMatrixf(a->transMat.m);      }
        else
        {
         
          a->ori.setMatrix(a->transMat.m); // use transmat for rotation
           glTranslatef(a->pos.x, a->pos.y, a->pos.z);
             glMultMatrixf(a->transMat.m);
              glScalef(a->scale.x, a->scale.y, a->scale.z);
        }//endif
  
    //todo -- vertex buffer

       glVertexPointer(3, GL_FLOAT, 36, &(vmesh->vecVert[0].pos.x));   
       glDrawElements(GL_TRIANGLES, fmesh->drawFace*3,  GL_UNSIGNED_INT, fmesh->vecIndex); 

      glPopMatrix();
  }//nexti



  glDisableClientState(GL_VERTEX_ARRAY);   
  
}//simprender














void 
xBucket::calcSort_Skin(void)
 {
  int i;
  xEnt * a;
  for (i = 0; i < it; i++)
  {
    a = vecEnt[i];
    a->sortCode = ((a->skin & 0xFFFF) << 16) | (a->skin2 &0xFFFF);
  }//nexti

 }//skin


void 
xBucket::calcSort_CamDist(gamex::cVec3f cpos)
 {
  int i;
  xEnt * a;
  gamex::cVec3f d;
  for (i = 0; i < it; i++)
  {
    a = vecEnt[i];
    d = cpos;
    d -= a->sortpos;
    a->sortCode = 0x0FFffFF - (int) (d.getMagSquared() * 0.1f); 

  }//nexti

 }//camdist



void 
xBucket::sortEnt(void)
  {
    //radix sort entities
      radixSort(vecEnt, it);
  }//sortent


void 
xBucket::radixSort(xEnt ** vec, int num)
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





xRender::xRender(void)
  {
    numBucket = 4;
  }//ctor
 
xRender::~xRender(void)
  { clear(); }


void 
xRender::clear()
  {
    int i;
    for (i = 0; i < numBucket; i++) { vecBucket[i].clear(); }
  }//clear


void 
xRender::setCam(gamex::cVec3f &pos, gamex::cQuat &ori)
  {
    camPos = pos;
    camOri = ori;
/*
    camOri.setMatrix(camMat.m);
    camMat.m[12] = pos.x;
    camMat.m[13] = pos.y;
    camMat.m[14] = pos.z;
*/
    camMat.setView(&pos, &ori);
    //gamex::setModelView2(camMat, pos, ori); 

  }//setcam




  //maxent is per bucket
void 
xRender::init(int maxEnt)
  {
    int i;
    for (i = 0; i < numBucket; i++) { vecBucket[i].init(maxEnt); }
  }//init


xEnt * 
xRender::addFrame(int bucket)
  {
    //note -- no error check, supposed to fail if we are out of range
    //todo -- consider putting in an assert or something 
    return vecBucket[bucket].addFrame();
  }//addframe


void 
xRender::resetFrame(void)
  {
    int i;
    for (i = 0; i < numBucket; i++) { vecBucket[i].it = 0; }
  }//resetframe

void 
xRender::render(bool bSort)
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


void
 xRender::renderBucket0(void)
{
    vecBucket[0].calcSort_Skin();
    vecBucket[0].sortEnt();
    vecBucket[0].render(); 
}//render


void 
xRender::renderBucket1(void)
{
  vecBucket[1].calcSort_CamDist(camPos);
  vecBucket[1].sortEnt();
  vecBucket[1].render(); 
}//render2




void
xRender::simpRender(int flag)
  {
    //only render solids
      vecBucket[0].simpRender(flag);

  }//simprender



