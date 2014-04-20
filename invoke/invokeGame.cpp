#pragma warning (disable : 4786)

#include "invokeGame.h"

#include "../gamex/xKey.h"
#include "../gamex/xTmx.h"


#include "xUnit.h"

#include "xTree.h"

#include "xPartTest.h"




invokeGame::invokeGame(void) 
{


  //init camera
  //myCam.moveForward(300);
  //myCam.strafeUp(150);

  myCam.pos.set(16*64, 1024, 16*64);

  camPos.set(16*64, 1024, 16*64);

//  myCam.ori.rotPitch(-0.8f);
  myCam.ori.rotPitch(-0.98f);

  //use the war3 angle by default

  //http://www.hiveworkshop.com/forums/general-mapping-tutorials-278/cameras-160369/
  //printf(" %0.2f \n ", (float)(56*(3.1415/180.0)) );


  myCam.neard = 1.0f;
  myCam.fard = 9000.0f;
  myCam.aspect = 640.0f/480.0f;
  myCam.fov = 70.0f;  //important


  curId = 0;

}//ctor


invokeGame::~invokeGame(void) 
{

}//dtor






void
invokeGame::init(void)
{

//todo -- load tilemap before world reset (so we know the maps size)
  resetWorld(64*128,64*128);

  //gameTime = 0;

  myRender.init(16384);


    myFont.loadCharDef("data/atari16.xfnt");
    myFontSkin.loadTex("data/atari16.png",true,true,true);
    myFontSkin.setTexEnv_Modulate();
    myFont.handle = myFontSkin.handle;

    myGui.init();
    myGui.pfont = &myFont;



 // myRand.setSeed(1); //important for gameplay


//load resources first (only do this once per game)
  myData.addMdx("data/tree.mdx3", "tree");
  myData.addSkin("data/treeskin.png", "treeskin",true,true);

 
  myData.addSkin("data/knight_skin.png", "knight_skin",true,true);
  
  myData.addSprite("data/mysmoke.xms","data/mysmoke.png");
  
  myData.addSprite("data/button.xms","data/button.png");
  

  xButton * b;
  b = myGui.addButton("btn1", "", 200, 610-36-36-36,380, 16, 32,32, getSprite("btn_move"), 1);

  b = myGui.addButton("btn2", "", 201, 610-36-36, 380, 16, 32,32, getSprite("btn_stop"), 1);

  b = myGui.addButton("btn3", "", 202, 610-36, 380, 16, 32,32, getSprite("btn_attack"), 0);

  //rest of buttons are just for alignment for now
  b = myGui.addButton("btn4", "", 1, 610, 380, 16, 32,32, getSprite("button64x64"), 0);


  b = myGui.addButton("btn5", "", 1, 610-36*3, 380+36, 16, 32,32, getSprite("button64x64"), 0);
  b = myGui.addButton("btn6", "", 1, 610-36*2, 380+36, 16, 32,32, getSprite("button64x64"), 0);
  b = myGui.addButton("btn7", "", 1, 610-36, 380+36, 16, 32,32, getSprite("button64x64"), 0);
  b = myGui.addButton("btn8", "", 1, 610, 380+36, 16, 32,32, getSprite("button64x64"), 0);



  b = myGui.addButton("btn9", "", 1, 610-36*3, 380+36+36, 16, 32,32, getSprite("button64x64"), 0);
  b = myGui.addButton("btn10", "", 1, 610-36*2, 380+36+36, 16, 32,32, getSprite("button64x64"), 0);
  b = myGui.addButton("btn11", "", 1, 610-36, 380+36+36, 16, 32,32, getSprite("button64x64"), 0);
  b = myGui.addButton("btn12", "", 1, 610, 380+36+36, 16, 32,32, getSprite("button64x64"), 0);


  //testing -- a button for the minimap (probably not the best way to go about this)



 b = myGui.addButton("btn_mini", "minimap", 500, 64+4, 480-64-8, 16, 128,128, getSprite("button64x64"), 1);
 b->drawMode = -1; //make button hidden but usable



//Create a tilemap and some units for testing

  //testing
  int i;
  xActor * a;
  for (i = 0; i < 16; i++)
  {
    a = new xUnit();
    a->pos.set(getRand()*1024,0, getRand()*1024);
      addActor(a);
  }//nexti


  xTile * tileSet;
  xTile * ta;
  xTmxLayer * layer;


  xTmx tmxLoader;
  tmxLoader.loadFile("data/testmap.tmx");

  layer = tmxLoader.getLayer("terra");
  myMap.initFromLayer(layer->vecGrid, layer->mwidth, layer->mheight);

    //rem: a 512x512 map is half million tris
  // myMap.initEmpty(128, 128);

   //myMap.initEmpty(32, 32);
    


/*
  tileSet = myMap.tileSet;

  ta = &(tileSet[0]);
  ta->height = 0;

  ta = &(tileSet[1]);
  ta->height = 0;
  ta->spec = 303;
 
  ta = &(tileSet[2]);
  ta->height = 1;

  ta = &(tileSet[3]);
  ta->height = 2;

  ta = &(tileSet[4]);
  ta->height = 3;
*/

  //set tile heights etc (todo -- load from xml?)
  tileSet = myMap.tileSet;
  ta = &(tileSet[0]);  ta->height = 0;  

  ta = &(tileSet[8]);  ta->height = 1;  
  ta = &(tileSet[8+1]);  ta->height = 2;  
  ta = &(tileSet[8+2]);  ta->height = 3;  
  ta = &(tileSet[8+3]);  ta->height = 4;  
  ta = &(tileSet[8+4]);  ta->height = 5;  
  ta = &(tileSet[8+5]);  ta->height = 6;  
  ta = &(tileSet[8+6]);  ta->height = 7;  
  ta = &(tileSet[8+7]);  ta->height = 8;  

  ta = &(tileSet[8+8]);  ta->height = 1;     ta->spec = 303;
  ta = &(tileSet[8+8+1]);  ta->height = 2;   ta->spec = 303;
  ta = &(tileSet[8+8+2]);  ta->height = 3;   ta->spec = 303;
  ta = &(tileSet[8+8+3]);  ta->height = 4;   ta->spec = 303;
  ta = &(tileSet[8+8+4]);  ta->height = 5;   ta->spec = 303;
  ta = &(tileSet[8+8+5]);  ta->height = 6;   ta->spec = 303;
  ta = &(tileSet[8+8+6]);  ta->height = 7;   ta->spec = 303;
  ta = &(tileSet[8+8+7]);  ta->height = 8;   ta->spec = 303;


  
  /*
  myMap.setRect(2,2, 10, 10, 4);
  myMap.setRect(1,1, 3, 3, 2);
  myMap.setRect(3,1, 3, 4, 3);
  myMap.setRect(4,4, 3, 3, 1);
  myMap.setRect(14,14, 9, 9, 4);
*/

  myMap.genHeightRect();
  //  myMap.debApplyHeightMap();

//todo -- maybe the extra wall rendering is not needed
//and instead it should be based on the heightmap?

  xImage hmap;
  hmap.loadImage("data/test_heightmap.png");
    myMap.applyHeightMap(&hmap, 4.0f);
  hmap.clear();


  //  xMdx3 temp;
      //ok so-- the thing is i chosen to use shorts for storing the number of faces possible in an mdx
      //and now it bite me in the ass as you cant have more than 16384 faces in a mesh
      //so you cant just init the collision mesh from taking the entire map
     // myMap.makeMesh(&temp, 0,0, myMap.mwidth, myMap.mheight);
      //myCol.initFromMesh(&temp);
     // temp.clear();


//todo -- the tileset atlas will need a larger edge around the tiles
  tileSkin.loadTex("data/out_set.png", true, false, false);
  //tileSkin.loadTex("data/out_set.png", false, false, false);

  //tileSkin.setTexEnv_Blend();
  //tileSkin.setTexEnv_Decal();
  myMap.debLoadUv("data/out_set.xms");

  layer = tmxLoader.getLayer("tex");
  layer->addNum(-65);
  myMap.setSkinFromLayer(layer->vecGrid, layer->mwidth, layer->mheight);

  



    myCol.clear();
    myCol.addTileMap(&myMap);
/*
    xMdx3 temp;
    myMap.makeMesh(&temp, 0,0, 32, 32);
    myCol.initFromMesh(&temp);
    temp.clear();
*/
    myDraw.initRect(&myMap);


    debmesh.initBox(16);
    //debmesh.initPlane(16);





  waterSkin.loadTex("data/watertex.png");
  waterDeb.initPlane(32768, 0, 0, 32,32);




//todo -- in the future load units after the map


  float wwidth, wheight;
  wwidth = myMap.mwidth * myMap.cw;
  wheight = myMap.mheight * myMap.ch;

//todo -- frustum cull units (and trees)

  for (i = 0; i < 256; i++)
  {
    a = new xTree(); 
     a->pos.set(getRand()*wwidth, 0, getRand()*wheight);
    addActor(a);

    //todo -- align trees to grid    
  }//nexti


  a = new xPartTest();
  a->pos.set(512,64,512);
    addActor(a);

    
    
    
    
  //minimap
  //todo -- set texture/image size based on map size
    myMini.init();
  
    myMini.updateImage(&myWorld);

}//init




void 
invokeGame::update(void)
{


  //cam speed
  float ms;
  ms = 32.0f;



  if (isKeyDown(KEY_W) || isKeyDown(KEY_UP)) { camPos.z -= ms;  } 
  if (isKeyDown(KEY_S) || isKeyDown(KEY_DOWN)) { camPos.z += ms;  }

  if (isKeyDown(KEY_A) || isKeyDown(KEY_LEFT)) { camPos.x -= ms; } 
  if (isKeyDown(KEY_D) || isKeyDown(KEY_RIGHT)) { camPos.x += ms; } 

  if (isKeyDown(KEY_R)) { camPos.y += ms; }
  if (isKeyDown(KEY_F)) { camPos.y -= ms; }


  //todo -- this needs some smoothing (adjusting?)
  myCam.pos = camPos;
  myCam.pos.y += myMap.getHeight(camPos.x, camPos.z);



  //transforming the universal mouse coordinates (0,1) to  (-1,1)
  float kx, ky;
  kx =  ((umx*2)-1)*-1;
  ky =  ((umy*2)-1)*-1;

  myCursor.update(&myCam, kx, ky, &myCol);
  //printf("mycursor %0.2f, %0.2f \n", umx, umy);


  myWorld.update();

  myGui.childUpdate(this);



  gameTime += 1;
}//update


void 
invokeGame::render(void) 
{
  //assuming zbuffer is cleared at this point

  gamex::cMat view;

  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);

	glMatrixMode(GL_MODELVIEW);

  view.setView(&myCam.pos, &myCam.ori);
    glLoadMatrixf(view.m);


  xFrustum frust;
      frust.setPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
	    frust.setPoints(myCam.pos, myCam.ori, 0, 0);





  myRender.resetFrame();
   myRender.setCam(myCam.pos, myCam.ori);
  

  //todo -- for actors do a query based on the frustum 
  //so instead of testing each one against the frustum
  //we just pick ones that are potentially inside it 
    myWorld.render2(&myRender);

    myDraw.render(&frust, &myRender, tileSkin.handle);


    
    xEnt * e;
    e = myRender.addFrame(0);
      e->pos = 0; //pos;
      e->pos.y = 200.0f + sinf( ((float)(gameTime % 314)) *0.01f)*16.0f;
      e->sortpos = e->pos;
      e->blend = 2; //0 solid   1 alpha test   2 transparent   3 additive
      e->fmesh = &waterDeb;
      e->vmesh = &waterDeb;
      e->alpha = 0.25;
      e->color = 1;
      e->twoSide = 1;
      e->skin = waterSkin.handle;
      e->useColor = 0; ///1 use color data of mesh or 0 for the e->color of xEnt
  


  myRender.render(true);


/*
  glColor3f(0,0,1);


    glPushMatrix();
      glTranslatef(0,200,0);
      waterDeb.render();
    glPopMatrix();

*/

  glColor3f(1,0,0);

  glPushMatrix();
    glTranslatef(myCursor.coord.x, myCursor.coord.y, myCursor.coord.z); 
       debmesh.render();
  glPopMatrix();


  glDisable(GL_DEPTH_TEST);
  glPushMatrix();
    glTranslatef(myCursor.coord.x, myCursor.coord.y+64, myCursor.coord.z); 
       debmesh.render();
  glPopMatrix();

 
 

  // glDisable(GL_TEXTURE_2D); debugDraw();

 

 // myGui.childRender(this);

 //2D drawing - gui and minimap etc
  
   glMatrixMode(GL_PROJECTION);  	glLoadIdentity();       glOrtho(0,640,480,0,-3000,3000); 
	 glMatrixMode(GL_MODELVIEW);	  glLoadIdentity();

   //as a flatrenderer doesnt store anything about the things its supposed to render
   //we might as well have a global/singleton one (?)
   xFlatRender * flat;
   flat = &(myGui.myFlat);


   flat->resetFrame();

   myGui.myWorld.frameRender(flat);
   myGui.cursor.frameRender(flat);    
      

  //render minimap
  //todo -- set update factor from some variable
    if ((gameTime % 30) == 0)  { myMini.updateImage(&myWorld); }

    //rem -- coordinates are the middle of the rectangle
    xFrame * f;
     f = flat->addFrame(64+4, 480-64-8, 300, 128,128, myMini.skin.handle);
     flat->render(true);
   

   glDisable(GL_TEXTURE_2D); myGui.debugDraw(); 

   upCursor();

}//render




static void drawRect(float cx, float cy, float cw, float ch)
{
  glBegin(GL_LINES);
    glVertex2f(cx,cy);       glVertex2f(cx+cw,cy);
    glVertex2f(cx,cy+ch);    glVertex2f(cx+cw,cy+ch);
    glVertex2f(cx,cy);       glVertex2f(cx,cy+ch);
    glVertex2f(cx+cw,cy);    glVertex2f(cx+cw,cy+ch);
  glEnd();
}//drawcube












static int get2DCoord(float * view, float * proj, gamex::cVec3f * pos, float * retx, float * rety)
{
	float ta[4];
	float tb[4];

	ta[0] = pos->x;
	ta[1] = pos->y;
	ta[2] = pos->z;
	ta[3] = 1.0f;

	tb[0] = ta[0] * view[0] + ta[1] * view[4] + ta[2] * view[8] + ta[3] * view[12];
	tb[1] = ta[0] * view[1] + ta[1] * view[5] + ta[2] * view[9] + ta[3] * view[13];
	tb[2] = ta[0] * view[2] + ta[1] * view[6] + ta[2] * view[10] + ta[3] * view[14];
	tb[3] = ta[0] * view[3] + ta[1] * view[7] + ta[2] * view[11] + ta[3] * view[15];

	ta[0] = tb[0] * proj[0] + tb[1] * proj[4] + tb[2] * proj[8] + tb[3] * proj[12];
	ta[1] = tb[0] * proj[1] + tb[1] * proj[5] + tb[2] * proj[9] + tb[3] * proj[13];
	ta[2] = tb[0] * proj[2] + tb[1] * proj[6] + tb[2] * proj[10] + tb[3] * proj[14];
	ta[3] = tb[0] * proj[3] + tb[1] * proj[7] + tb[2] * proj[11] + tb[3] * proj[15];

	if (ta[3] == 0.0f) return(0);
	ta[0] /= ta[3];
	ta[1] /= ta[3];
	//    ta[2] /= ta[3];
	/* Map x, y and z to range 0-1 */
	ta[0] = ta[0] * 0.5 + 0.5;
	ta[1] = ta[1] * 0.5 + 0.5;
	//    ta[2] = in[2] * 0.5 + 0.5;

	ta[0] = ((ta[0]) *2) -1;
	ta[1] = ((ta[1]) *2) -1;

	*retx = ta[0];
	*rety = ta[1];


	return (1);
}//get2d






void 
invokeGame::upCursor(void)
{
  //selecting units
  //for now in a brute-force way (going through all and projecting them on screen)

  static int se = 0; // selection started
  static float selx = 0;
  static float sely = 0;

  if (mDownLeft)
  {  
    if (se == 0)
    {
      se = 1;
      selx = mx;
      sely = my;
    }
    else
    {
      glColor3f(0,1,0);
      drawRect(selx,sely, mx-selx, my-sely);
    }
  }else { se = 0; }


  gamex::cMat proj;
  gamex::cMat view;

  proj.setPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
  view.setView(&myCam.pos, &myCam.ori);

  float sx, sy;
  sx = 0;
  sy = 0;

  get2DCoord(view.m, proj.m, &(myCursor.coord), &sx, &sy);

  //todo -- this will need some work
  drawRect(sx*320+320, 480-(sy*240+240), 4,4);



/*
   mat = projview;
            
    vx = cx;            vy = cy;            vz = cz;
            
    sx = vx * mat[0] + vy * mat[4] + vz * mat[8] + mat[12];
    sy = vx * mat[1] + vy * mat[5] + vz * mat[9] + mat[13];
    w = vx * mat[3] + vy * mat[7] + vz * mat[11] + mat[15];
     sx /= w;
     sy /= w;

  var rendw:Number; var rendh:Number;
     rendw = 320;
     rendh = 240;
     
       sx *= rendw;            sy *= -rendh;
       sx += rendw;			sy += rendh;


      mx = (1/w) * 240 * size;

*/

}//upcursor







float 
invokeGame::getHeight(float wx, float wz) 
{
 return myMap.getHeight(wx, wz);
}//getheight





xCam * 
invokeGame::getCamPtr(void)
{
  return &myCam;
}//getcamptr





//todo -- getanim, getskel



xMdx3 *
invokeGame::getMdx(std::string wname)
{
  return myData.getMdx(wname);
}//getmdx

xTexture *
invokeGame::getTex(std::string wname) 
{ 
  return myData.getTex(wname);
}//gettex  


unsigned int 
invokeGame::getSkin(std::string wname) 
{
 return myData.getSkin(wname);
}//getskin



xSprite * 
invokeGame::getSprite(std::string wname) 
{
 return myData.getSprite(wname);
}//getsprite





void 
invokeGame::gotCmd(int cmd, int arg0, int arg1)
{

 printf("invokegame gotcmd %d   %d %d \n", cmd, arg0, arg1);


 //for now the minimap movement is hacked together like this (should be refined in the future)
 if (cmd == 500)
 {
    camPos.x = arg0 * 64;
    camPos.z = arg1 * 64;
 }//endif



}//gotcmd







