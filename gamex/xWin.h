
#pragma once



namespace xwin
{

  //mouse buttons 
  extern bool mDownLeft;
  extern bool mDownRight;
  //extern POINT nMouseCoordinates;

  extern void setKeyDownFunc(void (*func)(short key) );
  extern void setKeyUpFunc(void (*func)(short key) );

  extern void setMouseDownFunc(void (*func)(int btn, int mx, int my));
  extern void setMouseUpFunc(void (*func)(int btn, int mx, int my));




  extern int xwinInit(int w=640, int h=480); //init (testing only, doesn't set screen yet)
  extern void xwinUpdate(void); //process messages (keyboard, mouse, window close etc)

  extern void xwinSleep(int msec);
  extern unsigned int xwinGetTime(void);





  extern void debugDraw(void); //do something in opengl
  extern void xwinSwapBuffer(void);  //swap opengl buffer




  extern void xwinExit(void);  //destroy window and opengl context
  extern void xwinError(const char * msg); //make error message box with msg



  extern bool xwinIsMini(void); //rename to isactive or something
  extern bool xwinIsFullScreen(void);






  extern void xwinShowCursor(bool enable);

  extern int getMousex(void); //gets actual window mouse coordinate 
  extern int getMousey(void);

  extern float getUnivMousex(void); //gets mouse coordinate in percentage between (0.0f - 1.0f)
  extern float getUnivMousey(void);

  
  extern void setMouseCursorPos(int x, int y);

  //used for mouselook (call every frame, get mouse speed in xspeed and yspeed)
  extern void wrapMousePos(int minx, int miny, int maxx, int maxy, int * xspeed=0, int * yspeed=0);
  



  extern void xwinSetWinTitle(const char * title);

  extern int xwinGetWinWidth(void); 
  extern int xwinGetWinHeight(void); 

  extern int xwinGetCanvasWidth(void); 
  extern int xwinGetCanvasHeight(void);

};//xwin




