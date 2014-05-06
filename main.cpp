#pragma warning (disable : 4786)
#pragma warning (disable : 4996) //fopen


#include <math.h>

#include "gamex/xWin.h"

#include "gamex/gamex.h"
#include "gamex/xGLCommon.h"
#include "gamex/xWin.h"
#include "gamex/xKey.h"







#include "invoke/invokeGame.h"
#include "invoke/invokeMenu.h"

#include "gamex/xDebug.h"

//invokeGame testGame;

invokeMenu testGame;





xFont debFont;
xTexture debFontSkin;



//run afer opengl is loaded (but before game is started)
void init(void)
{
 
  //todo - store this font and texture inside the exe(?)
  debFont.loadCharDef("data/atari16.xfnt");
  debFontSkin.loadTex("data/atari16.png",true,true,true);

  testGame.init();

}//init






//http://www.angelcode.com/dev/timefps/timefps.html
//http://speedrunsdev.blogspot.hu/2008/05/tutorial-3-timing-and-fps.html

float  timeAtGameStart;
UINT64 ticksPerSecond;

unsigned int getGameTime(void)
{
  UINT64 ticks;
  float time;

 

  static bool first = true;
  if (first)
  {
    if( !QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond) )  {  ticksPerSecond = 1000; }
    first = false;
  }//endif
  if( !QueryPerformanceCounter((LARGE_INTEGER *)&ticks) ) {  ticks = (UINT64)GetTickCount(); }

  time = (float)(__int64)ticks/(float)(__int64)ticksPerSecond;

 // printf("Ticks: %0.2f \n ",((float)(__int64)ticks) );
 // printf("Tickspersec: %0.2f \n ",(float)(__int64)ticksPerSecond );
 //  printf("Time: %0.2f   \n", time*1000   );   
 //  printf("gtic: %d \n",GetTickCount());   

  return (unsigned int) (time*1000);
  //return time;
}//getgametime








//variables to check if game should sleep
unsigned int diff = 0;
unsigned int startTime = 0;

//variables for frame skipping
int frame = 0;
int skipFrame = 0;
float skipAvg = 0.0f;
float prevSkip = 0.0f;
float frate = 0.0f;
int fastCount = 0;
int skipn = 0;


//variables for fps measuring
double curfps = 0.0f;
int tick = 0;
double prevTimer  = 0.0f;
double curTimer = 0.0f;

//http://www.flashandmath.com/howtos/memory/
void measureFps(void)
{
  tick += 1;
  curTimer = getGameTime(); // GetTickCount();
  if (curTimer - prevTimer >= 1000)
  {
    curfps = floor((tick * 1000 / (curTimer - prevTimer ))+0.5f);
    //curfps = (tick * 1000 / (curTimer - prevTimer ));
    prevTimer = curTimer;
    tick = 0;
  }//endif

}//measure







void update(void)
{ 

  testGame.setMousePos(xwin::getUnivMousex(), xwin::getUnivMousey() );

  testGame.update();

}//update





void render(void)
{

  //todo -- scaling for widescreen
  testGame.setViewPort(0, 0, xwin::xwinGetCanvasWidth(),  xwin::xwinGetCanvasHeight()); 

  glViewport(0, 0, xwin::xwinGetCanvasWidth(),  xwin::xwinGetCanvasHeight());
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 
 

    testGame.render();

//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //debugtext
     glMatrixMode(GL_PROJECTION);  	glLoadIdentity();       glOrtho(0,640,480,0,-1,1); 
	   glMatrixMode(GL_MODELVIEW);	  glLoadIdentity();
     glEnable(GL_BLEND);    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glDisable(GL_CULL_FACE);
     glDisable(GL_DEPTH_TEST);
    
     glEnable(GL_TEXTURE_2D);
     glColor4f(1,1,1,1);
     glBindTexture(GL_TEXTURE_2D, debFontSkin.handle);

      //drawRect(0,0,320,240);
/*
      debFont.printStr(16, 0, 0, "Testing ");
      debFont.printStr(32,0,32, "0123456789:;<=>?\n@ABCDEFGHIJKLMNO\nPQRSTUVWXYZ#####\n#abcdefghijklmno\npqrstuvwxyz#####");

      debFont.printStr(16, 640-16, 480-16, "X");


      glColor4f(1,0,0,1);
      debFont.printStr(16, 0, 240, "Testing ");
      glColor4f(0,1,0,1);
      debFont.printStr(16, 0, 240+16, "Testing ");
      glColor4f(0,0,1,1);
      debFont.printStr(16, 0, 240+32, "Testing ");
      glColor4f(1,1,1,0.5);
      debFont.printStr(16, 0, 240+32+16, "Testing ");
      glColor4f(1,1,1,0.25);
      debFont.printStr(16, 0, 240+32+32, "Testing ");
*/

      glColor4f(1,1,1,1);
      debFont.printStr(16, 8, 480-32, "Fps: %0.1f ", curfps);
      debFont.printStr(8, 8+256, 480-32, "Skipframe: %d   Diffavg: %0.1f    Diff: %d", skipFrame, frate, diff);
  

     glDisable(GL_TEXTURE_2D);

//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glFinish();
  glFlush();

  xwin::xwinSwapBuffer(); 
}//render




//todo -- put savecreenshot in own file

#define MINIZ_HEADER_FILE_ONLY
#define MINIZ_NO_STDIO
#define MINIZ_NO_TIME
#include "gamex\stb\miniz.h"
#include "gamex\xImage.h"


static void writePng(xImage * img, std::string fname)
{
  if (img->dat == 0) { return; }
  size_t size;
  void * buf;
  FILE * file;

 // img->endianSwap();
  buf = tdefl_write_image_to_png_file_in_memory(img->dat, img->mw, img->mh, 4, &size);
  //img->endianSwap();

    file = fopen(fname.c_str(), "wb");
    fwrite(buf, 1, size, file);
    fclose(file);
 
  mz_free(buf);
}//writepng



static void saveScreenshot_test(std::string fname)
{
  xImage shotImage;

  shotImage.init(xwin::xwinGetCanvasWidth(), xwin::xwinGetCanvasHeight() );

  glReadPixels(0,0, shotImage.mw, shotImage.mh, GL_RGBA, GL_UNSIGNED_BYTE, shotImage.dat);

  shotImage.mirVert();

  printf("Writing screenshot [%d][%d][%s]   \n", shotImage.mw, shotImage.mh, fname.c_str() );
  
  writePng(&shotImage, fname);
}//savescreen





void keyDown(short k)
{
	testGame.setKeyDown(k);

  if (k == KEY_F8) { saveScreenshot_test("test_01.png");} 

}//kdown

void keyUp(short k)
{
  testGame.setKeyUp(k);
}//kup

void mouseDown(int btn, int x, int y)
{
  testGame.setMouseDown(btn);
}//mdown

void mouseUp(int btn, int x, int y)
{
	testGame.setMouseUp(btn);
}//mup 








int main(int argc, char**argv)
{

  //todo -- set screen size from config
	xwin::xwinInit(800, 600);
  
  //LOAD EXTENSIONS
  extOgl::getOglExtensions();
  
  //it's actually hard to find a video card nowadays that doesn't support these
  //but better safe than sorry
    if (extOgl::extVbo == false) { printf("Error: VBO not supported"); return 1; }
    if (extOgl::extMultiTex == false) { printf("Error: MultiTexture not supported"); return 1; }



	xwin::xwinSetWinTitle("untitled ");

	xwin::setKeyDownFunc(keyDown);
	xwin::setKeyUpFunc(keyUp);
	xwin::setMouseDownFunc(mouseDown);
	xwin::setMouseUpFunc(mouseUp);





	diff = 0;
	startTime = 0;

  init();


  bool bRunGame; bRunGame = true;

	while (bRunGame)
	{

		//startTime = GetTickCount();
    startTime = getGameTime();

		xwin::xwinUpdate();

		if (xwin::xwinIsMini() == false)
		{

			update();

      

      //skip rendering of every other frame when rendering is slow

      if (frame == 0)
      {
        skipn += 1;
        skipAvg += diff;
        frate = skipAvg / skipn;
        if (skipn >= 120) { skipn = 1; skipAvg = frate; }
//        skipFrame =  ceil( frate / (( 16.0f)/2.0f));  //16 = 1000/60  (60fps)
        skipFrame = (int) ceil( frate / ( 16.0f) ); 
        if (skipFrame > 6) { skipFrame = 6; }
        if (skipFrame < 1) { skipFrame = 1; }
        //seems we dont have to do the crazy things needed in flash to keep framerate consistent
        //if (skipFrame > prevSkip) { prevSkip = skipFrame; fastCount = 0; }
        //else  {  fastCount += 1;  if (fastCount >= 80) { prevSkip = skipFrame; fastCount = 0; } }
        //skipFrame = prevSkip;
      }//endif

      frame += 1;
      if (frame >= skipFrame)  { frame = 0; }
			if (frame == 0) {   render();	}


		}
		else 
		{ 
			//xwin::xwinShowCursor(true);  
		}


	//	diff = GetTickCount() - startTime ;
  	diff = getGameTime() - startTime ;
  

		//if (diff < 0)  {diff = 0; }
		if (diff < 16) { Sleep(16 - diff); }

  
    measureFps();

    if (testGame.gameState == -999) { bRunGame = false; break; }

	}//wend



	return 0;
}//main 