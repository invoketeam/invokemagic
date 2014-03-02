#pragma warning (disable : 4786)

#include "xStore.h"


  std::string stripName(std::string str)
  {
    size_t f;
    size_t t;
    std::string tmp;
     f = str.find('.');
     tmp = str.substr(0, f);
     t = tmp.find('/');
     tmp = tmp.substr(t+1, tmp.size() );
     //printf("strip %s \n ", tmp.c_str());
    return tmp;
  }//strip

