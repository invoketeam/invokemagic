#pragma warning (disable : 4786)

#include "invokeGame.h"

#include "xInvokeCommon.h"


#include "../gamex/xKey.h"
#include "../gamex/xTmx.h"


#include "xUnit.h"

#include "xTree.h"

#include "xPartTest.h"

#include "xBuildTest.h"

#include "../gamex/xMultiGrid.h"
#include "../gamex/xCell.h"

#include "../gamex/xDebug.h"









//todo -- need a better name for this one
// dynamic texture?
// dynamic skin?


class xShadowTex
{
public:
  int mw, mh;
  unsigned int handle; //handle of color texture
  unsigned int depth; //handle of depth texture
  unsigned int fbo; //frambuffer

public:
  xShadowTex(void);
  ~xShadowTex(void);

  void clear(void);
  void init(int size);


};//shadowtex


xShadowTex::xShadowTex(void)
{
  mw = 0; mh = 0; handle = 0; fbo = 0; depth = 0;
}//ctor

xShadowTex::~xShadowTex(void)
{
  clear();
}//dtor

void 
xShadowTex::clear(void)
{
  if (handle != 0) { glDeleteTextures(1, &handle); handle = 0; }
  if (depth != 0) { glDeleteTextures(1, &depth); depth = 0; }
  if (fbo != 0) { glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); glDeleteFramebuffersEXT(1, &fbo); fbo=0;}
  mw = 0; mh = 0;
}//clear

void 
xShadowTex::init(int size)
{
  clear();

  float clampColor[] = { 0, 0, 0, 0 };

//  size = 512;
    size = 1024;

  //todo: check if size is power of 2
  mw = size;
  mh = size;


//COLOR TEXTURE
  glGenTextures(1, &handle);
	  glBindTexture(GL_TEXTURE_2D, handle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);	   
	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

 //DEPTH TEXTURE
   glGenTextures(1, &depth);
	  glBindTexture(GL_TEXTURE_2D, depth);

    
    
      //note -- this is not classic shadowing   
      //as instead of drawing the scene twice the shadows are just drawn in black afterwards everythings drawn 
       glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_ALPHA);
     // glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
      //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_GEQUAL);
       //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);

   
	   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
       glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    //depth component is default depth of the scene
      glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

   glGenFramebuffersEXT(1, &fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
     glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, handle, 0);
     glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depth, 0);

//todo
//https://www.opengl.org/wiki/Framebuffer_Object_Examples
// glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT   

//remember to unbind after rendering too
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}//init




















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
//far distance: too much and the zbuffer starts screwing up, too little and we wont see far
  myCam.fard = 7000.0f; //9000.0f;
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
  resetWorld(256*128,256*128);

  //gameTime = 0; //done in resetworld

  myRender.init(16384);


  myHand.viewBoxPtr = &viewBox;
  myHand.game = this;
  myHand.init();


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
     b->btnMode = 1; //make button react when you hold down the button on it





//Create a tilemap and some units for testing




  
  //todo -- read size from tmx map
  myHeight.cw = 128;
  myHeight.ch = 128;
  myHeight.initEmpty(128, 128);
  
  myHeight.loadTileSet("data/out_set.xms");
  tileSkin.loadTex("data/out_set.png", true, false);
  tileSkin.setMaxLod(4); //     edge*0.5

  xImage hmap;
       hmap.loadImage("data/test_heightmap.png");
          myHeight.loadHeight(&hmap, 4.0f);

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
  //myData.addSkin("data/knight_skin.png", "knight_skin",true,true);
  myData.addSkin("data/knight_skin2.png", "knight_skin",true,true);
  
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
  a->pos.set(512+2512,64,512+2512);
    addActor(a);

    
    
    
  //todo -- make minimap part of xHand (needs to be used to send units after all)

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


  //todo use as parameter
  float maxHeight;
  float minHeight;

  maxHeight = 2000.0f;
  minHeight = 500.0f;

  if (camPos.y > maxHeight) { camPos.y = maxHeight; }
  if (camPos.y < minHeight) { camPos.y = minHeight; }
  //also check map limits


//todo -- move this fransformation into xCursor
  //transforming the universal mouse coordinates (0,1) to  (-1,1)
  float kx, ky;
  kx =  ((umx*2)-1)*-1;
  ky =  ((umy*2)-1)*-1;

//  myCursor.update(&myCam, kx, ky, &myCol);

   myCam.pos = camPos;
   myCam.pos.y += myHeight.getHeight(camPos.x, camPos.z);

  myCursor.updateHmap(&myCam, kx, ky, &myHeight, 0.0f);
  wmx = myCursor.coord.x;
  wmy = myCursor.coord.y;
  wmz = myCursor.coord.z;


  myWorld.update();

  myGui.childUpdate(this);


    xFrustum frust;
      frust.setPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
	    frust.setPoints(myCam.pos, myCam.ori, 0, 0);
      viewBox.genBox(&frust, 0,0,0, 0, 1, 0);

  if ((gameTime % 30) == 0)  { myMini.updateImage(&myWorld); }

  gameTime += 1;
}//update




inline static void drawActor(xRender * rend, xMultiGrid * mgrid,   float ax, float az, float aw, float ah)
{
	xCell * c;	xActor * a;	
  float x0, x1, z0, z1; //rectangle on grid
 
  x0 = ax;  x1 = ax + aw;  z0 = az;  z1 = az + ah;

	c = mgrid->doQuery(x0, z0, aw, ah);
			
	for (c; c != 0; c= c->next )
	{
		for (a = c->first; a != 0; a = a->next)
		{
			if (a->dead) { continue; }
      if (a->visible == false) { continue; }

      	if (a->pos.x + a->xrad < x0) { continue; } 
				if (a->pos.x - a->xrad > x1) { continue; }
				if (a->pos.z + a->zrad < z0) { continue; }
				if (a->pos.z - a->zrad > z1) { continue; }      

       a->render2(rend);

		}//nexta
	}//nextc


}//drawactor






void 
invokeGame::render(void) 
{
  //assuming zbuffer is cleared at this point



//update camera based on camera pos
  myCam.pos = camPos;
  myCam.pos.y += myHeight.getHeight(camPos.x, camPos.z);


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

        myRender.setCam(myCam.pos, myCam.ori); //needed for sorting transparent entities
  
               //todo -- for actors do a query based on the frustum 
                //so instead of testing each one against the frustum
                //we just pick ones that are potentially inside it 
              //    myWorld.render2(&myRender);

              drawActor(&myRender, mgrid, viewBox.min.x,  viewBox.min.z,  viewBox.size.x,viewBox.size.z);

                

              xEnt * e;


                e = myRender.addFrame(0); 
                  e->pos = 0;
                  e->skin = tileSkin.handle;   e->skinBlend = 4; //0 replace   1 blend  2 decal  3 modulate  4 experiment
                  //e->skin2 = tileSkin.handle;  e->skin2Blend = 2;
                  e->fmesh = &(myHeight.mesh);
                  e->vmesh = &(myHeight.mesh);
                  e->useColor = 1;
                  e->blend = 0;
                  e->color = 1;
                  e->twoSide = 0;
                  e->flags = 4; //receive shadow



              float waterHeight; //todo -- make this a member variable (or add it as part of the tilemap)
              waterHeight = 50.0f;
    
               
                  e = myRender.addFrame(1);
                    e->pos = 0; //pos;
                    e->pos.y = waterHeight + sinf( ((float)(gameTime % 314)) *0.01f)*16.0f;
                    e->pos.x = 8192.0f; e->pos.z = 8192.0f;
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
                    e->flags = 1;//draw if no flags are specified (dont cast or receive shadow)
              //todo -- water is problematic as it will need to be drawn AFTER the shadows are drawn
              // (this is true for the full second bucket)




  static xShadowTex shadTex;

  if (shadTex.handle == 0) { shadTex.init(1024); }



  float lightProjectionMatrix[16];
  float lightViewMatrix[16];

  glPushMatrix();
    glLoadIdentity();
      //glOrtho(-512, 0, -512, 512, -9000, 9000);
      //glOrtho(-viewBox.size.x, viewBox.size.x, -viewBox.size.z, viewBox.size.z, -9000, 9000);
     // glOrtho(-viewBox.size.x*0.6f, viewBox.size.x*0.6f, -viewBox.size.z*0.5f, viewBox.size.z*0.5f, -9000, 9000);
      //glOrtho(-viewBox.size.z*0.5f, viewBox.size.z*0.5f, -viewBox.size.x*0.6f, viewBox.size.x*0.6f, -9000, 9000);
 
      glOrtho(-viewBox.size.x*0.5f, 0, -viewBox.size.z*0.6f, viewBox.size.z*0.6f, -4000, 4000);

        glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

    glLoadIdentity();
      //glRotatef(45, 0, 1, 0);  //looks good but is buggy for now
	    glRotatef(70, 1, 0, 0); //sun angle (90 is exactly down) (so far 90 works without bugs) (also try 45 and 135)
	    glRotatef(90, 0, 1, 0);
       glTranslatef(myCam.pos.x *-1 , 0, myCam.pos.z *-1 ); //todo -- needs tweaks
        glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);  glLoadMatrixf(lightProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);  glLoadMatrixf(lightViewMatrix);
 

 // glViewport(0, 0, shadTex.mw,  shadTex.mh);
  glViewport(1, 1, shadTex.mw-2,  shadTex.mh-2); // "Hang back, buddy, and observe my magic"

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, shadTex.fbo);
   
glEnable(GL_POLYGON_OFFSET_FILL);
glPolygonOffset(1.0f, 4.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
      glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    	glClearColor(0, 0, 0, 0); //  i tend to disable it to adjust the persepective matrix a bit
      glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);	 
      glDisable(GL_TEXTURE_2D);
      glColor4f(1,1,1,1);   //render shadows in white so we can recolor them later
         myRender.simpRender(2);
glDisable(GL_POLYGON_OFFSET_FILL);




//restore viewport
  glViewport(view_x, view_y, view_width,  view_height);

  glMatrixMode(GL_PROJECTION); glLoadIdentity();  gluPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
	glMatrixMode(GL_MODELVIEW);  view.setView(&myCam.pos, &myCam.ori);   glLoadMatrixf(view.m);
 
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
       myRender.render(true);
       //myRender.renderBucket0(); 


//todo -- instead of extra pass use the 3rd texture channel

 glEnable(GL_TEXTURE_2D);
//  glBindTexture(GL_TEXTURE_2D, shadTex.handle);
  glBindTexture(GL_TEXTURE_2D, shadTex.depth);

  float row0[4];float row1[4];float row2[4];float row3[4];
  float texm[16];
  float biasMatrix[16] = {0.5f, 0.0f, 0.0f, 0.0f,
								  0.0f, 0.5f, 0.0f, 0.0f,
								  0.0f, 0.0f, 0.5f, 0.0f,
                  0.5f, 0.5f, 0.5f, 1.0f};
  float temp[16];

  gamex::cMat::multMatrix(biasMatrix, lightProjectionMatrix, temp);
  gamex::cMat::multMatrix(temp, lightViewMatrix, texm);


  row0[0] = texm[0];  row0[1] = texm[4];  row0[2] = texm[8];   row0[3] = texm[12];
  row1[0] = texm[1];  row1[1] = texm[5];  row1[2] = texm[9];   row1[3] = texm[13];
  row2[0] = texm[2];  row2[1] = texm[6];  row2[2] = texm[10];   row2[3] = texm[14];
  row3[0] = texm[3];  row3[1] = texm[7];  row3[2] = texm[11];   row3[3] = texm[15];

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, row0);
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, row1);
  glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, row2);
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, row3);
	glEnable(GL_TEXTURE_GEN_Q);

   //glColor4f(1,1,1, 0.9f);
   glColor4f(0,0,0, 0.5f);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL); 

   glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

 //glEnable(GL_POLYGON_OFFSET_FILL);
 //glPolygonOffset(0.2f, 0.2f);
   //glEnable(GL_ALPHA_TEST);
 //  glAlphaFunc(GL_LEQUAL, 0.5f);
  //  glAlphaFunc(GL_GEQUAL, 0.99f);

       myRender.simpRender(4);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_GEN_Q);
  glDepthFunc(GL_LESS);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_POLYGON_OFFSET_FILL);

/*
  glMatrixMode(GL_PROJECTION); glLoadIdentity();  gluPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
	glMatrixMode(GL_MODELVIEW);  view.setView(&myCam.pos, &myCam.ori);   glLoadMatrixf(view.m);
  //render transparent
    myRender.renderBucket1(); 
*/





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

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  // testSelect.debRender(this);
  myHand.mySelect.debRender(this);
 
      //debug -- draw aabb around units
   //     glEnable(GL_DEPTH_TEST);
   //        glDisable(GL_TEXTURE_2D); debugDraw();

 

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
      


        //note -- minimap is updated in update, because frameskipping makes it lose frames if updated here
        //render minimap
        //todo -- set update factor from some variable
        //  if ((gameTime % 30) == 0)  { myMini.updateImage(&myWorld); }

          //rem -- coordinates are the middle of the rectangle
          xFrame * f;
           f = flat->addFrame(64+4, 480-64-8, 300, 128,128, myMini.skin.handle);
           f = flat->addFrame( 64+8,64+8, 350, 128,128, shadTex.handle);
           //f = flat->addFrame( 64+8,64+8, 350, 128,128, shadTex.depth);


         flat->render(true);
   

          //draw outline for buttons
         glDisable(GL_TEXTURE_2D); myGui.debugDraw(); 

         //debug -- update gui buttons (move this to update)
          upCursor();
 

}//render















void 
invokeGame::upCursor(void)
{

  myHand.update();


/*
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
  }
  else 
  { 
    if (se == 1)
    {
      if (isKeyDown(KEY_SHIFT) == false)
      {testSelect.resetSelect(); }
      
     
        //testSelect.selectOver(&myCam, mgrid, 0,0, 10000, 10000, selx<mx?selx:mx,sely<my?sely:my, abs(mx-selx), abs(my-sely));
        
        testSelect.appendOverToSelect();

    }//endif

    selx = mx;
    sely = my;  
    se = 0; 
  }//mdownleft


  //printf("clickright  %d  %d  \n ", mClickRight, gameTime);
  //because of debugging upcursor is called in render
  //so right click test will need to be altered later (aka fixed)
  if (mClickRight >= (gameTime-2) )
  {
  
    printf("clickright %d \n ", gameTime);
    //check which unit the cursor is over
    //to figure out what command to send

    //todo -- build formation out of the units

    //(still need to test if they attempt to reach something where another unit is standing)

    testSelect.sendMsg(this, MSG_MOVE, wmx, wmz, 0);
  
  }//endif
*/



/*
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
*/
/*
 // if (testSelect.maxSel <= 0) { testSelect.init(); }

  //todo -- optimisation
  //set viewbox, use viewbox for the size to use on mgrid
  //testSelect.selectOver(&myCam, mgrid, 0,0, 10000, 10000, selx<mx?selx:mx,sely<my?sely:my, abs(mx-selx), abs(my-sely));
  testSelect.selectOver(&myCam, mgrid, 0,0, myHeight.mw*myHeight.cw, myHeight.mh*myHeight.ch, selx<mx?selx:mx,sely<my?sely:my, abs(mx-selx), abs(my-sely));

*/

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


//todo -- indeed we are moving the camera with the minimap
//but dont exactly match the position on screen

 //for now the minimap movement is hacked together like this (should be refined in the future)
 if (cmd == 500)
 {
    camPos.x = arg0 * myHeight.cw ;
    camPos.z = arg1 * myHeight.ch ;
    lastPos = -1; //force update

 }//endif



}//gotcmd







