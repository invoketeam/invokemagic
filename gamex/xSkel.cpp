#pragma warning (disable : 4786)
#pragma warning (disable : 4996) //we dont care that fopen is considered deprecated (probably the new fopen is unsafe shit anyway)


#include "xSkel.h"

#include "xGLCommon.h"

xBone::xBone() 
  { 
    scale = 1.0f;
    parent = 0;
    id = 0; 
    depth = 0;
    spec = 0;
   
  }//ctorbone


 
xSkel::xSkel(void) 
{ 
  scale = 1.0f; 
  vecBone = 0; 
  vecVert = 0; 
  numBone = 0; 
  numVert = 0;  
  bCopied = false;
}//ctorskel


xSkel::~xSkel(void) { clear(); }

void 
xSkel::clear(void)
  { if (vecBone != 0) { delete [] vecBone; } vecBone = 0; numBone = 0;

    if (bCopied == false)
    { if (vecVert != 0) { delete [] vecVert; }  }
    
    vecVert = 0; numVert = 0; 
    bCopied = false;
  }//clear



void 
xSkel::update(void)
  {
    int i; //, num;
    xBone * a;
    xBone * p;
    float * mat;
    gamex::cMat * pmat;
    gamex::cQuat * pori;
    gamex::cMat tempMat;
    
    mat= baseMat.m;
    ori.setMatrix(mat);
      
        mat[12] = pos.x;      mat[13] = pos.y;       mat[14] = pos.z;

        mat[0] *= scale.x;    mat[1] *= scale.x;     mat[2] *= scale.x;        
        mat[4] *= scale.y;    mat[5] *= scale.y;     mat[6] *= scale.y;
        mat[8] *= scale.z;    mat[9] *= scale.z;     mat[10] *=scale.z;
             
      for (i = 0; i < numBone; i++)
      {
        a = &vecBone[i];
        
        if (a->depth <= 0)
        {
          //no parent bone: use root as parent
          pmat = &baseMat;
          pori = &ori;
        }
        else
        {
          p = &vecBone[a->parent];
          pmat = &p->transMat;
          pori = &p->wori;
        }
        
          mat = tempMat.m; 
          a->ori.setMatrix(mat);
          mat[12] = a->pos.x;            mat[13] = a->pos.y;            mat[14] = a->pos.z;
          mat[0] *= a->scale.x;          mat[1] *= a->scale.x;          mat[2] *= a->scale.x;  
          mat[4] *= a->scale.y;          mat[5] *= a->scale.y;          mat[6] *= a->scale.y;    
          mat[8] *= a->scale.z;          mat[9] *= a->scale.z;          mat[10] *= a->scale.z;

          gamex::cMat::multMatrix(*(pmat), tempMat,  a->transMat);
         
          //ori and position for attachments
          a->wori = *pori;
          a->wori.mul(a->ori);
          a->wori.normalise();
          a->wpos.set(a->transMat.m[12], a->transMat.m[13], a->transMat.m[14]);

          //matrix for vertices
          gamex::cMat::multMatrix(a->transMat, a->skinMat, a->vertMat); 
        
      }//nexti
  }//update



void 
xSkel::applySkin(xMdx3 * mesh, int num)
  {
    int i;
    xSkVert * a;
    xBone * b;
    float * mat;
    float rx, ry, rz;  
    mVert * vec;
    gamex::cVec3f * vp;
    vec = mesh->vecVert;
    
      for (i = 0; i < num; i++)
      {
        a = &(vecVert[i]);
        b = &(vecBone[a->bone[0]]);
        vp = &(vec[i].pos);

        rx = a->pos.x;   ry = a->pos.y;    rz = a->pos.z;     
        mat = b->vertMat.m;

        vp->x = mat[0] * rx + mat[4] * ry + mat[8] * rz + mat[12];  
        vp->y = mat[1] * rx + mat[5] * ry + mat[9] * rz + mat[13];  
        vp->z = mat[2] * rx + mat[6] * ry + mat[10] * rz + mat[14];  

      }//nexti    
  }//applyskin




//todo -- memory friendly copy: only store the pointer to vecVert, indicate this skeleton is not the sole owner (so it doesnt get cleared)
//of course if you attempt to use the copied one after it was deleted you will crash as that will be not intended use
//(optional) for now as in the age of 2 giga rams minimum its not something to worry about (about 50kb extra per character)

void 
xSkel::copySkel(xSkel * skel, bool deepCopy)
  {
    clear();

    pos = skel->pos;
    ori = skel->ori;
    scale = skel->scale;
    //baseMat = skel->baseMat; //autogen, no need for copy

    numBone = skel->numBone;
    vecBone = new xBone[numBone];
    //we dont store any pointers in them so we can do this safely
    memcpy(vecBone, skel->vecBone, numBone*sizeof(xBone));



    //todo -- option to shallow copy skin vertices
    numVert = skel->numVert;
    
    if (deepCopy)
    {
      vecVert = new xSkVert[numVert];
      memcpy(vecVert, skel->vecVert, numVert*sizeof(xSkVert));
    }
     else 
    {
      vecVert = skel->vecVert;
      bCopied = true;
    }//endif

  }//getcopy



void
xSkel::loadFile(std::string fname)
  {
    std::string fid;
    char fileid[13];

    FILE * file;
      file = fopen(fname.c_str(), "rb");
      if (file == 0) { printf("File not found: %s \n",fname.c_str());  return; } //todo -- error message
      
      //FILEID
        fread(fileid, 12, 1, file);
        fileid[12] = 0; //fix for old format
        fid = fileid;
        
        if (fid == "SKELBINF0001") { readSkelV1(file); }
        else if (fid == "SKELBINF002") { readSkelV2(file); }
        else { printf("xskel: invalid header %s \n", fid.c_str());  fclose(file); return; }

      fclose(file);
  }//loadskel
  
  
  
void
xSkel::readSkelV2(FILE * file)
  {
    int i;
    xBone * a;

    clear(); //get rid of old data
    
    fread(&numBone, 4,1, file);
    fread(&numVert, 4,1, file);
    fseek ( file , 16 , SEEK_CUR); //skip reserved (16 bytes)

    //printf("Readskelv2 %d %d \n", numBone, numVert);

    vecBone = new xBone[numBone];
    vecVert = new xSkVert[numVert];
    
    //SKIN POSE
      for (i = 0; i < numBone; i++)
      {
        a = &vecBone[i];
        fread(&a->id, 4, 1, file);
        fread(&a->parent, 4, 1, file);    
        fread(&a->depth, 4, 1, file);
        fread(&a->pos, 12, 1, file);
        fread(&a->ori, 16, 1, file);  
        fseek(file , 12 , SEEK_CUR); //skip scale
        a->scale = 1.0f;
        
      }//nexti


      //char test[8];
     // fread(test, 8,1,file);
      //printf("test %s \n", test);
     fseek ( file , 8 , SEEK_CUR); //skip 8 bytes "SKINPAR"+0


    //VERTEX WEIGHTS
      fread(vecVert, numVert, sizeof(xSkVert), file);
    
    //GENERATE SKIN POSE
    //note: if the skeleton is not valid we are gonna crash here

      update();
       
      for (i = 0; i < numBone; i++)
      {
        a = &vecBone[i];
        a->makeSkinMat(a->wpos, a->wori);
         // a->wori.invert();
          //a->wori.setMatrix(a->skinMat.m);
         // a->skinMat.m[12] = -a->wpos.x;
         // a->skinMat.m[13] = -a->wpos.y;
         // a->skinMat.m[14] = -a->wpos.z;
      }//nexti2
  }//readskelv2


void 
xSkel::readSkelV1(FILE * file)
  {
    gamex::cVec3f tv;
    gamex::cQuat tq;
    int i;
    xBone * a;


    clear(); //get rid of old data

    //HEADER
    fseek ( file , 4 , SEEK_CUR); //skip version
    fread(&numBone, 4,1, file);
    fread(&numVert, 4,1, file);
    fseek ( file , 4 , SEEK_CUR); //skip rigid
    fseek ( file , 64 , SEEK_CUR); //skip reserved[16]

    //printf("numbone numvert %d %d \n", numBone, numVert); 

    //DATA

     vecBone = new xBone[numBone];
     vecVert = new xSkVert[numVert];
    

    //SKIN POSE
    for (i = 0; i < numBone; i++)
    {
      a = &vecBone[i];
      fread(&a->id, 4, 1, file);
      fread(&a->parent, 4, 1, file);    
      fread(&a->depth, 4, 1, file);
      fread(&tv, 12, 1, file);
      fread(&tq, 16, 1, file);
      fseek(file , 12 , SEEK_CUR); //skip scale

      //set skin matrix (ori and pos stored inverted)
      tq.setMatrix(a->skinMat.m);
      a->skinMat.m[12] = tv.x;
      a->skinMat.m[13] = tv.y;
      a->skinMat.m[14] = tv.z;

    }//nexti   

    //DEFAULT POSE
    for (i = 0; i < numBone; i++)
    {
      a = &vecBone[i];
      fseek(file , 12 , SEEK_CUR); //skip id,parent, depth
      fread(&a->pos, 12, 1, file);
      fread(&a->ori, 16, 1, file);
      fseek(file , 12 , SEEK_CUR); //skip scale
      a->scale = 1.0f;
    }//nexti2

    //VERTEX WEIGHTS
      fread(vecVert, numVert, sizeof(xSkVert), file);

  }//readskelv1




xBoneAnim::xBoneAnim(void) { numBone = 0; numFrame = 0; vecFrame = 0; }

xBoneAnim::~xBoneAnim(void) { clear(); } 


void 
xBoneAnim::clear(void)
  {
    if (vecFrame != 0) { delete [] vecFrame; } vecFrame = 0;
    numBone = 0; numFrame = 0;
  }//clear



void 
xBoneAnim::applyFrame(int frame, xSkel * skel)
  {
   // if (frame < 0 || frame >= numFrame) { printf("Warning: Frame out of range %d \n", frame); }
    
    int k, i;  
    xBone * a;
    xBoneFrame * f;
    xBone * vec;

    vec = skel->vecBone;
      
    k = frame * numBone;
      
      for (i = 0; i < numBone; i++)
      {
        a = &(vec[i]);
        f = &(vecFrame[k + i]);
        
        a->pos = f->pos;
        a->ori = f->ori;
      }//nexti
  }//applyframe


  
void 
xBoneAnim::applyFrame2(float frame, xSkel * skel)
  {
    //if (frame < 0 || frame >= numFrame) { printf("Warning: Frame out of range %0.2f \n", frame); }
    
    float u;
    int sf, nf; //startframe nextframe
    int k, i, w;  
    xBone * a;
    xBoneFrame * f;
    xBoneFrame * f2;
    xBone * vec;

    sf = (int) floor(frame);
    u = frame - sf;
    if (u == 0.0f) { applyFrame(sf, skel); return; }
    nf = sf + 1;
    if (nf >= numFrame) {nf -= numFrame; } //assuming animation loops 

    vec = skel->vecBone;
      
      k = sf * numBone;
      w = nf * numBone; 
      
      for (i = 0; i < numBone; i++)
      {
        a = &(vec[i]);
        f = &(vecFrame[k + i]);
        f2 = &(vecFrame[w + i]);
     
        //a->pos = f.pos + (f2.pos - f.pos) * u;
        a->pos.x = f->pos.x + (f2->pos.x - f->pos.x) * u;
        a->pos.y = f->pos.y + (f2->pos.y - f->pos.y) * u;
        a->pos.z = f->pos.z + (f2->pos.z - f->pos.z) * u;

        //a.ori.nlerp(f.ori, f2.ori, u);
        a->ori.nlerp(f->ori, f2->ori, u);
      }//nexti
  }//applyframe2



void 
xBoneAnim::loadFile(std::string fname)
  {
    FILE * file;
      file = fopen(fname.c_str(), "rb");
      if (file == 0) { printf("File not found: %s \n",fname.c_str());  return; } //todo -- error message
      readAnim(file);
    fclose(file);
  }//loadanim



void 
xBoneAnim::readAnim(FILE * file)
  {
    int num;


    clear();

    fseek(file , 12 , SEEK_CUR); //skip identifier (for now)

    fread(&numBone, 4, 1, file);
    fread(&numFrame, 4, 1, file);

    fseek(file , 16 , SEEK_CUR); //skip reserved

    num = numBone * numFrame;

    vecFrame = new xBoneFrame[num];
    fread(vecFrame, sizeof(xBoneFrame), num, file);
 
  }//readanim
  
  
  
void 
xBone::makeSkinMat(gamex::cVec3f &p, gamex::cQuat &o)
  {

    //reference
    //https://github.com/openfl/openfl-native/blob/master/flash/geom/Matrix3D.hx
        
    float * raw;
    float d;
        
      raw = skinMat.m;

      o.setMatrix(raw);
      raw[12] = p.x;        
      raw[13] = p.y;      
      raw[14] = p.z;

      
      //determinant      
      d = -1 * ((raw[0] * raw[5] - raw[4] * raw[1]) * (raw[10] * raw[15] - raw[14] * raw[11])
      - (raw[0] * raw[9] - raw[8] * raw[1]) * (raw[6] * raw[15] - raw[14] * raw[7])
      + (raw[0] * raw[13] - raw[12] * raw[1]) * (raw[6] * raw[11] - raw[10] * raw[7])
      + (raw[4] * raw[9] - raw[8] * raw[5]) * (raw[2] * raw[15] - raw[14] * raw[3])
      - (raw[4] * raw[13] - raw[12] * raw[5]) * (raw[2] * raw[11] - raw[10] * raw[3])
      + (raw[8] * raw[13] - raw[12] * raw[9]) * (raw[2] * raw[7] - raw[6] * raw[3]));
      
      //is it invertible -- in theory this is not supposed to happen because the way we made the matrix
      if (((fabs(d)) > 0.00000000001f) == false) { printf("warning -- determinant problem [%f] [%f] \n", d, abs(d) ); return; }
      

      //invert the matrix
      d = -1 / d;
      float m11 = raw[0]; float m21 = raw[4]; float m31 = raw[8]; float m41 = raw[12];
      float m12 = raw[1]; float m22 = raw[5]; float m32 = raw[9]; float m42 = raw[13];
      float m13 = raw[2]; float m23 = raw[6]; float m33 = raw[10]; float m43 = raw[14];
      float m14 = raw[3]; float m24 = raw[7]; float m34 = raw[11]; float m44 = raw[15];

      raw[0] = d * (m22 * (m33 * m44 - m43 * m34) - m32 * (m23 * m44 - m43 * m24) + m42 * (m23 * m34 - m33 * m24));
      raw[1] = -d * (m12 * (m33 * m44 - m43 * m34) - m32 * (m13 * m44 - m43 * m14) + m42 * (m13 * m34 - m33 * m14));
      raw[2] = d * (m12 * (m23 * m44 - m43 * m24) - m22 * (m13 * m44 - m43 * m14) + m42 * (m13 * m24 - m23 * m14));
      raw[3] = -d * (m12 * (m23 * m34 - m33 * m24) - m22 * (m13 * m34 - m33 * m14) + m32 * (m13 * m24 - m23 * m14));
      raw[4] = -d * (m21 * (m33 * m44 - m43 * m34) - m31 * (m23 * m44 - m43 * m24) + m41 * (m23 * m34 - m33 * m24));
      raw[5] = d * (m11 * (m33 * m44 - m43 * m34) - m31 * (m13 * m44 - m43 * m14) + m41 * (m13 * m34 - m33 * m14));
      raw[6] = -d * (m11 * (m23 * m44 - m43 * m24) - m21 * (m13 * m44 - m43 * m14) + m41 * (m13 * m24 - m23 * m14));
      raw[7] = d * (m11 * (m23 * m34 - m33 * m24) - m21 * (m13 * m34 - m33 * m14) + m31 * (m13 * m24 - m23 * m14));
      raw[8] = d * (m21 * (m32 * m44 - m42 * m34) - m31 * (m22 * m44 - m42 * m24) + m41 * (m22 * m34 - m32 * m24));
      raw[9] = -d * (m11 * (m32 * m44 - m42 * m34) - m31 * (m12 * m44 - m42 * m14) + m41 * (m12 * m34 - m32 * m14));
      raw[10] = d * (m11 * (m22 * m44 - m42 * m24) - m21 * (m12 * m44 - m42 * m14) + m41 * (m12 * m24 - m22 * m14));
      raw[11] = -d * (m11 * (m22 * m34 - m32 * m24) - m21 * (m12 * m34 - m32 * m14) + m31 * (m12 * m24 - m22 * m14));
      raw[12] = -d * (m21 * (m32 * m43 - m42 * m33) - m31 * (m22 * m43 - m42 * m23) + m41 * (m22 * m33 - m32 * m23));
      raw[13] = d * (m11 * (m32 * m43 - m42 * m33) - m31 * (m12 * m43 - m42 * m13) + m41 * (m12 * m33 - m32 * m13));
      raw[14] = -d * (m11 * (m22 * m43 - m42 * m23) - m21 * (m12 * m43 - m42 * m13) + m41 * (m12 * m23 - m22 * m13));
      raw[15] = d * (m11 * (m22 * m33 - m32 * m23) - m21 * (m12 * m33 - m32 * m13) + m31 * (m12 * m23 - m22 * m13)); 

  }//makeskinmat







void 
xSkel::debRender(void)
  {
    int i;
    xBone * a;
    xBone * p;

      glBegin(GL_LINES);
        for (i = 0; i < numBone; i++)
        {
          a = &vecBone[i];
          if (a->depth == 0) { continue; }
          p = &vecBone[a->parent];

          glVertex3f(a->wpos.x, a->wpos.y, a->wpos.z);
          glVertex3f(p->wpos.x, p->wpos.y, p->wpos.z);

        }//nexti
      glEnd();


      glPointSize(8);
      glBegin(GL_POINTS);
        for (i = 0; i < numBone; i++)
        {
          a = &vecBone[i];
          //glVertex3f(a->pos.x, a->pos.y, a->pos.z);
          glVertex3f(a->wpos.x, a->wpos.y, a->wpos.z);

        }//nexti
      glEnd();


/*
       for (i = 0; i < numBone; i++)
        {
          a = &vecBone[i];
          glPushMatrix();
            glMultMatrixf(a->transMat.m);
            drawCube2(a->bmin.x, a->bmin.y, a->bmin.z, a->bmax.x, a->bmax.y, a->bmax.z);
          glPopMatrix();
        }//nexti
*/


   glPointSize(4);
      glColor3f(1,0,0);
    glBegin(GL_POINTS);

      for (i = 0; i < numVert; i++)
      {
        glVertex3f( vecVert[i].pos.x,vecVert[i].pos.y,vecVert[i].pos.z);
      }//nexti
     glEnd();



   glPointSize(1);
  }//render
































