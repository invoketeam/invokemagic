#pragma warning (disable : 4786)


#include "stb_image.h"

#include "xImage.h"







xImage::xImage(void)
  {
    mw = 0;
    mh = 0; 
    dat = 0;
    x = 0;
    y = 0;
  }//ctor




xImage::~xImage(void)
 { clear(); }
    



void 
xImage::init(int w, int h)
  {
    clear();
    mw = w;
    mh = h;
    dat = new unsigned int[mw*mh];
    memset(dat, 0, mw*mh*4);
  }//init







void 
xImage::mirVert(void)
    {
        if (dat == 0) { return; }
        int num = mw * mh;
        unsigned int *buf = new unsigned int[num];
        memcpy(buf, dat, num*4 );
        
        int i;
        int k;
        int y;
        int yt;
        int w;

        w = mw*4;
       
         //printf("width: %d  bit:%d w: %d \n",width, bit, w);
        
        for (i = 0; i < mh; i++)
        {
            k = (mh-1) - i;
            y = i * mw;
            yt = k * mw;
            memcpy(&dat[yt], &buf[y], w);      
        }//nexti
        
        delete [] buf;
    }//mirvert
    

/*

void 
xImage::mirVert()
    {
        printf("mirVert \n");
        if (data == 0) { return; } //no image
        int total = width * height * bit;
        unsigned char *buf = new unsigned char[total];
        memcpy(buf, data, total );
        
        int i;
        int k;
        int y;
        int yt;
        int w;
        w = width * bit;
        printf("width: %d  bit:%d w: %d \n",width, bit, w);
        
        for (i = 0; i < height; i++)
        {
            k = (height-1) - i;
            y = i * width * bit;
            yt = k * width * bit;
           // printf("i:%d k:%d \n",i,k);
            memcpy(&data[yt], &buf[y], w);      
        }//nexti
        
       // memset(data, 0, total/2);
        
        delete [] buf;
    }//mirvert
    
*/

/*

void 
xImage::toRgb()
    {
       if (data == 0) { return;}
       if (bit != 4) { return; }
       int total = width * height * bit;
     
       int i;
       int num;
       num = width * height;
       unsigned char *buf = new unsigned char[total];
       memcpy(buf, data, total );
       
       stbi_image_free(data);
        
       int newsize = width * height * 3;
       data = new unsigned char[newsize];
       bit = 3;
       
        int m;
        int g;
        for (i = 0; i < num; i++)
        {
            m = 4 * i;
            g = 3 * i;
            data[g] = buf[m];
            data[g+1] = buf[m+1];
            data[g+2] = buf[m+2];
            //data[g+3] = 255;
        }//nexti
        delete [] buf;
    }//torgb


    
void 
xImage::toRgba(unsigned char alpha)
    {
        if (data == 0) { return;}
        if (bit != 3) { return; }
        int total = width * height * bit;
        
        int i;
        int num;
        num = width * height;
        unsigned char *buf = new unsigned char[total];
        memcpy(buf, data, total );
    
        stbi_image_free(data);
        
        int newsize = width * height * 4;
        data = new unsigned char[newsize];
        bit = 4;
        
        int m;
        int g;
        for (i = 0; i < num; i++)
        {
            m = 3 * i;
            g = 4 * i;
            data[g] = buf[m];
            data[g+1] = buf[m+1];
            data[g+2] = buf[m+2];
            data[g+3] = alpha;
        }//nexti
        delete [] buf;
    }//rgba
    
*/


void 
xImage::clear(void)
    {
      if (dat != 0) { delete [] dat; } dat = 0;
      mw = 0; mh = 0; 
    }//clear
    
    
   



int 
xImage::loadImage(std::string fname)
  {
    unsigned char * temp;
    int width, height, bit;

    width = 0;
    height = 0;
    bit = 0;

    clear();

      temp = stbi_load(fname.c_str(), &width, &height, &bit, 0);
      //if (temp == 0) { return 0; }

   
    if (width == 0 || height == 0)
    { 
      printf("xImage: cannot load image [%s]  \n",fname.c_str());
      return -1;
    }
  
    printf("xImage [%s] w:%d h:%d bpp:%d \n", fname.c_str(), width, height, bit );


    //ok, this one is for RGB png
    int k, i, yt;
    int n, k3;
    mw = width;
    mh = height;
    dat = new unsigned int[mw*mh];

    if (bit == 3)
    {
      for (i = 0; i < mh; i++)
      {
        yt = i * mw;
        n = yt*3;
        for (k = 0; k < mw; k++)
        {    
          k3 = k*3;
          dat[yt+k] = (temp[n+k3]<<24) + (temp[n+k3+1]<<16) + (temp[n+k3+2]<<8) + 0x000000FF; 
        }//nextk
      }//nexti
    }
    else if (bit == 4)
    {
      memcpy(dat, temp, mw*mh*4);
      endianSwap();
    }//endif
    
 
    stbi_image_free(temp);
    return 0;
  }//loadimage



//funnily enough this shouldnt change the image
void 
xImage::extTemp(void)
{
  int num;    int i;    unsigned int c;    int r,g,b,a;
  num = mw * mh;
  for (i = 0; i < num; i++)
  {
    c = dat[i];      r = (c >> 24) & 0xff;      g = (c >> 16) & 0xff;      b = (c >> 8) & 0xff;      a = c & 0xff;


    r = r << 24;      g = g << 16;      b = b << 8;
    dat[i] = r | g | b | a;
  }//nexti
}//temp


void 
xImage::extReplaceAlpha(xImage * src)
{
  int num;    int i;    
  unsigned int c; 
  unsigned int k;
  int r,g,b,a;
  unsigned int * sdat;

  if (src->mw != mw || src->mh != mh) { return; }

  sdat = src->dat;
  num = mw * mh;
  for (i = 0; i < num; i++)
  {
    c = dat[i];  
    k = sdat[i];

    r = (c >> 24) & 0xff;      g = (c >> 16) & 0xff;      b = (c >> 8) & 0xff;      
    a = k & 0xff;
    r = r << 24;      g = g << 16;      b = b << 8;
    dat[i] = r | g | b | a;
  }//nexti
}//repalpha



void 
xImage::extReduceColor(int f)
{
    int num;    int i;    unsigned int c;
    int r,g,b,a;
    num = mw * mh;
    for (i = 0; i < num; i++)
    {
      c = dat[i];
      r = (c >> 24) & 0xff;
      g = (c >> 16) & 0xff;
      b = (c >> 8) & 0xff;
      a = c & 0xff;

      //r = (r / 32) * 32;      g = (g / 32) * 32;      b = (b / 32) * 32;      a = (a / 32) * 32;
      r = (r / f) * f;      g = (g / f) * f;      b = (b / f) * f;      a = (a / f) * f;

      r = r << 24;
      g = g << 16;
      b = b << 8;
      dat[i] = r | g | b | a;
    }//nexti

}//extreduce


void 
xImage::extRemoveRGB(void)
{
  int num;    int i;    unsigned int c;  // int r,g,b,a;
  num = mw * mh;
  for (i = 0; i < num; i++)
  {
    c = dat[i];      
    //r = (c >> 24) & 0xff;      g = (c >> 16) & 0xff;      b = (c >> 8) & 0xff; 
   // a = c & 0xff;
    //r = r << 24;      g = g << 16;      b = b << 8;
    dat[i] = c & 0xff;
  }//nexti
}//removergb



  //switch RGBA to ABGR (or ABGR back to RGBA)  
void 
xImage::endianSwap(void)
  {
    int num;    int i;    unsigned int c;
    num = mw * mh;
    for (i = 0; i < num; i++)
    {
      c = dat[i];
      dat[i] =  ((c>>24)&0xff) | ((c<<8)&0xff0000) | ((c>>8)&0xff00) | ((c<<24)&0xff000000);
    }

  }//endian 




void 
xImage::setPixel(int x, int y, unsigned int c)
  {
    if (dat == 0) { return; }
    if (x < 0) { return; } if (y < 0) { return; }
    if (x>=mw) { return; } if (y >=mh){ return; }
    dat[x+(y*mw)] = c;
  }//setpixel


unsigned int 
xImage::getPixel32(int x, int y, unsigned int c)
  {
    if (dat == 0) { return 0; }
    if (x < 0) { return 0; } if (y < 0) { return 0; }
    if (x>=mw) { return 0; } if (y >=mh){ return 0; }
    return dat[x+(y*mw)];
  }//setpixel



void 
xImage::fill(unsigned int c)
  { 
    if (dat == 0) { return; }
    int num; int i;
    num = mw * mh;
    for (i = 0; i < num; i++) { dat[i]=c;}
  }//fill


void 
xImage::drawRect(int x, int y, int w, int h, unsigned int c)
  {
    int sx, sy, ex, ey;
    int i, k;
    
    if (x+w < 0) { return; }
    if (y+h < 0) { return; }
    if (x > mw) { return; }
    if (y > mh) { return; }

     sx = x; ex = x+w;
     if (sx < 0) { sx = 0; }
     if (ex > mw) { ex = mw; }

     //todo -- maybe i screwed up this one as well
     if (y > 0)
     {   sy = y* mw;   for (k = sx; k < ex; k++) { dat[sy+k]=c; }   }

     if ((y+h) < mh)
     {   sy = (y+h)* mw;   for (k = sx; k < ex; k++) { dat[sy+k]=c; }   }
    
     sy = y; ey = y+h;
     if (sy < 0) { sy = 0; }
     if (ey > mh) { ey = mh; }

     if (x > 0)
     {  for (i = sy; i <ey;i++) { dat[(i*mw)+x] = c; }  }

     if ((x+w) < mw)
     { sx = (x+w);  for (i = sy; i <ey;i++) { dat[(i*mw)+sx] = c; }  }

  }//drawrect


void 
xImage::xorFill(void)
  {
    if (dat == 0) { return; }
    int num, i, k, yt;
    int c;
    num = mw * mh;
    for (i =0;i<mh;i++)
    { yt = i*mw;
      for (k = 0; k <mw;k++)
      {
        c= i^k;
        dat[yt+k] =(c<<8) + (c<<16)+ (c<<24)+0x000000FF;
      }
    }
  }//xorfill


//blend ref
//http://www.virtualdub.org/blog/pivot/entry.php?id=117

void 
xImage::blendImage(xImage * src, int dx, int dy, unsigned char op)
  {
    int sx, sy;    int smw, smh;    int y, i;    unsigned int * sd;
    int wr;

    smw = src->mw;    smh = src->mh;    sd = src->dat;

    //new based on ub11
    int ax,ay,aw,ah; //our rect
    int bx,by,bw,bh; //source rect
    int cx,cy,cx2,cy2, cw,ch; //resulting intersection of the 2 rectangles

    ax = 0; ay = 0;  //always 0
    aw = mw; ah = mh; //always same as mw and mh
    bx = dx; by = dy; bw = src->mw; bh = src->mh;

      cx = ax > bx ? ax : bx; // Math.max(ax, bx);
      cy = ay > by ? ay : by; // Math.max(ay, by);
      cx2 = aw < (bx+bw) ? aw : (bx+bw); //  Math.min(ax+aw, bx+bw);
      cy2 = ah < (by+bh) ? ah : (by+bh); // Math.min(ay+ah, by+bh);

      cw = cx2-cx; ch = cy2-cy;

      if (cw < 0 || ch < 0) { return; } //trying to draw outside the image

      sx = 0;  sy = 0;
      if (bx < 0) { sx = -bx; }
      if (by < 0) { sy = -by; }
      wr = cw * 4;
      
/*
      for (y = 0; y < ch; y++)
      {
        i = cy + y;
        //if (i < 0) { continue; }
        //if (i >= mh) { return; }
        //memcpy(dat+(i*mw)+dx, sd+((sy+y)*smw)+sx, wr);
        memcpy(dat+(i*mw)+cx, sd+((sy+y)*smw)+sx, wr);  
      }//nexty
*/
    unsigned int * d0;
    unsigned int * s0;

    unsigned int c1, c2;
    unsigned int a, drb, dg;
    unsigned int op2;
    int k;

    op2 = op;

    for (y = 0; y < ch; y++)
    {
      i = cy + y;

      //if (i < 0) { continue; }
      //if (i >= mh) { return; }

      d0 = dat+(i*mw)+cx;
      s0 = sd+((sy+y)*smw)+sx;
 
     for (k = 0; k < cw; k++)
     {

      c2 = s0[k]; //+sx];
      a = (c2 & 0x000000FF) ;
      a = (a*op2)>>8; //opacity

      if (a == 0) { continue; }

      if (a == 255) { d0[k] = s0[k]; continue; }   

       c1 = d0[k];

       a += (a > 0);

        c2 >>= 8;
        c1 >>= 8;

        drb = c1 & 0xff00ff;
        dg = c1 & 0x00ff00;

        d0[k] = ((((drb + (((( c2 & 0xff00ff) - drb) * a + 0x800080) >>8) ) & 0xff00ff)+
          ((dg  + ((((c2 & 0x00ff00) -  dg ) * a + 0x008000) >>8) ) & 0x00ff00)) <<8)|0xFF;


      }//nextk
    }//nexty

return;



/*
    int ex, ey, sx, sy;
    int smw, smh;
    int y; 
    int i, k;
    int wk; 
    unsigned int * sd;


    if (dx > mw) { return; }
    if (dy > mh) { return; }
    if (dx +src->mw < 0) { return; }
    if (dy +src->mh < 0) { return; }

    smw = src->mw;
    smh = src->mh;
    sd = src->dat;
    
    sx = 0;
    sy = 0;

    if (dx < 0) { sx = -dx; dx = 0; }
    if (dy < 0) { sy = -dy; dy = 0; }
    if (sx > smw) { return; }
    if (sy > smh) { return; }

    ex = smw - sx;
    ey = smh - sy;

    int wr;

    if ((dx + ex) > mw) { wr = smw-((dx+ex)-mw); }
    else { wr = ex; }

    if (wr < 0) { return; }
    ex = sx+wr;
    wk = wr;
    wr *= 4;
    unsigned int * d0;
    unsigned int * s0;
    unsigned int c1, c2;
    unsigned int a, drb, dg;
    unsigned int op2;
    op2 = op;
    
    wk -= sx;
    for (y = 0; y < ey; y++)
    {
      i = dy + y;
      if (i < 0) { continue; }
      if (i >= mh) { return; }

      d0 = dat+(i*mw)+dx;
      s0 = sd+((sy+y)*smw)+sx;
//.. why do i need to add sx 2x?
//i screwed the math somewhere
//.. nah its because for wk i take out one sx
//second thought.. still doesnt make sense
     for (k = 0; k < wk; k++)
     {
//just draw
  //printf("k %d, ",k);
   //    d0[k] = s0[k+sx];
  // continue;
/
//transparent color
 //      c = s0[k+sx];
  //     a = c & 0x000000FF;
  //     if (a <= 128) { continue; }
    //   d0[k] = s0[k];

//also blending breaks
      c2 = s0[k+sx];
//      a = (c2 & 0x000000FF) ;
      a = (c2 & 0x000000FF) ;
      a = (a*op2)>>8; //opacity
        //a = (a*128)>>8;
    
 
      if (a == 0) { continue; }
      if (a == 255) { d0[k] = s0[k]; continue; }   

       c1 = d0[k];
       a += (a > 0);

        c2 >>= 8;
        c1 >>= 8;

        drb = c1 & 0xff00ff;
        dg = c1 & 0x00ff00;

        d0[k] = ((((drb + (((( c2 & 0xff00ff) - drb) * a + 0x800080) >>8) ) & 0xff00ff)+
          ((dg  + ((((c2 & 0x00ff00) -  dg ) * a + 0x008000) >>8) ) & 0x00ff00)) <<8)|0xFF;

      }//nextk
    }//nexty
*/
  }//blendimg


void 
xImage::drawImage(xImage * src, int dx, int dy)
  {
    //int ex, ey,
    int sx, sy;
    int smw, smh;
    int y, i;
    unsigned int * sd;
    int wr;

    smw = src->mw;    smh = src->mh;    sd = src->dat;

    //new based on ub11
    int ax,ay,aw,ah; //our rect
    int bx,by,bw,bh; //source rect
    int cx,cy,cx2,cy2, cw,ch; //resulting intersection of the 2 rectangles

    ax = 0; ay = 0;  //always 0
    aw = mw; ah = mh; //always same as mw and mh
    bx = dx; by = dy; bw = src->mw; bh = src->mh;

      cx = ax > bx ? ax : bx; // Math.max(ax, bx);
      cy = ay > by ? ay : by; // Math.max(ay, by);
      cx2 = aw < (bx+bw) ? aw : (bx+bw); //  Math.min(ax+aw, bx+bw);
      cy2 = ah < (by+bh) ? ah : (by+bh); // Math.min(ay+ah, by+bh);

      cw = cx2-cx; ch = cy2-cy;

      if (cw < 0 || ch < 0) { return; } //trying to draw outside the image

      sx = 0;  sy = 0;
      if (bx < 0) { sx = -bx; }
      if (by < 0) { sy = -by; }


      wr = cw;
      wr *= 4;
      
      //printf("sx %d sy %d wr %d \n",sx,sy,wr);
      //printf("cx %d cy %d cw %d ch %d \n",cx, cy,cw,ch);

      for (y = 0; y < ch; y++)
      {
        i = cy + y;
        //if (i < 0) { continue; }
        //if (i >= mh) { return; }
        //memcpy(dat+(i*mw)+dx, sd+((sy+y)*smw)+sx, wr);
        memcpy(dat+(i*mw)+cx, sd+((sy+y)*smw)+sx, wr);  
      }//nexty

/*
    //images dont overlap
    if (dx > mw) { return; }
    if (dy > mh) { return; }
    if (dx +src->mw < 0) { return; }
    if (dy +src->mh < 0) { return; }

    smw = src->mw;
    smh = src->mh;
    sd = src->dat;
    
    sx = 0;
    sy = 0;

    //move the source image    
    if (dx < 0) { sx = -dx; dx = 0; }
    if (dy < 0) { sy = -dy; dy = 0; }
    if (sx > smw) { return; }
    if (sy > smh) { return; }

    //endx = source_width - source_start_x
    

    ex = smw - sx;
    ey = smh - sy;

    int wr;
   // printf("wr %d  ex %d  dx %d \n", wr, ex, dx);

    //limit drawn width
    if ((dx + ex) > mw) 
    {

       //wr = smw-((dx+ex)-mw); 
       wr = ex - ((dx+ex) - mw);
    
      //copylength  =
      // source_width - ((destination_x+end_x) - dest_width)

    } 
    else { wr = ex; }

    if ((dy+ey)>mh) { ey = ey - ((dy+ey)-mh); }

 //   printf("wr %d  dy %d ey %d  \n", wr, dy, ey);
 
    if (wr < 0) { return; }
    wr *= 4;

    for (y = 0; y < ey; y++)
    {
      i = dy + y;
      if (i < 0) { continue; }
      if (i >= mh) { return; }

      memcpy(dat+(i*mw)+dx, sd+((sy+y)*smw)+sx, wr); 

    }//nexty
*/
  }//drawimg 




void 
xImage::resize(xImage * src, int w, int h)
{
  if (w == 0) { }

  clear();

  init(w, h);

  //orig 128
  //targ 64
  //128/64  2


  float ix;
  float iy;
  int i;
  int k;
  int yt;
  int syt;
  unsigned int * sdat;
  int sy;
  int sx;
  
  ix = (float) src->mw / (float) w;
  iy = (float) src->mh / (float) h;

  //printf("ix iy %0.2f %0.2f \n", ix ,iy );

  sdat = src->dat;

  for (i = 0; i < h; i++)
  {
    yt = i * w; //yt in new image
    sy = (i*iy);
    //printf("sy %d \n ", sy);
    syt = sy * src->mw;  //yt in old image
    for (k = 0; k < w; k++)
    {
      sx = k*ix;
      dat[yt + k] = sdat[syt+sx];
    }//nextk
  }//nexti


}//resize



































