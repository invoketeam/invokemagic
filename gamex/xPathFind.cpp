#pragma warning (disable : 4786)

#include <math.h>
#include <algorithm> 



#include <math.h>

#include "xGLCommon.h"


#include "xPathFind.h"
#include "xTileMap.h"
#include "xRand.h"


class xQuad
{
public:
	tdVecZone vec; //only store pointers, main storage is xPathFind
	int tx;
	int ty;
	int dirty;

public:
	xQuad(void);

	void clear(void); //mark all zones in vec as dead
	void cleanDead(void); //get rid of dead zones from the vecNext (connections) of each zone in vec

};//xquad


xQuad::xQuad(void)
{
	tx = 0;
	ty = 0;
	dirty = 0;
}//ctor



void
xQuad::clear(void)
{ 
	tdVecZone ::iterator it;

//printf("quad clear %d \n", vec.size() );

	for (it = vec.begin(); it != vec.end(); it++)
	{
		(*it)->dead = true;
	}
	
	vec.clear();
}//clear



void 
xQuad::cleanDead(void)
{
	//int i;
	//int num;
	//xPathZone * a;
	xPathZone * b;
	int numk;
	int k;

	tdVecZone ::iterator it;
	
  
 	for (it = vec.begin(); it != vec.end(); it++)
	{
		tdVecZone &vk = ((*it)->vecNext);
		numk = vk.size();

		for (k = 0; k < numk; k++)
		{
			b = vk[k];
			if (!(b->dead)) { continue; }

      
			vk[k] = vk[numk - 1];
			k -= 1;
			numk -= 1;
			vk.pop_back();

		}//nextk
	
	}//nextit

}//clean













xPathZone::xPathZone()
{
	cx = 0;
	cy = 0;
	cw = 0;
	ch = 0;
	mx = 0;
	my = 0;
	cx2 = 0;
	cy2 = 0;

  zt = 0;
  height = 0;
  spec = 0;

	group = 0;
	col = 0;

	cost = 0;
	frame = 0;

	parent = 0;

	dead = false;


}//ctor_pathzone

















xPathFind::xPathFind(void) 
{
	maxOpen = 8192;
	vecOpen = 0;
	vecOpen = new tdZonePtr[maxOpen];

	curFrame = 0;
	oit = 0;

	startx = 0;
	starty = 0;
	endx = 0;
	endy = 0;

	start = 0;
	goal = 0;
	lastZone = 0;
	
	bFinish = false;

		qcell = 16;
	qwidth = 0;
	qheight = 0;
	cw = 0;
	ch = 0;

  mw = 0;
  mh = 0;
  
  vecQuad  = 0;


}//ctor_pathfind



xPathFind::~xPathFind(void) 
{
	clear();

	if (vecOpen != 0) { delete [] vecOpen; vecOpen = 0; }

}//dtor



void 
xPathFind::clear(void)
{
	
  if (vecQuad != 0) { delete [] vecQuad; vecQuad = 0; }
  
	//vecQuad.clear(); //only contains pointers to zones

	tdVecZone ::iterator it;

	for (it = vecZone.begin(); it != vecZone.end(); it++)
	{
		delete (*it);
	}

	vecZone.clear();
}//clear




void 
xPathFind::debRender(float ay)
{
	xPathZone * a;
  xPathZone * b;

	tdVecZone ::iterator it;
	tdVecZone ::iterator kt;

	glColor3f(0.5f, 0.5f, 0.5f);
	glLineWidth(1);


  float m;
  float y;

  m = 0; 


	for (it = vecZone.begin(); it != vecZone.end(); it++)
	{
		a = (*it);

  /*  glPushMatrix();
     
      glScalef(a->cw, 0.5, a->ch);
       glTranslatef(a->cx, 0, a->cy);

        glutWireCube(1);
    glPopMatrix();*/

    y = ay + (a->height * 32.0f);


    glColor3f(0,1, 0);
		glBegin(GL_LINES);
			glVertex3f(a->cx+m, y, a->cy+m);
			glVertex3f(a->cx+m, y, a->cy2-m);
		
			glVertex3f(a->cx2-m, y, a->cy+m);
			glVertex3f(a->cx2-m, y, a->cy2-m);


			glVertex3f(a->cx+m, y, a->cy+m);
			glVertex3f(a->cx2-m, y, a->cy+m);

			glVertex3f(a->cx+m, y, a->cy2-m);
			glVertex3f(a->cx2-m, y, a->cy2-m);

		glEnd();

    glColor3f(1,0,0);

//seems that certain areas are not connected ???
    for (kt = a->vecNext.begin(); kt != a->vecNext.end(); kt++)
    {
        b = (*kt);
        glBegin(GL_LINES);
            glVertex3f(a->mx, y, a->my);
            glVertex3f(b->mx, y, b->my);

        glEnd();
    }//nexkt

	}//nextit

	glLineWidth(1);
}//debrender







//todo -- two findpath mode, one to consider unwalkable zones as walls
//and one that doesnt
//only use the second if the first one fails

void 
xPathFind::findPath(int numstep, int mode)
{
//it is oit

    if (bFinish) { return; }
    
    int i;
    xPathZone * a;
    int k;
    xPathZone * b;
    //int num;
    int nk;
    double mcost;
    
	tdVecZone ::iterator kt;

    for (i = 0; i < numstep; i++)
    {
      if (oit <= 0)  //path not found
      {
        lastZone = 0;
        bFinish = true;
        return; 
      }//endif
    

		//get node with shortest cost from node
		//(this could use an associative array but was fast enough in flash, therefore its fast enough for c++)
        mcost = 4503599627370496;
        a = vecOpen[0];
        for (k = 0; k < oit; k++)
        {
            b = vecOpen[k];
            if (b->cost < mcost)
            {
               mcost = b->cost;
               a = b;
               nk = k;
            }
        }//nextk
        

        //take out the node from the open
          vecOpen[nk] = vecOpen[oit - 1];
          oit -= 1;

        //check if goal is reached
          if (a == goal)
          { 
            lastZone = a;
            bFinish = true;
            return; 
          }//endif
                
                
          //add neighbours of node to open
          
         //  vec = a.vecNext;
         //   num = vec.length;
            
           // for (k = 0; k < num; k++)
			for (kt = a->vecNext.begin(); kt != a->vecNext.end(); kt++)
            {
                  //b = vec[k];
			          b = (*kt);
               if (b->frame == curFrame) { continue; } 
               b->frame = curFrame;

                    if (mode == 0 && ((b->zt & zoneFlag) == 0)) 
                    {     continue;    }


              //todo -- add zone height to equation? so far height change has no movement speed penalty

              //the start position is special
              //we need to consider our start point instead of the zone center
              if (a == start)
              {
                  b->cost = mcost + sqrtf((startx - b->mx) * (startx - b->mx) + (starty - b->my) * (starty - b->my));
              }
              else
              {
              
                     b->cost = mcost + sqrtf((a->mx - b->mx) * (a->mx - b->mx) + (a->my - b->my) * (a->my - b->my));
                    
                     if ((b->zt & zoneFlag) == 0) 
                     {
                        b->cost+= 99999999; 
                       // b->cost = 4503599627370496;
                     }


              }//endif


               b->parent = a;
               

                vecOpen[oit] = b;
                oit += 1;
            }//nextk
    
    }//nexti
    
}//findpath







int 
xPathFind::startPath(float sx, float sy, float ex, float ey, int zflag)
{
  xPathZone * a;
  xPathZone * b;
  
  a = getZone(sx, sy);
  b = getZone(ex, ey);
  
  if (a == 0) { return -1; } //no start
  if (b == 0) { return -2; } //no goal
  if (a == b) { return 0; } //same zone
  if (a->group != b->group) { return -99; } //unreachable
  

  zoneFlag = zflag;

  startx = sx;
  starty = sy;
  
  endx = ex;
  endy = ey;
  
  startPath2(a, b);
  
  return 1;
}//startpath






void 
xPathFind::startPath2(xPathZone * startzone, xPathZone * goalzone)
{
     bFinish = false;
    
     lastZone = 0;
        
     start = startzone;
     goal = goalzone;
            
     curFrame += 1;
        
     vecOpen[0] = start;
     oit = 1;
            
     start->frame = curFrame;
     start->parent = 0;
     start->cost = 0; //set first zone cost to 0
}//startpath2



//todo -- find quad and check zones in that

xPathZone * 
xPathFind::getZone(float wx, float wy)
{
	tdVecZone ::iterator it;
	xPathZone * a;

	for (it = vecZone.begin(); it != vecZone.end(); it++)
	{
		a = (*it);
		if (wx < a->cx) { continue; }
		if (wy < a->cy) { continue; }
    if (wx > a->cx+a->cw) { continue; }
    if (wy > a->cy+a->ch) { continue; }
        
     return a;
	}//nextit

	return 0;
}//getzone




void 
xPathFind::setPath(std::vector <gamex::cVec3f> &vec)
{
  xPathZone * a;
  xPathZone * b;

  a = lastZone;
  
  vec.clear();
  
  if (a == 0) { return; }


  float prevx, prevy;
  float kx, ky;
  float ry, sy;
  
  prevx = -1;
  prevy = -1;
  
  kx = a->mx;
  ky = a->my;
  
  vec.push_back(gamex::cVec3f(endx, 0, endy));
  
  while (a != 0)
  {
    b = a;
    a = a->parent;
    
    if (a == 0) { break; } 


      if (a->cx + a->cw-0.1 < b->cx)
        {
            
            kx = a->cx + a->cw;
            
              ry = a->cy > b->cy ? a->cy : b->cy;
            sy = a->cy + a->ch < b->cy + b->ch ? a->cy + a->ch : b->cy + b->ch;
            
            ky = (ry + sy) * 0.5f; 
            
            //correction -- visit the center if both portals are on the same side
            //todo -- sometimes it does it, sometimes it dont (???)
            //maybe we need a.mx in some(?)
                if (prevx == kx) { vec.push_back(gamex::cVec3f(b->mx, 0, b->my)); }
            
            vec.push_back(gamex::cVec3f(kx, 0, ky) );
            
        }
        else if (b->cx + b->cw-0.1 < a->cx) 
        {
            kx = b->cx + b->cw;

            ry = a->cy > b->cy ? a->cy : b->cy;
            sy = a->cy + a->ch < b->cy + b->ch ? a->cy + a->ch : b->cy + b->ch;
            
            ky = (ry + sy) * 0.5f; 
            
                   if (prevx == kx) { vec.push_back(gamex::cVec3f(b->mx, 0, b->my)); }
                
            vec.push_back(gamex::cVec3f(kx, 0, ky) );
        }
        else if (a->cy + a->ch-0.1 < b->cy)
        {
            ky = a->cy + a->ch;
            
            ry = a->cx > b->cx ? a->cx : b->cx;
            sy = a->cx + a->cw < b->cx + b->cw ? a->cx + a->cw : b->cx + b->cw;
            
             if (prevy == ky) { vec.push_back(gamex::cVec3f(b->mx, 0, b->my)); }
            
            kx = (ry + sy) * 0.5f; 
    
          vec.push_back(gamex::cVec3f(kx, 0, ky) );
        }
        else //b->cy+b->ch < a->cy
        {
              ky = b->cy + b->ch;
            
            ry = a->cx > b->cx ? a->cx : b->cx;
            sy = a->cx + a->cw < b->cx + b->cw ? a->cx + a->cw : b->cx + b->cw;
         
              if (prevy == ky) { vec.push_back(gamex::cVec3f(b->mx, 0, b->my)); }
            
            kx = (ry + sy) * 0.5f; 
                  
            vec.push_back(gamex::cVec3f(kx, 0, ky) );
    
        }//endif
        
        prevx = kx;
        prevy = ky;
  }//wend
  
  vec.push_back(gamex::cVec3f(startx, 0, starty) );
  
  std::reverse(vec.begin(), vec.end());
  

}//setpath


void
xPathFind::buildZone(xTileMap * tmap)
{

  int k, i; //, yt;
  int num;
  
//  xQuad * q;
  
  clear();
  
  mw = tmap->mw;
  mh = tmap->mh;
  cw = tmap->cw;
  ch = tmap->ch;
 
  qcell = 16;
  qwidth = tmap->mw / qcell;
  qheight = tmap->mh / qcell;
  
  num = qwidth * qheight;
  vecQuad = new xQuad[num];

  for (i = 0; i < qheight; i++)
  {
    for (k = 0; k < qwidth; k++)
    {
      xQuad &q = vecQuad[(i*qwidth)+k];
      q.tx = k * qcell;
      q.ty = i * qcell;
      q.dirty = 1;
    }//nextk
  }//nexti


  this->rebuildQuad(tmap);


}//buildzone





//note  qa == qb is special
void 
xPathFind::conQuad(xQuad * qa)
{
  xPathZone * a;
  xPathZone * b;

  int num;  
  int i;
  int k;


  tdVecZone &vec = qa->vec;
   num = vec.size();
    

  for (i = 0; i < num; i++)
  {
      a = vec[i];
    for (k = i+1; k < num; k++)
    {
      b = vec[k];
    
      //height check
      if (a->height != b->height) { if (a->spec != 303 && b->spec != 303) { continue; } }
                    

      //aabb check
        if (a->cx2 < b->cx) { continue; }
        if (b->cx2 < a->cx) { continue; } 
        if (a->cy2 < b->cy) { continue; }
        if (b->cy2 < a->cy) { continue; }
       
      //corner fix
         if ((a->cx2) == b->cx && (a->cy2) == b->cy) { continue; }
         if ((a->cx2) == b->cx && (b->cy2) == a->cy) { continue; }
         if ((b->cx2) == a->cx && (b->cy2) == a->cy) { continue; }
         if ((b->cx2) == a->cx && (a->cy2) == b->cy) { continue; }
      
            a->vecNext.push_back(b);
            b->vecNext.push_back(a);
    
    }//nextkt
  
  }//nextit

}//conquad




void 
xPathFind::conQuad2(xQuad * qa, xQuad * qb)
{
  xPathZone * a;
  xPathZone * b;

  
  tdVecZone ::iterator it;
  tdVecZone ::iterator kt;
  
  for (it = qa->vec.begin(); it != qa->vec.end(); it++)
  {
    a = (*it);
    
    for (kt = qb->vec.begin(); kt != qb->vec.end(); kt++)
    {
      b = (*kt);
      
    
      //height check
      if (a->height != b->height) { if (a->spec != 303 && b->spec != 303) { continue; } }
    
      //aabb check
        if (a->cx2 < b->cx) { continue; }
        if (b->cx2 < a->cx) { continue; } 
        if (a->cy2 < b->cy) { continue; }
        if (b->cy2 < a->cy) { continue; }
        
      //corner fix
         if ((a->cx2) == b->cx && (a->cy2) == b->cy) { continue; }
         if ((a->cx2) == b->cx && (b->cy2) == a->cy) { continue; }
         if ((b->cx2) == a->cx && (b->cy2) == a->cy) { continue; }
         if ((b->cx2) == a->cx && (a->cy2) == b->cy) { continue; }
         
            a->vecNext.push_back(b);
            b->vecNext.push_back(a);
      
    }//nextkt
  }//nextit

}//conquad2

void 
xPathFind::genGroup()
{

  tdZonePtr * temp;
  int ti;
  int num;
//  int numk;
//  int i;
//  int k;
  int group;
 // int debcol;
  
  xPathZone * a;
  //xPathZone * b;


//todo -- calculate all zones in quads
//and all zones in vecquad
//if they are not the same
//then i need to rewrite all kinds of stuff

  
  
  
  tdVecZone ::iterator it;
  tdVecZone ::iterator kt;
  
  
  num = vecZone.size();
  if (num <= 0) {return;}
 
//maybe it should be .. num*num or something??
//todo -- need to test in flash

  temp = new tdZonePtr[num*10]; //now that it doesnt leaks this should be sufficient


  group = 1; //group 0 or below means no group


  for (it = vecZone.begin(); it != vecZone.end(); it++)
  {
    a = (*it);
   // printf("a %p \n", a);
    if (a->group > 0) { continue; }
  
  
  // debcol = 0x888888 +  mr.getRand() * 0x777777;//  (Math.random() * 0x777777);
     
    group += 1;
    ti = 0;

    temp[0] = a;
    ti = 1;
  
    while (ti > 0)
    {
        a = temp[ti - 1];
        ti -= 1;
        
        if (a->group != 0) { continue;}
        a->group = group;
        //a->col = debcol;
//this error should have happened in flash as well(?)
        for (kt = a->vecNext.begin(); kt != a->vecNext.end(); kt++)
        {
          if ( (*kt)->group != 0) { continue; }
          temp[ti] = (*kt);
          ti += 1;
          // if (ti > num) { printf("too many zones %d/%d \n",ti,num ); }
        }             
    }//wend
  
  }//nextit

//  printf("gengroup end %d \n", ti);
  
  delete [] temp;
  
  
  //number of groups is group - 1

}//gengroup



void 
xPathFind::setDirtyRect(float wx, float wy, float ww, float wh)
{
  float x;
  float y;
  float ex;
  float ey;

  ex = wx+ww;
  ey = wy+wh;

  //not much of an optimised method but whatever

  for (y = wy; y < ey; y+=ch)
  {
    for (x = wx; x < ex; x+=cw)
    {
      setDirty(x, y);
    }//nextx
  }//nexty
  

}//dirtyrect



void 
xPathFind::setDirty(float wx, float wy)
{
  //only needed for optimised map update
  //not implemented (aka not translated from as3) yet

  float qcw;
  float qch;
  int tx;
  int ty;

  qcw = qcell * cw;
  qch = qcell * ch;

  tx = (int) floorf(wx/qcw);
  ty = (int) floorf(wy/qch);

  if (tx < 0) { return; }
  if (ty < 0) { return; }
  if (tx > qwidth) { return; }
  if (ty > qheight) { return; } 
  
  vecQuad[ (ty * qwidth) + tx].dirty = 1;
      
}//setdirty




//todo -- maybe zone size should be limited
//like to be about 8x8 or something to generate more optimal paths

void
xPathFind::makeZoneQuad(xTileMap * tmap, xQuad * qa)
{
  int ex, ey;
  int i, k;
  int t;
  int k2, i2, ex2;
  int ww, wh;
  int zt;

  xTile * xt;
  int height;
  int spec;
  
  xPathZone * a;

  

  ex = qa->tx + qcell;
  ey = qa->ty + qcell;

  if (ex > mw) { ex = mw;}
  if (ey > mh) { ey = mh;}

//todo -- column buffer should be static maybe? like init only once in the ctor or something?
//its just a vector of ints after all (and the average map size is going to be about 256x256 or something)
//  int * vecCol = new int[mw];
  int vecCol[2048];
  memset(vecCol, 0, 2048*4);



                for (i = qa->ty; i < ey; i++)
                {

                 for (k = qa->tx; k < ex; k++)
                 {
                    if (vecCol[k] > i) { continue; }    
                    xt = tmap->getTileAt(k, i);
                        zt =  tmap->getZoneAt(k, i);
                    height = tmap->getHeightAt(k,i); //xt->height;
                    spec = xt->spec;
                     if (zt < 0) { continue;  } //wall

                

                    //find next wall in line
                    for (k2 = k; k2 < ex; k2++)
                    {                      
                      xt =  tmap->getTileAt(k2, i); 
                      t = tmap->getZoneAt(k2, i);
                      if ((k2-k)>7) { break; }
                      if (vecCol[k2] > i) { break; } 
                      if (t != zt) { break; }
                      if (height != tmap->getHeightAt(k2, i)) { break; }
                      if (spec != xt->spec) { break; }

                    }

     
                    ww = k2-k;
                    wh = 1;
                    ex2 = k2;  


                    //find the length of zone downwards
                    for (i2 = i+1; i2 <ey; i2++)
                    {          
                      
                      for (k2 =k; k2 <ex2; k2++)
                      {
                         xt = tmap->getTileAt(k2, i2);
                         //t = xt->zt;

                         t = tmap->getZoneAt(k2, i2);
                         if (vecCol[k2] > i2) { break; } 
                         if (t != zt) { break;} 
                         if (height != tmap->getHeightAt(k2, i2)) { break; }
                         if (spec != xt->spec) { break; }
                      }
                      if (k2 < ex2) { break; }
                      wh+=1;  
                      if (wh >= 8) { break; }
                    }

                    //set column for already visited ones
                    for (k2 = k; k2 < ex2; k2++)
                    {
                      vecCol[k2] = i+wh;
                    }
                          

                     a = new xPathZone();
                     a->cx = k*cw;     a->cw = ww*cw;
                     a->cy = i*ch;     a->ch = wh*ch;
                     a->group = 0;  
                     a->zt = zt;

	
                      a->cx2 = a->cx + a->cw;
                      a->cy2 = a->cy + a->ch;
                      a->mx = a->cx + (a->cw * 0.5f);
                      a->my = a->cy + (a->ch * 0.5f);

                      a->height = height;
                      a->spec = spec;


                    qa->vec.push_back(a); 
                    this->vecZone.push_back(a);                    
                    
                }//nextk
            }//nexti


//  delete [] vecCol;
  
  


}//makezonequad



/*
void
xPathFind::makeZoneQuad(xTileMap * tmap, xQuad * qa)
{
  int * vec;
  int ex; 
  int ey;
  tdVecZone * vz;
  int * vecCol;
  int c, i, yt, t;
  int k, mn, h, ht, n;
  xPathZone * a;
//  int qi, qk;

//  mw = tmap->mwidth;
//  mh = tmap->mheight;
//  cw = tmap->cw;
//  ch = tmap->ch;

  vec = &(tmap->vecGrid[0]);

  ex = qa->tx + qcell;
  ey = qa->ty + qcell;

 
  if (ex > mw) { ex = mw; }
  if (ey > mh) { ey = mh; }

  vz = &(qa->vec);

//still makes more zones than it should??
//but how and .. why?
//and shouldnt these zones also count in veczone?? 

  vecCol = new int[mw]; 
   for (c = 0; c < mw; c++) { vecCol[c] = -2; }
            
  //remember -- start at qa->tx, ty,  NOT 0,0
  //todo -- this recalc will sure to be fucked up
  for (i = qa->ty; i < ey; i++)
  {

   yt = i * mw;

  // qi = (int) ( (floorf(i/qcell) * qcell) + (qcell - 1));û
  //  qi = ey;

//generating more zones just makes the error more visible

   for (k = qa->tx; k < ex; k++)
   {
      if (vecCol[k] > i) { continue;}

      t = tmap->getZoneType2(vec[yt+k]);
      
      if (t > 0) { continue; }
      

//        qk = (int)( (floorf(k/qcell) * qcell) + (qcell - 1));
//          qk = ex;
          
      mn = mw; //ex?
 //     for (h = i; h < (qi+1); h++)
      for (h = i; h < (ey); h++) 
      {
        ht = h * mw; //ex?
        //t = vec[ht + k];
        t = tmap->getZoneType2(vec[ht+k]);

        if (t > 0) { break; }  
        
        
              for (n = k; n < (ex); n++)
              {
                if (vecCol[n]>h) { break;}
                t =  tmap->getZoneType2(vec[ht+n]); //vec[ht + n];   
                if (t > 0) { break; }  
              }
                
        if (mn > n) { mn = n;}            
      }//nexth

      
//      if (mn > qk) { mn = qk+1;}

  //    if (mn > qk) { mn = qk+1;}
      if (mn > (ex)) { mn = (ex+1); }

      
      for (n = k; n < mn; n++)
      {
          vecCol[n] = h;
      }  



       a = new xPathZone();
        a->cx = k * cw;
        a->cy = i * ch;
        a->cw = (mn - k) * cw;
        a->ch = (h - i) * ch;
        a->group = 0;
        a->cx2 = a->cx + a->cw;
        a->cy2 = a->cy + a->ch;
        a->mx = a->cx + (a->cw * 0.5f);
        a->my = a->cy + (a->ch * 0.5f);

       vecZone.push_back(a); //add to global zones
       vz->push_back(a); //add to quad zones

     }//nextk
    
    }//nexti
    

    delete [] vecCol; 
}//makezonequad
*/








//todo -- its supposed to be the same tilemap (size everything)
//that was used in the buildzone

void
xPathFind::rebuildQuad(xTileMap * tmap)
{
  int num;
  int i;
  int numk;
  int k;
  int yt;
  xQuad * a;
  xQuad * qa;
  xQuad * qb;

  num = qwidth * qheight;

//for dirty quads remove all zones
  for (i = 0; i < num; i++)
  {
    a = &(vecQuad[i]);
//so the rebuilt from ground up quads now work together
//but not with the old ones.. ??
//nah its still shit
//.. maybe makezonequads generates the same zone.. twice?
 //a->clear();
    if (a->dirty <= 0) { continue; }
    
	 a->clear();
	  	makeZoneQuad(tmap, a);

  }//nexti

  num = qwidth * qheight;

//clean up all zones //this seem to not work that well (?)
  for (i = 0; i < num; i++)
  {
    vecQuad[i].cleanDead();
  }



	    num = qheight;
			numk = qwidth;
			
          for (i = 0; i < num; i++)
            {
                yt = i * qwidth;
                for (k = 0; k < numk; k++)
                {
                    qa = &(vecQuad[yt + k]);

                    if (qa->dirty > 0) { conQuad(qa); }
                  

                    if (k < numk - 1)
                    {
                      qb = &(vecQuad[yt + k + 1]);
                    
                    //test on right
                
                      if (qa->dirty > 0 || qb->dirty > 0) 
                        {

                         // printf("test right \n");
                         conQuad2(qa, qb);
                        }
                    
                    }
                    
                    if (i < num -1)
                    {
                      qb = &(vecQuad[yt + k + qwidth]);
  

                      if (qa->dirty > 0 || qb->dirty > 0) 
                        { 
                           // printf("test down \n");
                          conQuad2(qa, qb); 

                        }
                    }

                 
                }//nextk
            }//nexti



  num = qwidth * qheight; // IMPORTANT
    for (i = 0; i < num; i++)
    {
      vecQuad[i].dirty = 0;
    }




    xPathZone * m;
			
			num = vecZone.size();
			for (i = 0; i < num; i++)
			{

				m = vecZone[i];

				//reset group
  				m->group = 0;

				if (m->dead == false) { continue; }


	
				vecZone[i] = vecZone[num - 1];

				num -= 1;
				i -= 1;
        delete m; //important
				vecZone.pop_back();
				
			}//nexti


			genGroup();


}//rebuildquad

