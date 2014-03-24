#pragma once


#include <string>

#include "xStore.h"


#include "openal/al.h"
#include "openal/alc.h"
#pragma comment (lib, "gamex/openal/OpenAL32.lib")
#include "stb_vorbis.h"







class xSoundData
{
public:
  std::string name;
  unsigned int handle;

public:
  xSoundData(void);
  ~xSoundData(void);

  void clear(void);
  void loadFile(std::string fname);

};//xsnddata






class xSound
{
public:
  unsigned int handle;
  unsigned int buffer;
  int id;

public:
  xSound(void);
  ~xSound(void);

  void clear(void) ;
  void stop(void) ;
  void pause(void);

  //for 2D sounds: set relative to true, ax,ay,az to 0.0f
  void play(float ax, float ay, float az, float gain = 1.0f, bool bRelative = false, bool bLoop = false);

  void makeSource(unsigned int buf);

  bool isStopped(void);
  bool isPaused(void);
  bool isPlaying(void);


};//xsound





//windows supports max 256 sounds played at once
//we probably dont need that many
#define XSOUNDMAN_MAXSOUND 16 //32



class xSoundMan
{
public:

  ALCdevice* pDevice;
  ALCcontext* pContext;

  xStore <xSoundData> storeBuff; 

  xSound vecSound[XSOUNDMAN_MAXSOUND];

  int it;
  int curId;

public:
  xSoundMan(void);
  ~xSoundMan(void) ;


  void init(void);
  void clear(void);


  //note: only .ogg is supported (mono or stereo only)
  void addSoundFile(std::string fname, std::string altName="0");
  int playSound(std::string wname);


  xSound * getSound(int id);


  void stopAllSound(void);
  void testPause(void);
  void testResume(void);


  void startMusic(std::string fname);
  void updateMusic(void);
  void pauseMusic(void);
  void resumeMusic(void);
  void stopMusic(void);


  //TODO
  //void pauseAllSound(void) { }
  //void resumeAllSound(void) { }

  //void enableSound(void) { }
  //void muteSound(void) { }


};//classend
