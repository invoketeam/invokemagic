#pragma warning (disable : 4786)


#include "xHeightMap.h"

#include "pugixml/pugixml.hpp"








xHeightTile::xHeightTile(void)
  {
    u0 = 0.0f; v0 = 0.0f;
    u1 = 0.0f; v1 = 0.0f;
  }//ctor





xHeightMap::xHeightMap(void)
{
  vecHeight = 0;
  vecColor = 0;
  vecGrid = 0;
  vecTop = 0;
  mw = 0;
  mh = 0;
  cx = 0.0f;
  cz = 0.0f;
  cw = 64.0f;
  ch = 64.0f;
}//ctor


void 
xHeightMap::clear(void)
{
  if (vecHeight != 0) { delete [] vecHeight; vecHeight = 0; }
  if (vecColor != 0) { delete [] vecColor; vecColor = 0; }
  if (vecGrid != 0) { delete [] vecGrid; vecGrid = 0;}
  if (vecTop != 0) { delete [] vecTop; vecTop = 0; }
  mw = 0;  mh = 0;
  mesh.clear();
}//clear

float 
xHeightMap::getHeight(float wx, float wy)
{
  int tx, ty;
  tx = (int) floorf(wx/cw);
  ty = (int) floorf(wy/ch);

	if (tx < 0) { return 0.0f; }
	if (ty < 0) { return 0.0f; }
	if (tx + 1 >= mw) { return 0.0f; }
	if (ty + 1 >= mh) { return 0.0f; }

  float h1, h2, h3;
  float u, v;
  float y0, y1;

  u = (wx/cw) - tx;
  v = (wy/ch) - ty;

  y0 = vecHeight[tx+(ty*mw)];
  y1 = vecHeight[tx+(ty*mw)+1];

    h1 = y0 + (y1 - y0) * u;

  y0 = vecHeight[tx+((ty+1)*mw)];
  y1 = vecHeight[tx+((ty+1)*mw)+1];

    h2 = y0 + (y1 - y0) * u;
  
     h3 = h1 + (h2 - h1) * v;

     return h3;

 /*
     //ab
     //cd

        h1 = a + (b-a) * h;
        h2 = c + (d-c) * h;
        h3 = h1 + (h2-h1) * v;
  */

 
}//getheight


void 
xHeightMap::loadHeight(xImage * img, float yscale)
{
  xImage * tmp;   unsigned int * dat;  int num;  int i;  

  tmp = 0;

  //resize image if not exact size (todo -- issue an warning if this happens)
    tmp = new xImage();
    tmp->init(mw, mh);
    if (img->mw != mw || img->mh != mh) {    tmp->resize(img, mw, mh); } else { tmp->drawImage(img, 0,0); }
 
    num = mw * mh;
    dat = tmp->dat;

  //convert blue channel to height
    for (i = 0; i < num; i++) 
    { 
      vecHeight[i] =  (float)((dat[i]>>8) & 0xFF) * yscale; 
    }

  if (tmp != 0) { delete tmp; }
}//loadheight


void 
xHeightMap::loadColor(xImage * img)
{
  xImage * tmp; int num;

  tmp = 0;

  //resize image if not exact size (todo -- issue an warning if this happens)
    tmp = new xImage();
    tmp->init(mw, mh);
    if (img->mw != mw || img->mh != mh) {    tmp->resize(img, mw, mh); } else { tmp->drawImage(img, 0,0); }
 
    num = mw * mh;
   
    memcpy(vecColor, tmp->dat, num*4);

  if (tmp != 0) { delete tmp; }
}//loadcolor



void 
xHeightMap::render(void)
{
  mesh.render();
}//render


//set max face ?
void
xHeightMap::setBufferMesh(int maxQuad)
{
  mesh.clear();

  int numQuad;
  int numVert;

  numQuad = maxQuad;
  numVert = numQuad * 4;

 // numQuad = 32 * 32;
  //numVert = numQuad * 4;

  mesh.numFace = numQuad*2;
  mesh.numVert = numVert;
  mesh.vecIndex = new int[mesh.numFace * 3];
  mesh.vecVert = new mVert[mesh.numVert]; 

  int i;
  int num;
  int k;
  
  num = mesh.numFace * 3;
  k = 0;

  for (i = 0; i < num; i+=6)
  {
    mesh.vecIndex[i] = 0+k;
    mesh.vecIndex[i+1] = 1+k;
    mesh.vecIndex[i+2] = 2+k;
    mesh.vecIndex[i+3] = 2+k;
    mesh.vecIndex[i+4] = 1+k;
    mesh.vecIndex[i+5] = 3+k;
  
    k += 4;
  }//nexti

  //todo upload to vbo (?)

}//setbuffer

//todo -- precalculate normals?
// (only needed for shaders or lighting)
//http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=24

//texturing
//oblivion tech
//http://stackoverflow.com/questions/11730759/texturing-opengl-terrain

//water reflection
//http://www.ohadpr.com/2001/08/opengl-terrain-engine/

//interesting blending tech (using 2D texture)
//http://3dgep.com/?p=1116

//some older tech
//https://www.cs.auckland.ac.nz/~jvan006/multitex/multitex.html

//another approach: just megatexture it 
//http://forum.unity3d.com/threads/225765-Is-high-quality-rts-terrain-possible-in-Unity

//thing to experiment with: paintable decals
//so something like this but for terrain only
//http://blog.wolfire.com/2009/06/decals-editor-part-three/
//http://blog.wolfire.com/2009/06/how-to-project-decals/
//http://blog.wolfire.com/2009/05/decals-editor-part-one/

//(these could be added in tiled perhaps)
//(but i guess using a premade editor would be the best and hardest to do for it)
//so instead of storing a second uv set for decal info
//the decals are a second pass on the terrain
//aka a sort of partial megatexture


//starcraft2 tech
//http://www.gamedev.net/topic/607426-modern-rts-style-terrains-starcraft-ii-dow2-renegade-ops/



//set box to use
//todo -- save last update -- and only update if position changed considerably
// (but use a different method etc. for that)
void
xHeightMap::updateMesh(float x0, float y0, float w0, float h0)
{
  int i, k,yt;
  int ek, ei;
  int sk, si;

  sk = (int) floorf(x0 / cw);
  si = (int) floorf(y0 / ch);
  ek = sk + (int) ceilf(w0/cw);
  ei = si + (int) ceilf(h0/ch);

  //test values
  //sk = 0;  si = 0;  ek = 16;  ei = 16;

  if (sk > (mw-1)) { return; }
  if (si > (mh-1)) { return; }
  if (ek < 1) { return; }
  if (ei < 1) { return; }

  if (sk < 0) { sk = 0; }
  if (si < 0) { si = 0; }
  if (ek > (mw - 1)) { ek = mw-1;}
  if (ei > (mh - 1)) { ei = mh-1;}

  float h;
  unsigned int c;
  int v;
  int nface;

  int t;
  xHeightTile * ht;

  int t2;
  xHeightTile * uht;

  v = 0;
  nface = 0;

  for (i = si; i < ei; i++)
  {
    yt = i * mw;
    for (k = sk; k < ek; k++)
    {
      t = vecGrid[yt+k]; ht = &(tileSet[t]);
      t2 = vecTop[yt+k]; uht = &(tileSet[t2]);
    

      h = vecHeight[yt+k];
      c = vecColor[yt+k];
 
      mesh.vecVert[v].pos.set(k*cw+cx, h, i*ch+cz);
      mesh.vecVert[v].u = ht->u0; // 0.0f;
      mesh.vecVert[v].v = ht->v0; //0.0f;
      mesh.vecVert[v].u2 = uht->u0; // 0.0f;
      mesh.vecVert[v].v2 = uht->v0; //0.0f;
      //mesh.vecVert[v].u2 = (float)(k+1) / (float)mw;
      //mesh.vecVert[v].v2 = (float)(i+1) / (float)mw;
      mesh.vecVert[v].rgba = c;

      h = vecHeight[yt+k+1];
      c = vecColor[yt+k+1];

      mesh.vecVert[v+1].pos.set(k*cw+cx+cw, h, i*ch+cz);
      mesh.vecVert[v+1].u = ht->u1; //1.0f;
      mesh.vecVert[v+1].v = ht->v0; //0.0f;
      mesh.vecVert[v+1].u2 = uht->u1; //1.0f;
      mesh.vecVert[v+1].v2 = uht->v0; //0.0f;
      //mesh.vecVert[v+1].u2 = (float)(k+1) / (float)mw;
      //mesh.vecVert[v+1].v2 = (float)(i+1) / (float)mw;
      mesh.vecVert[v+1].rgba = c;
      
      h = vecHeight[yt+k+mw];
      c = vecColor[yt+k+mw];

      mesh.vecVert[v+2].pos.set(k*cw+cx, h, i*ch+cz+ch);
      mesh.vecVert[v+2].u = ht->u0; //0.0f;
      mesh.vecVert[v+2].v = ht->v1; //1.0f;
      mesh.vecVert[v+2].u2 = uht->u0; //0.0f;
      mesh.vecVert[v+2].v2 = uht->v1; //1.0f;
      //mesh.vecVert[v+2].u2 = (float)(k+1) / (float)mw;
      //mesh.vecVert[v+2].v2 = (float)(i+1) / (float)mw;
      mesh.vecVert[v+2].rgba = c;

      h = vecHeight[yt+k+1+mw];
      c = vecColor[yt+k+1+mw];

      mesh.vecVert[v+3].pos.set(k*cw+cx+cw, h, i*ch+cz+ch);
      mesh.vecVert[v+3].u = ht->u1; //1.0f;
      mesh.vecVert[v+3].v = ht->v1; //1.0f;
      mesh.vecVert[v+3].u2 = uht->u1; //1.0f;
      mesh.vecVert[v+3].v2 = uht->v1; //1.0f;
      //mesh.vecVert[v+3].u2 = (float)(k+1) / (float)mw;
      //mesh.vecVert[v+3].v2 = (float)(i+1) / (float)mw;
      mesh.vecVert[v+3].rgba = c;

 
      
      v += 4;
      nface += 2;
      if (v >= mesh.numVert) { mesh.drawFace = nface; return; } //reached max
    }//nextk
  }//nexti

  mesh.drawFace = nface;

}//updatemesh





void 
xHeightMap::initEmpty(int pw, int ph)
{
  clear();
  
 // int i, k, yt;
//  float h;
//  xRand xr;

  mw = pw;
  mh = ph;
    
  int num;
  num = mw*mh;
  


  vecGrid = new int[num];
  memset(vecGrid, 0, num*4);

  vecTop = new int[num];
  memset(vecTop, 0, num*4);

  vecColor = new unsigned int[num];
  memset(vecColor, 255, num*4);
  
  vecHeight = new float[num];
  memset(vecHeight, 0, num*4);
 
  /*
  for (i = 0; i < mh; i++)
  {
    yt = i * mw;
    for (k = 0; k < mw; k++)
    {
      //h = 0.0f;
//      h = xr.getRand() * 64;
      vecHeight[yt+k] = h;
    }//nextk
  }//nexti
*/
  
 
}//initempty


void 
xHeightMap::loadTileSet(std::string fname)
{
    pugi::xml_document xms;
 	  pugi::xml_node tex;
 	  pugi::xml_node img;
    xHeightTile * a;

    if  (!(xms.load_file(fname.c_str(), 0) ))	{		printf("Couldn't load file [%s] \n", fname);		return;	}//endif

    tex = xms.child("tiletex");
	    if (tex.empty() ) { return; } //todo error invalid file



    float picw;
    float pich;
    std::string picname;

    picw = tex.attribute("width").as_float();
    pich = tex.attribute("height").as_float();
    picname = tex.attribute("name").value(); 


    int id;
    float u0, v0;
    float u1, v1;

    //add half pixel to the uv (doesnt help much)
    float ax, ay;
    ax = (1.0f/picw)*0.5f;
    ay = (1.0f/pich)*0.5f;


     for (img = tex.child("image"); img; img = img.next_sibling("image")) 
     {
        id =  img.attribute("id").as_int();
        if (id < 0 || id >= XHEIGHTMAP_MAX_TILE) { continue; }

        a = &(tileSet[id]);

        u0 = img.attribute("x").as_float() / picw;
        v0 = img.attribute("y").as_float() / pich;
        u1 = u0 + img.attribute("width").as_float() / picw;
        v1 = v0 + img.attribute("height").as_float() / pich;

        //u0+=ax; v0+=ay;        u1-=ax; v1-=ay;

        a->u0 = u0;        a->u1 = u1;
        a->v0 = v0;        a->v1 = v1;

     }//nextimg


}//loaduv


void 
xHeightMap::loadLayer(int * vec, int w, int h)
{

 //todo -- only warning -- and crash 
 // if (w != mw || h != mh) { return; }

  int num;
  num = mw * mh;
   memcpy(vecGrid, vec, num*4); 

}//loadlayer



void 
xHeightMap::loadTopLayer(int * vec, int w, int h)
{

 //todo -- only warning -- and crash 
 // if (w != mw || h != mh) { return; }

  int num;
  num = mw * mh;
   memcpy(vecTop, vec, num*4); 

}//loadlayer


