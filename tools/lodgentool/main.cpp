#pragma warning (disable : 4786)



#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <conio.h>

#include <math.h>

#include <iostream>
#include <fstream>
#include <cctype>



#include "xMdx3.h"
#include "xSkel.h"
#include "xLodGen.h"












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
  
  xLodGen myGen;
  xMdx3 mesh;
  xSkel mySkel;

  
  //todo 
  // -- set output file name
  // -- more options for mesh level generation (maybe -levels 90,80,70,60 ?)
    
  if (myCommand.hasCommand("mesh"))
  {
    mesh.loadFile(myCommand.getCommandArg("mesh"));
    if (mesh.numFace == 0) { printf (" error, mesh file not found \n "); return -1; }
  
    if (myCommand.hasCommand("skel") )
    {
      mySkel.loadFile( myCommand.getCommandArg("skel") );

      if (mySkel.numBone == 0)
      { printf (" error, skeleton file not found \n "); return -1;  }
      
      if (mySkel.numVert != mesh.numVert) { printf (" error, skeleton and mesh vertex count mismath %d %d \n ", mesh.numVert, mySkel.numVert ); return -1; }  
    }//endif2    
   
  
    myGen.addBase(&mesh);
      myGen.addLevel(&mesh, mesh.numFace*0.75);
      myGen.addLevel(&mesh, mesh.numFace*0.5);
      myGen.addLevel(&mesh, mesh.numFace*0.25);
      myGen.addLevel(&mesh, mesh.numFace*0.125);  
      
    myGen.reIndex(&mesh);
      myGen.reSaveMdx(&mesh, "out_100.mdx3");  
      myGen.saveLevelAsMdx(&mesh, 1, "out_75.mdx3");
      myGen.saveLevelAsMdx(&mesh, 2, "out_50.mdx3");
      myGen.saveLevelAsMdx(&mesh, 3, "out_25.mdx3");
      myGen.saveLevelAsMdx(&mesh, 4, "out_12.mdx3");

      
    if (mySkel.numBone > 0)
    {
      mySkel.reIndexSkin(myGen.vecPos, mesh.numVert);
      mySkel.saveFile("out_skel.xskb");
    }//endif2
      
    return 0;
  }//endif
  
  printf("Usage:  \n");
  printf("mdxLodGen.exe  -mesh myMesh.mdx3 [ -skel mySkel.xskb ]  \n");
  
 

  return 0;
}//main

