#pragma warning (disable : 4786)

#include <windows.h>
#include <Winuser.h>

#include "xSound.h"

#include <vector>




xSoundData::xSoundData(void)
  { 
    handle = 0;  
  }

xSoundData::~xSoundData(void)
  { 
   clear();
  }

void
xSoundData::clear(void)
  { 
    if (handle != 0) {	  alDeleteBuffers(1, &handle); } handle = 0;

  }//clear


void
xSoundData::loadFile(std::string fname) 
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














xSound::xSound(void) { handle = 0; buffer = 0; id = -1;  }
xSound::~xSound(void) { clear(); }

void 
xSound::clear(void) 
  {
    stop();

    if (handle != 0) {  if (alIsSource(handle))     {  alDeleteSources(1, &handle); }   }
    handle = 0;
  }//clear


void 
xSound::stop(void) 
  {
    if (handle == 0) { return; }
    alSourceStop(handle);
  }//stop


void 
xSound::pause(void) 
  { 
    if (handle == 0) { return; }
    alSourcePause(handle);  
  }//pause



  //for 2D sounds: set relative to true, ax,ay,az to 0.0f
void 
xSound::play(float ax, float ay, float az, float gain, bool bRelative, bool bLoop) 
  {
    if (handle == 0) { return; }
    

    alSourcei (handle, AL_SOURCE_RELATIVE, bRelative);
	  alSourcef (handle, AL_GAIN,     gain     );
  	alSourcef (handle, AL_PITCH,    1.0      );
	  alSource3f(handle, AL_POSITION, ax, ay, az);
  	alSource3f(handle, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei (handle, AL_LOOPING,  bLoop     );


    alSourcePlay(handle);

   // int error;
   // error = alGetError();
   // if (error != AL_NO_ERROR) {  printf("xSound: play error %d \n", error); }
   // else {  printf("xSound: play no error %d \n", error); }
   
   // printf("sourceplay %u %d \n", handle, isPlaying() ? 1 : 0);



  }//play




void 
xSound::makeSource(unsigned int buf)
  {
    clear();

  	alGenSources(1, &handle);
  	if(alGetError() != AL_NO_ERROR) { printf("xSound: makesource -- error \n");	return;}

    //printf("makesource %u %d \n", handle, alIsSource(handle) ? 1 : 0);

    buffer = buf;
	    alSourcei (handle, AL_BUFFER,   buffer   );

    if(alGetError() != AL_NO_ERROR) { printf("xSound: makesource -- error 2 \n");	return; }

  }//makesource



bool 
xSound::isStopped(void)
  {
  if (handle == 0) { return false; }

    int state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return (state == AL_STOPPED);   

  }//ispaused


bool
xSound::isPaused(void)
  {
  if (handle == 0) { return false; }

    int state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return (state == AL_PAUSED);    

  }//ispaused


bool 
xSound::isPlaying(void)
  {
    if (handle == 0) { return false; }

    int state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);    
  }//isplaying















xSoundMan::xSoundMan(void) { pDevice = 0; pContext = 0; it = 0; curId = 1; }
xSoundMan::~xSoundMan(void) { clear(); }


void 
xSoundMan::init(void)
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


void 
xSoundMan::clear(void)
  {
    stopAllSound();
    storeBuff.clear();

    alcMakeContextCurrent(0);
    alcDestroyContext(pContext);
    alcCloseDevice(pDevice);
  }//clear


  //note: only .ogg is supported (mono or stereo only)
void
xSoundMan::addSoundFile(std::string fname, std::string altName)
  {
    xSoundData * a;
    a = new xSoundData;
    a->loadFile(fname);
    if (altName != "0") { fname = altName;  }
    else { fname = stripName(fname); }
    storeBuff.addData( fname , a);
  }//addsound



int
xSoundMan::playSound(std::string wname) 
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


xSound * 
xSoundMan::getSound(int id) 
  { 
    //go through all 32 sounds and select one with id
    int i;
    for (i = 0; i < XSOUNDMAN_MAXSOUND; i++)     {   if (vecSound[i].id == id) { return &(vecSound[i]); }     }
    return 0;
  }//getsound


void
xSoundMan::stopAllSound(void) 
  {
    int i;
    for (i = 0; i < XSOUNDMAN_MAXSOUND; i++)  {  vecSound[i].clear(); }
    it = 0;
    curId = 0;
  }//stopall














#define XMUSNUMBUFFER 3


//TODO -- allow more than one
class xSoundStream
{
public:
  std::string wname;

public:
  	unsigned int que;
    unsigned int offset;
    stb_vorbis * oggfile;
    stb_vorbis_info ogginfo; 
    
    unsigned int numChan;
    unsigned int rate;
    unsigned int numSamp;
    unsigned int format;

    unsigned int handle; //sound handle
  	ALuint vecBuffer[XMUSNUMBUFFER]; 
    ALuint curBuffer; 
   
    int numShort; //aka filesize 
    short * buffer;
 
public:

  xSoundStream(void)
  {
    que = 0;
    offset = 0;
    oggfile = 0;
    numChan = 0;
    rate = 0;
    numSamp = 0;
    format = 0;
    handle = 0;  
    buffer = 0;
    numShort = 0;
  }//ctor

  ~xSoundStream(void)
  {
    clear();
    
  }//dtor


  void clear(void)
  {
    stop();
    if (buffer != 0) { delete [] buffer; } buffer = 0;
    numShort = 0;
  }//clear



  void loadMusic(std::string fname) 
  {
    clear();


     oggfile = stb_vorbis_open_filename((char *) fname.c_str() , 0, 0);
       if ( oggfile == 0 ) { printf ("xSoundStream: couldnt load ogg %s \n ", fname.c_str() );	return;	}

     ogginfo = stb_vorbis_get_info(oggfile);
       if (ogginfo.channels > 2) { printf ("xSoundStream:  number of channels not supported: %s \n ", fname.c_str() ); return; }



     numChan = ogginfo.channels;
	   rate = ogginfo.sample_rate;
	   numSamp = stb_vorbis_stream_length_in_samples(oggfile) * (unsigned int)ogginfo.channels;
	   format =  (ogginfo.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

     numShort = rate * numChan * 1; //1 -- number of seconds
     buffer = new short[numShort];
   
    
      int bufsize;
      bufsize = (rate * numChan * 2);
        printf("number of bytes in buffer:  %d   kb: %d \n ", bufsize, (bufsize / 1024)   );


  
   	  alGenBuffers(XMUSNUMBUFFER, vecBuffer);

	    alGenSources(1, &handle);
  	  alSourcei(handle, AL_BUFFER, 0);

      unsigned int k;
      int i;
        for (i = 0; i < XMUSNUMBUFFER;)
        {
          k = (unsigned int) stb_vorbis_get_samples_short_interleaved(oggfile, numChan, &buffer[0], numShort);

          k *= numChan;
          que += k;


          if (k > 0) { alBufferData(vecBuffer[i++], format, &buffer[0], (ALsizei)(k * 2), rate); }
        
          if (k < numShort) { break; }
        }//nexti


        if (i > 0)      {  alSourceQueueBuffers(handle, i, vecBuffer);    alSourcePlay(handle);   }
        if (que >= numSamp)    {   que = 0;   stb_vorbis_seek_start(oggfile);   }
  }//playmusic



  void stop(void) 
  {
    if (alIsSource(handle) == false) { return; } 

    alSourceStop(handle);
  	alSourcei(handle, AL_BUFFER, 0);
  	alDeleteBuffers(XMUSNUMBUFFER, vecBuffer);
    
    stb_vorbis_close(oggfile);

  	alSourcei(handle, AL_BUFFER, 0);
    alDeleteSources(1, &handle);

    handle = 0;
  }//stop



  void pause(void)
  {
    if (alIsSource(handle) == false) { return; } 
    alSourcePause(handle); 
  }//pause

  void resume(void)
  {
   if (alIsSource(handle) == false) { return; } 
   alSourcePlay(handle); 
  }//resume




  void update(void) 
  {
 
    if (alIsSource(handle) == false) { return; } 

    int state;
    unsigned int samples;
    ALint bytes, bits;
    int k;


    alGetSourcei(handle, AL_SOURCE_STATE, &state);

		if (state == AL_PLAYING && que < numSamp)
		{

			alGetSourcei(handle, AL_BUFFERS_PROCESSED, &k);

      while (k > 0)
			{
        k -= 1;

				alSourceUnqueueBuffers(handle, 1, &curBuffer);


				alGetBufferi(curBuffer, AL_SIZE, &bytes);
				alGetBufferi(curBuffer, AL_BITS, &bits);

				offset += (bytes * 8) / bits;

        samples = (unsigned int) stb_vorbis_get_samples_short_interleaved(oggfile, numChan, &buffer[0], numShort);

				samples *= numChan;
				que += samples;


				if (samples > 0)
				{
					alBufferData(curBuffer, format, &buffer[0], (ALsizei)(samples * 2), rate);
					alSourceQueueBuffers(handle, 1, &curBuffer);
				}
				

        if (samples < numShort) { break; }
    
 			}//wend


			if (que >= numSamp) { que = 0;  stb_vorbis_seek_start(oggfile); printf(" replaying music \n");    }
			if (offset >= numSamp) { offset -= numSamp; }


		}//endif

  }//update




};//xmusic




//there can be only one
static xSoundStream myMusic;


void xSoundMan::startMusic(std::string fname){  myMusic.loadMusic(fname);  }
void xSoundMan::updateMusic(void) { myMusic.update(); }
void xSoundMan::stopMusic(void) {  myMusic.stop(); }
void xSoundMan::pauseMusic(void) {  myMusic.pause();}
void xSoundMan::resumeMusic(void){  myMusic.resume();}

