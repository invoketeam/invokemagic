#pragma warning (disable : 4786)
#pragma warning (disable : 4996) //we dont care that fopen is considered deprecated (probably the new fopen is unsafe shit anyway)


#include "xSkel.h"




xBone::xBone() 
  { 
    scale = 1.0f;
    parent = 0;
    id = 0; 
    depth = 0;
    spec = 0;
  }//ctor


xSkel::xSkel(void) { scale = 1.0f; vecBone = 0; vecVert = 0; numBone = 0; numVert = 0;  }

xSkel::~xSkel(void) { clear(); }

void 
xSkel::clear(void)
  { if (vecBone != 0) { delete [] vecBone; } vecBone = 0; numBone = 0;
    if (vecVert != 0) { delete [] vecVert; } vecVert = 0; numVert = 0; 
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
//    int k;
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



void 
xSkel::copySkel(xSkel * skel)
  {
    clear();

    pos = skel->pos;
    ori = skel->ori;
    scale = skel->scale;
    //baseMat = skel->baseMat; //autogen, no need for copy

    numBone = skel->numBone;
    numVert = skel->numVert;
    
    vecBone = new xBone[numBone];
    vecVert = new xSkVert[numVert];
    
    //we dont store any pointers in them so we can do this safely
    memcpy(vecBone, skel->vecBone, numBone*sizeof(xBone));
    memcpy(vecVert, skel->vecVert, numVert*sizeof(xSkVert));

  }//getcopy



void
xSkel::loadSkel(std::string fname)
  {
    FILE * file;
      file = fopen(fname.c_str(), "rb");
      if (file == 0) { printf("File not found: %s \n",fname.c_str());  return; } //todo -- error message
      readSkel(file);
      fclose(file);
  }//loadskel



void 
xSkel::readSkel(FILE * file)
  {
    std::string skid;
    std::string fid;
    char fileid[13];
    gamex::cVec3f tv;
    gamex::cQuat tq;
    int i;
    xBone * a;


    clear(); //get rid of old data

    //forgot about the ending 0 for strings, so its actually 12 characters without the 0
    skid = "SKELBINF0001";

    //HEADER
    fread(fileid, 12, 1, file);
    fileid[12] = 0;
    fid = fileid;
    if (skid != fid) { printf("xskel: invalid header %s \n", fid.c_str()); return; } //todo -- error invalid file
 
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

  }//readskel




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
xBoneAnim::loadAnim(std::string fname)
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











































