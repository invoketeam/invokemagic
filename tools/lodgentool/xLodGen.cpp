
#pragma warning (disable : 4786)

#include <vector>
#include <set>
#include <map>
#include <math.h>
#include <algorithm>
#include <cstring>

#include "xLodGen.h"

#include "xMath.h"


namespace lodgen
{

class xTri;

class xVert
{
public:
  int id;
  int posid;
  int group;
  gamex::cVec3f pos;
  float u, v; //texcoordinates are for debugging, not needed for generation

  bool bOpen;
  bool bDead;

  float r, g, b; //vertex color - used for testing groups

  float cost;
  int destPos; 
public:
  std::set <xVert *> setNear; //neighbours
  std::set <xTri *> setTri; //triangles vertex is in


public:
  xVert(void) { id = -1; posid = -1; group = -1; bOpen = false; bDead = false;  cost = 0; destPos = -1;}
  
};//xvert


class xTri
{
public:
  int id;
  int vert[3];
  gamex::cVec3f norm;

  bool bDead;

public:
  xTri(void) { id = -1; vert[0] = -1; vert[1] = -1; vert[2] = -1; bDead = false; }

};//xtri




typedef std::vector < std::vector<xVert *> > tdVecLookUp;

class xVertLookUp
{
public:
   tdVecLookUp vecLookUp;
   std::vector <gamex::cVec3f> vecPos;
public:
  void clear(void)
  {
    vecLookUp.clear();
    vecPos.clear();
  }//clear

  int addVert(xVert * a)
  {
    int i;
    int num;
    gamex::cVec3f *v;

    num = vecPos.size();
    for (i = 0; i < num; i++)
    {
      v = &(vecPos[i]);
      if (v->x == a->pos.x 
          && v->y == a->pos.y
          && v->z == a->pos.z)
      {
         a->posid = i;
         vecLookUp[i].push_back(a); 
         return i; 
      }
    }//nexti
    vecPos.push_back(a->pos);
    std::vector<xVert *> tmp;
    vecLookUp.push_back(tmp);
    i = vecPos.size() - 1;
    vecLookUp[i].push_back(a);
    a->posid = i;
    return i;
  }//addvert


  xVert * getVert(int posid, int group, xVert * m)
  {
    if (posid > vecPos.size()) { return 0; }  
  
    std::vector<xVert *> * vec;
    std::vector<xVert *>  ::iterator it;
    vec = &vecLookUp[posid];

    for (it = vec->begin(); it != vec->end(); it++)
    {
      if ( (*it) == m ) { continue; }
      if ( (*it)->group == group) { return (*it);}
    }//nextit
   return 0;
  }//posid



};//classend





class xModel
{
public:
  xTri * vecTri;
  xVert * vecVert;

  int numVert;
  int numFace;



  xVertLookUp mLookUp;

public:
  xModel(void);
  ~xModel(void);

  void clear(void);
  void fromMdx(xMdx3 * mesh);
  void render(void);
  void replaceVert(int idFrom, int idTo);


  void calcVertAdj(xVert * v);
  void setGroup(void); //find groups
  void setOpen(void); //find which vertices are on open edges

  void calcNormal(xTri * t); //calculate normal of triangle
  void calcCost(xVert * v); //calculate cost to replace vertex (or weld edge)


  bool collapse3(int posfrom, int posto, bool check=false);
  bool cheapCol2(void);

  int getNumFace(void); //get number of faces still living
  void downSize(int num); 

  short * getVec(int &ret_numFace);
};//classend




xModel::xModel(void) { vecTri = 0; vecVert = 0; numVert = 0; numFace = 0;}
xModel::~xModel(void) { clear(); }




//calculate neighbours for single vertex
void
xModel::calcVertAdj(xVert * v)
{
  xTri * t;
  int i;
  
  
  v->setNear.clear();
  v->setTri.clear();
  
  //if (v->dead) { return; } //check it by self
  
  for (i = 0; i < numFace; i++)
  {
    t = &(vecTri[i]);

    //if tri doesnt have this vertex skip
      if (t->vert[0] != v->id && 
          t->vert[1] != v->id && 
          t->vert[2] != v->id) { continue; } 
    
    //if tri has this vertex add tri
     v->setTri.insert(t);
    
    //add all verts that are not this as neighbors
      if (t->vert[0] != v->id) { v->setNear.insert( &vecVert[t->vert[0]] ); }
      if (t->vert[1] != v->id) { v->setNear.insert( &vecVert[t->vert[1]] ); }
      if (t->vert[2] != v->id) { v->setNear.insert( &vecVert[t->vert[2]] ); }
    
  }//nextit
  

}//calcvert


void
xModel::calcCost(xVert * v)
{
  
  std::set <xVert *> ::iterator vt; 
  std::set <xTri *> ::iterator it; 

  static xTri * vec[8192]; //todo -- so far its not tested if the vertex is connected to more tris than this 
  int num;
  int i;
  xTri * t;

  float dot; float minc; float curve;

  float cost;
  xVert * b;

  float mcost;
  int mdest;  
  

  mdest = -1;
  mcost = 99999990.0f;


  for (vt = v->setNear.begin(); vt != v->setNear.end(); vt++)
  {
    b = (*vt);

    if (v->bOpen) { if (b->bOpen == false) { continue; }} //open edges are not allowed to collapse ont non-open ones

    num = 0;

    //find tris that have both vertices
    for (it = v->setTri.begin(); it != v->setTri.end(); it++)
    {    
      t = (*it);
      if (t->vert[0] != b->id && t->vert[1] != b->id && t->vert[2] != b->id) { continue; }
      vec[num] = t; num++; if (num >= 8192) { num = 8191; }
    }//nextit

    //calculate curve
    for (it = v->setTri.begin(); it != v->setTri.end(); it++)
    {  
      t = (*it);  
      minc = 1.0f;
      
      for (i = 0;i < num; i++)
      {
         dot = gamex::cVec3f::dot(t->norm, vec[i]->norm);
         dot = (1.0f - dot) * 0.5f;
         minc = minc < dot ? minc : dot; 
      }//nexti3 
      curve = curve > minc ? curve : minc;  
    }//nexti2

    cost = curve * (v->pos - b->pos).getMag();

     if (v->bOpen) { cost *= 4; } //open edges should cost more (todo -- set this factor in member variable of xModel)

    if (cost < mcost)
    {
      mcost = cost;
      mdest = b->posid;
    }
  }//nextvt

  v->cost = mcost;
  v->destPos = mdest;

}//calccost






class xEdge
{
public:
  xTri * tri[2];
  xVert * vert[2];
public:
  xEdge(void)
  { tri[0] = 0; tri[1] = 0; vert[0] = 0; vert[1] = 0; }

};//xedge



void 
xModel::setOpen(void)
{
  static std::vector <xEdge * > vecEdge; //only needed here
  std::vector <xEdge * > ::iterator it;

  int i;  int k;
  xTri * t;  xEdge * a;  xVert * va;  xVert * vb;

  //go through all triangles
  for (i = 0; i < numFace; i++)
  {

    //go through all 3 vertex pairs
    for (k = 0; k < 3; k++)
    {
      t = &(vecTri[i]);
      va = &(vecVert[t->vert[k]]);   
      vb = &(vecVert[t->vert[(k+1) % 3]]);

    
      //go through all edges and see if vertex pair is already used
      for (it = vecEdge.begin(); it < vecEdge.end();it++)
      {
        a = (*it);

        //edge already inside
        if ((a->vert[0] == va && a->vert[1] == vb) || (a->vert[1] == va && a->vert[0] == vb) )   
        {
            if (a->tri[0] == t || a->tri[1] == t) { break; } //already in this triangle
            if (a->tri[1] == 0) { a->tri[1] = t; } //this is the second triangle for this edge
            break;     
        }//endif
       }//nextit

       //edge was not found -- add new one
         if (it == vecEdge.end())
         {
          a = new xEdge();
          a->vert[0] = va;
          a->vert[1] = vb;
          a->tri[0] = t;
          vecEdge.push_back(a); 
         }//endif
        
      
    }//nextk


  }//nexti


  //go through all edges -- for every vertex that is on an open edge (only 1 triangle) set it to open
  for (it = vecEdge.begin(); it < vecEdge.end();it++)
  {
    a = (*it);
    if (a->tri[1] != 0) { continue; }
    a->vert[0]->bOpen = true;
    a->vert[1]->bOpen = true;  
  }//nexti



//free up memory
  for (it = vecEdge.begin(); it < vecEdge.end();it++)
  { delete (*it); }
  vecEdge.clear();
  
}//setopen




void
xModel::calcNormal(xTri * t)
{
  xVert * va;
  xVert * vb;
  xVert * vc;

  va = &(vecVert[ t->vert[0] ]);
  vb = &(vecVert[ t->vert[1] ]);
  vc = &(vecVert[ t->vert[2] ]);


  gamex::cVec3f e0, e1;

  e0 = vb->pos - va->pos;
  e1 = vc->pos - va->pos;

  t->norm = gamex::cVec3f::cross(e0, e1);
  t->norm.normPrec(); 

}//calcnormal


void 
xModel::setGroup(void)
{

  int i;
  static xVert * temp[32768]; //same as a global variable
  int it;
  xVert * a;
  std::set <xVert *> ::iterator vt;
  int curGroup;  

  float red, green, blue;


  curGroup = 0;

  //only done once so groups are not reset beforehand 

  for (i = 0; i < numVert; i++)
  {
    if (vecVert[i].group > 0) { continue; }

    a = &vecVert[i];
    temp[0] = a;
    it = 1;
    curGroup += 1;
    a->group = curGroup;

    //for debugging purposes each group gets a random color
        red = (float)(rand() % 32768) / 32768.0f;
        green = (float)(rand() % 32768) / 32768.0f;
        blue = (float)(rand() % 32768) / 32768.0f;

    while (it > 0)
    {
      it -= 1;
      if (it < 0) { break; }
      a = temp[it];
      a->r = red;
      a->g = green;
      a->b = blue;
      
      for (vt = a->setNear.begin(); vt != a->setNear.end(); vt++)
      {
        if ( (*vt)->group == curGroup)  { continue; }
        temp[it] = (*vt); it += 1;
        (*vt)->group = curGroup;
      }//nextvt


    }//wend
  }//nexti

  //printf("number of groups %d \n ", curGroup);

}//setgroup




void 
xModel::clear(void)
  {
    if (vecTri != 0) { delete [] vecTri; } vecTri = 0; 
    if (vecVert != 0) { delete [] vecVert;} vecVert = 0;
    numVert = 0; numFace = 0;

    mLookUp.clear();
  }//clear

void 
xModel::fromMdx(xMdx3 * mesh)
  {
    clear();

    numFace = mesh->numFace;
    numVert = mesh->numVert;

    int i;
    mVert * vec;
    short * vec2;
    

    vecTri = new xTri[numFace];
    vecVert = new xVert[numVert];

    vec = mesh->vecVert;

    for (i = 0; i < numVert; i++)
    {
       vecVert[i].pos = vec[i].pos;
       vecVert[i].u = vec[i].u; 
       vecVert[i].v = vec[i].v;
       vecVert[i].id = i;
       mLookUp.addVert(&vecVert[i]);
    }//nexti


    vec2 = mesh->vecIndex;
    for (i = 0; i < numFace; i++)
    {
      vecTri[i].vert[0] = vec2[i*3];
      vecTri[i].vert[1] = vec2[i*3+1];
      vecTri[i].vert[2] = vec2[i*3+2];
 
    }//nexti
   



    //Calculate helper values

  
   //calculate open edges for all vertices
      setOpen();   

    //calculate normals for all triangles
      for (i = 0; i < numFace; i++) { calcNormal( &(vecTri[i]) ); }
 
    //calculate neighbours for all vertices
      for (i = 0; i < numVert; i++)  { calcVertAdj( &(vecVert[i]) ); }

    //calculate groups for all vertices
      setGroup();  

    //calculate cost to collapse vertex
      for (i = 0; i < numVert; i++)  { calcCost( &(vecVert[i]) ); }
  
     

  }//frommdx

 
void 
xModel::replaceVert(int idFrom, int idTo)
  {
    xVert * v1;
    xVert * v2;

    xTri * f;

    v1 = &(vecVert[idFrom]);
    v2 = &(vecVert[idTo]);

    
    int i;
    
    for (i = 0; i < numFace; i++)
    {
      f = &(vecTri[i]);

      if ( f->vert[0] != idFrom &&  f->vert[1] != idFrom && f->vert[2] != idFrom) { continue; }

      if ( f->vert[0] == idTo ||   f->vert[1] == idTo ||  f->vert[2] == idTo) { f->bDead = true; }
      
      if ( f->vert[0] == idFrom) { f->vert[0] = idTo; }
      if ( f->vert[1] == idFrom) { f->vert[1] = idTo; }
      if ( f->vert[2] == idFrom) { f->vert[2] = idTo; }

    }//nexti

    //recalculate neighboring vertices cost and triangles

      std::set <xVert *> ::iterator st;
      std::set <xTri *> ::iterator tk;

       //recalc tris normals of v2 old tris
        for (tk = v2->setTri.begin(); tk != v2->setTri.end(); tk++)    { calcNormal( (*tk) );    }
      
        //recalculate neighbours for v2's old neighbours
        for (st = v2->setNear.begin(); st != v2->setNear.end(); st++)    {  calcVertAdj( (*st) ); calcCost((*st) );  }
      
            calcVertAdj(v2); //recalculate neighbours for v2
            calcCost(v2); //recalculate cost of v2

        //recalc tris normals of v2 new tris
        for (tk = v2->setTri.begin(); tk != v2->setTri.end(); tk++)  { calcNormal( (*tk) );    }

        //recalculate neighbours for v2's new neighbours
        for (st = v2->setNear.begin(); st != v2->setNear.end(); st++)   {   calcVertAdj( (*st) );  calcCost((*st) );  }

  }//replacevert


void 
xModel::render(void)
  {

    //no rendering in the command line tool
    /*
        int i;
        int k;
        xVert * v;
        gamex::cVec3f n;    


        for (i = 0; i < numFace; i++)
        {
          if (vecTri[i].bDead) { continue; }

          glBegin(GL_TRIANGLES);
            for (k = 0; k < 3; k++)
            {
               v = &(vecVert[ vecTri[i].vert[k] ]);
               glTexCoord2f(v->u, v->v);

                // col  =  v->group  ^ 0x49616E42;
                // col = ((float)(col % 32768) /32768.0f) * 0xFFffFFff;
                 //glColor3ub( (col >> 16) & 0xFF, (col >> 8) & 0xFF, col & 0xFF);
               
              glColor3f(v->r, v->g, v->b);

                 // n = v->pos; n.normalise(); glColor3f(n.x, n.y, n.z); //i was too lazy to generate random colors
               glVertex3f(v->pos.x, v->pos.y, v->pos.z);
            }//nextk
          glEnd();

          glColor3f(0,0,0);
          glBegin(GL_LINE_LOOP);
            for (k = 0; k < 3; k++)
            {
               v = &(vecVert[ vecTri[i].vert[k] ]);
               glVertex3f(v->pos.x, v->pos.y, v->pos.z);
            }//nextk
          glEnd();
        }//nexti
    */


  }//render






bool 
xModel::collapse3(int posfrom, int posto, bool check)
  {
    xVert * a;    xVert * b;

    std::vector<xVert *> * vec;
    std::vector<xVert *>  ::iterator it;
    vec = &(mLookUp.vecLookUp[posfrom]);


    for (it = vec->begin(); it != vec->end(); it++)
    {
      a = (*it);
      b = mLookUp.getVert(posto, a->group, a);
      //if (a == b) { b = 0; printf("something wrong \n");}

      if (b == 0)
      {   if (check) { return false; } }
      else  {   if (!check) { replaceVert(a->id, b->id);}   }
    }

    return true;
 }//collapse3


bool
xModel::cheapCol2(void)
{
  int i;
  xTri * t;
  int k;
  xVert * v;

  float mcost;
  xVert * mv;

  mcost = 99999999.0f;
  mv = 0;

  for (i = 0; i < numFace; i++)
  {
    t = &(vecTri[i]);

    for (k = 0; k < 3; k++)
    {
      v = &(vecVert[t->vert[k]]);

      if (v->destPos == -1) { continue; }

      if (v->cost > mcost) { continue; }      

      if ( collapse3(v->posid, v->destPos, true) == false) { continue; } //collapse would end up in a disaster

      mcost = v->cost;
      mv = v;
    }//nextk    

  }//nexti    

  if (mv == 0) { return false; }

  // printf("lowest cost %0.2f \n", mcost);

   collapse3(mv->posid, mv->destPos, false);

  return true;
}//cheapcol2


int 
xModel::getNumFace(void)
{
  int i;  int ret;  
  ret = 0;
  for (i = 0; i < numFace; i++)
  { if (vecTri[i].bDead ==false) { ret += 1; } }
  return ret;
}//nexti




void 
xModel::downSize(int num)
  {
    int nf;
    int k;

    nf = getNumFace();

    printf("downsize %d -> %d \n", nf, num);
  
    if (nf <= num) { return; }
   
    k = 0;
    while (nf > num)
    {
      if (k >= 32)
      {
        printf(".%d.",nf); 
        k = 0;
      }
      
      k += 1;

      if ( cheapCol2() == false) { return; }
      
      nf = getNumFace();
    }//wend
    
    printf("\n downsize end \n ");
  }//collapseto 







//return a vector of shorts for the old lod tool interface
short *
xModel:: getVec(int &ret_numFace)
  {
    std::vector <short> vec;
    xTri * t;
    short * ret;

    int i;
    int num;

     for (i = 0; i < numFace; i++)
     {
        t = &(vecTri[i]);
        if (t->bDead) { continue; }

        if (t->vert[0] == t->vert[1]) { continue; }
        if (t->vert[1] == t->vert[2]) { continue; }
        if (t->vert[2] == t->vert[0]) { continue; }
 
        vec.push_back(t->vert[0]);
        vec.push_back(t->vert[1]);
        vec.push_back(t->vert[2]);      
     }//nexti
    
    
    num = vec.size();
       
    ret = new short[num];
    ret_numFace = num/3;
    for (i = 0; i < num; i++) { ret[i] = vec[i]; }

    return ret;
  }//getvec
  


}//namespace lodgen





xLodLevel::xLodLevel() { vecIndex = 0; numFace = 0;}
xLodLevel::~xLodLevel() {  clear(); }

void 
xLodLevel::clear()
{ if (vecIndex != 0) {delete [] vecIndex;} vecIndex=0; numFace=0; }



xLodGen::xLodGen()
{
  vecPos = 0;
  numPos = 0;
}//ctor
  
xLodGen::~xLodGen()
{
  clear();
}//dtor

/*
void 
xLodGen::init(xMdx3 * mdx)
{
  model = new lodgen::xModel();
  model.fromMdx3(mdx);
}//init
*/

void
xLodGen::addBase(xMdx3 * mdx)
{
  short * vec;
  int num;
  xLodLevel * w;

  num = mdx->numFace;
  vec = new short[num*3];
  
  memcpy(vec, mdx->vecIndex, num*3*2);

  w = new xLodLevel();
  w->vecIndex = vec;
  w->numFace = num;  
  
  vecLod.push_back(w);
}//addbase

void 
xLodGen::addLevel(xMdx3 * mdx, int nface)
{
  lodgen::xModel m;
  xLodLevel * w;
  short * vec;
  int num;

printf("addlev start \n");
  m.fromMdx(mdx);
printf("downsize start \n");
  m.downSize(nface);
printf("addlev end \n");

  vec = m.getVec(num);
  
  w = new xLodLevel();
  w->vecIndex = vec;
  w->numFace = num;  
  
  vecLod.push_back(w);
  printf("\n lod size: %d \n", vecLod.size());
}//addlevel


void 
xLodGen::clear()
{
  if (vecPos != 0) { delete [] vecPos; } vecPos = 0; numPos = 0;

  tdVecLod ::iterator it;
  for (it = vecLod.begin(); it != vecLod.end(); it++)
  { delete (*it); }
  vecLod.clear();
}//clear




struct svert
{
  int old_id;
  int d; //number of lods it appears in
};//svert

 static void radixSort(svert ** vec, int num);

static bool compVert(svert a, svert b)
{ return a.d > b.d;
  //return a.old_id < b.old_id;
 }


void
xLodGen::reIndex(xMdx3 * mdx)
{
 int i;
 int num;
  tdVecLod ::iterator it;
  short * vec;
  xLodLevel * w;
  int k;
  int numk;

 num = mdx->numVert;

 if (vecPos != 0) { delete [] vecPos; vecPos = 0; }

 vecPos = new short[num];
 memset(vecPos, 0, num*2);
 numPos = num;

 for (i = 0; i < num; i++) { vecPos[i] = i; }

  svert ** vecVert;
  vecVert = new svert*[num];
  for (i =0;i<num;i++) { vecVert[i]=new svert();}
  for (i = 0; i < num; i++) { vecVert[i]->old_id= i; vecVert[i]->d = 0;}


  k = 0;
  //so far its not sorted, its assumed
  //that they were generated from biggest to smallest
  for (it = vecLod.begin(); it != vecLod.end(); it++)
  {
    w = (*it);

    vec = w->vecIndex;
    numk = w->numFace * 3;

    for (i = 0; i < numk; i++)
    {
      vecVert[ vec[i] ]->d = 1024-k;
    }//nexti

    k += 1;
  }//nextit


  radixSort(vecVert, mdx->numVert);

  num = mdx->numVert;

/*
//debug
  for (i = 0; i < num; i++)
  {
      //printf(" %d(%d), ", vecVert[i]->old_id, vecVert[i]->d);
      printf(" %d, ", vecVert[i]->old_id, vecVert[i]->d);

  }
*/

  for (i = 0; i < num; i++)
  {
    //vecPos[i] = vecVert[i]->old_id;
    vecPos[vecVert[i]->old_id] = i;
  }




  for (it = vecLod.begin(); it != vecLod.end(); it++)
  {
    w = (*it);

    vec = w->vecIndex;
    num = w->numFace * 3;
    printf("lodlevel %d \n", w->numFace);
    for (i = 0; i < num; i++)
    {
      k = vec[i];
    //  printf("reindex: %d  ", vec[i] );
      vec[i] = vecPos[k];
    //  printf("-> %d  \n", vec[i] );
      
    }//nexti
  }//nextit


}//reindex





void 
xLodGen::saveVecPos(std::string fname)
{
 if (vecPos == 0) { return; }

 char mdxid[] = "MDX3I0_2013";
 FILE * file;
 int reserve[] = {0,0,0,0, 0,0,0,0};

 file = fopen(fname.c_str(), "wb");


 fwrite(mdxid, 1, 12, file);
 fwrite(&(numPos), 1, 4, file); 
 fwrite(reserve, 8, 4, file); //32 bytes reserved

 fwrite(vecPos, 2, numPos, file);

 fclose(file);
}//savevecpos


void 
xLodGen::reSaveMdx(xMdx3 * mdx, std::string fname)
{
 char mdxid[] = "MDX003_2013";
 FILE * file;
 int reserve[] = {0,0,0,0, 0,0,0,0};
 mVert * vecVert;
 short * vecIndex;
 int i;
 int num;

 file = fopen(fname.c_str(), "wb");
 if (file == 0) { printf("error: cannot open file for writing: %s \n", fname.c_str() ); return; }

//HEADER
//todo -- mdx header struct 
 fwrite(mdxid, 1, 12, file);
 fwrite(&(mdx->numFace), 1, 4, file);
 fwrite(&(mdx->numVert), 1, 4, file);
 fwrite(&(mdx->rad), 1, 4, file);
 fwrite(&(mdx->min), 1, 12, file);
 fwrite(&(mdx->max), 1, 12, file);
 fwrite(&(mdx->off), 1, 12, file);
 fwrite(reserve, 8, 4, file); //32 bytes reserved

//convert vertices to their new position
 vecVert = new mVert[mdx->numVert];
 num = mdx->numVert;
 int k;
 for (i = 0; i < num; i++)
 {
   k = vecPos[i];
//   vecVert[i] = mdx->vecVert[k];
    vecVert[k] = mdx->vecVert[i];

  // vecVert[i] = mdx->vecVert[i];

 }


//use lod 0 
 vecIndex = vecLod[0]->vecIndex;

//DATA
 fwrite(vecIndex, 2, mdx->numFace*3, file);
 fwrite(vecVert, sizeof(mVert), mdx->numVert, file);

 //fwrite(mdx->vecIndex, 2, mdx->numFace*3, file);
 //fwrite(mdx->vecVert, sizeof(mVert), mdx->numVert, file);
 

 fclose(file);
 delete [] vecVert;
 
}//resave




void 
xLodGen::saveLod(xMdx3 * mdx, std::string fname)
{
  tdVecLod ::iterator it;
  xLodLevel * w;
  FILE * file;
  char lodid[] = "MDLOD3_2013";
  int num;

  file = fopen(fname.c_str(), "wb");
  if (file == 0) { printf("error: cannot open file for writing: %s \n", fname.c_str() ); return; }

//HEADER
  fwrite( lodid, 1, 12, file);
  fwrite(&(mdx->numFace), 1, 4, file); //mdx data to help match the file (only vertnum is important though)
  fwrite(&(mdx->numVert), 1, 4, file);
  fwrite( &num, 1, 4, file);

//lod0 is supposed to be the original
  for (it = vecLod.begin(); it != vecLod.end(); it++)
  {
    w = (*it);
    fwrite(&(w->numFace), 1, 4, file);
    fwrite(&(w->vecIndex), 2, w->numFace*3, file);
  }//nexit

}//savelod


static short getMax(short * vec, int num)
{
  short ret;
  int i;
  ret = 0;
  for (i = 0; i <num; i++)
  {
    if (vec[i] > ret) { ret = vec[i];}
  }//nexti
  return ret;
}//getmax


void 
xLodGen::saveLevelAsMdx(xMdx3 * mdx, int lev, std::string fname)
{
 char mdxid[] = "MDX003_2013";
 FILE * file;
 int reserve[] = {0,0,0,0, 0,0,0,0};
 mVert * vecVert;
 short * vecIndex;
 int i;
 int num;
 int numvert2;
 xLodLevel * w;

  //printf("reach 0\n");

 if (lev >= vecLod.size() ) { printf("xlodgen::saveasmdx -- lev out of range \n"); }
 w = vecLod[lev];
 vecIndex = w->vecIndex;
  //printf("reach 0b \n");
  
 numvert2= getMax(w->vecIndex, w->numFace*3) + 1; //+1 is needed as this returns the vertex index and not number of verts used.. nvm
// numvert2 = mdx->numVert;
  
  //printf("numvert2 %d/%d \n", mdx->numVert, numvert2);
  if (numvert2 > mdx->numVert) { printf("error \n"); return; }

  //printf("reach1\n");

 file = fopen(fname.c_str(), "wb");

//HEADER
//todo -- mdx header struct 
 fwrite(mdxid, 1, 12, file);
 fwrite(&(w->numFace), 1, 4, file);
 //fwrite(&(mdx->numFace), 1, 4, file);
 //fwrite(&(mdx->numVert), 1, 4, file);
 fwrite(&(numvert2), 1, 4, file);
 fwrite(&(mdx->rad), 1, 4, file);
 fwrite(&(mdx->min), 1, 12, file);
 fwrite(&(mdx->max), 1, 12, file);
 fwrite(&(mdx->off), 1, 12, file);
 fwrite(reserve, 8, 4, file); //32 bytes reserved

//  printf("reach2\n");

//convert vertices to their new position
 vecVert = new mVert[mdx->numVert];
 num = mdx->numVert;
 int k;
 for (i = 0; i < num; i++)
 { 
   k = vecPos[i];
   vecVert[k] = mdx->vecVert[i]; 

 }

  //printf("reach3\n");


//DATA
 fwrite(vecIndex, 2, w->numFace*3, file);
 fwrite(vecVert, sizeof(mVert), numvert2, file);

//printf("reach4");

 fclose(file);
 delete [] vecVert;

}//savelevel


// 32767 is the maximum number of vertices a mdx3 supports
  #define MAXSVERT 32767
   static svert * tempVec[MAXSVERT];
   static int bucket[256];

   static void radixSort(svert ** vec, int num)
    {
      int i;
      int shift;

      shift = 0;

		    while (shift < 32)
            {		      
              memset(bucket, 0, 1024);         
              for (i = 0; i < num; i++) { bucket[(vec[i]->d >> shift) & 0xFF]++;  }      
              for (i = 1; i < 256; i++) { bucket[i] += bucket[i - 1];  }          
              for (i = num - 1; i >= 0; i--)  { tempVec[--bucket[(vec[i]->d >> shift) & 0xFF]] = vec[i];  }        
              for (i = 0; i < num; i++) { vec[i] = tempVec[i]; }     
              shift +=  8;    
            }//wend
    }//radixsort 




