#pragma once

#include <string>

//data shared by actors belonging to same group
//multiple actors can refer to the the same actorinfo
//they are not meant to be destroyed or created during gameplay (maybe between levels)

//maybe could be called groupInfo.. ?

class xActorInfo 
{
public:
  std::string wname;

  
  
};//xActorInfo