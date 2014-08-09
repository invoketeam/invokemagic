#pragma warning (disable : 4996) //vc2010 no longer likes fopen
#pragma warning (disable : 4786)

#include "xMultiMdx3.h"


xMultiMdx3::xMultiMdx3(void)
{
  numMesh = 0;
  vecMesh = 0;
}//ctor

xMultiMdx3::~xMultiMdx3(void)
{
 clear();
}//dtor


xMdx3 * 
xMultiMdx3::getMesh(int i)
{
  if (i < 0) { return 0; }
  if (i >= numMesh) { return 0; }
  return &(vecMesh[i]);
}//getmesh

 
void 
xMultiMdx3::clear(void)
{
  if (vecMesh != 0) { delete [] vecMesh; } vecMesh = 0;
  numMesh = 0;
}//clear

void 
xMultiMdx3::loadFile(std::string fname)
{
                      
    std::string comp = "MULTI3_2014";
    char buf[12];
    FILE * file;

    clear();
   
    file = fopen(fname.c_str(), "rb");
    if (file == 0) { printf("xMultiMdx3 -- Error -- invalid file %s \n", fname.c_str()); return; } //todo error
    
    //FILE_ID
     fread(buf, 12, 1, file);
     if (comp != buf) { printf("xMultiMdx3 -- Error -- invalid format %s \n", fname.c_str()); fclose(file); return; } //todo error invalid format

    //HEADER
      fread(&numMesh, 4, 1, file);  
      
      //reserved -- 32 bytes
        fseek ( file , (16+16) , SEEK_CUR);
    
      vecMesh = new xMdx3[numMesh];
      
      int i;
      for (i = 0; i < numMesh; i++)
      {
        vecMesh[i].readFile(file);
      }//nexti

    //CLOSE FILE
    fclose(file);

    //its cheap so calculate the min max for the whole multimesh
     calcMinMax();

}//loadfile



void 
xMultiMdx3::calcMinMax(void)
{
  int i;
  int num;
  xMdx3 * a;

  num = numMesh;

  //dont make it smaller than this
  //min = -1024.0f;    max = 1024.0f;

  min = -16.0f;  max = 16.0f;
    
    for (i = 0; i < num; i++)
    {
      a = &(vecMesh[i]);

      min.x = min.x < a->min.x ? min.x : a->min.x;
      min.y = min.y < a->min.y ? min.y : a->min.y;
      min.z = min.z < a->min.z ? min.z : a->min.z;
     
      max.x = max.x > a->max.x ? max.x : a->max.x;
      max.y = max.y > a->max.y ? max.y : a->max.y;
      max.z = max.z > a->max.z ? max.z : a->max.z;
     
    }//nexti  

    //add some extra (just in case)
    min -= 16.0f;    max += 16.0f;

    size = max - min;

    //printf("multimdx min [%0.2f][%0.2f][%0.2f] max [%0.2f][%0.2f][%0.2f] \n ", min.x, min.y, min.z,  max.x, max.y, max.z); 

}//calcminmax