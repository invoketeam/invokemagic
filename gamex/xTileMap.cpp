#pragma warning (disable : 4786)


#include <math.h>
#include "xGLCommon.h"


#include "xTileMap.h"
#include "xRand.h"

#include "pugixml/pugixml.hpp"


xTileMap::xTileMap(void)
{
	mwidth = 0;
	mheight = 0;
	vecGrid = 0;
  vecRect = 0;
  vecZtype = 0;

  crise = 32.0f;
	cw = 64.0f;
	ch = 64.0f;
	icw = 1.0f / cw;
	ich = 1.0f / ch;

	rayEndCx = 0.0f;
	rayEndCy = 0.0f;

	bVoidWall = true;


}//ctor




xTileMap::~xTileMap(void)
{
	clear();
}//dtor




void 
xTileMap::clear(void)
{
	if (vecGrid != 0) { delete [] vecGrid; } vecGrid = 0; 
  if (vecRect != 0) { delete [] vecRect; } vecRect = 0; 
  if (vecZtype != 0) { delete [] vecZtype; } vecZtype = 0;
	mwidth = 0;
	mheight = 0;
}//clear
	

bool 
xTileMap::isWallWorld(float wx, float wy)
{
	return isWall((int) floorf(wx*icw), (int) floorf(wy*ich));

}//iswall2


bool 
xTileMap::isWall(int tx, int ty)
{
	if (tx < 0) { return bVoidWall; }
	if (ty < 0) { return bVoidWall; }
	if (tx >= mwidth) { return bVoidWall; }
	if (ty >= mheight) { return bVoidWall; }

	return tileSet[vecGrid[tx+(ty*mwidth)]].wall > 0;
}//iswall


void 
xTileMap::setWall(int tx, int ty, int t)
{
  if (tx < 0) { return; }
	if (ty < 0) { return; }
	if (tx >= mwidth) { return; }
	if (ty >= mheight) { return; }

  vecGrid[tx+(ty*mwidth)] = t;
}//setwall


void 
xTileMap::setWallWorld(float wx, float wy, int t)
{
  setWall((int) floorf(wx*icw), (int) floorf(wy*ich), t);
}//setwallworld







int 
xTileMap::getZoneType(int tx, int ty)
{
	if (tx < 0) { return 0; }
	if (ty < 0) { return 0; }
	if (tx >= mwidth) { return 0; }
	if (ty >= mheight) { return 0; }

  return vecZtype[tx+(ty*mwidth)];
  //return tileSet[vecGrid[tx+(ty*mwidth)]].zt;
}//getzonetype
  

void 
xTileMap::setZoneType(int tx, int ty, int zt)
{
	if (tx < 0) { return; }
	if (ty < 0) { return; }
	if (tx >= mwidth) { return; }
	if (ty >= mheight) { return; }

  vecZtype[tx+(ty*mwidth)] = zt;
}//setzonetype

  
/*
int xTileMap::getZoneType2(int tile)
{
  if (tile < 0) { return -1; }
  if (tile > XTILEMAP_MAX_TILE) { return -1; }

 return tileSet[tile].zt;
}//getztype2
*/


xTile * xTileMap::getXTile(int tx, int ty)
{
	if (tx < 0) { return 0; }
	if (ty < 0) { return 0; }
	if (tx >= mwidth) { return 0; }
	if (ty >= mheight) { return 0; }

  return &(tileSet[vecGrid[tx+(ty*mwidth)]]);
}//getzone

  




void 
xTileMap::initEmpty(int mw, int mh)
{
  int num;

  clear();

	mwidth = mw;
	mheight = mh;

	num = mwidth * mheight;

  vecRect = new xRect[num];
  
  vecGrid = new int[num];
	memset(vecGrid, 0, num * 4);

  vecZtype = new int[num];
  memset(vecZtype, 0, num * 4);

  int i;
  for (i =0;i < num; i++) { vecZtype[i] = 1;}

}//initempty




void 
xTileMap::initFromLayer(int * vec, int mw, int mh)
{
  int num;

  clear();

	  mwidth = mw;
	  mheight = mh;
	  num = mwidth * mheight;

  
    vecRect = new xRect[num];
  
    vecGrid = new int[num];
      memcpy(vecGrid, vec, num*4); 


    //problem -- terrain related zonetype?
    //-- nah just use zonetype for dynamic objects for now
     vecZtype = new int[num];
       memset(vecZtype, 0, num * 4);

    //note -- zone type is flags -- 0 is not walkable under any circumstance
    //so we cant use memset as it sets it per byte, it would set it to 0x01010101 instead of 0x00000001
    int i;
    for (i =0;i < num; i++) { vecZtype[i] = 1;}

}//initlayer






/*
void 
xTileMap::genDebug(void)
{
	clear();

	int num;
  gamex::xRand r;



	tileSet[0].wall = 1;
  tileSet[0].mdx = 3;
  tileSet[0].zt = 8;

	tileSet[1].wall = 0;
  tileSet[1].mdx = 2;
  tileSet[1].zt = 2;

	tileSet[2].wall = 0;
  tileSet[2].mdx = 1;
  tileSet[2].zt = 1;

	tileSet[3].wall = 1;
  tileSet[3].mdx = 0;
  tileSet[3].zt = 2;


  



	mwidth = 64;
	mheight = 64;

	num = mwidth * mheight;

	vecGrid = new int[num];

	memset(vecGrid, 0, num * 4);

	int i;
	int k;


	k = (mheight - 1) * mwidth;
	for (i = 0; i < mwidth; i++)
	{
		vecGrid[i] = 1;
		vecGrid[i+ k] = 1;
		
	}//nexti

	for (i = 0; i < mheight; i++)
	{
		vecGrid[i*mwidth] = 1;
		vecGrid[(i*mwidth) + (mwidth-1) ] = 1;
		
		//vecGrid[(i+mwidth-1)*mwidth] = 1;

	}//nexti


  int x;
  int y;
  for (i = 0; i < 128; i++)
  {
      x = r.getRand() * mwidth;
      y = r.getRand() *mheight;

//      vecGrid[ x +(y*mwidth)] = 1;
      this->setRect(x,y, r.getRand()*8, r.getRand()*4,1);

  }

}//gendebug
*/


void 
xTileMap::genHeightRect(void)
{ 
  int yt;
	int i;
	int k;
  float ax, ay;
  float ah;
	xTile * a;
  xRect * w;

	  for (i = 0; i < mheight; i++)
	  {
	    yt = i * mwidth;

		  for (k = 0; k < mwidth; k++)
		  {
			  a = &(tileSet[vecGrid[k+yt]]);
        w = &(vecRect[k+yt]);

        ax = k * cw;
        ay = i * ch;
        ah =  a->height * crise;

        w->vert[0].set(ax,ah,ay);
        w->vert[1].set(ax,ah,ay+ch);  
        w->vert[2].set(ax+cw,ah,ay);
        w->vert[3].set(ax+cw,ah,ay+ch);


      }//nextk
    }//nexti


    int ei;
    int ek;

    ei = mheight- 1;
    ek = mwidth - 1;

	  for (i = 1; i < ei; i++)
	  {
	    yt = i * mwidth;

		  for (k = 1; k < ek; k++)
		  {
        w = &(vecRect[k+yt]);

  /*
  //check neighboring tiles
  //adjust vertex height    
    aaa
    aWa
    aaa
    */
       
  //corners
        a = getTile(k-1, i-1);
        if (a->spec == 303)
        { w->vert[0].y = a->height * crise; }

        a = getTile(k+1, i-1);
        if (a->spec == 303)
        { w->vert[2].y = a->height * crise; }

        a = getTile(k-1, i+1);
        if (a->spec == 303)
        { w->vert[1].y = a->height * crise; }


        a = getTile(k+1, i+1);
        if (a->spec == 303)
        { w->vert[3].y = a->height * crise; }


    //edges
        a = getTile(k, i-1);
        if (a->spec == 303) 
        { w->vert[0].y = a->height * crise;
          w->vert[2].y = a->height * crise; }


        a = getTile(k-1, i);
        if (a->spec == 303) 
        { w->vert[0].y = a->height * crise;
          w->vert[1].y = a->height * crise; }


        a = getTile(k+1, i);
        if (a->spec == 303) 
        { w->vert[2].y = a->height * crise;
          w->vert[3].y = a->height * crise; }

        a = getTile(k, i+1);
        if (a->spec == 303) 
        { w->vert[1].y = a->height * crise;
          w->vert[3].y = a->height * crise; }

      }//nextk
    }//nexti


}//genhrect



void 
xTileMap::setSkinFromLayer(int * vec, int mw, int mh)
{
  if (mw != mwidth) { return; }
  if (mh != mheight) { return; }

  int i;
  int num;
  num = mwidth * mheight;

  for (i = 0; i < num; i++)
  {
    vecRect[i].skin = vec[i] - 1; 
  }

}//setskin



void 
xTileMap::debApplyHeightMap(void)
{
  xRand rnd;
  float * temp;
  int tw, th;
  int num;
  int i, k, yt;
  xRect * w;

  tw = mwidth + 1;
  th = mheight + 1;
  num = tw * th;

  temp = new float[num];
  //for (i = 0; i < num; i++) { temp[i] = 0;  }
   for (i = 0; i < num; i++) { temp[i] =  rnd.getRand()*64;}

  //02
  //13

  for (i = 0; i < mheight; i++)
	{
	    yt = i * mwidth;

		  for (k = 0; k < mwidth; k++)
		  {
        w = &(vecRect[k+yt]);

        w->vert[0].y += temp[k+yt];
        w->vert[1].y += temp[k+yt+mwidth];
        w->vert[2].y += temp[k+yt+1];
        w->vert[3].y += temp[k+yt+mwidth+1];

        //also debug -- skin
        w->skin = 1; // 24 + rnd.getRand() * 40;
  
      }//nextk
  }//nexti

  delete [] temp;
}//debapplyheight

 

void 
xTileMap::applyHeightMap(xImage * img, float yscale)
{
  //if (vecRect == 0) { return; } //nah, crash instead

  xImage * tmp; 
  float * vec;
  unsigned int * dat;
  int num;
  int i;
  int k;
  int yt;  
  xRect * w;

  tmp = 0;
  vec = 0;

  //resize image if not exact size (todo -- issue an warning if this happens)

  tmp = new xImage();
   tmp->init(mwidth, mheight);
  if (img->mw != mwidth || img->mh != mheight) {    tmp->resize(img, mwidth, mheight); } else { tmp->drawImage(img, 0,0); }
 
  num = mwidth * mheight;
  vec = new float[num];
  dat = tmp->dat;

  //todo -- set this from parameter
  //yscale = 4.0f; //2.0f;

  //convert blue channel to height
  for (i = 0; i < num; i++) 
  { 
    //if (i < 10) { printf("height %0.2f \n ", (float)((dat[i]>>8) & 0xFF));  }
    vec[i] =  (float)((dat[i]>>8) & 0xFF) * yscale; 
  }

  for (i = 0; i < mheight; i++)
	{
	    yt = i * mwidth;

		  for (k = 0; k < mwidth; k++)
		  {
        w = &(vecRect[k+yt]);

        w->vert[0].y += vec[k+yt];
        w->vert[1].y += vec[k+yt+mwidth];
        w->vert[2].y += vec[k+yt+1];
        w->vert[3].y += vec[k+yt+mwidth+1];

      }//nextk
  }//nexti



  if (tmp != 0) { delete tmp; }
  if (vec != 0) { delete [] vec; }
}//applyheight





void 
xTileMap::debLoadUv(std::string fname)
{
    pugi::xml_document xms;
 	  pugi::xml_node tex;
 	  pugi::xml_node img;
    xTile * a;

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
        if (id < 0 || id >= XTILEMAP_MAX_TILE) { continue; }

        a = &(tileSet[id]);

        u0 = img.attribute("x").as_float() / picw;
        v0 = img.attribute("y").as_float() / pich;
        u1 = u0 + img.attribute("width").as_float() / picw;
        v1 = v0 + img.attribute("height").as_float() / pich;


        u0+=ax; v0+=ay;
        u1-=ax; v1-=ay;

        a->uv[0].x = u0;
        a->uv[0].y = v0;

        a->uv[1].x = u0;
        a->uv[1].y = v1;
 
        a->uv[2].x = u1;
        a->uv[2].y = v0;
 
        a->uv[3].x = u1;
        a->uv[3].y = v1;


     }//nextimg


}//loaduv







int
xTileMap::getNumTri(int sx, int sy, int sw, int sh)
{
  int yt, i, k;
  xTile * st;  xRect * w;  xRect * w2;
  int nface, nvert;  
  int ex, ey;
  xRand rnd;
 
  nface = 0;
  nvert = 0;

  ex = sx + sw;
  ey = sy + sh;

  if (sx < 0) { sx = 0; } if (sy < 0) { sy = 0;}
  if (ex < 0) { return 0; } if (ey < 0) { return 0;}
  if (sx >= mwidth) { return 0; } if (sy >= mheight) { return 0; }
  if (ex > mwidth) { ex = mwidth; } if (ey > mheight) { ey = mheight;}

  //count how many faces/vertices we need
  //(need to do the full loop because we need to know how many walls we need)
	  for (i = sy; i < ey; i++)
	  {  yt = i * mwidth;
		  for (k = sx; k < ex; k++)
		  {    
        w = &(vecRect[k+yt]);
        st = &(tileSet[w->skin]);

        nface += 2;
        nvert += 4;
        if (i < (mheight-1))
        {
          w2 = &(vecRect[k+yt+mwidth]);
          if (w->vert[1].y != w2->vert[0].y) { nface += 1; nvert += 3; }       
          if (w->vert[3].y != w2->vert[2].y) { nface += 1; nvert += 3; }
        }//endif

        if (k < (mwidth-1))
        {
          w2 = &(vecRect[k+yt+1]);       
          if (w->vert[2].y != w2->vert[0].y) { nface += 1; nvert += 3; }
          if (w->vert[3].y != w2->vert[1].y) { nface += 1; nvert += 3; }
        }//endif
      }//nextk
    }//nexti

  
//  printf("numface needed %d   numvert %d \n", nface, nvert);

  return nface;
}//getnumtri



//todo -- makeMesh in multiple resolutions (allow skipping of every second vertex or something )
//todo -- set start and end position of tiles to generate
void 
xTileMap::makeMesh(xMdx3 * mesh, int sx, int sy, int sw, int sh)
{
  int yt, i, k;
  xTile * st;  xRect * w;  xRect * w2;
  int nface, nvert;  int f, v;
  short * vecIndex;  mVert * vecVert;
  int ex, ey;
  xRand rnd;

  nface = 0;
  nvert = 0;

  ex = sx + sw;
  ey = sy + sh;

  if (sx < 0) { sx = 0; } if (sy < 0) { sy = 0;}
  if (ex < 0) { return; } if (ey < 0) { return;}
  if (sx >= mwidth) { return; } if (sy >= mheight) { return; }
  if (ex > mwidth) { ex = mwidth; } if (ey > mheight) { ey = mheight;}


  //count how many faces/vertices we need
  //(need to do the full loop because we need to know how many walls we need)
	  for (i = sy; i < ey; i++)
	  {  yt = i * mwidth;
		  for (k = sx; k < ex; k++)
		  {    
        w = &(vecRect[k+yt]);
        st = &(tileSet[w->skin]);

        nface += 2;
        nvert += 4;
        if (i < (mheight-1))
        {
          w2 = &(vecRect[k+yt+mwidth]);
          if (w->vert[1].y != w2->vert[0].y) { nface += 1; nvert += 3; }       
          if (w->vert[3].y != w2->vert[2].y) { nface += 1; nvert += 3; }
        }//endif

        if (k < (mwidth-1))
        {
          w2 = &(vecRect[k+yt+1]);       
          if (w->vert[2].y != w2->vert[0].y) { nface += 1; nvert += 3; }
          if (w->vert[3].y != w2->vert[1].y) { nface += 1; nvert += 3; }
        }//endif
      }//nextk
    }//nexti

  
  //printf("numface needed %d   numvert %d \n", nface, nvert);
 
  mesh->clear();

  mesh->numFace = nface;
  mesh->numVert = nvert;

  vecIndex = new short[nface*3];
  vecVert = new mVert[nvert];

  mesh->vecIndex = vecIndex;
  mesh->vecVert  = vecVert;


  f = 0;
  v = 0;

  unsigned int topColor;
  unsigned int sideColora;
  unsigned int sideColorb;
  //topColor = 0xFFffFFff;
  topColor =0x00FFFFFF;
  sideColora = 0xFF808080;
  sideColorb = 0xFFa0a0a0;


	  for (i = sy; i < ey; i++)
	  {
	    yt = i * mwidth;
		  for (k = sx; k < ex; k++)
		  {
        w = &(vecRect[k+yt]);
        st = &(tileSet[w->skin]);

        //printf("f %d v %d \n",f,v);

        //ground quad

        vecIndex[f] = v + 0;
        vecIndex[f+1] = v + 1;
        vecIndex[f+2] = v + 2;
        vecIndex[f+3] = v + 2;
        vecIndex[f+4] = v + 1;
        vecIndex[f+5] = v + 3;
        f += 6;

        vecVert[v].pos = w->vert[0];
        vecVert[v].u = st->uv[0].x;
        vecVert[v].v = st->uv[0].y;
        vecVert[v].rgba = topColor;
        v+=1;

        vecVert[v].pos = w->vert[1];
        vecVert[v].u = st->uv[1].x;
        vecVert[v].v = st->uv[1].y;
        vecVert[v].rgba = topColor;
        v+=1;

        vecVert[v].pos = w->vert[2];
        vecVert[v].u = st->uv[2].x;
        vecVert[v].v = st->uv[2].y;
        vecVert[v].rgba = topColor;
        v+=1;

        vecVert[v].pos = w->vert[3];
        vecVert[v].u = st->uv[3].x;
        vecVert[v].v = st->uv[3].y;
        vecVert[v].rgba = topColor;
        v+=1;



        //walls
        if (i < (mheight-1))
        {
          w2 = &(vecRect[k+yt+mwidth]);

          //02
          //13
          //02
          //13

          if (w->vert[1].y != w2->vert[0].y)
          {
            vecIndex[f] = v + 0;  vecIndex[f+1] = v + 1;    vecIndex[f+2] = v + 2;
            f += 3;

            vecVert[v].pos = w->vert[1];
            vecVert[v].u = st->uv[1].x;
            vecVert[v].v = st->uv[1].y;
            vecVert[v].rgba = sideColora;
            v+=1;

            vecVert[v].pos = w2->vert[0];
            vecVert[v].u = st->uv[0].x;
            vecVert[v].v = st->uv[0].y;
            vecVert[v].rgba = sideColora;
            v+=1;

            vecVert[v].pos = w2->vert[2];
            vecVert[v].u = st->uv[2].x;
            vecVert[v].v = st->uv[2].y;
            vecVert[v].rgba = sideColora;
            v+=1;

          }//endif2
         
          if (w->vert[3].y != w2->vert[2].y)
          {      
            vecIndex[f] = v + 0;  vecIndex[f+1] = v + 1;    vecIndex[f+2] = v + 2;
            f += 3;

            vecVert[v].pos = w->vert[1];
            vecVert[v].u = st->uv[1].x;
            vecVert[v].v = st->uv[1].y;
            vecVert[v].rgba = sideColora;
            v+=1;

            vecVert[v].pos = w2->vert[2];
            vecVert[v].u = st->uv[2].x;
            vecVert[v].v = st->uv[2].y;
            vecVert[v].rgba = sideColora;
            v+=1;

            vecVert[v].pos = w->vert[3];
            vecVert[v].u = st->uv[3].x;
            vecVert[v].v = st->uv[3].y;
            vecVert[v].rgba = sideColora;
            v+=1;
          }//endif2


        }//endif


        if (k < (mwidth-1))
        {
          w2 = &(vecRect[k+yt+1]);

          //0202
          //1313
         
          if (w->vert[2].y != w2->vert[0].y)
          {
            vecIndex[f] = v + 0;  vecIndex[f+1] = v + 1;    vecIndex[f+2] = v + 2;
            f += 3;

            vecVert[v].pos = w->vert[2];
            vecVert[v].u = st->uv[2].x;
            vecVert[v].v = st->uv[2].y;
            vecVert[v].rgba = sideColorb;
            v+=1;

            vecVert[v].pos = w2->vert[1];
            vecVert[v].u = st->uv[1].x;
            vecVert[v].v = st->uv[1].y;
            vecVert[v].rgba = sideColorb;
            v+=1;

            vecVert[v].pos = w2->vert[0];
            vecVert[v].u = st->uv[0].x;
            vecVert[v].v = st->uv[0].y;
            vecVert[v].rgba = sideColorb;
            v+=1;
          }//endif2


          if (w->vert[3].y != w2->vert[1].y)
          {
            vecIndex[f] = v + 0;  vecIndex[f+1] = v + 1;    vecIndex[f+2] = v + 2;
            f += 3;

            vecVert[v].pos = w->vert[3];
            vecVert[v].u = st->uv[3].x;
            vecVert[v].v = st->uv[3].y;
            vecVert[v].rgba = sideColorb;
            v+=1;

            vecVert[v].pos = w2->vert[1];
            vecVert[v].u = st->uv[1].x;
            vecVert[v].v = st->uv[1].y;
            vecVert[v].rgba = sideColorb;
            v+=1;

            vecVert[v].pos = w->vert[2];
            vecVert[v].u = st->uv[2].x;
            vecVert[v].v = st->uv[2].y;
            vecVert[v].rgba = sideColorb;
            v+=1;

          }//endif2
        }//endif

      }//nextk
    }//nexti

   // printf("v f %d %d \n", v, f);

    mesh->calcMinMax();
}//makemesh2






void
xTileMap::render(void)
{
  int yt;
	int i;
	int k;
//	xTile * a;
  xTile * st; //skintile
  xRect * w;
  xRect * w2;

  glColor3f(1,1,1);
  glPointSize(4);
  //glBegin(GL_POINTS);
  glBegin(GL_TRIANGLES);
	  for (i = 0; i < mheight; i++)
	  {
	    yt = i * mwidth;

		  for (k = 0; k < mwidth; k++)
		  {
/*
			  a = &(tileSet[vecGrid[k+yt]]);    
        //todo -- maybe height should be in units? .. 
        glVertex3f(k*cw, a->height*crise, i*ch);
*/
      //note -- immedieate mode is debug only
      //finished rendering should be based on chunks or something
      //aka -- generate the whole heightmap into a mdx

        w = &(vecRect[k+yt]);

        st = &(tileSet[w->skin]);

     glColor3f(1,1,1);
      glTexCoord2f(st->uv[0].x, st->uv[0].y);
        glVertex3f(w->vert[0].x,w->vert[0].y,w->vert[0].z );
      glTexCoord2f(st->uv[1].x, st->uv[1].y);
        glVertex3f(w->vert[1].x,w->vert[1].y,w->vert[1].z );
      glTexCoord2f(st->uv[2].x, st->uv[2].y);
        glVertex3f(w->vert[2].x,w->vert[2].y,w->vert[2].z );

      glColor3f(0.8f,0.8f,0.8f);
      glTexCoord2f(st->uv[2].x, st->uv[2].y);
        glVertex3f(w->vert[2].x,w->vert[2].y,w->vert[2].z );
      glTexCoord2f(st->uv[1].x, st->uv[1].y);
        glVertex3f(w->vert[1].x,w->vert[1].y,w->vert[1].z );        
      glTexCoord2f(st->uv[3].x, st->uv[3].y);
        glVertex3f(w->vert[3].x,w->vert[3].y,w->vert[3].z );
         

/*
      glColor3f(1,1,1);
        glVertex3f(w->vert[0].x,w->vert[0].y,w->vert[0].z );
        glVertex3f(w->vert[1].x,w->vert[1].y,w->vert[1].z );
        glVertex3f(w->vert[2].x,w->vert[2].y,w->vert[2].z );

      glColor3f(0.8,0.8,0.8);
        glVertex3f(w->vert[2].x,w->vert[2].y,w->vert[2].z );
        glVertex3f(w->vert[1].x,w->vert[1].y,w->vert[1].z );        
        glVertex3f(w->vert[3].x,w->vert[3].y,w->vert[3].z );
*/



//no texture for these yet

      glColor3f(1,0,0);
        //walls
        if (i < (mheight-1))
        {
          w2 = &(vecRect[k+yt+mwidth]);
          //02
          //13
          //02
          //13

          if (w->vert[1].y != w2->vert[0].y)
          {
          glTexCoord2f(st->uv[1].x, st->uv[1].y);
            glVertex3f(w->vert[1].x,w->vert[1].y,w->vert[1].z );
          glTexCoord2f(st->uv[0].x, st->uv[0].y);
            glVertex3f(w2->vert[0].x,w2->vert[0].y,w2->vert[0].z );
          glTexCoord2f(st->uv[2].x, st->uv[2].y);
            glVertex3f(w2->vert[2].x,w2->vert[2].y,w2->vert[2].z );
          }
         
          if (w->vert[3].y != w2->vert[2].y)
          {
          glTexCoord2f(st->uv[1].x, st->uv[1].y);
            glVertex3f(w->vert[1].x,w->vert[1].y,w->vert[1].z );
          glTexCoord2f(st->uv[2].x, st->uv[2].y);
            glVertex3f(w2->vert[2].x,w2->vert[2].y,w2->vert[2].z );
          glTexCoord2f(st->uv[3].x, st->uv[3].y);
            glVertex3f(w->vert[3].x,w->vert[3].y,w->vert[3].z );
          }


        }//endif


       glColor3f(0.5,0,0);
        if (k < (mwidth-1))
        {
          w2 = &(vecRect[k+yt+1]);

          //0202
          //1313
         
          if (w->vert[2].y != w2->vert[0].y)
          {
           glTexCoord2f(st->uv[2].x, st->uv[2].y);
            glVertex3f(w->vert[2].x,w->vert[2].y,w->vert[2].z );
           glTexCoord2f(st->uv[1].x, st->uv[1].y);
            glVertex3f(w2->vert[1].x,w2->vert[1].y,w2->vert[1].z );
           glTexCoord2f(st->uv[0].x, st->uv[0].y);
            glVertex3f(w2->vert[0].x,w2->vert[0].y,w2->vert[0].z );

          }


          if (w->vert[3].y != w2->vert[1].y)
          {
           glTexCoord2f(st->uv[3].x, st->uv[3].y);
            glVertex3f(w->vert[3].x,w->vert[3].y,w->vert[3].z );
          glTexCoord2f(st->uv[1].x, st->uv[1].y);
            glVertex3f(w2->vert[1].x,w2->vert[1].y,w2->vert[1].z );
          glTexCoord2f(st->uv[2].x, st->uv[2].y);
            glVertex3f(w->vert[2].x,w->vert[2].y,w->vert[2].z );

          }

        }//endif

      }//nextk

    }//nexti
  glEnd();

}//render



bool 
xTileMap::rayCast2(float ax, float ay, float bx, float by)
{
	return rayCast(		(int) floorf(ax * icw),		(int) floorf(ay * ich),
		(int) floorf(bx * icw),		(int) floorf(by * ich) );
}//raycast


bool 
xTileMap::rayCast(int x0, int y0, int x1, int y1)
{
	int w, h;
	int kx, ky;
	int dx0, dy0, dx1, dy1;
	int i;
	int longest, shortest, numerator;
	bool ret;

	ret = false;
	
		 
	w = x1 - x0;
	h = y1 - y0;
				
	dx1 = 0; dy1 = 0;
	dx0 = 0; dy0 = 0;
				
	if (w<0) dx0 = -1 ; 
	else if (w>0) dx0 = 1 ;
				
	if (h<0) dy0 = -1 ; 
	else if (h>0) dy0 = 1 ;
				
	if (w<0) dx1 = -1 ; 
	else if (w>0) dx1 = 1 ;
				
	longest = w < 0 ? -w : w; // Math.abs(w) ;
	shortest = h < 0 ? -h : h; // Math.abs(h) ;
				
	if (!(longest>shortest)) {
				
      longest = h < 0 ? -h: h; // Math.abs(h) ;
      shortest = w < 0 ? - w : w; // Math.abs(w) ;
            
      if (h<0)  { dy1 = -1; } 
      else if (h>0) { dy1 = 1;  }				
               
           dx1 = 0;
           
    }//endif
    
	numerator= longest >> 1 ;
					 
	 kx = x0;
	 ky = y0;

					
					for (i=0;i<=longest;i++) {
				
					   
                
							 numerator += shortest ;
					
							 if (!(numerator<longest)) {          
								 numerator -= longest ;
								 kx += dx0 ;
								 ky += dy0 ;
						  
	
								  if (isWall(kx, ky)) { kx -= dx0; ky -= dy0; ret = true; break; }
										  
							 } else {  
								 kx += dx1 ;
								 ky += dy1 ;
 
								  if (isWall(kx, ky)) { kx -= dx1; ky -= dy1; ret = true; break; }
							 }//endif
							
							 
							 //add depth (hack -- only string pull if you 300% sure you can fit through
							  if (isWall(kx + 1, ky - 1)) { ret = true; break; }
							  if (isWall(kx + 1, ky + 1)) { ret = true; break;}
							  if (isWall(kx - 1, ky - 1)) { ret = true; break; }
							  if (isWall(kx - 1, ky + 1)) { ret = true; break; }
						
						
                 }//nexti
				 
				 rayEndCx = kx*cw + (cw*0.5f);
				 rayEndCy = ky*ch + (ch*0.5f);
				 
				 //problem is that so far false was wall and true was no wall
				 
				 return ret;

}//raycast


void 
xTileMap::copyTmap(xTileMap * a)
{
	clear();

	mwidth = a->mwidth;
	mheight = a->mheight;
	
	int num;
	num = mwidth * mheight;

	vecGrid = new int[num];

	memcpy(vecGrid, a->vecGrid, num * 4);

	//for now copy the whole tileset as is
	memcpy(tileSet, a->tileSet, (XTILEMAP_MAX_TILE * sizeof(xTile)) );


	cw = a->cw;
	ch = a->ch;
	
	icw = 1.0f / cw;
	ich = 1.0f / ch;

	rayEndCx = 0.0f;
	rayEndCy = 0.0f;

	bVoidWall = a->bVoidWall;
}//copytmap


void 
xTileMap::debHeightRender(void)
{
  float k;
  float i;

glPointSize(3);
glColor3f(0,1,0);
glBegin(GL_POINTS);
  for (i = 0; i < 512; i+=16)
  {
    for (k = 0; k < 512; k+=16)
    {
      glVertex3f(k, getHeight(k,i) , i);
    }
  }//nexti
glEnd();

}//debheight


float 
xTileMap::getHeight(float wx, float wy)
{
  int tx, ty;
  xRect * a;
  tx = (int) floorf(wx*icw);
  ty = (int) floorf(wy*ich);

	if (tx < 0) { return 0.0f; }
	if (ty < 0) { return 0.0f; }
	if (tx >= mwidth) { return 0.0f; }
	if (ty >= mheight) { return 0.0f; }

  a = &(vecRect[tx+(ty*mwidth)]);

  //todo -- calculate height from 4 vertices

  float h1, h2, h3;
  float u, v;
  
  u = (wx*icw) - tx;
  v = (wy*ich) - ty;

  h1 = a->vert[0].y + (a->vert[2].y - a->vert[0].y) * u;
  h2 = a->vert[1].y + (a->vert[3].y - a->vert[1].y) * u;
  h3 = h1 + (h2 - h1) * v;

 //02
 //13

 //ab
 //cd

        /*
        h1 = a + (b-a) * h;
        h2 = c + (d-c) * h;
        h3 = h1 + (h2-h1) * v;
*/

  return h3;
}//getheight


xTile * 
xTileMap::getTile(int tx, int ty)
{
	if (tx < 0) { return 0; }
	if (ty < 0) { return 0; }
	if (tx >= mwidth) { return 0; }
	if (ty >= mheight) { return 0; }

	return &(tileSet[vecGrid[tx+(ty*mwidth)]]) ;
}//gettile


xTile *
xTileMap::getTileWorld(float wx, float wy)
{
  int tx, ty;
  tx = (int) floorf(wx*icw);
  ty = (int) floorf(wy*ich);

	if (tx < 0) { return 0; }
	if (ty < 0) { return 0; }
	if (tx >= mwidth) { return 0; }
	if (ty >= mheight) { return 0; }

	return &(tileSet[vecGrid[tx+(ty*mwidth)]]) ;

}//gettile2




bool
xTileMap::canMove(float wx, float wy, float gx, float gy)
{
  int h;
  xTile * a;
  xTile * b;
  
  h = 0;

  a = getTileWorld(wx, wy);
  b = getTileWorld(gx, gy);

  if (b == 0) { return false; }
  if (a != 0) { h = a->height; }

  //special -- ramp
  if (a->spec == 303 || b->spec == 303) { return true; }

  if (h != b->height) { return false;}
  if (b->wall > 0) { return false; }

  //same height
  return true;
}//canmove



void 
xTileMap::setRectWorld(float wx, float wy, float ww, float wh, int c)
{
  setRect( (int) floorf(wx*icw), (int) floorf(wy*ich), (int) floorf(ww*icw), (int) floorf(wh*ich) , c);
}//rectworld


void 
xTileMap::setRect(int ax, int ay, int aw, int ah, int c)
{
	int ex, ey;
	int i;
	int k;
	int yt;

	ex = ax + aw;   
	ey = ay + ah;
           
	if (ax < 0) { ax = 0;  }       
	if (ay < 0) { ay = 0;  }
    
	if (ex > mwidth) { ex = mwidth;   }    
	if (ey > mheight) { ey = mheight;    }
    
	if (ax >= mwidth) { return;  }    
	if (ay >= mheight){ return;  }
    
	if (ex < 0) { return;  }        
	if (ey < 0) { return;  }
            
	for (i = ay; i < ey; i++) 
	{ 
		yt = i * mwidth;	
		for (k = ax; k < ex; k++) 
		{
			vecGrid[yt+k] = c;  
		} 
	
	}
	
}//setrect


