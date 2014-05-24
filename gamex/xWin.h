
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



extern void debugDraw(); //do something in opengl
extern int xwinInit(int w=640, int h=480); //init (testing only, doesn't set screen yet)
extern void xwinUpdate(); //process messages (keyboard, mouse, window close etc)


extern void xwinSwapBuffer();  //swap opengl buffer
extern void xwinExit();  //destroy window and opengl context
extern void xwinError(const char * msg); //make error message box with msg


extern void xwinSleep(int msec);
extern unsigned int xwinGetTime();

extern bool xwinIsMini(); //rename to isactive or something
extern bool xwinIsFullScreen();


extern void xwinShowCursor(bool enable);

extern short getMousex(); //gets actual window mouse coordinate 
extern short getMousey();

extern float getUnivMousex(); //gets mouse coordinate in percentage between (0.0f - 1.0f)
extern float getUnivMousey();

//extern POINT getWinCenter(void);


extern void xwinSetWinTitle(const char * title);

extern int xwinGetWinWidth(); 
extern int xwinGetWinHeight(); 
extern int xwinGetCanvasWidth(); 
extern int xwinGetCanvasHeight();

};//xwin