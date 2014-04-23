#pragma warning (disable : 4786)

#include "invokeGame.h"

#include "../gamex/xKey.h"
#include "../gamex/xTmx.h"


#include "xUnit.h"

#include "xTree.h"

#include "xPartTest.h"

#include "xBuildTest.h"




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
    //myFontSkin.setTexEnv_Modulate();
    myFont.handle = myFontSkin.handle;

    myGui.init();
    myGui.pfont = &myFont;



 // myRand.setSeed(1); //important for gameplay




//setup ingame gui

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





  myHeight.cw = 128;
  myHeight.ch = 128;
  myHeight.initEmpty(128, 128);
  
  myHeight.loadTileSet("data/out_set.xms");
  tileSkin.loadTex("data/out_set.png", true, false);
  tileSkin.setMaxLod(4); //     edge*0.5

  xImage hmap;
       hmap.loadImage("data/test_heightmap.png");
          myHeight.loadHeight(&hmap, 1.0f);

       hmap.loadImage("data/colormap.png");
       hmap.endianSwap();
           myHeight.loadColor(&hmap);


    

  xTmx tmx;
  xTmxLayer * layer;
   tmx.loadFile("data/testmap2.tmx");

    layer = tmx.getLayer("tex");
      layer->addNum(-1); //correct tilemap
        myHeight.loadLayer(layer->vecGrid, layer->mwidth, layer->mheight);

    layer = tmx.getLayer("top");
     layer->addNum(-1); //correct tilemap
       myHeight.loadTopLayer(layer->vecGrid, layer->mwidth, layer->mheight);
    

   myHeight.setBufferMesh(128*128);
    myHeight.updateMesh(0,0, 2048,2048);








    




//preload all unit models and textures
//todo -- this part will need to use an xml or something

  xMdx3 * mesh;

  mesh = myData.addMdx("data/build/kunyho.mdx3", "kunyho");  mesh->makeVbo();
  myData.addSkin("data/build/kunyho_textura_walpha.png", "kunyho", true, true);


  mesh = myData.addMdx("data/tree.mdx3", "tree"); mesh->makeVbo();
  myData.addSkin("data/treeskin.png", "treeskin",true,true);

 

  mesh = new xMdx3();
  mesh->initPlane(64.0f);
  myData.addMdxFromPtr(mesh, "shadow");  mesh->makeVbo();
  myData.addSkin("data/shadlarge.png", "shadow", true,true);






//data used for debug
  myData.addSkin("data/knight_skin.png", "knight_skin",true,true);
  
  myData.addSprite("data/mysmoke.xms","data/mysmoke.png");
  


//mesh used for debugging
  debmesh.initBox(16);
 

//a big plane used for the water
  waterSkin.loadTex("data/watertex.png");
  waterDeb.initPlane(32768, 0, 0, 32,32);











//todo -- in the future load units after the map


  float wwidth, wheight;
  wwidth = myHeight.mw * myHeight.cw;
  wheight = myHeight.mh * myHeight.ch;



  //testing
  int i;
  xActor * a;
  for (i = 0; i < 16; i++)
  {
    a = new xUnit();
    a->pos.set(getRand()*1024,0, getRand()*1024);
      addActor(a);
  }//nexti



//todo -- frustum cull units (and trees)

  for (i = 0; i < 256; i++)
  {
    a = new xTree(); 
     a->pos.set(getRand()*wwidth, 0, getRand()*wheight);
    addActor(a);

    //todo -- align trees to grid    
  }//nexti


  for (i = 0; i < 16; i++)
  {
    a = new xBuildTest(); 
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
  myCam.pos.y += myHeight.getHeight(camPos.x, camPos.z);



  //transforming the universal mouse coordinates (0,1) to  (-1,1)
  float kx, ky;
  kx =  ((umx*2)-1)*-1;
  ky =  ((umy*2)-1)*-1;

//  myCursor.update(&myCam, kx, ky, &myCol);
  myCursor.updateHmap(&myCam, kx, ky, &myHeight, 0.0f);

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


 
     





//check if camera moved enough that we need to update the heightmap

    xFrustum frust;
    gamex::cVec3f dist;
    float mag;
    float diff;
      dist = lastPos - camPos;
    //diff = 128.0f;
      diff = myHeight.cw * 2.0f;
      mag = dist.getMag();

        if (mag >= diff)
        {
          lastPos = camPos;

           diff *= 2.0f;

           frust.setPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
	         frust.setPoints(myCam.pos, myCam.ori, 0, 0);

          //todo -- make viewbox use the camera as parameter(?)          
            viewBox.genBox(&frust, 0,0,0, 0, 1, 0);

         
           //todo -- only update if box changes considerably
            //no need to check sides for difference as it seems to naturally be overestimated
            myHeight.updateMesh(viewBox.min.x, viewBox.min.z-diff, (viewBox.max.x-viewBox.min.x), (viewBox.max.z-viewBox.min.z)+diff*2.0f);

          //printf("updatemesh %d %d \n", gameTime, myHeight.mesh.drawFace);
        }//endif
      







//3D rendering part

        myRender.resetFrame();
         myRender.setCam(myCam.pos, myCam.ori);
  
               //todo -- for actors do a query based on the frustum 
                //so instead of testing each one against the frustum
                //we just pick ones that are potentially inside it 
                  myWorld.render2(&myRender);

                

              xEnt * e;


                e = myRender.addFrame(0); 
                  e->pos = 0;
                  e->skin = tileSkin.handle;   e->skinBlend = 0; //0 replace   1 blend  2 decal  3 modulate  4 experiment
                  e->skin2 = 0; //mySkin2.handle; e->skin2Blend = 1;
                  e->fmesh = &(myHeight.mesh);
                  e->vmesh = &(myHeight.mesh);
                  e->useColor = 1;
                  e->blend = 0;
                  e->color = 1;
                  e->twoSide = 1;



              float waterHeight; //todo -- make this a member variable (or add it as part of the tilemap)
              waterHeight = 50.0f;
    
               
                  e = myRender.addFrame(1);
                    e->pos = 0; //pos;
                    e->pos.y = waterHeight + sinf( ((float)(gameTime % 314)) *0.01f)*16.0f;
                    e->sortpos = e->pos;
                   
                    e->fmesh = &waterDeb;
                    e->vmesh = &waterDeb;
                    e->alpha = 0.25;
                    e->color = 1;
                    e->twoSide = 1;
                    e->skin = waterSkin.handle;
                    e->blend = 2; //0 solid   1 alpha test   2 transparent   3 additive
                    e->skinBlend = 3; //0 replace   1 blend  2 decal  3 modulate
                    e->useColor = 0; ///1 use color data of mesh or 0 for the e->color of xEnt
  


        myRender.render(true);



glDisable(GL_TEXTURE_2D);
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

 
 
      //debug -- draw aabb around units
        glEnable(GL_DEPTH_TEST);
           glDisable(GL_TEXTURE_2D); debugDraw();

 

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
 return myHeight.getHeight(wx, wz);
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
    camPos.x = arg0 * myHeight.cw;
    camPos.z = arg1 * myHeight.ch;
 }//endif



}//gotcmd







