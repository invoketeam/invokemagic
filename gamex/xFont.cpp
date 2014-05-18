
#pragma warning (disable : 4786) //std map vc6
#pragma warning (disable : 4996) //_vsnsprintf is unsafe or deprecated or something

#include "xFont.h"
#include "xGLCommon.h"

#include "xFlatRender.h"




xChar::xChar(void) 
  {
        valid = 0;     addx = 0.0f;
        u0 = 0.0f;        v0 = 0.0f;
        u1 = 0.0f;        v1 = 0.0f;
        width = 0.0f;     height = 0.0f;
        ox = 0.0f; oy = 0.0f;
  }//ctor 


xFont::xFont(void)
 { 
   chwidth = 16.0f;
   chheight = 16.0f; 
   fontsize = 1.0f;

   ascent = 0.0f;
   descent = 0.0f;
   linegap = 0.0f;    

    handle = 0;
 }//ctor


xFont::~xFont(void) { clear(); }


void 
xFont::clear(void)
    {
      //  tex.clear();

        int i;
        for (i = 0; i < MAX_XCHAR; i++)
        {
            vecChar[i].valid = 0;
        }

    }//clear

void 
 xFont::loadCharDef(std::string fname)
    {
      

        float imgWidth;        float imgHeight;   
        int c;
        float x;        float y;        float w;        float h;
        float ox;       float oy;
        xChar * a;
        float addx;


         pugi::xml_document xm;
         pugi::xml_node root;
         pugi::xml_node k;


         clear();

         if  (!(xm.load_file(fname.c_str(), 0) ))	{ printf("xFont:Error -- couldn't find chardef file %s \n", fname.c_str());	return; } //exit? quit? throw?

   

         root = xm.child("xfont");

         skin = root.attribute("image").value(); 
         imgWidth =  root.attribute("w").as_float();
         imgHeight = root.attribute("h").as_float();
      
         ascent = root.attribute("ascent").as_float();
         descent = root.attribute("descent").as_float();
         linegap = root.attribute("linegap").as_float();


         fontsize = root.attribute("size").as_float();

      
         chwidth = fontsize  /2;
         chheight =  fontsize  /2;


    
           for (k = root.child("char"); k; k = k.next_sibling("char"))
           {
                c = k.attribute("c").as_int(); //image name is used as number in this case
               x = k.attribute("x").as_float();
               y = k.attribute("y").as_float();
               w = k.attribute("w").as_float();
               h = k.attribute("h").as_float();
               ox =  k.attribute("ox").as_float();
               oy =  k.attribute("oy").as_float();

               addx = k.attribute("addx").as_float();

               if (c <= 0 || c>= MAX_XCHAR) {continue;}

               a = &vecChar[c];
               
               a->valid = 1;
               a->width = w;
               a->height = h;
               a->u0 = x / imgWidth;
               a->v0 = y / imgHeight;
               a->u1 = (x+w) / imgWidth;
               a->v1 = (y+h) / imgHeight;

               a->v0 = 1 - a->v0;
               a->v1 = 1 - a->v1;

                a->addx = addx;
                a->ox = ox;
                a->oy = oy;

               // a->addy = addy;
           }//nextk

    }//loadchardef




//todo -- option to  align to middle

void
xFont::printStr(float size, float cx, float cy, const char* str, ...)
{
  if (size == 0.0f) { return; }
	static char buffer[1024];

	va_list params;
	va_start(params, str);
		_vsnprintf(buffer, sizeof(buffer), str, params);
	va_end(params);

   size = size / fontsize;

   writeStr(cx, cy, buffer, size);
  


  /*
	glRasterPos2f(x, y);

	char* c = buffer;

	while(*c)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		c++;
	}
  */
}//print


void 
xFont::printStrFrame(xFlatRender * render, unsigned int skin, float size, float cx, float cy,float cz, const char* str, ...)
{
  if (size == 0.0f) { return; }
	static char buffer[1024];

	va_list params;
	va_start(params, str);
		_vsnprintf(buffer, sizeof(buffer), str, params);
	va_end(params);

   size = size / fontsize;

   writeStrFrame(render, skin, cx, cy,cz, buffer, size);
  


}//printframe




//this is now the legit version, the old one is used for debug
void
xFont::writeStrFrame(xFlatRender * render, unsigned int skin, float cx, float cy, float cz, std::string str, float scale)
  {
        float dx;       float dy;
        float ax;       float ay;        float aw;        float ah;
        unsigned char c;
        int i;    int num;
        float addy;
        xChar * a;
        xFrame * f;

        addy = (ascent - descent + linegap) * scale;

        dx = cx;
        dy = cy + addy;

        num = str.size();

       
           
        for (i = 0; i < num; i++)
        {
           c = str[i];
           if (c == '\n') { dx = cx; dy += addy; continue;}   //end of line
           if (c == 32) { dx += (chwidth/2)*scale; continue; } //space
         
           a = &vecChar[c];
           if (a->valid <= 0) { dx += chwidth*scale;  continue; }  //invalid character is replaced with space


           ax = dx + (a->ox*scale);           ay = dy + (a->oy*scale); 
           aw = a->width*scale;           ah = a->height*scale;
             
           f = render->addFrame(ax+aw*0.5f, ay+ah*0.5f, cz, aw, ah, skin);
           f->u0 = a->u0;
           f->v0 = a->v0;
           f->u1 = a->u1;
           f->v1 = a->v1;
          
           
/*
            glBegin(GL_TRIANGLES); //for immediate mode its better to have lot of glbegins
             glTexCoord2f(a->u0, a->v1);
             glVertex2f(ax, ay+ah);

             glTexCoord2f(a->u1, a->v1);
             glVertex2f(ax+aw, ay+ah);

             glTexCoord2f(a->u0, a->v0);
             glVertex2f(ax, ay);
             

             glTexCoord2f(a->u0, a->v0);
             glVertex2f(ax, ay);
             
             glTexCoord2f(a->u1, a->v1);
             glVertex2f(ax+aw, ay+ah);
             
             glTexCoord2f(a->u1, a->v0);
             glVertex2f(ax+aw, ay);
           glEnd();
  */         
           dx += a->addx *scale;
           
        }//nexti


  }//writestrframe  




//todo -- this is the unoptimised debug version -- 
//the real one should just dump the contents in an array
//and thats it
  void
  xFont::writeStr(float cx, float cy, std::string str, float scale)
    {
        float dx;       float dy;
        float ax;       float ay;        float aw;        float ah;
        unsigned char c;
        int i;    int num;
        float addy;
        xChar * a;



        addy = (ascent - descent + linegap) * scale;

        dx = cx;
        dy = cy + addy;

        num = str.size();

       
           
        for (i = 0; i < num; i++)
        {
           c = str[i];
           if (c == '\n') { dx = cx; dy += addy; continue;}   //end of line
           if (c == 32) { dx += (chwidth/2)*scale; continue; } //space
         
           a = &vecChar[c];
           if (a->valid <= 0) { dx += chwidth*scale;  continue; }  //invalid character is replaced with space


           ax = dx + (a->ox*scale);           ay = dy + (a->oy*scale); 
           aw = a->width*scale;           ah = a->height*scale;
             
            glBegin(GL_TRIANGLES); //for immediate mode its better to have lot of glbegins
             glTexCoord2f(a->u0, a->v1);
             glVertex2f(ax, ay+ah);

             glTexCoord2f(a->u1, a->v1);
             glVertex2f(ax+aw, ay+ah);

             glTexCoord2f(a->u0, a->v0);
             glVertex2f(ax, ay);
             

             glTexCoord2f(a->u0, a->v0);
             glVertex2f(ax, ay);
             
             glTexCoord2f(a->u1, a->v1);
             glVertex2f(ax+aw, ay+ah);
             
             glTexCoord2f(a->u1, a->v0);
             glVertex2f(ax+aw, ay);
           glEnd();
           
           dx += a->addx *scale;
           
        }//nexti

   

    }//writestr
    
    



float 
xFont::getXcoordFromCharPos(std::string &str, int pos, float scale)
{
  float dx; 
  unsigned char c;
  int i; int num;
  xChar * a;
  

  num = str.size();

  if (num > pos) { num = pos; }
  if (num <= 0) { return 0; }


  for (i = 0; i < num; i++)
  {
     c = str[i];
     if (c == '\n') { return -1; }   //end of line -- treated as not found
     if (c == 32) { dx += (chwidth/2)*scale; continue; } //space
   
     a = &vecChar[c];
     if (a->valid <= 0) { dx += chwidth*scale;  continue; }  //invalid character is replaced with space

     dx += a->addx *scale;
  }//nexti
  if (i >= (int) str.size()) { dx -= a->addx; dx += (chwidth / 2); }
  return dx;
}//getxfromcharpos



//todo -- getting it in 2D would need to test aabb for all chars.. i guess?

int
xFont::getCharPos(std::string &str, float xcoord, float scale, unsigned char * ch, float * endx)
{
  float dx;
  unsigned char c;
  int i; int num;
  xChar * a;
  

  num = str.size();

  for (i = 0; i < num; i++)
  {
     c = str[i];
     if (c == '\n') { return -1; }   //end of line -- treated as not found
     if (c == 32) { dx += (chwidth/2)*scale; continue; } //space
   
     a = &vecChar[c];
     if (a->valid <= 0) { dx += chwidth*scale;  continue; }  //invalid character is replaced with space

     dx += a->addx *scale;

          if (dx > xcoord)
          {
            if (ch != 0) { (*ch) = c;}
            if (endx != 0) { (*endx) = dx - a->addx * scale; }
            return i; 
    
          }//endif

  }//nexti

   if (ch != 0) { (*ch) = 0;}
   if (endx != 0) { if (i == 0) {*endx = 0; } else { (*endx) = dx - a->addx * scale + (chwidth / 2); } }
  return -1;
}//getcharno




    
float    
xFont::getWidth(std::string &str, float scale)
    {
        float dx;       float dy;
        unsigned char c;
        int i;    int num;
        float addy;
        float ret;
        xChar * a;

        addy = (ascent - descent + linegap) * scale;

        dx = 0;
        dy = 0 + addy;

        num = str.size();

        ret = 0;
  
        for (i = 0; i < num; i++)
        {
           c = str[i];
           if (c == '\n') { dx = 0; dy += addy; continue;}   //end of line
           if (c == 32) { dx += (chwidth/2)*scale; continue; } //space
         
           a = &vecChar[c];
           if (a->valid <= 0) { dx += chwidth*scale;  continue; }  //invalid character is replaced with space

           dx += a->addx *scale;
           if (ret < dx) { ret = dx; }
        }//nexti

        return ret;
    }//getrect

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    