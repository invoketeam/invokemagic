#pragma warning (disable : 4786)

#include "xTmx.h"

#include <math.h>


//todo -- miniz might be used in more than one classes (for example storing stuff in .zip)
//is there a .h .c  version?
//if not: should there be a wrapper close for it?

//#define MINIZ_NO_STDIO
#define MINIZ_NO_TIME
#define MINIZ_HEADER_FILE_ONLY
#include "stb/miniz.h"

#include "pugixml/pugixml.hpp"




static void base64_decode(std::string &encoded, std::string &ret );

static int * uncompressLayer(std::string data, int mw, int mh);


xTmxLayer::xTmxLayer(void)
{
  mwidth = 0;
  mheight = 0;
  vecGrid = 0;
}//ctor


xTmxLayer::~xTmxLayer(void)
{
  clear();
}//dtor



void 
xTmxLayer::clear(void)
{
  if (vecGrid != 0) { delete [] vecGrid; } vecGrid = 0;
  mwidth = 0;
  mheight = 0;
  
}//clear


void 
xTmxLayer::addNum(int n)
{
 
  int i;
  int k;
  int num;
  num = mwidth * mheight;

  for (i = 0; i < num; i++)
  {
    k = vecGrid[i];
    k += n;
    if (k < 0) { k = 0; }
    vecGrid[i] = k;
  }//nexti

}//addnum






xTmx::xTmx(void)
{
  mwidth = 0;
  mheight = 0;
  
}//ctor




xTmx::~xTmx(void)
{
  clear();
}//dtor



void 
xTmx::clear()
{
  tdVecTmxRect ::iterator it;
  tdVecTmxLayer ::iterator kt; 
 
  mwidth = 0;
  mheight = 0;

  for (it = vecRect.begin(); it != vecRect.end(); it++)
  {
    delete (*it);
  }//nextit

  vecRect.clear();
  mapRect.clear();


  for (kt = vecLayer.begin(); kt != vecLayer.end(); kt++)
  {
    delete (*kt);
  }//nextk

  vecLayer.clear();
  mapLayer.clear();

}//clear





bool 
xTmx::loadFile(std::string fname)
{
  pugi::xml_document tmx;
 	pugi::xml_node map;
  pugi::xml_node tset;
  pugi::xml_node m;
  pugi::xml_node og;
  pugi::xml_node data;
  xTmxRect * a;
  xTmxLayer * layer;


  clear();


	if  (!(tmx.load_file(fname.c_str(), 0) ))	{		printf("xTmx: Couldn't load tmx [%s] \n", fname.c_str());		return false;	}//endif

	map = tmx.child("map");
	if (map.empty() ) { return false; } //todo error invalid file



    mwidth = map.attribute("width").as_int();
  	mheight = map.attribute("height").as_int(); 

    

  tset = map.child("tileset");
	if (tset.empty() ) { return false; } //todo error invalid file
     setName = tset.attribute("name").value(); 


  float rx, ry, rw, rh;

  for (og = map.child("objectgroup"); og; og = og.next_sibling("objectgroup")) 
  {
    for (m = og.child("object"); m; m = m.next_sibling("object"))
    {
      a = new xTmxRect();
      a->name = m.attribute("name").value();  
      a->type = m.attribute("type").value();  

      a->rx = m.attribute("x").as_float();
      a->ry = m.attribute("y").as_float();
      a->rw = m.attribute("width").as_float();
      a->rh = m.attribute("height").as_float();

      a->cx = (a->rx + a->rw*0.5f);
      a->cy = (a->ry + a->rh*0.5f);

      a->ang = m.attribute("rotation").as_float()  * (3.1415f/180.0f); //convert rotation to radians
      
  
     //todo -- need to rotate to get real position
     //problem: for rectangles tiled uses the upper left corner
     //even when the rectangle is rotated
     if (a->ang != 0)
     {
       rw = a->rw * 0.5f; rh = a->rh * 0.5f;
       rx = cos(a->ang) * rw - sin(a->ang) * rh;
       ry = sin(a->ang) * rw + cos(a->ang) * rh;
       a->cx += rx-rw;  a->cy += ry-rh;             
     }//endif



      vecRect.push_back(a);
      mapRect[a->name] = a;

    }//nextm_object
  }//nextog


  for (m = map.child("layer"); m; m = m.next_sibling("layer"))
  { 
    layer = new xTmxLayer();
    layer->name = m.attribute("name").value(); 

      vecLayer.push_back(layer);
      mapLayer[layer->name] = layer;

    
    

     data = m.child("data");
    
    	std::string comp = data.attribute("compression").value() ;
      std::string enc = data.attribute("encoding").value();

      if (comp != "zlib") { printf("xTmx::loadFile -- error, compression needs to be zlib \n"); continue; }//endif
      if (enc != "base64")	{	printf("xTmx::loadFile --  error, encoding needs to be base64 \n"); continue; }//endif


      
    layer->mwidth = mwidth;
    layer->mheight = mheight;
    layer->vecGrid = uncompressLayer(data.child_value(), mwidth, mheight);


    //debug print
    /*
        int t, i, k, yt;
        for (i = 0; i < mheight; i++)
        {
          yt = i * mwidth;
          for (k = 0; k < mwidth; k++)
          {
            t = layer->vecGrid[k+yt];
            printf("%d", t);
          }//nextk
          printf("\n");
        }//nexti
          printf("\n");
    */

  }//nextm_layer


//
/*
  tdVecTmxRect ::iterator it;
  
  for (it = vecRect.begin(); it != vecRect.end(); it++)
  {
    a = (*it);
    printf("rect n:[%s]  t:[%s]  [%0.2f][%0.2f] [%0.2f][%0.2f] \n", a->name.c_str(), a->type.c_str(), a->cx, a->cy, a->cw, a->ch); 

  }//nextit
*/

  return true;
}//loadfile








xTmxLayer * 
xTmx::getLayer(std::string name)
{
  return mapLayer[name];
}//getlayer


xTmxRect * 
xTmx::getRect(std::string name)
{
  return mapRect[name];
}//getrect



static void remChar(char ch, std::string &str)
{
//	int i;
	int num;
	int c;
		num = (int) str.size();

 	c = (int) str.find(ch);
  while (c > -1)
	{
			str.erase(str.begin()+c);
      c = (int) str.find(ch);
	}//nexti
}//remchar


static int * uncompressLayer(std::string data, int mw, int mh)
{
  unsigned long numbyte;
  std::string decoded;
  int * vec;
  int success;
  
  numbyte = mw * mh * 4;
  if (numbyte == 0) { return 0; }

	//remove all spaces and newlines from layerdata 
  	remChar('\n', data); 
  	remChar(' ', data);

 	
	base64_decode(data, decoded);

  vec = new int[numbyte];

  //uncompress the decoded string into vec as our layer
   success = mz_uncompress( (Bytef*)vec, &numbyte, (const Bytef*) decoded.c_str(), (mz_ulong) decoded.size() );

	if (success == Z_OK)
  {	printf("xTmx:: uncompression succesful \n"); }
	else
	{
    delete [] vec; vec = 0;
		printf("xTmx:: uncompression error: \n");
	
		if (success == Z_VERSION_ERROR) printf("incompatible zlib version \n"); 
		if (success == Z_MEM_ERROR) printf("there was not enough memory \n"); 
		if (success == Z_BUF_ERROR) printf("there was not enough room in the output buffer \n");
		if (success == Z_DATA_ERROR) printf("the input data was corrupted or incomplete \n");

    return 0;
	}//endif
  
  return vec;
}//uncompress









static void base64_decode(std::string &encoded, std::string &ret )
{
  static std::string base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	ret = "";

	int i;
	int num;

	unsigned char a1, a2, a3;
	unsigned char b1, b2, b3, b4;

	i = 0;
	num = (int) encoded.size();

	int k;
	k = 0;

	while (i < num)
	{
		b1 = (unsigned char) base64.find(encoded[i]); i++; k++;
		b2 = (unsigned char) base64.find(encoded[i]); i++; k++;
		b3 = (unsigned char) base64.find(encoded[i]); if (encoded[i] == '=') break; i++; k++;
		b4 = (unsigned char) base64.find(encoded[i]); if (encoded[i] == '=') break; i++;

		k = 0;

  	a1 = (b1 << 2) + ((b2 & 0x30) >> 4);
    a2 = ((b2 & 0xf) << 4) + ((b3 & 0x3c) >> 2);
    a3 = ((b3 & 0x3) << 6) + b4;

			ret += a1;
			ret += a2;
			ret += a3;
	}//wend


	if (k == 3)
	{
		b4 =  0x00;

		a1 = (b1 << 2) + ((b2 & 0x30) >> 4);
		a2 = ((b2 & 0xf) << 4) + ((b3 & 0x3c) >> 2);
		a3 = ((b3 & 0x3) << 6) + b4;

		ret += a1;
		ret += a2;
		ret += a3;
	
	}//endif

	if (k == 2)
	{
		b3 = 0x00;
		b4 =  0x00;

		a1 = (b1 << 2) + ((b2 & 0x30) >> 4);
		a2 = ((b2 & 0xf) << 4) + ((b3 & 0x3c) >> 2);
		a3 = ((b3 & 0x3) << 6) + b4;

		ret += a1;
		ret += a2;
		ret += a3;
	}//endif
}//decode 


