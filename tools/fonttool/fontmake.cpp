#pragma warning (disable : 4786)

#include <cctype>
#include <math.h>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cctype> 

#include "fontmake.h"

#include "xImage.h"

#define MINIZ_NO_STDIO
#define MINIZ_NO_TIME
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.h"
#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb_truetype.h"



class xImageExt : public xImage
{
public:
  int cx, cy;
  int ox, oy;
  int chcode;
  int addx;

public:
  xImageExt(void)
  {
    cx = 0; cy = 0;
    ox = 0; oy = 0;
    chcode = 0;
    addx = 0;
  }//ctor

 void writePng(std::string fname)
  {
    if (dat == 0) { return; }
    size_t size;
    void * buf;
    FILE * file;

    buf = tdefl_write_image_to_png_file_in_memory(&dat[0], mw, mh, 4, &size);

      file = fopen(fname.c_str(), "wb");
      fwrite(buf, 1, size, file);
      fclose(file);
   
    mz_free(buf);
  }//writepng


};//classend



//sorting based on algorithm at
//http://www.blackpawn.com/texts/lightmaps/

 class xNode
  {
  public:
    static int border;
  public:
    float rx, ry, rw, rh;
    xImageExt * img;
    xNode * childa;
    xNode * childb;

  public:
    xNode(void) 
    {
      rx = 0;  ry = 0;  rw = 0;   rh = 0;
      img = 0;   childa = 0;  childb = 0;
    } //ctor


    ~xNode(void) 
    {
      clear();
    }//dtor

    void clear(void)
    {
      if (childa != 0) { delete childa; childa = 0; }
      if (childb != 0) { delete childb; childb = 0; }    
      rx = 0;  ry = 0;  rw = 0;   rh = 0; img = 0;
    }//clear
  
    xNode * insertRect(xImageExt * b)
    {
        xNode * n;
        float dw, dh;
        float bw, bh; 

        //use bigger size than actual size of the image
        // so we got an 1 pixel border, needed for opengl (bilinear filter)

      //  bw = b->mw + 2;
      //  bh = b->mh + 2;
        bw = b->mw + (border+border);
        bh = b->mh + (border+border);
        
        if (childa != 0)
        {
            //not leaf
            n = childa->insertRect(b);
            if (n == 0)
            { n = childb->insertRect(b);   }
            return n;    
        }
        else
        {
            //already has image
           if (img != 0) { return 0; }
           
           //doesn't fit
           if (bw > rw || bh > rh)      { return 0; }
           
           //just right
           if (bw == rw && bh == rh)
           { 
            img = b; 
//            b->cx = rx + 1; 
//            b->cy = ry + 1;
            b->cx = rx + border; 
            b->cy = ry + border;
            
            return this;    
           }
           
           //split
           childa = new xNode();
           childb = new xNode();
           
           dw = rw - bw;
           dh = rh - bh;
           
           if (dw > dh)
           {
               childa->rx = rx;               childa->ry = ry;
               childa->rw = bw;               childa->rh = rh;
               
               childb->rx = rx + bw;               childb->ry = ry;
               childb->rw = rw-bw;               childb->rh = rh;
  
           }
           else
           {
               childa->rx = rx;               childa->ry = ry;
               childa->rw = rw;               childa->rh = bh;
               
               childb->rx = rx;               childb->ry = ry+bh;
               childb->rw = rw;               childb->rh = rh-bh;
           }//endif
        
          return childa->insertRect(b);
        
        }//endif
    }//insrect 
  };//xNode

int
xNode::border = 1;

/*
extern void stbtt_GetCodepointHMetrics(const stbtt_fontinfo *info, int codepoint, int *advanceWidth, int *leftSideBearing);
// leftSideBearing is the offset from the current horizontal position to the left edge of the character
// advanceWidth is the offset from the current horizontal position to the next horizontal position
//   these are expressed in unscaled coordinates

extern int  stbtt_GetCodepointKernAdvance(const stbtt_fontinfo *info, int ch1, int ch2);
// an additional amount to add to the 'advance' value between ch1 and ch2
// @TODO; for now always returns 0!

extern int stbtt_GetCodepointBox(const stbtt_fontinfo *info, int codepoint, int *x0, int *y0, int *x1, int *y1);
// Gets the bounding box of the visible part of the glyph, in unscaled coordinates

extern void stbtt_GetGlyphHMetrics(const stbtt_fontinfo *info, int glyph_index, int *advanceWidth, int *leftSideBearing);
extern int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2);
extern int  stbtt_GetGlyphBox(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1);
// as above, but takes one or more glyph indices for greater efficiency
*/

#define NUMCHAR 256

static xImageExt vecChar[NUMCHAR];
static bool vecSkip[NUMCHAR] = {0};

static stbtt_fontinfo font;
static unsigned char ttf_buffer[1<<25];


void setImage(xImageExt * img, char c, int s, int edge)
{
  //no idea if this should be freed
  unsigned char *bitmap;
  int w, h;
  int i, j;
  int ox, oy;


  bitmap = stbtt_GetCodepointBitmap(&font, 0,stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, &ox, &oy);

  //int edge;
  //edge = 4;

  img->init(w+edge*2, h+edge*2);
  img->ox = ox - edge;
  img->oy = oy - edge;

   int k;
   int v;
   
   for (j=0; j < h; ++j) {
      for (i=0; i < w; ++i)
      {
        v = bitmap[j*w+i];
        k = v; //(255-v);
        img->setPixel(i+edge,j+edge,k+(k<<8)+(k<<16)+(v<<24));
      }
   }
}//setimage




///todo -- add string with charcodes

//test.ttf, export.xml, export.png
void saveFont(std::string fontFile, std::string xmName, std::string skinName, int psize, int endchar, int edge,int addspace)
{
/*
  xImageExt mini;
  mini.init(32,32);
  mini.fill(0xff0000ff);
  mini.xorFill();

  xImageExt test;
  test.init(64,64);
  test.fill(0xff00ffFF);
  test.drawImg(&mini,-7,-4);
 // test.writePng("test2.png");
 */

  //http://forums.libsdl.org/viewtopic.php?t=8531&sid=455b03949417d6340a80912730a0f9b5

   fread(ttf_buffer, 1, 1<<25, fopen(fontFile.c_str(), "rb"));
   stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer,0));

  xNode top;
  std::string fname;
  int s;
  int i;
  int b;
  float scale;


  s = psize; //64;

   scale = stbtt_ScaleForPixelHeight(&font, s);
   
  //todo -- set border values (how many pixels to leave out around a character)
  //todo -- for the border it will need to adjust the generated xml (img size and offset)
  
  b = 1;
  xNode::border = b;

  int ascent, descent, linegap; 

  stbtt_GetFontVMetrics(&font, &ascent, &descent, &linegap);
  ascent *= scale;
  descent *= scale;
  linegap *= scale;


  int advance, lsb;
//todo -- sort by size(?)

  int start;
  int end;
   start = 32;
   //end = 128;
   //end = 165;
   end = endchar;


  xImageExt * ch;;
  for (i = start; i < end; i++)
  {
    ch = &(vecChar[i]);
    setImage(ch, (char) i, s, edge);

    ch->chcode = i;
    stbtt_GetCodepointHMetrics(&font, (char)i, &advance, &lsb);
    ch->addx = (float) advance * scale;
    //ch->addx += addspace;
    //nah -- dont bake the extra space
  }//nexti

  int imgSize;
  int size;
  bool done;
   done = false;
   top.clear();
   imgSize = 128;
   size = 128;

  while (!done)
  {
    done = true;
    for (i = start; i < end; i++)
    {
      ch = &(vecChar[i]);
      if (top.insertRect(ch) == 0)
      {
          top.clear();
          done = false; 
          if (imgSize >= size) { size *= 2; }
          imgSize += 64;
          printf("imgsize %d \n", imgSize);
          top.rw = imgSize;
          top.rh = imgSize;
        break;
      }//endif
    }//nexti2
  }//wend

  if (imgSize > size) { size *= 2; }

  xImageExt large;
  large.init(size, size);
  for (i = start; i < end; i++)
  {
    ch = &(vecChar[i]);
    large.drawImage(ch, ch->cx, ch->cy);
  }//nexti3

  std::ofstream myfile;
	  myfile.open (xmName.c_str());
	  myfile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    myfile << "<xfont image=\"" << skinName << "\"  w=\""<< size << "\" h=\"" << size << "\"  size=\""<<s << "\"  extraspace=\""<< addspace  <<  "\"  ascent=\""<< ascent <<  "\" descent=\""<<descent<<"\" linegap=\""<<linegap<<"\" >\n";

  for (i = start; i < end; i++)
  {
    ch = &(vecChar[i]);
    myfile << "<char c=\""<<ch->chcode<<"\" ";
    myfile << " x=\""<<ch->cx<<"\" ";
    myfile << " y=\""<<ch->cy<<"\" ";
    myfile << " w=\""<<ch->mw<<"\" ";
    myfile << " h=\""<<ch->mh<<"\" ";
    myfile << " ox=\""<<ch->ox<<"\" ";
    myfile << " oy=\""<<ch->oy<<"\" ";
    myfile << " addx=\""<<ch->addx<<"\" ";
    myfile << " />\n";
  }//nexti4

  myfile << "</xfont>";
  myfile.close();

  //large.drawRect(16,16,256-32,256-32,0xff000000);
  large.writePng(skinName);

  for (i = start; i < end; i++)
  {
    ch = &(vecChar[i]);
    large.drawRect(ch->cx-1, ch->cy-1, ch->mw+2, ch->mh+2, 0xff00ff00);
  }//nexti4
  large.writePng("out_guide.png");




  printf("Font saved \n");
}//init  