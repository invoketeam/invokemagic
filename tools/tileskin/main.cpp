#pragma warning (disable : 4786)


#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <cstdlib>

#include <conio.h>

#include <math.h>

#include <iostream>
#include <fstream>
#include <cctype>

#define MINIZ_HEADER_FILE_ONLY
#define MINIZ_NO_STDIO
#define MINIZ_NO_TIME
#include "miniz.h"


#include "xImage.h"



void writePng(xImage * img, std::string fname)
{
  if (img->dat == 0) { return; }
  size_t size;
  void * buf;
  FILE * file;

  img->endianSwap();
  buf = tdefl_write_image_to_png_file_in_memory(img->dat, img->mw, img->mh, 4, &size);
  img->endianSwap();

    file = fopen(fname.c_str(), "wb");
    fwrite(buf, 1, size, file);
    fclose(file);
 
  mz_free(buf);
}//writepng




class xCmnd
{
public:
  std::vector <std::string> vecArg;
  std::map <std::string, std::string> mapCmd;
public:
  xCmnd() {}

  bool hasCommand(std::string c)
  { return (mapCmd.count(c) > 0); }

  std::string getCommandArg(std::string  c)
  {
    if (mapCmd.count(c) < 1) { return "null"; }
    return mapCmd[c];
  }//getarg


  void initArg(int argc, char * argv[])
  {
    int i;
    std::string temp;
    for (i = 0; i < argc; i++)
    {
      temp = argv[i];
      vecArg.push_back(temp);
    }//nexti

    int num;
    num = vecArg.size() - 1;
    for (i = 0; i < num; i++)
    {
      if (isCommand(vecArg[i]))
      {
        if (isCommand(vecArg[i+1])==false)
        {
            temp = getCmdStr(vecArg[i]);
            mapCmd[temp] = vecArg[i+1];
            printf("Found command %s \n", temp.c_str());
        }//endif2
      }//endif
    }//nexti
  }//initarg


  std::string getCmdStr(std::string in)
  {
    int k;
    k = in.find_first_not_of('-');
    return in.substr(k);
  }//getcmdstr


  bool isCommand(std::string &str)
  {
    if (str.size() < 2) { return false; }
    if (str[0] == '-') { return true; }
    if (str[1] == '-') { return true; }
    return false;
  }//iscommand


};//xcmnd


//maximum number of tiles in tiled
#define MAX_TILENUM 9999

xImage vecTile[MAX_TILENUM]; 



int getSize(int tw, int th, int num)
{
  int i;
  int size;
  int cx, cy;
  bool bDone;

  size = 64;

  
  bDone = false;

  while (bDone == false)
  {
    size += size; //*2
    cx = 1;
    cy = 1;
    bDone = true;
  
    for (i = 0; i < num; i++)
    {
      cx += tw + 2;
      if (cx+tw+tw+2 >= size) { cx = 1; cy += th + 2;}
      if (cy >= size) { bDone = false; break; }  
    }//nexti
  }//wend


  xImage testImage;
  testImage.init(size, size);

  cx = 1;
  cy = 1;
  for (i = 0; i < num; i++)
  {
    testImage.drawRect(cx, cy,tw,th, 0xFF00FF00);

    cx += tw + 2;
    if (cx+tw+tw+2 >= size) { cx = 1; cy += th + 2;}
    if (cy >= size) { break; }  
  }//nexti
  writePng(&testImage, "out_testImage.png");


  return size;
}//getsize

//might also need to save some file about the new coordinates of the tiles
//(a better idea is to generate the texture ingame)
void procTile(void)
{
  xImage myImage;
  xImage myTile;
  xImage outImage;
  int tw;
  int th;
  int size;
  int maxNum;
  int num;

  //todo -- set these from parameters (min 8x8)
  tw = 64; //32;
  th = 64;// 32;

  //todo -- also check the maximum number of tiles that fit in the image
  num = 256; //512;
  

//  myImage.loadImage("megaset.png");
  myImage.loadImage("terratex.png");

  maxNum = (myImage.mw/tw) * (myImage.mh/th);
  printf("num %d  maxNum %d \n", num, maxNum);
  if (num > maxNum) { num = maxNum;}
  if (num > MAX_TILENUM) { num = MAX_TILENUM; }

  size = getSize(tw, th, num);
  printf("size %d \n", size);

  myTile.init(tw, th);
  myTile.drawImage(&myImage, 0, 0);

  writePng(&myTile, "out_test.png");

  xImage * a;
  int i;
  int setw;
  setw = myImage.mw/tw;

  int kx, ky;
  int cx, cy;

  cx = 1;
  cy = 1;
  for (i = 0; i < num; i++)
  {
    kx = i % setw;
    ky = i / setw;

    a = &(vecTile[i]);
    a->init(tw, th);
    a->drawImage(&myImage, -(kx*tw), -(ky*th));
    //a->drawImage(&myImage, 0, 0);

    a->x = cx;
    a->y = cy;
    cx += tw + 2;
    if (cx+tw+tw+2 >= size) { cx = 1; cy += th + 2;}
     
  }//nexti


  //save out tile
  outImage.init(size, size);
  for (i = 0; i < num; i++)
  {
    a = &(vecTile[i]);

    //outImage.drawRect(a->x-1, a->y-1,tw+1,th+1, 0xFF00FF00);
  
    //bleeding
      outImage.drawImage(a, a->x-1, a->y);
      outImage.drawImage(a, a->x+1, a->y);
      outImage.drawImage(a, a->x, a->y-1);
      outImage.drawImage(a, a->x, a->y+1);
      
    //corners
      outImage.setPixel(a->x-1, a->y-1, a->getPixel32(0,0));
      outImage.setPixel(a->x+tw, a->y-1, a->getPixel32(tw-1,0));
      outImage.setPixel(a->x-1, a->y+th, a->getPixel32(0,th-1));
      outImage.setPixel(a->x+tw, a->y+th, a->getPixel32(tw-1,th-1));


    outImage.drawImage(a, a->x, a->y);

  }//outimage


  writePng(&outImage, "out_done.png");

  //the sprite file is special -- as it uses id instead of name
  std::string picname;
  std::ofstream myfile;

  picname = "out_set";
  myfile.open ((picname+".xms").c_str());
  myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  myfile << "<tiletex width=\""<< size <<"\" height=\""<< size <<"\" name=\"" << picname << ".png\">\n";
  
  for (i = 0; i < num; i++)
  {
    a = &(vecTile[i]);
    
    myfile << "  <image  x=\""<< (a->x) <<"\" y=\"" << (a->y) << "\" width=\""<< (a->mw) <<"\" height=\""<< (a->mh) <<"\" id=\""<< i <<"\"/>\n";
  }//nextit

  myfile << "</tiletex>\n";
  myfile.close();
  
  

}//proctile



int main(int argc, char* argv[])
{
  xCmnd myCommand;

  myCommand.initArg(argc, argv);

  procTile();


  return 0;
}//main


