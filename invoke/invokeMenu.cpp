#pragma warning (disable : 4786)

#include "invokeMenu.h"



#define STATE_MAINMENU 0
#define STATE_GAME 1
#define STATE_LOADMENU 2
#define STATE_SETTINGS 4
#define STATE_PAUSEMENU 5


#define CMD_NEWGAME 101
#define CMD_LOADGAME 102
#define CMD_SETTINGS 103
#define CMD_CREDITS 104
#define CMD_EXIT 105




invokeMenu::invokeMenu(void) 
{
  
  mState = 0;

  mState = STATE_GAME; //debug

}//ctor

 
invokeMenu::~invokeMenu(void) 
{

}//dtor


void 
invokeMenu::init(void)
{


   myFlat.init();

      myFont.loadCharDef("data/atari16.xfnt");
      myFontSkin.loadTex("data/atari16.png",true,true,true);
      //myFontSkin.setTexEnv_Modulate();
      myFont.handle = myFontSkin.handle;
    

    myData.addSkin("data/test.png","test",true);
    myData.addSkin("data/menu_placeholder.jpg","placeholder",true,true);


    myGui.init();
    myGui.pfont = &myFont;

    testGame.init();



    xButton * b;
   
    b = myGui.addButton("btn1", "New Game", 101,  465, 126, 0,  128, 32, getSprite("test"),0); 
    b->curSpr = getSprite("test");

    b = myGui.addButton("btn2", "Load Game", 102,  512, 212, 0,  128, 32, getSprite("test"),0); 
    b->curSpr = getSprite("test");
   
    b = myGui.addButton("btn3", "Settings", 103,  516, 300, 0,  128, 32, getSprite("test"),0);   
    b->curSpr = getSprite("test");

    b = myGui.addButton("btn4", "Credits", 104,  480, 370, 0,  128, 32, getSprite("test"),0);   
    b->curSpr = getSprite("test");

    b = myGui.addButton("btn5", "Exit Game", 105,  400, 440, 0,  128, 32, getSprite("test"),0);   
    b->curSpr = getSprite("test");


    myGui.cursor.curSpr = getSprite("test"); 

}//init







void 
invokeMenu::update(void)
{

//todo -- setstate method -- disable buttons based on layers
  if (mState != STATE_GAME)
  {
    myWorld.update();
    myGui.childUpdate(this);
  }


  if (mState == STATE_MAINMENU)
  {


    if (isKeyPress(KEY_ESCAPE)) { mState = STATE_GAME; } //debug
  }
  else if (mState == STATE_GAME)
  {

   //this will do for testing at least (final will probably have just some sort of function hiding most of this)
   //note -- this is is problematic as bKeyPress is dependent on gametime (so this will have to change)(exclamation mark)

   testGame.update();

    if (isKeyPress(KEY_ESCAPE)) { mState = STATE_MAINMENU; } //todo use pausemenu
  
  }
  else if (mState == STATE_LOADMENU)
  {

  }
  else if (mState == STATE_SETTINGS)
  {

  }
  else if (mState == STATE_PAUSEMENU)
  {

  }

  
   gameTime += 1;

}//update


void 
invokeMenu::render(void) 
{

 if (mState == STATE_GAME)
 {
   testGame.render();
   return;
 }//endif


     glMatrixMode(GL_PROJECTION);  	glLoadIdentity();       glOrtho(0,640,480,0,-3000,3000); 
	   glMatrixMode(GL_MODELVIEW);	  glLoadIdentity();


      myFlat.resetFrame();

        //myFlat.addFrame(0,0,0, 64,64, 0);

    
      //just a quick and dirty way to get the menu placeholder on screen
      xFrame * f;    
      f = myFlat.addFrame(320,240,0,640,480, myData.getSkin("placeholder") );
      f->u0 = 0.0f;
      f->u1 = 1.0f;
      f->v0 = 1.0f; //1.0f;//-(448.0f / 1024.0f);//1.0f;// 625f; //1.0f- (576.0f / 1024.0f);
      f->v1 = (448.0f / 1024.0f);



      myWorld.frameRender(&myFlat);    

     
      myFlat.render(true);


      myGui.childRender(this);


    glDisable(GL_TEXTURE_2D);

    debugDraw();

    myGui.debugDraw();
/*
    glColor4f(1,1,1,1);
     glBegin(GL_LINES);
        glVertex2f(mx,0);
        glVertex2f(mx,480);

        glVertex2f(0,my);
        glVertex2f(640,my);
     glEnd();
*/
}//render




void 
invokeMenu::gotCmd(int cmd, int arg0, int arg1)
  {
    //printf("got command [%d]  -  [%d][%d] \n", cmd, arg0, arg1);

    switch (cmd)
    {
      case CMD_NEWGAME: 
        mState = STATE_GAME;
      break;
      
      case CMD_LOADGAME:
      break;
      
      case CMD_SETTINGS:
      break;
      
      case CMD_CREDITS: 
      break;
      
      case CMD_EXIT: 
        gameState = -999;    
        //exit(0); //might be not a good idea (crashes on debug) -- needs some testing (low priority)
      break;

    }//swend

  }//gotcmd
  
 

xSprite * 
invokeMenu::getSprite(std::string wname) 
  {
    return myData.getSprite(wname);    
  }//getsprite












void 
invokeMenu::setKeyDown(short k)
{
 xGame::setKeyDown(k);

 testGame.setKeyDown(k);


}//kdown
  
void 
invokeMenu::setKeyUp(short k)
{ 
 xGame::setKeyUp(k);

  testGame.setKeyUp(k);
}//kup




void 
invokeMenu::setMouseDown(int btn)
{
  xGame::setMouseDown(btn);

  testGame.setMouseDown(btn);
}//mdown

void 
invokeMenu::setMouseUp(int btn)
{
  xGame::setMouseUp(btn);
 
  testGame.setMouseUp(btn);
}//mup



void 
invokeMenu::setMousePos(float ax, float ay)
{
  xGame::setMousePos(ax, ay);

  testGame.setMousePos(ax, ay);

}//setmousepos


void 
invokeMenu::setViewPort(int x, int y, int w, int h)
{
  xGame::setViewPort(x, y, w, h);

  testGame.setViewPort(x,y,w,h);

}//viewport
















