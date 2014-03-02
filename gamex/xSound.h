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
  xSoundData(void)
  { 
    handle = 0;  
  }

  ~xSoundData(void)
  { 
   clear();
  }

  void clear(void)
  { 
    if (handle != 0) {	  alDeleteBuffers(1, &handle); } handle = 0;

  }//clear


  void loadFile(std::string fname) 
  {
   	ALenum format;
	  ALsizei size;
  	ALsizei freq;
    stb_vorbis * oggfile;
    stb_vorbis_info ogginfo;
    int numshort;
    short * data;

    clear();


	  alGenBuffers(1, &handle);
    if(alGetError() != AL_NO_ERROR) {	printf ("xSoundData: couldnt make buffer %s  \n ", fname.c_str());	return; }

  
    oggfile = stb_vorbis_open_filename((char *) fname.c_str() , 0, 0);

  	  if ( oggfile == 0 ) { printf ("xSoundData: couldnt load ogg %s \n ", fname.c_str() );		return;	}

    ogginfo = stb_vorbis_get_info(oggfile);
      
      if (ogginfo.channels > 2) { printf ("xSoundData:  ogg format not supported %s \n ", fname.c_str() );	 return; } //crazy ass ogg files are not supported yet


    format = (ogginfo.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    freq = ogginfo.sample_rate;
    numshort = stb_vorbis_stream_length_in_samples(oggfile) * (unsigned)ogginfo.channels;
    size = numshort * 2; // 2 == sizeof(short)


    data = new short[numshort];
    stb_vorbis_get_samples_short_interleaved(oggfile, ogginfo.channels, data, numshort);
    stb_vorbis_close(oggfile);

      alBufferData(handle, format, data, size, freq);
      delete [] data;


     // printf("xSoundData: %s size: %d bytes  (%d k) (%0.2f m) \n", fname.c_str(), size, (size/1024), (((float)size/1024.0f)/1024.0f) );


  }//loadfile


};//xsnddata






class xSound
{
public:
  unsigned int handle;
  unsigned int buffer;
  int id;
//  int prio; //priority
 // int playing;

public:
  xSound(void) { handle = 0; buffer = 0; id = -1;  }
  ~xSound(void) { clear(); }

  void clear(void) 
  {
    stop();

    if (handle != 0) {  if (alIsSource(handle))     {  alDeleteSources(1, &handle); }   }
    handle = 0;
  }//clear


  void stop(void) 
  {
    if (handle == 0) { return; }
    alSourceStop(handle);
  }//stop


  void pause(void) 
  { 
    if (handle == 0) { return; }
    alSourcePause(handle);  
  }//pause



  //for 2D sounds: set relative to true, ax,ay,az to 0.0f
  void play(float ax, float ay, float az, float gain = 1.0f, bool brelative = false, bool bloop = false) 
  {
    if (handle == 0) { return; }
    

    alSourcei (handle, AL_SOURCE_RELATIVE, brelative);
	  alSourcef (handle, AL_GAIN,     gain     );
  	alSourcef (handle, AL_PITCH,    1.0      );
	  alSource3f(handle, AL_POSITION, ax, ay, az);
  	alSource3f(handle, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei (handle, AL_LOOPING,  bloop     );


    alSourcePlay(handle);

   // int error;
   // error = alGetError();
   // if (error != AL_NO_ERROR) {  printf("xSound: play error %d \n", error); }
   // else {  printf("xSound: play no error %d \n", error); }
   
   // printf("sourceplay %u %d \n", handle, isPlaying() ? 1 : 0);



  }//play

 

  void makeSource(unsigned int buf)
  {
    clear();

  	alGenSources(1, &handle);
  	if(alGetError() != AL_NO_ERROR) { printf("xSound: makesource -- error \n");	return;}

    //printf("makesource %u %d \n", handle, alIsSource(handle) ? 1 : 0);

    buffer = buf;
	    alSourcei (handle, AL_BUFFER,   buffer   );

    if(alGetError() != AL_NO_ERROR) { printf("xSound: makesource -- error 2 \n");	return; }

  }//makesource



  bool isStopped(void)
  {
  if (handle == 0) { return false; }

    int state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return (state == AL_STOPPED);   

  }//ispaused


  bool isPaused(void)
  {
  if (handle == 0) { return false; }

    int state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return (state == AL_PAUSED);    

  }//ispaused


  bool isPlaying(void)
  {
    if (handle == 0) { return false; }

    int state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);    
  }//isplaying



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
  xSoundMan(void) { pDevice = 0; pContext = 0; it = 0; curId = 1; }
  ~xSoundMan(void) { clear(); }


  void init(void)
  {

    pDevice = alcOpenDevice(0);
    if (pDevice == 0) { printf("xSoundMan: error: couldn't open device \n"); return; }
  
    pContext = alcCreateContext(pDevice, 0);
    alcMakeContextCurrent(pContext);
    if (pContext == 0)  { printf("xSoundMan: error: couldn't create context \n");  alcCloseDevice (pDevice);    return;   }

    if (!alcMakeContextCurrent (pContext))  {  printf("xSoundMan: error: couldn't set context \n");   alcDestroyContext (pContext);      alcCloseDevice (pDevice); return; }
 
    //alGetError();

    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

   	alListenerfv(AL_POSITION,    ListenerPos);
	  alListenerfv(AL_VELOCITY,    ListenerVel);
  	alListenerfv(AL_ORIENTATION, ListenerOri);

  }//init


  void clear(void)
  {
    stopAllSound();
    storeBuff.clear();

    alcMakeContextCurrent(0);
    alcDestroyContext(pContext);
    alcCloseDevice(pDevice);
  }//clear


  //note: only .ogg is supported (mono or stereo only)
  void addSoundFile(std::string fname, std::string altName="0")
  {
    xSoundData * a;
    a = new xSoundData;
    a->loadFile(fname);
    if (altName != "0") { fname = altName;  }
    else { fname = stripName(fname); }
    storeBuff.addData( fname , a);
  }//addsound



  int playSound(std::string wname) 
  {
    xSoundData * buff;
    buff = storeBuff.getData(wname); 
    if (buff == 0) { return -1; }
    if (buff->handle == 0) { return -2;}

    xSound * a;
    int i;
    for (i = 0; i < XSOUNDMAN_MAXSOUND; i++) 
    {
      a = &(vecSound[it]);
      if (a->isPlaying() == false)  {  break;  }
      
      it += 1; if (it >= XSOUNDMAN_MAXSOUND) { it = 0;}
    }//nexti

    a = &(vecSound[it]);
    
    curId += 1;    
    a->id = curId;
    a->makeSource(buff->handle);
    a->play(0,0,0);

      printf("playing sound %s  %d  %d \n", wname.c_str(), a->id, it );

    it += 1; if (it >= XSOUNDMAN_MAXSOUND) { it = 0;}

    return a->id;
  }//playsound


  xSound * getSound(int id) 
  { 
    //go through all 32 sounds and select one with id
    int i;
    for (i = 0; i < XSOUNDMAN_MAXSOUND; i++)     {   if (vecSound[i].id == id) { return &(vecSound[i]); }     }
    return 0;
  }//getsound


  void stopAllSound(void) 
  {
    int i;
    for (i = 0; i < XSOUNDMAN_MAXSOUND; i++)  {  vecSound[i].clear(); }
    it = 0;
    curId = 0;
  }//stopall


/*


  void pauseAllSound(void) { }
  void resumeAllSound(void) { }

  void enableSound(void) { }
  void muteSound(void) { }

*/


};//classend
