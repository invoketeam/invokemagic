#pragma once

#include <string>
#include <cstdlib>
#include "pugixml/pugixml.hpp"


#include "xTexture.h"

#define MAX_XCHAR 256



class xChar
{
public:
    int valid;  //is actually a character or just empty def
    float u0, v0, u1, v1; //texcoord
    float width, height; //image size
    float addx; //add to position
    float ox, oy; //offset
public:
  xChar() 
  {
        valid = 0;     addx = 0.0f;
        u0 = 0.0f;        v0 = 0.0f;
        u1 = 0.0f;        v1 = 0.0f;
        width = 0.0f;     height = 0.0f;
        ox = 0.0f; oy = 0.0f;
  }//ctor 

};//xchar



class xFont
{
public:
    std::string skin;
    xChar vecChar[MAX_XCHAR];
    float chwidth;
    float chheight;
    float fontsize;

    float ascent;
    float descent;
    float linegap;
public:
    xFont();
    ~xFont();
    void clear();

public:
    //xfnt files
    void loadCharDef(std::string fname);

    //use this to write text (works like printf)
    //e.g. printStr(16, 0,0, "Hello %s !", name.c_str() );
    //size: characters will be drawn in this size regardless of fontsize
    void printStr(float size, float cx, float cy, const char* str, ...);

    void writeStr(float cx, float cy, std::string str, float scale=1.0f);


    //todo -- setmesh (set an xMdx3 model data to the string mesh)
    

public:
    float getXcoordFromCharPos(std::string &str, int pos, float scale = 1.0f);
    int getCharPos(std::string &str, float xcoord, float scale=1.0f, unsigned char * ch = 0, float * endx = 0);
    float getWidth(std::string &str, float scale=1.0f);


};//xfont


