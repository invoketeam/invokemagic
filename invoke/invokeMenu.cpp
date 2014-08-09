#pragma warning (disable : 4786)

#include "invokeMenu.h"

#include "../gamex/xAsset.h"






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
  if (assetMan != 0) { delete assetMan; assetMan = 0; } 
 
}//dtor


void 
invokeMenu::init(void)
{

  resetWorld(640, 480);


  //set asset directories (aka. look for resources here)
  if (assetMan == 0)
  {
    assetMan = new xAssetMan(); 
    assetMan->addDir("data",0);
    assetMan->addDir("data/build",1);
    assetMan->addDir("data/gui",0);

  }//endif




   myFlat.init();

      myFont.loadCharDef("data/atari16.xfnt");
      //myFontSkin.loadTex("data/atari16.png",true,true,true);
      assetMan->initTexture("atari16", true,true,true);
      //myFont.handle = myFontSkin.handle;
      myFont.handle = assetMan->getTexture("atari16")->handle;

    

//  myData.addSkin("data/test.png","test",true);
//  myData.addSkin("data/menu_placeholder.jpg","placeholder",true,true);
   // assetMan->initTexture("menu_placeholder", true,true,true);
    assetMan->initTexture("bg", true,true,true);



    myGui.assetMan = assetMan;
    myGui.init();
    myGui.pfont = &myFont;

    //testGame.assetMan = assetMan; //important (invokeMenu is the main game class, others share the assets)
    //testGame.init();

    mySprite.assetMan = assetMan;
    mySprite.addSpriteTex("test"); 
    
    mySprite.addSpriteTex("gomb"); 
    mySprite.addSpriteXms("gomb"); 



    xButton * b;

    float btnx;
    float btny;
    float btndist;

    btnx = 32+72+16;
    btny = 180;
    btndist = 52;
   
    b = myGui.addButton("btn1", "New Game", 101,  btnx, btny, 0,  144, 48, getSprite("gombkicsialap"),0); 
    b->curSpr = getSprite("gombkicsialap");

    b = myGui.addButton("btn2", "Load Game", 102,  btnx, btny+btndist, 0,  144, 48, getSprite("gombkicsialap"),0); 
    b->curSpr = getSprite("gombkicsialap");
   
    b = myGui.addButton("btn3", "Settings", 103,  btnx, btny+btndist*2, 0,  144, 48, getSprite("gombkicsialap"),0);   
    b->curSpr = getSprite("gombkicsialap");

    b = myGui.addButton("btn4", "Credits", 104,  btnx, btny+btndist*3, 0,  144, 48, getSprite("gombkicsialap"),0);   
    b->curSpr = getSprite("gombkicsialap");

    b = myGui.addButton("btn5", "Exit Game", 105,  btnx, btny+btndist*4, 0,  144, 48, getSprite("gombkicsialap"),0);   
    b->curSpr = getSprite("gombkicsialap");



    b = myGui.addButton("logo", "", 105,  120, 80, 0,  413*0.5, 264*0.5, getSprite("logo_kicsi"),0);   
    b->curSpr = getSprite("logo-kicsi");
    b->spectype = 0; //dont consider as a button

    myGui.cursor.curSpr = getSprite("test"); 
    myGui.cursor.pos.z = 1024;



   
     mySnd.init();

     mySnd.startMusic("data/mus/RTSmenu.ogg");

      
    mState = STATE_STARTUP;

}//init







void 
invokeMenu::update(void)
{

  //debug -- set starting state
  if (mState == STATE_STARTUP)
  {
    mState = STATE_MAINMENU;
  }//endif



  if (mState == STATE_MAINMENU)
  {


    if (isKeyPress(KEY_ESCAPE)) { mState = STATE_GAME; } //debug
  }
  else if (mState == STATE_GAME)
  {

    //first run? -- load up game
    if (testGame.gameState < 0) 
    {
      testGame.assetMan = assetMan;
      testGame.init();

      mySnd.startMusic("data/mus/RTSSSSS.ogg");

      return;
    }//endif


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



//todo -- setstate method -- disable buttons based on layers
  if (mState != STATE_GAME)
  {
    myWorld.update();

    myGui.childUpdate(this);
  }

  
  mySnd.updateMusic();
  


 gameTime += 1;

}//update


void 
invokeMenu::render(void) 
{

 if (mState == STATE_GAME)
 {
   if (testGame.gameState < 0) { return; } //game not loaded yet
   testGame.render();
   return;
 }//endif


     glMatrixMode(GL_PROJECTION);  	glLoadIdentity();       glOrtho(0,640,480,0,-3000,3000); 
	   glMatrixMode(GL_MODELVIEW);	  glLoadIdentity();


      myFlat.resetFrame();

        //myFlat.addFrame(0,0,0, 64,64, 0);

    
      //just a quick and dirty way to get the menu placeholder on screen
      xFrame * f;    
      f = myFlat.addFrame(320,240,0,640,480,  assetMan->getTexHandle("bg") );
      f->u0 = 0.0f;
      //f->u1 = 1.0f;
      f->u1 = (1680.0f / 2048.0f);
      f->v0 = 1.0f; //1.0f;//-(448.0f / 1024.0f);//1.0f;// 625f; //1.0f- (576.0f / 1024.0f);
      //f->v1 = (448.0f / 1024.0f);
      f->v1 = (1024.0f / 2048.0f);


      myWorld.frameRender(&myFlat);    

      myFlat.render(true);


      myGui.childRender(this);


    glDisable(GL_TEXTURE_2D);

    debugDraw();

 //   myGui.debugDraw();


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
     return mySprite.getSprite(wname);    
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
















