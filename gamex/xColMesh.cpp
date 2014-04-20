#pragma warning (disable : 4786)


#include "xColMesh.h"
#include "xGLCommon.h"

#include <math.h>


xColMesh::xColMesh(void)
  {
    colTri = 0;
  }//ctor

xColMesh::~xColMesh(void)
  { clear(); }

void 
xColMesh::clear(void)
  {
    tdVecTri ::iterator it;
    for (it = vecTri.begin(); it != vecTri.end(); it++)    { delete (*it); }
    vecTri.clear();
    rect.clear();
  }//clear




float 
xColMesh::lineTest(gamex::cVec3f * start, gamex::cVec3f  * end, float rad)
  {
    float ret;
      ret = rect.lineTest(start, end, rad);
      colTri = rect.colTri;
    return ret;
  }//linetest
  
/*
//old brute force version (ok for debugging, or small meshes)
float 
xColMesh::lineTest(gamex::cVec3f &start, gamex::cVec3f &end, float rad)
  {
  
    int i;
    xTri * a;
    xTri * c;
    float t;
    float dist;

    float ax,ay,az;
    float bx,by,bz;

    ax = start.x; 
    ay = start.y; 
    az = start.z;
    bx = end.x;
    by = end.y; 
    bz = end.z;

    c = 0; t = -999;

    //printf("numtri %d \n", numTri);

    for (i = 0; i < numTri; i++)
    {
      a = &(vecTri[i]);
      
      dist = a->lineTest(ax, ay, az, bx, by, bz, rad);
      if (dist == -999.0f) { continue; }

      if (c == 0) { t = dist; c = a; continue; }
        
      if (dist < t) { t = dist; c = a; }

      //printf(" t %0.2f \n ", t);
    }//nexti

    return t;
  }//linetest
*/


//note: only meant for debugging
void 
xColMesh::render(void)
  {
    xTri * a;
   
    //glColor3f(0,0,0);
    
    tdVecTri ::iterator it;
    //num =numTri;
    for (it = vecTri.begin(); it != vecTri.end(); it++)
    {
      glBegin(GL_TRIANGLES);
        //a = &(vecTri[i]);
        a = (*it);

        glVertex3f(a->x0, a->y0, a->z0);
        glVertex3f(a->x1, a->y1, a->z1);
        glVertex3f(a->x2, a->y2, a->z2);

      glEnd();
    }//nextit
    
    //glColor3f(1,1,1);
    //rect.render(64);

  }//render


void 
xColRect::render(float yc)
 {

    float yw;
    //yw = yc + vecTri.size()*2;
    yw = yc;

    glBegin(GL_LINES);
      glVertex3f(rx, yw, ry);
      glVertex3f(rx+rw, yw, ry);
      glVertex3f(rx, yw, ry+rh);
      glVertex3f(rx+rw, yw, ry+rh);
      glVertex3f(rx, yw, ry);
      glVertex3f(rx, yw, ry+rh);
      glVertex3f(rx+rw, yw, ry);
      glVertex3f(rx+rw, yw, ry+rh);
    glEnd();

    if (child0 != 0)
    {
      child0->render(yc+1);
      child1->render(yc+1);
      child2->render(yc+1);
      child3->render(yc+1);    
    
    }//endif

 }//render 


void 
xColMesh::addMesh(xMdx3 * mdx, int maxd)
  {
    short * vecIndex;    mVert * vecVert;
    int num, i, t;
    xTri * a;
    bool b;
    int numLeft;
    
    num = mdx->numFace * 3;

    vecIndex = mdx->vecIndex;
    vecVert = mdx->vecVert;

    numLeft = 0;    

    t = 0;
    for (i = 0; i < num; i+= 3)
    {
      a = new xTri();
      vecTri.push_back(a);
        a->setValue(&(vecVert[vecIndex[i]].pos),&(vecVert[vecIndex[i+1]].pos),&(vecVert[vecIndex[i+2]].pos));
        a->calcNormal();

        b = rect.addTri(a, maxd);

        if (b == false) { numLeft += 1; }
    }//nexti

    if (numLeft > 0) { printf("xColMesh:: warning  triangles were left out  %d  \n", numLeft); }
  }//initmesh


void
xColMesh::initRect(float ax, float ay, float aw, float ah)
  {
    clear();
    //rect.clear();
    rect.rx = ax; rect.ry = ay;
    rect.rw = aw; rect.rh = ah;
    
  }//initrect

 
void 
xColMesh::initMesh(xMdx3 * mdx, int maxd)
  {
    short * vecIndex;    mVert * vecVert;
    int num;    int i;    int t;
    xTri * a;

    num = mdx->numFace * 3;

    vecIndex = mdx->vecIndex;
    vecVert = mdx->vecVert;


    //note -- min max need to be calculated beforehand
    //rect.clear();
    clear();
    
    rect.rx = mdx->min.x-16.0f;
    rect.ry = mdx->min.z-16.0f;
    rect.rw = (mdx->max.x - mdx->min.x)+32.0f;
    rect.rh = (mdx->max.z - mdx->min.z)+32.0f;

    bool b;
    int numLeft;
  
    numLeft = 0;    

    t = 0;
    for (i = 0; i < num; i+= 3)
    {
      a = new xTri();
      vecTri.push_back(a);
        a->setValue(&(vecVert[vecIndex[i]].pos),&(vecVert[vecIndex[i+1]].pos),&(vecVert[vecIndex[i+2]].pos));
        a->calcNormal(); //important

        //todo -- maxdepth based on triangle size
        b = rect.addTri(a, maxd); //(todo -- add triangles as new objects instead and dont keep them all in xcolmesh)

        if (b == false) { numLeft += 1; }
    }//nexti

    if (numLeft > 0) { printf("xColMesh:: warning  triangles were left out  %d  \n", numLeft); }

  }//initmesh





void 
xColMesh::addTileMap(xTileMap * tmap)
{
  //clear();

  xMdx3 temp;
  xTri * a;


  

  //note -- min max need to be calculated beforehand
  rect.clear();
  rect.rx = -16.0f;
  rect.ry = -16.0f;
  rect.rw = (tmap->mwidth * tmap->cw)+32.0f;
  rect.rh = (tmap->mheight * tmap->ch )+32.0f;

 
 
//  numTri = tmap->getNumTri(0,0,tmap->mwidth, tmap->mheight);
 // printf("numtri needed %d \n", numTri);
  //vecTri = new xTri[numTri];

    int i, k, yt;
    int mw, mh;
    int cellw, cellh;
    int numRect;

    cellw = 8; cellh = 8;

    mw = (int) ceil((float)tmap->mwidth / (float) cellw);
    mh = (int) ceil((float)tmap->mheight / (float) cellh);
    
    numRect = mw * mh;
    printf("numrect %d \n", numRect);

    int maxd;   maxd = 4;

    short * vecIndex;
    mVert * vecVert;
    int f;
    int num;
    int t;
    bool b;

    int numLeft;
   
    t = 0;
    numLeft = 0;   
    for (i = 0; i < mh; i+=1)
    {
      yt = i * mw;
      for (k = 0; k < mw; k+=1)
      {
          tmap->makeMesh(&temp,k*cellw,i*cellh,cellw,cellh);
         //a->mesh.calcMinMax(); //no need - tilemap already calculated them
   
           vecIndex = temp.vecIndex;
           vecVert = temp.vecVert;
           num = temp.numFace * 3;
    

            for (f = 0; f < num; f+= 3)
            {
              //a = &(vecTri[t]); t += 1;
			  a = new xTri();
			  vecTri.push_back(a);

              a->setValue(&(vecVert[vecIndex[f]].pos),&(vecVert[vecIndex[f+1]].pos),&(vecVert[vecIndex[f+2]].pos));
              a->calcNormal(); //important

              b = rect.addTri(a, maxd); //(todo -- add triangles as new objects instead and dont keep them all in xcolmesh)

              if (b == false) { numLeft += 1; }
            }//nextf
            

      }//nextk
    }//nexti
  if (numLeft > 0) { printf("xColMesh:: warning  triangles were left out  %d  \n", numLeft); }

   // printf(" inittilemap finished %d/%d \n", t,numTri);

    temp.clear(); //would happen anyway but whatever
}//initfromtmap











xColRect::xColRect(void)
  {
    rx = 0; ry = 0; rw = 0; rh = 0; d= 0;
    child0 = 0; child1 = 0; child2 = 0; child3 = 0;
    colTri = 0;
  }//ctor

xColRect::~xColRect(void)
  {
    clear();
  }//dtor


void 
xColRect::clear(void)
  {
    if (child0 != 0) { delete child0; } child0 = 0;
    if (child1 != 0) { delete child1; } child1 = 0;
    if (child2 != 0) { delete child2; } child2 = 0;
    if (child3 != 0) { delete child3; } child3 = 0;
    vecTri.clear(); //only ptr, dont delete (tri itself is in colmesh)
  }//clear



static int numTest = 0;
//

float 
xColRect::lineTest(gamex::cVec3f * start, gamex::cVec3f * end, float rad)
  {
    int i,  num;
    float u, t;
    xTri * a;
    
    colTri = 0;
    
    if (isOverLine(start->x, start->z, end->x, end->z, rad) == false) { return 999.0f; }
   
    u = 999.0f;
    num = vecTri.size();
    for (i = 0; i < num; i++)
    {
      a = vecTri[i];
      t = a->lineTest(start->x, start->y, start->z, end->x, end->y, end->z, rad);
      if (t == -999.0f) { continue; }
      if (t < u) { u = t; colTri = a; }
    }//nexti
  
    if (child0 == 0) { return u; }
    
    t = child0->lineTest(start, end, rad);    if (t < u) { u = t; colTri = child0->colTri; }
    t = child1->lineTest(start, end, rad);    if (t < u) { u = t; colTri = child1->colTri; }
    t = child2->lineTest(start, end, rad);    if (t < u) { u = t; colTri = child2->colTri; }
    t = child3->lineTest(start, end, rad);    if (t < u) { u = t; colTri = child3->colTri; }

    return u;
  }//linetest




void 
xColRect::genChild(int md)
  {
    float w2, h2;
    w2 = rw * 0.5f;
    h2 = rh * 0.5f;

    if (child0 == 0)
    {
      child0 = new xColRect();
      child0->d = d + 1;
      child0->rx = rx;      child0->ry = ry;
      child0->rw = w2;      child0->rh = h2;

      child1 = new xColRect();
      child1->d = d + 1;
      child1->rx = rx+w2;      child1->ry = ry;
      child1->rw = w2;      child1->rh = h2;

      child2 = new xColRect();
      child2->d = d + 1;
      child2->rx = rx;      child2->ry = ry+h2;
      child2->rw = w2;      child2->rh = h2;

      child3 = new xColRect();
      child3->d = d + 1;
      child3->rx = rx+w2;      child3->ry = ry+h2;
      child3->rw = w2;      child3->rh = h2;

    }//endif
  
    if (md == -1) { return; }
    if (d >= md) { return; }

    child0->genChild(md);    child1->genChild(md);
    child2->genChild(md);    child3->genChild(md);

  }//genchild






bool 
xColRect::isTriInside(xTri * a)
  {
    if (a->x0 < rx) { return false; }
    if (a->x0 > rx+rw) { return false; }

    if (a->x1 < rx) { return false; }
    if (a->x1 > rx+rw) { return false; }

    if (a->x2 < rx) { return false; }
    if (a->x2 > rx+rw) { return false; }

    if (a->z0 < ry) { return false; }
    if (a->z0 > ry+rh) { return false; }

    if (a->z1 < ry) { return false; }
    if (a->z1 > ry+rh) { return false; }

    if (a->z2 < ry) { return false; }
    if (a->z2 > ry+rh) { return false; }

    return true;
  }//istriinside





bool 
xColRect::addTri(xTri * a, int maxd)
  {
    if (isTriInside(a) == false) 
    { /* //we get a nan for somet tris??
      if (d == 0)
      {
        isTriInside(a); 
      }*/
      return false;
    }
  
    //make children if below max depth
    if (child0 == 0 && d < maxd) { genChild(-1); }
  
    if (child0 == 0) { vecTri.push_back(a); return true; }
  
    if (child0->addTri(a, maxd)) { return true; }
    if (child1->addTri(a, maxd)) { return true; }
    if (child2->addTri(a, maxd)) { return true; }
    if (child3->addTri(a, maxd)) { return true; }

    //couldnt fit in any childs
    vecTri.push_back(a); return true;
  }//addtri



//overoptimised version
//see commented out one for proper
bool
xColRect::isOverLine(float ax, float ay, float bx, float by, float rad)
{
  float dx, dy, kw, kh; //rad reused as m
  kw = rw + rad + rad;        kh = rh + rad + rad;   
  dx = bx - ax;              dy = by - ay;
  rad = ax + bx - rx - rx - rw;   ay = ay + by - ry - ry - rh;  ax = rad; //ax ay is reused as mx my 
  bx = dx < 0 ? -dx : dx;              by = dy < 0 ? -dy : dy; //bx by is reused as adx ady from here
  if ((ax<0?-ax:ax)  > kw + bx) { return false; }    if ((ay<0?-ay:ay) > kh + by) { return false; }  
  bx += 0.000001f;      by += 0.000001f; 
  rad = (ax * dy - ay * dx);
  if ((rad<0? -rad:rad) > kw * by + kh * bx) return false;
  return true;
}//rectline


/*
//based on: Christer Ericson (2005) 'Real-Time Collision Detection' p183-184
bool
xColRect::isOverLine(float ax, float ay, float bx, float by, float rad)
{
  float dx, dy, mx , my;  float adx, ady, m; float kx, ky, kw, kh;
  kx = rx - rad;              ky = ry - rad;
  kw = rw + rad + rad;        kh = rh + rad + rad;   
  dx = bx - ax;              dy = by - ay;
  mx = ax + bx - kx - kx - kw;          my = ay + by - ky - ky - kh; 
  adx = dx < 0 ? -dx : dx;              ady = dy < 0 ? -dy : dy; //ax or bx could be reused
  if ((mx<0?-mx:mx)  > kw + adx) { return false; }    if ((my<0?-my:my) > kh + ady) { return false; }  
  adx += 0.000001;      ady += 0.000001; 
  m = (mx * dy - my * dx);
  if ((m<0? -m:m) > kw * ady + kh * adx) return false;
  return true;
}//rectline
*/
/*
//prototype: pg3 and pg4
bool 
xColRect::isOverLine(float ax, float ay, float bx, float by, float rad)
   {
    float cx, cy, dx, dy, dz0; float kx, ky, kw, kh;
    kx = rx-rad;          kw = rw+ rad +rad;
    if (ax < bx){ if (kx + kw < ax) {  return false; }  if (kx > bx) { return false; }
    } else {  if (kx + kw < bx) { return false; }  if (kx > ax) { return false; }  }
    ky = ry- rad;          kh = rh+ rad +rad;
    if (ay < by)
    { if (ky + kh < ay) { return false; }  if (ky > by) { return false; } } else 
    { if (ky + kh < by) { return false; }  if (ky > ay) { return false; } }

    cx = bx - ax;      cy = by - ay;
    if (cx == 0 && cy == 0)          { return true; }
    dx = kx - ax;          dy = ky - ay;       
    dz0 = (cx * dy) - (cy * dx); 
  
    dx = kx+kw - ax;       //dy = ky - ay;       
    if ((cx * dy) - (cy * dx) > 0) { if (dz0 < 0) { return true; }}
    else { if (dz0 > 0) { return true; }} 
  
    dx = kx - ax;        dy = ky+kh - ay;       
    if ((cx * dy) - (cy * dx) > 0) { if (dz0 < 0) { return true; }}
    else { if (dz0 > 0) { return true; }}
  
    dx = kx+kw - ax;    // dy = ky+kh - ay;       
    if ((cx * dy) - (cy * dx) > 0) { if (dz0 < 0) { return true; }}
    else { if (dz0 > 0) { return true; }}
    return false;
  }///isoverline
  */





void 
xTri::setValue(gamex::cVec3f * a, gamex::cVec3f * b, gamex::cVec3f * c)
  {
    x0 = a->x;       y0 = a->y;      z0 = a->z;
    x1 = b->x;       y1 = b->y;      z1 = b->z;
    x2 = c->x;       y2 = c->y;      z2 = c->z;
  }//setvalue




void 
xTri::calcNormal(void)
    {
      float ax0, ay0, az0;
      float bx0, by0, bz0;
      float mag;

      ax0 = x1 - x0;      ay0 = y1 - y0;      az0 = z1 - z0;
      bx0 = x2 - x0;      by0 = y2 - y0;      bz0 = z2 - z0;

      nx = (ay0 * bz0) - (az0 * by0);
      ny = (az0 * bx0) - (ax0 * bz0);
      nz = (ax0 * by0) - (ay0 * bx0);

      mag = sqrt(nx*nx + ny*ny +nz*nz);
      if (mag == 0.0f) {mag = 0.00001f;}
      mag = 1 / mag;
      nx *= mag;      ny *= mag;      nz *= mag;
    }//calcnormal




float 
xTri::lineTest(float ax, float ay, float az,    float bx, float by, float bz, float rad)
      {
        float vx, vy, vz;
        float rx, ry, rz;
        float u;
        float kx, ky, kz;
        float dist;

        vx = bx-ax; 
        vy = by - ay; 
        vz = bz-az;
        rx = x0-ax; 
        ry = y0-ay; 
        rz = z0-az;


        u = (nx*rx + ny*ry +nz*rz);
        if (u == 0.0f) { return 999.0f; }

        u /= (nx*vx+ny*vy+nz*vz);

        if (u < 0 || u > 1) { return 999.0f; }

        kx = ax +(bx-ax)*u;
        ky = ay +(by-ay)*u;
        kz = az +(bz-az)*u;


   
        dist = getDist(kx, ky, kz, 0);

        if (dist > (rad * rad)) { return 999.0f; }
              
        return u;
      }//linetest




float 
xTri::getDist(float wx, float wy, float wz, gamex::cVec3f * ret)
{

  float ax0, ay0, az0, bx0, by0, bz0;
  float ax1, ay1, az1, bx1, by1, bz1;
  float ax2, ay2, az2, bx2, by2, bz2;
  float dz0, dz1, dz2;
  float rx, ry, rz;
  float kx, ky, kz;
  float wd, d;


  ax0 = x1 - x0;      ay0 = y1 - y0;      az0 = z1 - z0;
  bx0 = wx - x0;      by0 = wy - y0;      bz0 = wz - z0;
  kx = (ay0 * bz0) - (az0 * by0);
  ky = (az0 * bx0) - (ax0 * bz0);
  kz = (ax0 * by0) - (ay0 * bx0);
  dz0 = nx*kx + ny*ky + nz*kz;


  ax1 = x2 - x1;      ay1 = y2 - y1;      az1 = z2 - z1;
  bx1 = wx - x1;      by1 = wy - y1;      bz1 = wz - z1;     
  kx = (ay1 * bz1) - (az1 * by1);
  ky = (az1 * bx1) - (ax1 * bz1);
  kz = (ax1 * by1) - (ay1 * bx1);
  dz1 = nx*kx + ny*ky + nz*kz;


  ax2 = x0 - x2;      ay2 = y0 - y2;      az2 = z0 - z2;
  bx2 = wx - x2;      by2 = wy - y2;      bz2 = wz - z2;
  kx = (ay2 * bz2) - (az2 * by2);
  ky = (az2 * bx2) - (ax2 * bz2);
  kz = (ax2 * by2) - (ay2 * bx2);
  dz2 = nx*kx + ny*ky + nz*kz;


  //+++ //inside -- closest to plane
  if (dz0 >= 0 && dz1 >= 0 && dz2 >=0) 
  {
    d = (wx-x0)*nx  + (wy-y0)*ny  + (wz-z0)*nz; 
    //printf("d %0.2f \n", d);
    if (ret) {
      //closest point is the tested point projected to the triangle's plane 
      ret->x = wx - ( d * nx);
      ret->y = wy - ( d * ny);
      ret->z = wz - ( d * nz);
    }//endif2
    
    d *= d; //we need to return the squared distance to keep things simple

  }
  else   //-+- //vertex
  if (dz0 < 0 && dz1 >= 0 && dz2 < 0) 
  { 
    d = bx0*bx0 + by0*by0 + bz0*bz0;
    if (ret) { ret->x = x0; ret->y = y0; ret->z = z0; }
  } 
  else   //--+ //vertex
  if (dz0 < 0 && dz1 < 0 && dz2 >= 0) 
  {
    d = bx1*bx1 + by1*by1 + bz1*bz1;
    if (ret) { ret->x = x1; ret->y = y1; ret->z = z1; }
  }
  else   //+-- //vertex
  if (dz0 >= 0 && dz1 < 0 && dz2 < 0)
  { 
    d = bx2*bx2 + by2*by2 + bz2*bz2;
    if (ret) { ret->x = x2; ret->y = y2; ret->z = z2; }
  }
  else //-++  //edge
  if (dz0 < 0 && dz1 >= 0 && dz2 >= 0) 
  {
   wd = ((ax0 * bx0) + (ay0 * by0) + (az0*bz0) ) / ((ax0 * ax0) + (ay0 * ay0) + (az0*az0));
   if (wd < 0) { wd = 0;} if (wd > 1) { wd = 1; }
   rx = x0 + (x1 - x0) * wd;
   ry = y0 + (y1 - y0) * wd;
   rz = z0 + (z1 - z0) * wd;
   if (ret) { ret->x = rx; ret->y = ry; ret->z = rz;}  
   rx = wx-rx;   ry = wy-ry;  rz = wz-rz;
   d = rx*rx + ry*ry + rz*rz;
  }
  else  //+-+ //edge
  if (dz0 >= 0 && dz1 < 0 && dz2 >= 0) 
  {
   wd = ((ax1 * bx1) + (ay1 * by1) + (az1*bz1) ) / ((ax1 * ax1) + (ay1 * ay1) + (az1*az1));
   if (wd < 0) { wd = 0;} if (wd > 1) { wd = 1; }
   rx = x1 + (x2 - x1) * wd;
   ry = y1 + (y2 - y1) * wd;
   rz = z1 + (z2 - z1) * wd;
   if (ret) { ret->x = rx; ret->y = ry; ret->z = rz;}  
   rx = wx-rx;   ry = wy-ry;  rz = wz-rz;
   d = rx*rx + ry*ry + rz*rz;
  }
  else   //++- //edge
  if (dz0 >= 0 && dz1 >= 0 && dz2 < 0) 
  {
   wd = ((ax2 * bx2) + (ay2 * by2) + (az2*bz2) ) / ((ax2 * ax2) + (ay2 * ay2) + (az2*az2));
   if (wd < 0) { wd = 0;} if (wd > 1) { wd = 1; }
   rx = x2 + (x0 - x2) * wd;
   ry = y2 + (y0 - y2) * wd;
   rz = z2 + (z0 - z2) * wd;
   if (ret) { ret->x = rx; ret->y = ry; ret->z = rz;}  
   rx = wx-rx;   ry = wy-ry;  rz = wz-rz;
   d = rx*rx + ry*ry + rz*rz;
  }//endif
  else { return 0; } //got nan

  return d;
}//isinside












xColOb::xColOb(void) { colMesh = 0; }


void 
xColOb::updateMatrix(gamex::cVec3f * pos, gamex::cQuat * ori)
{
  ori->setMatrix(transMat.m);
  transMat.m[12] = pos->x;
  transMat.m[13] = pos->y;
  transMat.m[14] = pos->z;  
}//updatemtx
  
float 
xColOb::lineTest(gamex::cVec3f * start, gamex::cVec3f * end, float rad)
{
  static gamex::cVec3f va;
  static gamex::cVec3f vb;
  va = *start;
  vb = *end;

  float * raw;
  float rx, ry, rz;

  //transform line to object coordinates

  raw = transMat.m;
  rx = va.x - raw[12];  ry = va.y - raw[13];  rz = va.z - raw[14];

    va.x = raw[0] * rx + raw[1] * ry + raw[2] * rz;
    va.y = raw[4] * rx + raw[5] * ry + raw[6] * rz;
    va.z = raw[8] * rx + raw[9] * ry + raw[10] * rz;
        
  rx = vb.x - raw[12];  ry = vb.y - raw[13];  rz = vb.z - raw[14];

    vb.x = raw[0] * rx + raw[1] * ry + raw[2] * rz;
    vb.y = raw[4] * rx + raw[5] * ry + raw[6] * rz;
    vb.z = raw[8] * rx + raw[9] * ry + raw[10] * rz;
   
  bHit = false;

  if (colMesh == 0) { return 999.0f; }
  
  t = colMesh->lineTest(&va, &vb, rad);

  if (t >= 0.0f && t <= 1.0f )
  {

    //transform triangle normal by colob normal
    rx = colMesh->colTri->nx;    ry = colMesh->colTri->ny;    rz = colMesh->colTri->nz;    

    lastNorm.x = raw[0] * rx + raw[4] * ry + raw[8] * rz;
    lastNorm.y = raw[1] * rx + raw[5] * ry + raw[9] * rz;
    lastNorm.z = raw[2] * rx + raw[6] * ry + raw[10] * rz;
    
    bHit = true;

    colPos = *start +(*end - *start) * t;
  }//endif 

  return t;
}//linetest











