
#include "xGLCommon.h"	

#include <stdio.h> //used for debugging
#include <math.h>



#include "xWin.h"
#include "xKey.h"



namespace xwin
{

	//static WNDCLASS  gWinClass;
	static HWND  gHwnd; //this is our window
	static HDC   gDeviceContext;
	static HGLRC gRenderContext;





	void xwinSleep(int msec)
	{
		if (msec < 0) { return;}
		Sleep(msec);
	}//xwinsleep

/*
	unsigned int xwinGetTime(void)
	{
		return GetTickCount();
	}//gettime
*/


//a more precise time measuring thing

    //http://www.angelcode.com/dev/timefps/timefps.html
    //http://speedrunsdev.blogspot.hu/2008/05/tutorial-3-timing-and-fps.html

    //float  timeAtGameStart;
    //__int64
    static UINT64 ticksPerSecond;

    unsigned int xwinGetTime(void)
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

      return (unsigned int) (time*1000);
    }//getgametime





	static int winWidth = 640;
	static int winHeight = 480;

	static int canvasWidth = 640;
	static int canvasHeight = 480;

  //static int desktopWidth = 1024;
  //static int desktopHeight = 768;

	int xwinGetWinWidth(void) { return winWidth; }
	int xwinGetWinHeight(void) { return winHeight; }

	int xwinGetCanvasWidth(void) { return canvasWidth; }
	int xwinGetCanvasHeight(void) { return canvasHeight; }

	//int xwinGetDesktopWidth(void) { return canvasWidth; }
	//int xwinGetDesktopHeight(void) { return canvasHeight; }


	static void windowResized(int w, int h)
	{
		printf("Resizewindow %d  %d \n", w, h);

		winWidth = w;
		winHeight = h;
		/*
		if (w < 320) { w = 320; }
		if (h < 240) { h = 240; }

		canvasWidth = w;
		canvasHeight = h;
		*/
		//todo -- store these values instead

		// glViewport(0, 0, w, h);
	}//rewin


	static bool bCursor = true;
	void xwinShowCursor(bool enable)
	{
		//http://msdn.microsoft.com/en-us/library/windows/desktop/ms648396%28v=vs.85%29.aspx
		//once again, microsoft did
		//just what the fuck

		//show cursor true adds 1 to visible

		if (enable == bCursor) { return; }
		bCursor = enable;
		ShowCursor(bCursor);
	}//xwinshowcursor



	static short mousex = 0;
	static short mousey = 0;

	short getMousex(void) { return mousex; }
	short getMousey(void) { return mousey; }

	float getUnivMousex(void) {  return  (float)mousex/(float)canvasWidth  ; }
	float getUnivMousey(void) {  return  (float)mousey/(float)canvasHeight ; }

  void setMouseCursorPos(int x, int y)
  {
      if (xwinIsMini()) { return; }

      SetCursorPos(x, y);
  }//setmousepos

  void limitMousePos(int minx, int miny, int maxx, int maxy)
  {
     if (xwinIsMini()) { return; }

    int x;    int y;
    int dx;   int dy;

    POINT mcoord;
		GetCursorPos(&mcoord);

  
    dx = mcoord.x;
    dy = mcoord.y;
   
     ScreenToClient(gHwnd, &mcoord); //Map to window coordinates

    x = mcoord.x;
    y = mcoord.y;
    dx -= x;
    dy -= y;
    

     //printf(" mousex mousey %d %d \n", mousex, mousey);

     if (x < minx) { x += (maxx-minx);}
     else if (x > maxx) { x -= (maxx-minx); }
     if (y < miny) { y += (maxy-miny);}
     else if (y > maxy) { y -= (maxy-miny); }

     SetCursorPos(x+dx, y+dy);
      
  }//limitmouse


	static void mouseMove(WPARAM wParam, LPARAM lParam)
	{
		mousex = (short)LOWORD(lParam);
		mousey = (short)HIWORD(lParam);
	}//mouseMove


	static void (*kdFunc)(short key) = 0;
	static void (*kuFunc)(short key) = 0;


	void setKeyDownFunc(void (*func)(short key) )
	{
		kdFunc = func;
	}//setkeydown


	void setKeyUpFunc(void (*func)(short key) )
	{
		kuFunc = func;
	}//setkeyup



	static void (*mdFunc)(int btn, int mx, int my) = 0;
	static void (*muFunc)(int btn, int mx, int my) = 0;


	void setMouseDownFunc(void (*func)(int btn, int mx, int my))
	{
		mdFunc = func;
	}//mousedown


	void setMouseUpFunc(void (*func)(int btn, int mx, int my))
	{
		muFunc = func;
	}//mouseup







	static void keyDown(WPARAM wParam, LPARAM lParam)
	{
		if (kdFunc == 0) {return;}

		kdFunc(wParam);
	}//kdown

	static void keyUp(WPARAM wParam, LPARAM lParam)
	{
		if (kuFunc == 0) {return;}

		kuFunc(wParam);
	}//kup



	static bool bFullScreen = false;

	bool xwinIsFullScreen(void) { return bFullScreen; }


	//source
	//http://cppkid.wordpress.com/2009/01/07/how-to-get-the-screen-resolution-in-pixels/
	// Get the horizontal and vertical screen sizes in pixel
	static void GetDesktopResolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		horizontal = desktop.right;
		vertical = desktop.bottom;
	}//getdesktop





	//http://msdn.microsoft.com/en-us/library/windows/desktop/ms633591%28v=vs.85%29.aspx

	//center -- place window on center of screen (ignores x and y)
	//caption -- set true to windowed mode, false for fullscreen mode
	static void placeWindow(HWND &hWnd, int x, int y, int w, int h, bool center = false, bool caption = true)
	{
		DWORD		dwsEx;
		DWORD		dws;
		RECT		WindowRect;				

		WindowRect.left=(long)0;			
		WindowRect.right=(long)w;
		WindowRect.top=(long)0;			
		WindowRect.bottom=(long)h;

		canvasWidth = w;
		canvasHeight = h;

		int screenw;
		int screenh;
		int pw;
		int ph;

		screenw = 640;
		screenh = 480;
		pw = 0;
		ph = 0;

		if (center)
		{
			GetDesktopResolution(screenw, screenh);

			pw = (screenw / 2) - (w / 2);
			ph = (screenh / 2) - (h /2);
			x = pw;
			y = ph;
		}//endif



		if (caption)
		{
			//windowed
			dws = WS_OVERLAPPEDWINDOW;
			dws |= WS_CAPTION;
			dws &= ~WS_THICKFRAME;
			//dws &= ~WS_MAXIMIZEBOX;
			dwsEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

			AdjustWindowRectEx(&WindowRect, dws, FALSE, dwsEx);

			dws = GetWindowLong(hWnd, GWL_STYLE);
			dws |= WS_CAPTION;
			dws &= ~WS_THICKFRAME;
			SetWindowLong(hWnd, GWL_STYLE, dws);
		}
		else
		{
			//fullscreen
			dws = WS_POPUP;
			dws &= ~WS_CAPTION;  
			dws &= ~WS_THICKFRAME;
			dwsEx = WS_EX_APPWINDOW; 

			AdjustWindowRectEx(&WindowRect, dws, FALSE, dwsEx);


			dws = GetWindowLong(hWnd, GWL_STYLE);
			dws &= ~WS_CAPTION;  
			dws &= ~WS_THICKFRAME;
			SetWindowLong(hWnd, GWL_STYLE, dws);

		}//endif


		//  SetWindowLong(hWnd, GWL_EXSTYLE, dwsEx);

		w = abs(WindowRect.right) + abs( WindowRect.left);
		h =  abs(WindowRect.bottom) + abs( WindowRect.top);

		printf("placeWindow %d %d \n", w, h);

		SetWindowPos(hWnd, 0, x, y, 
			w, 
			h,         
			0);
	}//placewindow


	//http://stackoverflow.com/questions/1400654/how-do-i-put-my-opengl-app-into-fullscreen-mode

	// http://www.gamedev.net/topic/536958-win32-api-fullscreen-mode/
	static void setFullScreen(HWND &hWnd, int w = 640, int h = 480)
	{



		if (bFullScreen)
		{ 
			if ( w == canvasWidth && h == canvasHeight) { return; } //already on this resolution
		}//endif

		// SetWindowPos(hWnd, 0, 50, 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		//SetWindowPos(hWnd, 0, 0, 0, winWidth, winHeight, 0);
		/*
		RECT		WindowRect;			
		WindowRect.left=(long)0;		
		WindowRect.right=(long)w;
		WindowRect.top=(long)0;			
		WindowRect.bottom=(long)h;

		DWORD		dwsEx;
		DWORD		dws;
		*/

		DEVMODE dm;	
		EnumDisplaySettings(0, 0, &dm); //get current
		dm.dmSize=sizeof(dm);
		dm.dmPelsWidth  = w	;
		dm.dmPelsHeight = h	;
		dm.dmBitsPerPel = 32;	
		dm.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		//http://msdn.microsoft.com/en-us/library/dd183411%28v=vs.85%29.aspx

		if ( ChangeDisplaySettings(&dm  ,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL ) { return; }  else
		{

			placeWindow(hWnd,0,0,w,h,false,false);

			//remove window title
			//DWORD
			//  dws =  GetWindowLong(hWnd, GWL_STYLE);
			//  dws &= ~WS_CAPTION;
			/*
			dws = WS_POPUP;
			dwsEx = WS_EX_APPWINDOW; // | WS_EX_WINDOWEDGE;	




			AdjustWindowRectEx(&WindowRect, dws, FALSE, dwsEx);


			dws =  GetWindowLong(hWnd, GWL_STYLE);
			dws &= ~WS_CAPTION;  
			dws &= ~WS_THICKFRAME;
			SetWindowLong(hWnd, GWL_STYLE, dws);
			//now it works fine but it wont resize the window
			//no matter what i do it doesnt give a fuck about window size
			//move to 0,0
			SetWindowPos(hWnd, 0, 0, 0,
			abs(WindowRect.right) + abs( WindowRect.left), 
			abs(WindowRect.bottom) + abs( WindowRect.top),  

			0);
			*/
			// InvalidateRect(hWnd, 0, true);

			bFullScreen = true;
		}//endif

		//SetForegroundWindow(hWnd);
		//SetFocus(hWnd);

	}//setfull

	
	static void resetScreen(void)
	{
		if (bFullScreen) 
		{
			ChangeDisplaySettings(NULL, CDS_FULLSCREEN);
		}//endif
	}//resetscreen


	//so this is actually setting it fullscreen.. seriously what the fuck
	static void setWindow(HWND &hWnd, int w = 640, int h = 480)
	{

		if (bFullScreen) 
		{
			// SetWindowPos(hWnd, 0, 13, 13, winWidth, winHeight, 0);

			//todo -- do a CDS_TEST and see if it possible
			if ( ChangeDisplaySettings(NULL, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) { xwinExit(); }
			placeWindow(hWnd,0,0,w,h,true, true);
		}
		else
		{
			if ( w == canvasWidth && h == canvasHeight) { return; } 

			placeWindow(hWnd,0,0,w,h,true, true);
		}//endif


		bFullScreen = false;


	}//setwin


	bool mDownLeft = false;
	bool mDownRight = false;

	//POINT nMouseCoordinates;

	static bool bMinimized = false;
	bool xwinIsMini(void) { return bMinimized;}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_ACTIVATE: 
			if ( (LOWORD(wParam) == WA_INACTIVE) || HIWORD(wParam) )
			{
				//minimized
				bMinimized = true;
				printf("INACTIVE \n");
				//ShowCursor(true);

				//not holding down mouse
				mDownLeft = false;
				mDownRight = false;

			}
			else
			{
				//no longer minimized
				bMinimized = false;
				// ShowCursor(false);
				printf("active \n");
			}//endif
			break;

		case WM_SYSCOMMAND: 

			break;

		case WM_DESTROY:
			//already everything destroyed
			break;

		case WM_CLOSE: 
			xwinExit();
			// exit(0); //seems to clean up everything, better use this
			//PostQuitMessage (0);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: 
      //TODO -- remember these commands were only used to test screen size switching

			if (wParam == VK_F1)
			{ 

				int screenw;
				int screenh;

				GetDesktopResolution(screenw, screenh);

				//  setFullScreen(hWnd);
				setFullScreen(hWnd, screenw, screenh);
				Sleep(50);
				// SetWindowPos(hWnd, 0, 50, 50, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}
			else
				if (wParam == VK_F2)
				{ 
					setWindow(hWnd);
					Sleep(50);
					//SetWindowPos(hWnd, 0, 13, 13, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				}
				else
					if (wParam == VK_F3)
					{ 
						setWindow(hWnd,800,600);
						Sleep(50);

					}
/*
					else	if (wParam == VK_ESCAPE)
						{
							//fix: change back to small screen before exiting
							setWindow(hWnd);
							Sleep(50);

							xwinExit();

						}//endif
*/
						//TODO
						//instead of changing resolution
						//resize window to desktop screen size on fullscreen mode 


						//todo --
						//DETECT   ALT-F4   ALT-ENTER   WINDOWS BUTTON

						keyDown(wParam, lParam);

						break;

		case WM_KEYUP:
		case WM_SYSKEYUP: 

			keyUp(wParam, lParam);
			break;

		case WM_MOUSEMOVE:
      POINT nMouseCoordinates;
			  GetCursorPos(&nMouseCoordinates); // Get mouse coords for GUI
			  ScreenToClient(hWnd, &nMouseCoordinates); //Map to window coordinates
			  mouseMove(wParam, lParam);
			break;

		case WM_RBUTTONDOWN:
			mDownRight = true;		
			if (mdFunc) { mdFunc(1, mousex, mousey); }
			break;

		case WM_RBUTTONUP:
			mDownRight = false;		
			if (muFunc) { muFunc(1, mousex, mousey); }
			break;

		case WM_LBUTTONDOWN:
			mDownLeft = true;
			if (mdFunc) { mdFunc(0, mousex, mousey); }
			break;

		case WM_LBUTTONUP:
			mDownLeft = false;	
			if (muFunc) { muFunc(0, mousex, mousey); }
			break;


		case WM_SIZE:   
			//don't allow resize
			//it seem to fuck things up (?)
			windowResized( LOWORD(lParam), HIWORD(lParam) );
			break;
		};//swend



		return DefWindowProc(hWnd, msg, wParam, lParam);
	}//windowproce



	//http://forums.codeguru.com/showthread.php?t=283606
	static bool makeWindow(HWND &hWnd, int ww_=640, int wh_=480)
	{


		//  HWND hWnd;
		DWORD dwExWindowStyle;
		DWORD dwWindowStyle;
		WNDCLASS wcWindowClass;


		wcWindowClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcWindowClass.lpfnWndProc   = WndProc;
		wcWindowClass.cbClsExtra    = 0;
		wcWindowClass.cbWndExtra    = 0;
		wcWindowClass.hInstance     = GetModuleHandle(NULL);
		wcWindowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wcWindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wcWindowClass.hbrBackground = NULL;
		wcWindowClass.lpszMenuName  = NULL;
		wcWindowClass.lpszClassName = "xwinWindow";

		try {    RegisterClass(&wcWindowClass); }
		catch (...) { return false; }

		dwExWindowStyle = WS_EX_APPWINDOW; // | WS_EX_WINDOWEDGE;
		dwWindowStyle = WS_OVERLAPPEDWINDOW; //| WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		dwWindowStyle &= ~WS_THICKFRAME;
		dwWindowStyle &= ~WS_MAXIMIZEBOX;

		//todo -- get rid of maximise button (?)
		//or make it do something else (?)
		RECT		WindowRect;			
		WindowRect.left=(long)0;			
		WindowRect.right=(long)ww_; //640;	
		WindowRect.top=(long)0;			
		WindowRect.bottom=(long)wh_;//480;



		///CENTER WINDOW
		int screenw;
		int screenh;
		int pw;
		int ph;

		screenw = ww_; //640;
		screenh = wh_; //480;
		pw = 0;
		ph = 0;

		GetDesktopResolution(screenw, screenh);

		//        pw = (screenw / 2) - (640 / 2);
		//        ph = (screenh / 2) - (480/2);
		pw = (screenw / 2) - (ww_ / 2);
		ph = (screenh / 2) - (wh_/2);


		//printf("screenwidht  %d %d  --  %d %d \n ", screenw, screenh, pw, ph);
		//
		//  WindowRect.left += pw;
		//  WindowRect.right += pw;
		//



		canvasWidth = ww_;
		canvasHeight = wh_;





		AdjustWindowRectEx(&WindowRect, dwWindowStyle, FALSE, dwExWindowStyle );

		printf("windowrect %d %d %d %d \n", WindowRect.left, WindowRect.right, WindowRect.top, WindowRect.bottom  );

		hWnd = CreateWindowEx(
			dwExWindowStyle, 
			"xwinWindow", 
			"Untitled", 
			dwWindowStyle,
			pw, ph, 
			abs(WindowRect.right) + abs( WindowRect.left), 
			abs(WindowRect.bottom) + abs( WindowRect.top), 
			HWND_DESKTOP, NULL, GetModuleHandle(NULL), NULL);

		// HWND_DESKTOP  is 0 so NULL is perfectly fine




		ShowWindow(hWnd, SW_SHOW);
		SetForegroundWindow(hWnd);
		SetFocus(hWnd);

		//Sleep(5000);


		return true;
	}//






	static bool setupOpenGL(HDC &hDeviceContext, HGLRC &hRenderContext, bool bMultSamp = false)
	{
		//set hDC

		// http://msdn.microsoft.com/en-us/library/dd368826%28v=vs.85%29.aspx

		//http://msdn.microsoft.com/en-us/library/dd144871%28v=vs.85%29.aspx

		//		HDC hDeviceContext  = GetDC(hWnd);

		//set pixel format


		//todo -- need different pixelformat for full screen multisampling
		//check out NEHE lesson 46

		/*
		float	fAttributes[] = {0,0};

		int iAttributes[] =
		{
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,24,
		WGL_ALPHA_BITS_ARB,8,
		WGL_DEPTH_BITS_ARB,16,
		WGL_STENCIL_BITS_ARB,0,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB,4,
		0,0
		};

		wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
		*/




		PIXELFORMATDESCRIPTOR pfd;

		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the structure

		pfd.nVersion = 1; //always 1

		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 

		pfd.iPixelType = PFD_TYPE_RGBA; //only 32bit is supported by xwin

		//needs at 

		//      cColorBits

		//  Specifies the number of color bitplanes in each color buffer. For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes. For color-index pixels, it is the size of the color-index buffer.


    //todo -- set these from arguments/parameters

		pfd.cColorBits = 24; //exclude 8 for alpha for 32bit -- so 24 is correct 

		pfd.cDepthBits = 24; 

		pfd.dwLayerMask = PFD_MAIN_PLANE; 

		pfd.cAccumBits = 0;

		pfd.cStencilBits = 8;


		int pixformat;

		pixformat = ChoosePixelFormat(hDeviceContext, &pfd);

		if (pixformat == FALSE) {	return false;	}

		if (SetPixelFormat(hDeviceContext, pixformat, &pfd) == FALSE) {  	return false;	}

		//hRC

		//HGLRC 
		hRenderContext = wglCreateContext( hDeviceContext );

		wglMakeCurrent( hDeviceContext, hRenderContext );


		//set gl hints
		//http://www.opengl.org/sdk/docs/man/xhtml/glHint.xml

		//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);


		glClearColor(1.0,1.0,1.0,0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 

		SwapBuffers(hDeviceContext);

		return true; //success

	}	//setopengl
	////////////////////////////////////////////


	static float dx = 0;

	void debugDraw(void)
	{

		dx += 0.1f;
		if (dx >= 1) { dx= -1;}

		//Viewport(0,0,200,200);
		glClearColor(1.0,1.0,1.0,0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, 640.0f/480.0f, 0.1, 300);
		gluLookAt(1,0,5, 0,0,0, 0,1,0);
		// glOrtho(1,-1,-1,1, -1,1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPointSize(3);

		glColor3f(0,0,0);
		glBegin(GL_POINTS);
		glVertex3f(dx, 0, 0);
		glEnd();

		glFlush();


		xwinSwapBuffer();
	}//debdraw







	static POINT getWinCenter(void)
	{
		POINT pt;
		pt.x = winWidth / 2;
		pt.y = winHeight / 2;
		ClientToScreen(gHwnd, &pt);
		//pt.x += winWidth / 2;
		//pt.y += winHeight / 2;
		return pt;
	}//getcneter



	void xwinKillWindow(void)
	{
		if (!gHwnd) { return; }
		//deleteWindow(gHwnd);
		xwinShowCursor(true);

		DestroyWindow(gHwnd);

		HINSTANCE	hInstance;
		hInstance	= GetModuleHandle(NULL);
		UnregisterClass("xwinWindow",hInstance);

	}//killwin

	void xwinKillContext()
	{  

		if (gDeviceContext) 
		{
			ReleaseDC(gHwnd, gDeviceContext);
			gDeviceContext=NULL;
		}//endif

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(gRenderContext);
		gDeviceContext=NULL; 


	}//killcont




	void xwinExit(void)
	{
		printf("xwinExit \n");

		xwinShowCursor(true);
		resetScreen();

		xwinKillContext();
		xwinKillWindow();


		//  PostQuitMessage (0);

		exit(0);
	}//exit



	void xwinUpdate(void)
	{
		//peek messages

		//prob .. only should update/render when there is no message received
		//otherwise we end up processing them one by one in 60 fps
		//other idea.. try to process a crazy amount of messages at once
		//i want to avoid making a seperate thread for this
		// (lets just hope no one will pres 4096 keys at once)

		//http://msdn.microsoft.com/en-us/library/windows/desktop/ms644943%28v=vs.85%29.aspx

		MSG  msg; 
		int i;

		for (i = 0; i < 4096; i++)
		{
			if (PeekMessage(&msg, gHwnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else 
			{return;}
		}//nexti
	}//update



	void xwinSetWindowSize(int width=640, int height=480, bool full=false)
	{

		// i dont remember why i commented these out
		///  resizeWindow(width, height);
		/*
		//todo -- need to place the window change size somewhere else
		if (full) 
		{
		setFullScreen(gHwnd);
		}
		else 
		{
		setWindow(gHwnd);
		}//endif

		Sleep(100); //wait a bit
		*/
	}//setwindow


	void xwinSwapBuffer(void)
	{
		SwapBuffers(gDeviceContext);
	}//swapbuffer


	void xwinError(const char * msg)
	{
		MessageBox(NULL, "Error",  msg , MB_ICONERROR);
	}//xwinerror



	void xwinSetWinTitle(const char * title)
	{
		SetWindowText(gHwnd, (LPCSTR) title);

	}//setwintitle



	/*

	GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
	{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
	ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
	ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
	if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
	{
	MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}

	if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
	{
	MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}
	hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
	MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
	MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
	MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	hInstance=NULL;									// Set hInstance To NULL
	}
	}
	*/



	int xwinInit(int w, int h)
	{

		//setup window

		if ( makeWindow(gHwnd, w, h) == false) { xwinError("Couldn't create window"); return 0;}

		gDeviceContext = GetDC(gHwnd);

		if ( setupOpenGL(gDeviceContext, gRenderContext) == false) { xwinError("Couldn't init OpenGL"); xwinKillWindow(); return 0;}



		return 1;
	}//init


};//xwin//namespaceend
