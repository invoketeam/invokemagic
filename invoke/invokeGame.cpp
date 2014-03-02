#pragma warning (disable : 4786)

#include "invokeGame.h"

#include "../gamex/xKey.h"

//#include <GL/GLUT.h>

#include "xUnit.h"

#include "xTree.h"

#include "xPartTest.h"


invokeGame::invokeGame(void) 
{


  //init camera
  myCam.moveForward(300);
  myCam.strafeUp(150);

  myCam.pos.set(16*64,256, 16*64);

  myCam.neard = 1.0f;
  myCam.fard = 5000.0f;
  myCam.aspect = 640.0f/480.0f;
  myCam.fov = 70.0f;  //important


  curId = 0;

}//ctor


invokeGame::~invokeGame(void) 
{

}//dtor





//todo -- these methods about storing data should be in their own class in the future

//stores auto handle duplicates (but don't warn about them yet (todo)
void
invokeGame::addMdx(std::string fname, std::string altName)
{
  xMdx3 * a;
  a = new xMdx3();
  a->loadFile(fname);
  if (altName != "0") { fname = altName;  }
  else { fname = stripName(fname); }
  printf("addmdx %s \n" ,fname.c_str());

  storeMesh.addData( fname , a);
}//addanim


void
invokeGame::addSkin(std::string fname, std::string altName, bool mip)
{
  xTexture * a;
  a = new xTexture();
  a->loadTex(fname, mip);
  a->setTexEnv_Replace();
  if (altName != "0") { fname = altName;  }
  else { fname = stripName(fname); }

  storeSkin.addData(fname, a);

   printf("addSkin %s \n" ,fname.c_str());

  //todo -- for each texture make a sprite with its own name
}//addskin






void
invokeGame::init(void)
{

  myRender.init(16384);

  myRand.setSeed(1); //important for gameplay


//load resources first (only do this once per game)
  addMdx("data/tree.mdx3");
  addSkin("data/treeskin.png");

 
  addSkin("data/knight_skin.png");
  

  myMan.addSkin("data/mysmoke.png");
  myMan.addSprite("data/mysmoke.xms");







 // testSkin.loadTex("data/knight_skin.png");

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


}//init





void 
invokeGame::update(void)
{

  float ms;
  ms = 64;

  if (isKeyDown(KEY_W)) { myCam.moveForward(-ms); } 
  if (isKeyDown(KEY_S)) { myCam.moveForward(ms); }

  if (isKeyDown(KEY_A)) { myCam.strafeRight(-ms); } 
  if (isKeyDown(KEY_D)) { myCam.strafeRight(ms); } 

  if (isKeyDown(KEY_R)) { myCam.strafeUp(ms); }
  if (isKeyDown(KEY_F)) { myCam.strafeUp(-ms); }

  float umx, umy;

  umx =  ((mx*2)-1)*-1;
  umy =  ((my*2)-1)*-1;

//  myCursor.update(&myCam, umx, umy, 0);
  myCursor.update(&myCam, umx, umy, &myCol);


  myWorld.update();

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
  /*
  glPointSize(8);
  glBegin(GL_POINTS);
	glVertex3f(myCursor.coord.x, myCursor.coord.y, myCursor.coord.z); 
  glEnd();
*/
  glPushMatrix();
    glTranslatef(myCursor.coord.x, myCursor.coord.y, myCursor.coord.z); 
       debmesh.render();
  glPopMatrix();

    glDisable(GL_DEPTH_TEST);
  glPushMatrix();
    glTranslatef(myCursor.coord.x, myCursor.coord.y+64, myCursor.coord.z); 
       debmesh.render();
  glPopMatrix();

  /*
  glColor3f(1,0,0);
  glPushMatrix();
    glTranslatef(myCursor.coord.x, myCursor.coord.y, myCursor.coord.z); 
        glutSolidCube(8);
        glutWireCube(16);

  glPopMatrix();
  */


  // printf("invokegame -- render \n");
}//render






float 
invokeGame::getHeight(float wx, float wz) 
{
 return myMap.getHeight(wx, wz);
}//getheight





void
invokeGame::addActor(xActor * a) 
{
  //if a doesnt exist -> crash

  a->game = this; //important
  curId += 1;
  a->id = curId;
  myWorld.addActor(a);
  a->init();
}//addactor


xActor * 
invokeGame::getActor(int id) 
{ 

  return myWorld.getActor(id);
}//getactor



xCam * 
invokeGame::getCamPtr(void)
{
  return &myCam;
}//getcamptr





//todo -- getanim, getskel



xMdx3 *
invokeGame::getMdx(std::string wname)
{
  return storeMesh.getData(wname);
}//getmdx

xTexture *
invokeGame::getTex(std::string wname) 
{ return 0; }  


unsigned int 
invokeGame::getSkin(std::string wname) 
{
 xTexture * a; 
 a = storeSkin.getData(wname);
 if (a == 0) { printf("not found %s \n", wname.c_str());  return 0;}
 return a->handle;
}//getskin



xSprite * 
invokeGame::getSprite(std::string wname) 
{
 return myMan.getSprite(wname);
}//getsprite





float 
invokeGame::getRand()
 {
  return myRand.getRand();
 }//getrand

	
float
 invokeGame::getRand2()
 {
   return (myRand.getRand() - 0.5f);
 }//getrand2
