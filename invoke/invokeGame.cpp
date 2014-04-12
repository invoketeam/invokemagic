#pragma warning (disable : 4786)

#include "invokeGame.h"

#include "../gamex/xKey.h"


#include "xUnit.h"

#include "xTree.h"

#include "xPartTest.h"


invokeGame::invokeGame(void) 
{


  //init camera
  //myCam.moveForward(300);
  //myCam.strafeUp(150);

  myCam.pos.set(16*64, 1024, 16*64);

  myCam.ori.rotPitch(-0.8f);


  myCam.neard = 1.0f;
  myCam.fard = 5000.0f;
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

  gameTime = 0;

  myRender.init(16384);


  myGui.init();

/*
    myGui.addButton("test", 320+240,240,0, "TEST", "test", 1);
    myGui.addButton("test2", 320+240,240+40,0, "TEST2", "test2", 1);
    myGui.addButton("test3", 320+240,240+80,0, "TEST3", "test3", 1);
    myGui.addButton("test4", 320+240,240+120,0, "TEST4", "test4", 1);
*/

  myRand.setSeed(1); //important for gameplay


//load resources first (only do this once per game)
  myData.addMdx("data/tree.mdx3", "tree");
  myData.addSkin("data/treeskin.png", "treeskin",true,true);

 
  myData.addSkin("data/knight_skin.png", "knight_skin",true,true);
  

  myData.addSprite("data/mysmoke.xms","data/mysmoke.png");

  


  //testing
  int i;
  xActor * a;
  for (i = 0; i < 16; i++)
  {
    a = new xUnit();
    a->pos.set(getRand()*1024,0, getRand()*1024);
      addActor(a);
  }//nexti



    //rem: a 512x512 map is half million tris
    myMap.initEmpty(128, 128);
    
  xTile * tileSet;
  xTile * ta;

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
  


 myMap.setRect(2,2, 10, 10, 4);


  myMap.setRect(1,1, 3, 3, 2);

  myMap.setRect(3,1, 3, 4, 3);

  myMap.setRect(4,4, 3, 3, 1);


  myMap.setRect(14,14, 9, 9, 4);



    myMap.genHeightRect();
    myMap.debApplyHeightMap();

  //  xMdx3 temp;
      //ok so-- the thing is i chosen to use shorts for storing the number of faces possible in an mdx
      //and now it bite me in the ass as you cant have more than 16384 faces in a mesh
      //so you cant just init the collision mesh from taking the entire map
     // myMap.makeMesh(&temp, 0,0, myMap.mwidth, myMap.mheight);
      //myCol.initFromMesh(&temp);
     // temp.clear();





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

  float ms;
  ms = 64;

/*
  if (isKeyDown(KEY_W)) { myCam.moveForward(-ms); } 
  if (isKeyDown(KEY_S)) { myCam.moveForward(ms); }

  if (isKeyDown(KEY_A)) { myCam.strafeRight(-ms); } 
  if (isKeyDown(KEY_D)) { myCam.strafeRight(ms); } 

  if (isKeyDown(KEY_R)) { myCam.strafeUp(ms); }
  if (isKeyDown(KEY_F)) { myCam.strafeUp(-ms); }
*/

  if (isKeyDown(KEY_W)) { myCam.pos.z -= ms;  } 
  if (isKeyDown(KEY_S)) { myCam.pos.z += ms;  }

  if (isKeyDown(KEY_A)) { myCam.pos.x -= ms; } 
  if (isKeyDown(KEY_D)) { myCam.pos.x += ms; } 

  if (isKeyDown(KEY_R)) { myCam.pos.y += ms; }
  if (isKeyDown(KEY_F)) { myCam.pos.y -= ms; }



//todo -- move a point instead and set the camera position according o that


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

  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    myCam.camLookAt();

     // glutWireCube(32);

 // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //todo -- render tilemap with xRender
  xFrustum frust;
      frust.setPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
	    frust.setPoints(myCam.pos, myCam.ori, 0, 0);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glLineWidth(1);
    glColor3f(0,0,1);
     myDraw.render(&frust);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0,0,0);
     myDraw.render(&frust);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_LESS);
/*
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
glColor3f(1,1,1);
myCol.render();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
*/
  myRender.resetFrame();
  myRender.setCam(myCam.pos, myCam.ori);
  

  //todo -- for actors do a query based on the frustum 
  //so instead of testing each one against the frustum
  //we just pick ones that are potentially inside it 
    myWorld.render2(&myRender);

  myRender.render(true);

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
     f = flat->addFrame(64, 64, 300, 128,128, myMini.skin.handle);
     flat->render(true);

}//render






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


