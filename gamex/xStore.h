#pragma once

#include <map>
#include <string>

extern  std::string stripName(std::string str);

//usage is: xStore <xData> myStore;  (rem dont use pointer, <xData*> is not recommended)
template <typename T>
class xStore
{
public:
  std::map <std::string, T *> mapData;
public:
  xStore(void) {}
  ~xStore(void) { clear(); }
  
  void clear(void)
  {
     typename std::map <std::string, T *> ::iterator it;
     for (it = mapData.begin(); it != mapData.end(); it++)
     { delete it->second; }
     mapData.clear();
  }//clear

  void addData(std::string wname, T* data)
  {
    T * a;
    a = mapData[wname];
    if (a != 0) { delete a; a = 0; }
    mapData[wname] = data;
  }//add

  T * getData(std::string wname)
  {
   return mapData[wname]; 
  }//getdata


  void removeData(std::string wname)
  {
    T * a;
    a = mapData[wname];
    if (a != 0) { delete a; a = 0; }
    mapData.erase(wname);
  }//removedata



};//xstorage
