#pragma warning (disable : 4786)

#include "xDrawMesh.h"

#include "xGLCommon.h"


xDrawTree::xDrawTree(void)
  {
    rad = 1; 
    depth = 0;
    child0 = 0; child1 = 0;
    child2 = 0; child3 = 0;
    child4 = 0; child5 = 0;
    child6 = 0; child7 = 0;

  }//ctor

xDrawTree::~xDrawTree(void)  { clear(); }

void 
xDrawTree::clear(void)
  {
    if (child0 != 0) { delete child0; } child0 = 0;
    if (child1 != 0) { delete child1; } child1 = 0;
    if (child2 != 0) { delete child2; } child2 = 0;
    if (child3 != 0) { delete child3; } child3 = 0;
    if (child4 != 0) { delete child4; } child4 = 0;
    if (child5 != 0) { delete child5; } child5 = 0;
    if (child6 != 0) { delete child6; } child6 = 0;
    if (child7 != 0) { delete child7; } child7 = 0;
 
    vecMesh.clear();  
  }//clear





void
xDrawTree::genChild(void)
  {
    if (child0 != 0) { return; }

    gamex::cVec3f half;
    gamex::cVec3f hh;
    float r2;

    half = size * 0.5f;
    r2 = rad * 0.5f;
    hh = half * 0.5f;

    child0 = new xDrawTree();
    child0->pos = pos;
    child0->size = half;
    child0->rad = r2;
    child0->mid = child0->pos + hh;
    child0->depth = depth + 1;

    child1 = new xDrawTree();
    child1->pos = pos; child1->pos.x += half.x;
    child1->size = half;
    child1->rad = r2;
    child1->mid = child1->pos + hh;
    child1->depth = depth + 1;


    child2 = new xDrawTree();
    child2->pos = pos; child2->pos.z += half.z;
    child2->size = half;
    child2->rad = r2;
    child2->mid = child2->pos + hh;
    child2->depth = depth + 1;

    child3 = new xDrawTree();
    child3->pos = pos; child3->pos.x += half.x; child3->pos.z += half.z;
    child3->size = half;
    child3->rad = r2;
    child3->mid = child3->pos + hh;
    child3->depth = depth + 1;


    child4 = new xDrawTree();
    child4->pos = pos;
    child4->size = half;
    child4->rad = r2;
    child4->pos.y += half.y;
    child4->mid = child4->pos + hh;
    child4->depth = depth + 1;

    child5 = new xDrawTree();
    child5->pos = pos; child5->pos.x += half.x;
    child5->size = half;
    child5->rad = r2;
    child5->pos.y += half.y;
    child5->mid = child5->pos + hh;
    child5->depth = depth + 1;

    child6 = new xDrawTree();
    child6->pos = pos; child6->pos.z += half.z;
    child6->size = half;
    child6->rad = r2;
    child6->pos.y += half.y;
    child6->mid = child6->pos + hh;
    child6->depth = depth + 1;

    child7 = new xDrawTree();
    child7->pos = pos; child7->pos.x += half.x; child7->pos.z += half.z;
    child7->size = half;
    child7->rad = r2;
    child7->pos.y += half.y;
    child7->mid = child7->pos + hh;
    child7->depth = depth + 1;

  }//genchild





bool 
xDrawTree::addMesh(xMdx3 * a, int maxd)
  {
    //aabb test
    /*
    if (r->pos.x - r->rad  > pos.x+size.x) { return false; }
    if (r->pos.x + r->rad  < pos.x) { return false; }
    if (r->pos.y - r->rad  > pos.y+size.y) { return false; }
    if (r->pos.y + r->rad  < pos.y) { return false; }
    if (r->pos.z - r->rad  > pos.z+size.z) { return false; }
    if (r->pos.z + r->rad  < pos.z) { return false; }
   */
    //nah, need contain test
    /*
    if (r->pos.x + r->rad > pos.x + size.x) { return false; }
    if (r->pos.x - r->rad < pos.x - size.x) { return false; }
    if (r->pos.y + r->rad > pos.y + size.y) { return false; }
    if (r->pos.y - r->rad < pos.y - size.y) { return false; }
    if (r->pos.z + r->rad > pos.z + size.z) { return false; }
    if (r->pos.z - r->rad < pos.z - size.z) { return false; }
*/

    if (a->min.x < pos.x) { return false; }
    if (a->min.x > pos.x+size.x) { return false; }
    if (a->min.y < pos.y) { return false; }
    if (a->min.y > pos.y+size.y) { return false; }
    if (a->min.z < pos.z) { return false; }
    if (a->min.z > pos.z+size.z) { return false; }
  
    if (a->max.x < pos.x) { return false; }
    if (a->max.x > pos.x+size.x) { return false; }
    if (a->max.y < pos.y) { return false; }
    if (a->max.y > pos.y+size.y) { return false; }
    if (a->max.z < pos.z) { return false; }
    if (a->max.z > pos.z+size.z) { return false; }
  

  

    if (child0 == 0 && depth < maxd) { genChild(); }

    if (child0 == 0) { vecMesh.push_back(a); return true; }
  
    if (child0->addMesh(a, maxd)) { return true; }
    if (child1->addMesh(a, maxd)) { return true; }
    if (child2->addMesh(a, maxd)) { return true; }
    if (child3->addMesh(a, maxd)) { return true; }
    if (child4->addMesh(a, maxd)) { return true; }
    if (child5->addMesh(a, maxd)) { return true; }
    if (child6->addMesh(a, maxd)) { return true; }
    if (child7->addMesh(a, maxd)) { return true; }
   
    vecMesh.push_back(a); return true;
  }//addrect


static int debNumNodeDrawn = 0;

void 
xDrawTree::render(xFrustum * frust)
  {
    if (frust->isPointInside(mid, rad) == false) { return; }

  //  if (depth == 0) { debNumNodeDrawn = 0; }
/*
    glLineWidth(2);
    glColor3f(1,0,0);
    glPushMatrix();
      glTranslatef(mid.x, mid.y, mid.z);
      glScalef(size.x, size.y, size.z);
        glutWireCube(1);
    glPopMatrix();
*/

    debNumNodeDrawn += 1;


    //glLineWidth(1);
    //glColor3f(0,0,1);
    xMdx3 * a;
    int i, num;
    num = vecMesh.size();
    for (i = 0; i < num; i++) { a = vecMesh[i];  a->render(); }


    if (child0 == 0) { return; }
 
    child0->render(frust);
    child1->render(frust);
    child2->render(frust);
    child3->render(frust);
    child4->render(frust);
    child5->render(frust);
    child6->render(frust);
    child7->render(frust);
    
   // if (depth == 0) { printf("drawn nodes: %d \n", debNumNodeDrawn);}

  }//render




























xDrawMesh::xDrawMesh(void) 
  {
    vecRect = 0; numRect = 0;
  }

xDrawMesh::~xDrawMesh(void)  { clear(); }


void 
xDrawMesh::clear(void)
  {
    if (vecRect = 0) { delete [] vecRect; } vecRect = 0; numRect = 0;
    tree.clear();
  }//clear


void 
xDrawMesh::initRect(xTileMap * tmap)
  {
    clear();

    int i, k, yt;
    int mw, mh;
    int cellw, cellh;
    xDrawRect * a;

    cellw = 8; cellh = 8;

    mw = (int) ceil((float)tmap->mwidth / (float) cellw);
    mh = (int) ceil((float)tmap->mheight / (float) cellh);
    
    numRect = mw * mh;
    vecRect = new xDrawRect[numRect];

    tree.pos.set(0,-1024,0);
    tree.size.set(tmap->mwidth*tmap->cw, 2048, tmap->mheight*tmap->ch);
    tree.mid = tree.pos + (tree.size * 0.5f);
    tree.rad = (tree.size.getMag()) * 0.5f;

       //  printf("drawmesh mw mh %d %d \n",mw,mh);

    //todo  set tree maximum depth based on map size (?)
    int maxd;
    maxd = 4;

    for (i = 0; i < mh; i+=1)
    {
      yt = i * mw;
      for (k = 0; k < mw; k+=1)
      {
         a = &(vecRect[k+yt]);
         tmap->makeMesh(&(a->mesh),k*cellw,i*cellh,cellw,cellh);
         //a->mesh.moveMesh(0,-8,0); //debug
         a->mesh.calcMinMax();

         a->pos = a->mesh.min + (a->mesh.max - a->mesh.min) * 0.5f;
         a->rad = ((a->mesh.max - a->mesh.min).getMag() *0.5f);

         tree.addMesh(&(a->mesh), maxd);
      }//nextk
    }//nexti
  
  }//initrect




















