#pragma warning (disable : 4786)
#pragma once

#include "xPartSys.h"

#include "xGLCommon.h"


xPartSprite::xPartSprite(void)
{
  skin = 0;
}//ctor

void
xPartSprite::addSprite(xSprite * a) { vecSprite.push_back(a); }

xPart * 
xPartSprite::getAnim(std::string wname) { return storeAnim.getData(wname); }

xPart *
xPartSprite::addAnim(xSpriteMan * sman, std::string wname, float speed, std::string frames)
{
  //based  on xSprite.cpp xAnim setFrames
     
  xSprite * a;
  xPart * p;
  std::string tmp;
  int f;  int i;  int num;  int e;

  num = (int) frames.length();
      
  p = new xPart();
  storeAnim.addData(wname, p);
  p->startFrame = (float) vecSprite.size();
  p->curFrame = p->startFrame;
  p->animSpeed = speed; 
  for (i = 0; i < num; i++)
  {
    //skip spaces at start
    if (frames[i] == ' ') { continue; }
    f = (int) frames.find_first_of(',', i);
    if (f == -1) { f = num; } 
    tmp = frames.substr(i, f - i);
    //remove spaces from end
      e = (int) tmp.find_first_of(' ', 0); tmp = tmp.substr(0, e);
    //printf("Found frame: [%s] \n", tmp.c_str() );
    i = f;        
    a = sman->getSprite(tmp);
    if (a == 0) { printf("xPartSys: frame not found [%s] \n", tmp.c_str() );}
    skin = a->handle; //also rem: only use sprites that belong to a single texture
    vecSprite.push_back(a);
  }//nexti 
  p->endFrame = (float) vecSprite.size() - 1;

  return p;
}//addanim
 






xPart::xPart(void)
  {
   sortCode = 0;
   size = 1; hp = 1; ang = 0; image = -1;
   fade_size = 0;
   ang_speed = 0;
   animSpeed = 0.0f;
   curFrame = 0.0f;
   startFrame = 0.0f;
   endFrame = 0.0f;
   length = 0.0f;
   fadehp = 100.0f;
  }//ctor











xPartSys::xPartSys(void) 
  {
    numPart = 0;
    maxPart = 0;
    vecPart = 0; pindex = 0;
    sprAnim = 0;
    it = 0;
    skin = 0;
  }//ctor

xPartSys::~xPartSys(void) { clear(); }

void 
xPartSys::clear(void)
  {
   mesh.clear();
    if (vecPart != 0) { delete [] vecPart; } vecPart = 0;
    if (pindex != 0) { delete [] pindex; } pindex = 0;
    numPart = 0; maxPart = 0;
    it = 0;
    sprAnim = 0;
  }//clear



void 
xPartSys::initPart(int maxp)
  { 
    mVert * vecVert;
    int * vecIndex;
    int i;    int k;    int num;
  
    clear(); //does mesh.clear

    maxPart = maxp;
    if (maxPart < 16) { maxPart = 16; }
    if (maxPart > XPART_MAXPART) { maxPart = XPART_MAXPART; }

    mesh.numFace = maxPart * 2;
    mesh.numVert = maxPart * 4;
       
    vecVert = new mVert[mesh.numVert];
    vecIndex = new int[mesh.numFace*3];
    vecPart = new xPart[maxPart];

    pindex = new xPart*[maxPart];
    for (i = 0; i < maxPart; i++) { pindex[i] = &vecPart[i];}

    mesh.vecVert = vecVert;
    mesh.vecIndex = vecIndex;
  
    k = 0;    num = (mesh.numFace * 3);
    for (i = 0; i < num; i+=6)
    {
      vecIndex[i] = k+0;     vecIndex[i+1] = k+1;      vecIndex[i+2] = k+2;
      vecIndex[i+3] = k+2;   vecIndex[i+4] = k+1;      vecIndex[i+5] = k+3;
      k+=4;
    }//nexti
 
  }//init





xPart * 
xPartSys::addPart(float cx, float cy, float cz)
  {
    xPart * a;
      a = &(vecPart[it]);
      a->hp = 100;      a->vel = 0;
      a->pos.set(cx, cy, cz);   
      a->size = 8;
      a->ang = 0; 
      if (sprAnim) { a->image = 0;  }
      else { a->image = -1; }
      a->animSpeed = 0.0f;
      a->curFrame = 0.0f;
      a->fade_size = 0; //-0.2;
      a->ang_speed = 0; //.01;
      a->fadehp = 100.0f;

      it += 1; 	  if (it >= maxPart) { it = 0; }   
    return a;
  }//addpart
 


xPart * 
xPartSys:: addPart2(std::string wanim, float cx, float cy, float cz)
  {
    if (sprAnim == 0) { } //todo -- error/warning message

    xPart * m;
    xPart * a;
      m = sprAnim->getAnim(wanim);
      a = &(vecPart[it]);
      a->image = 0;  

      a->pos.set(cx, cy, cz);   
      a->hp = m->hp;
      a->vel = m->vel;
      a->size = m->size;
      a->ang = m->ang; 
      a->animSpeed = m->animSpeed;
      a->curFrame = m->curFrame;
      a->startFrame = m->startFrame;
      a->endFrame = m->endFrame;
      a->fade_size = m->fade_size; 
      a->ang_speed = m->ang_speed; 
      a->fadehp = 100.0f;

      it += 1; 	  if (it >= maxPart) { it = 0; }   
    return a;
  }//addpart2



void 
xPartSys::update(void)
  {
   int i;
   xPart * a;
   
    for (i = 0; i < maxPart; i++)
    {
      a = &(vecPart[i]);
      if (a->hp <= 0) { continue; }

      a->pos += a->vel;   
        a->vel += grav; //new -- add gravity
      a->hp -= 1;
      a->size += a->fade_size;
      if (a->size <= 0) { a->hp = -1; continue;}
      a->ang += a->ang_speed;

      if (a->image != -1)
      {
        a->curFrame += a->animSpeed;
        if (a->curFrame >= a->endFrame) { a->curFrame = a->startFrame; }
        a->image = (int) floor(a->curFrame);
      }
    }//nexti

  }//update



void 
xPartSys::updateMesh(float * mat)
  {
    xPart * a;
    int i, k, n, num;
    float rx, ry; //, ang;	
    float ca, sa;
    gamex::cVec3f * p;			
    float raw0, raw1, raw4, raw5, raw8, raw9;
    mVert * vecVert;
    xSprite * sr;
    int wrgba;

    vecVert = mesh.vecVert;

    
    int numimg;
    numimg = 0;

    if (sprAnim != 0)
    {
      numimg = (int) sprAnim->vecSprite.size();
    }//endif

    raw0=mat[0];    raw1=mat[1];
    raw4=mat[4];    raw5=mat[5];
    raw8=mat[8];    raw9=mat[9];
    		
		numPart  = 0;
    num = maxPart;
		k = 0;

    mesh.drawFace = 0;

    //maybe the vertices should be in a seperate array or something
 			
		for (i = 0; i < num; i++)
		{
			//a = &(vecPart[i]);
      a = pindex[i];
			if (a->hp <= 0) { continue;}
    	
      mesh.drawFace += 2;
			numPart += 1;
      n = k * 4; k += 1;
			
			ca = cos(a->ang) * a->size;
			sa = sin(a->ang) * a->size;

      if (a->image < 0 || sprAnim == 0)
      {
        //wrgba = a->hp > 100.0f ? 255 : (int)(a->hp*2.55f);
        wrgba = a->hp > a->fadehp ? 255 : (int)((a->hp/a->fadehp)*255.0f);
        //wrgba = 20;
        wrgba = wrgba << 24;
        wrgba += 0xffFFff;

        vecVert[n].u   = 0; vecVert[n].v   = 1;     vecVert[n].rgba = wrgba;
        vecVert[n+1].u = 1; vecVert[n+1].v = 1;     vecVert[n+1].rgba =  wrgba;
        vecVert[n+2].u = 0; vecVert[n+2].v = 0;     vecVert[n+2].rgba =  wrgba;
        vecVert[n+3].u = 1; vecVert[n+3].v = 0;     vecVert[n+3].rgba =  wrgba;


      }
      else
      {
        sr = sprAnim->vecSprite[a->image];

        //wrgba = a->hp > 100.0f ? 255 : (int)(a->hp*2.55f);
        wrgba = a->hp > a->fadehp ? 255 : (int)((a->hp/a->fadehp)*255.0f);
        wrgba = wrgba << 24;
        wrgba += 0xffFFff; //todo -- particle color

        vecVert[n].u   = sr->u0; vecVert[n].v   = sr->v1;        vecVert[n].rgba = wrgba;
        vecVert[n+1].u = sr->u1; vecVert[n+1].v = sr->v1;        vecVert[n+1].rgba =  wrgba;
        vecVert[n+2].u = sr->u0; vecVert[n+2].v = sr->v0;        vecVert[n+2].rgba =  wrgba;
        vecVert[n+3].u = sr->u1; vecVert[n+3].v = sr->v0;        vecVert[n+3].rgba =  wrgba;
      }//endif
  
		  
		  rx = -ca+sa;	ry = -sa-ca;
      p = &(vecVert[n].pos);      

			p->x = raw0*rx + raw1*ry + a->pos.x;
			p->y = raw4*rx + raw5*ry + a->pos.y;
			p->z = raw8*rx + raw9*ry + a->pos.z;
			 
			rx = ca +sa;	ry = sa-ca;
		  p = &(vecVert[n+1].pos);      

			p->x = raw0*rx + raw1*ry + a->pos.x;
			p->y = raw4*rx + raw5*ry + a->pos.y;
			p->z = raw8*rx + raw9*ry + a->pos.z;
		
			rx = -ca-sa;	ry = -sa+ca;
		  p = &(vecVert[n+2].pos);      

			p->x = raw0*rx + raw1*ry + a->pos.x;
			p->y = raw4*rx + raw5*ry + a->pos.y;
			p->z = raw8*rx + raw9*ry + a->pos.z;
			  			  
			rx = ca-sa;		ry = sa+ca;
      p = &(vecVert[n+3].pos);      

			p->x = raw0*rx + raw1*ry + a->pos.x;
			p->y = raw4*rx + raw5*ry + a->pos.y;
			p->z = raw8*rx + raw9*ry + a->pos.z;
		
		}//nexti
  }//makemesh 



void 
xPartSys::render(void)
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
     glVertexPointer(3, GL_FLOAT, 36, &(mesh.vecVert[0].pos.x));   
     glTexCoordPointer(2, GL_FLOAT, 36, &(mesh.vecVert[0].u));

    	glDrawElements(GL_TRIANGLES, numPart*6,  GL_UNSIGNED_SHORT, mesh.vecIndex); 
 
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }//render


static xPart * tempVec[XPART_MAXPART];
static int bucket[256];

static void radixSort(xPart ** vec, int num)
  {
    int i;    int shift;
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

void 
xPartSys::sortPart(gamex::cVec3f p)
{
  int i;
  gamex::cVec3f d;
  xPart * a;
  for (i = 0; i < maxPart; i++)
  {
      a = &(vecPart[i]);
      if (a->hp <= 0) { continue; }
      d = p;
      d -= a->pos;
      a->sortCode =  0x0FFffFF - (int) (d.getMagSqr() * 0.1f);   
  }//next

  radixSort(pindex, maxPart);

}//sortpart






void 
xPartSys::updateMesh2(float * mat)
  {
    xPart * a;
    int i, k, n, num;
    //float rx, ry; //, ang;	
    //float ca, sa;
    //gamex::cVec3f * p;			
    //float raw0, raw1, raw4, raw5, raw8, raw9;
    mVert * vecVert;
    xSprite * sr;
    int wrgba;

    vecVert = mesh.vecVert;

    gamex::cVec3f w, wx;
    gamex::cVec3f front;
    front.set(mat[2], mat[6], mat[10]);
    


    int numimg;    numimg = 0;
    if (sprAnim != 0)
    {      numimg = (int) sprAnim->vecSprite.size();  }

    //raw0=mat[0];    raw1=mat[1];
    //raw4=mat[4];    raw5=mat[5];
    //raw8=mat[8];    raw9=mat[9];
    		
		numPart  = 0;    num = maxPart;		k = 0;
    mesh.drawFace = 0;

		for (i = 0; i < num; i++)
		{
      a = pindex[i];			if (a->hp <= 0) { continue;}
    	
      mesh.drawFace += 2;			numPart += 1;
      n = k * 4; k += 1;	
			//ca = cos(a->ang) * a->size;
			//sa = sin(a->ang) * a->size;
      if (a->image < 0 || sprAnim == 0)
      {
        //wrgba = a->hp > 100.0f ? 255 : (int)(a->hp*2.55f);
        wrgba = a->hp > a->fadehp ? 255 : (int)((a->hp/a->fadehp)*255.0f);
        //wrgba = 20;
        wrgba = wrgba << 24;
        wrgba += 0xffFFff;

        vecVert[n].u   = 0; vecVert[n].v   = 1;     vecVert[n].rgba = wrgba;
        vecVert[n+1].u = 1; vecVert[n+1].v = 1;     vecVert[n+1].rgba =  wrgba;
        vecVert[n+2].u = 0; vecVert[n+2].v = 0;     vecVert[n+2].rgba =  wrgba;
        vecVert[n+3].u = 1; vecVert[n+3].v = 0;     vecVert[n+3].rgba =  wrgba;
      }
      else
      {
        sr = sprAnim->vecSprite[a->image];

        //wrgba = a->hp > 100.0f ? 255 : (int)(a->hp*2.55f);
        wrgba = a->hp > a->fadehp ? 255 : (int)((a->hp/a->fadehp)*255.0f);
        wrgba = wrgba << 24;
        wrgba += 0xffFFff;

        vecVert[n].u   = sr->u0; vecVert[n].v   = sr->v1;        vecVert[n].rgba = wrgba;
        vecVert[n+1].u = sr->u1; vecVert[n+1].v = sr->v1;        vecVert[n+1].rgba =  wrgba;
        vecVert[n+2].u = sr->u0; vecVert[n+2].v = sr->v0;        vecVert[n+2].rgba =  wrgba;
        vecVert[n+3].u = sr->u1; vecVert[n+3].v = sr->v0;        vecVert[n+3].rgba =  wrgba;
      }//endif
  
      w = a->vel;
		 //  w = *p1 - *p0;
      w.normalise();
    
      wx.setCross(w, front);
      wx.normalise();
      wx *= a->size;

      vecVert[n].pos = a->pos - wx;  
      vecVert[n+1].pos = a->pos + wx;  
      w *= a->length; //not based on velocity
      w += a->pos;
      vecVert[n+2].pos = w - wx;  
      vecVert[n+3].pos = w + wx;  



      /*
		  rx = -ca+sa;	ry = -sa-ca;
      p = &(vecVert[n].pos);      

			p->x = raw0*rx + raw1*ry + a->pos.x;
			p->y = raw4*rx + raw5*ry + a->pos.y;
			p->z = raw8*rx + raw9*ry + a->pos.z;
			 
			rx = ca +sa;	ry = sa-ca;
		  p = &(vecVert[n+1].pos);      

			p->x = raw0*rx + raw1*ry + a->pos.x;
			p->y = raw4*rx + raw5*ry + a->pos.y;
			p->z = raw8*rx + raw9*ry + a->pos.z;
		
			rx = -ca-sa;	ry = -sa+ca;
		  p = &(vecVert[n+2].pos);      

			p->x = raw0*rx + raw1*ry + a->pos.x;
			p->y = raw4*rx + raw5*ry + a->pos.y;
			p->z = raw8*rx + raw9*ry + a->pos.z;
			  			  
			rx = ca-sa;		ry = sa+ca;
      p = &(vecVert[n+3].pos);      

			p->x = raw0*rx + raw1*ry + a->pos.x;
			p->y = raw4*rx + raw5*ry + a->pos.y;
			p->z = raw8*rx + raw9*ry + a->pos.z;
*/		

		}//nexti
  }//updatemesh2
































