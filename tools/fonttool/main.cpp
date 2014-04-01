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
#include "fontmake.h"



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







int main(int argc, char* argv[])
{
  xCmnd myCommand;

  myCommand.initArg(argc, argv);


  int fontSize;
 // int skinSize; //no longer needed
  int edge;
  int addSpace;
  int endchar;

  fontSize = 32;
  //skinSize = 512; //now its autosize
//  edge = 8;
//  addSpace = 2;
  edge = 2;
  addSpace = 0;
  endchar = 128;

  //todo -- set which characters to export (so far its only 32-128)
  // which characters to skip?

  //edge is needed when you want to add outlines later in photoshop or something
  //addspace -- because the outline the letters will be bigger, add some extra spacing

  //todo -- output filename


  if (myCommand.hasCommand("size"))
  {
    fontSize = atoi(myCommand.getCommandArg("size").c_str() );
    if (fontSize < 1 || fontSize > 512) { printf("Font size invalid %d [max 1-512 ] \n", fontSize); return 1; }
  }//endif

  
  if (myCommand.hasCommand("endchar"))
  {
    endchar = atoi(myCommand.getCommandArg("endchar").c_str() );
    if (endchar < 32 || endchar > 256) { printf("Endchar invalid %d [max 32-256 ] \n", endchar); return 1; }
  }//endif
  
/*
  if (myCommand.hasCommand("skin"))
  {
    skinSize = atoi(myCommand.getCommandArg("skin").c_str() );
    if (skinSize < 64 || skinSize > 4096) { printf("Texure size invalid %d [max 64-4096 ] \n", skinSize); return 1; }
  }//endif
 */

  if (myCommand.hasCommand("edge"))
  {
    edge = atoi(myCommand.getCommandArg("edge").c_str() );
    if (edge < 0 || edge > 512) { printf("Edge size invalid %d [max 0-512 ] \n", edge); return 1; }
  }//endif


  if (myCommand.hasCommand("space"))
  {
    addSpace = atoi(myCommand.getCommandArg("space").c_str() );
    if (addSpace < 0 || addSpace > 512) { printf("Space size invalid %d [max 0-512 ] \n", addSpace); return 1; }
  }//endif



  if (myCommand.hasCommand("font"))
  {

     saveFont( myCommand.getCommandArg("font"), "out.xfnt", "out.png", fontSize,endchar,edge,addSpace);
     return 0;

  }
  else
  {
    printf("No font file was specified \n");
    return 1;
  }

  printf("Usage: \n");
  printf("fontTool.exe -font fontname.ttx  [-size 32] [-edge 8] [-space 2] [-endchar 128]  \n");



  return 0;
}//main