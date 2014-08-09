#pragma warning (disable : 4786)

#include "invokeGame.h"

#include "../gamex/xAsset.h"


#include "xInvokeCommon.h"

#include "../gamex/xWin.h"

#include "../gamex/xGLCommon.h"
#include "../gamex/oglExt.h"
#include "../gamex/xKey.h"
#include "../gamex/xTmx.h"
#include "../gamex/xMultiGrid.h"
#include "../gamex/xDebug.h"
#include "../gamex/xPartSys.h"



#include "xUnit.h"
#include "xTree.h"
#include "xPartTest.h"
#include "xBuildTest.h"
#include "xGroundDecal.h"



#include "unit/xTower.h"
#include "unit/xSlave.h"
#include "unit/xMine.h"
#include "unit/xTown.h"
#include "unit/xBuilder.h"





invokeGame::invokeGame(void) 
{
  shadowMode = 0;


  myCam.pos.set(16*64, 1024, 16*64);
  camPos.set(16*64, 1024, 16*64);
  myCam.ori.rotPitch(-0.98f);

  //use the war3 angle by default

  //http://www.hiveworkshop.com/forums/general-mapping-tutorials-278/cameras-160369/
  //printf(" %0.2f \n ", (float)(56*(3.1415/180.0)) );


  myCam.neard = 1.0f;
//far distance: too much and the zbuffer starts screwing up, too little and we wont see far
  myCam.fard = 7000.0f; //9000.0f;
  myCam.aspect = 640.0f/480.0f;
  myCam.fov = 70.0f;  //important


  curId = 1; //start at 1 

  gameState = -1;

}//ctor


invokeGame::~invokeGame(void) 
{

}//dtor








void 
invokeGame::drawLoading(int stat)
{

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);

  //black backdrop
  glColor3f(0,0,0);
     fillRect(-1.0f, -1.0f, 642.0f, 482.0f);

  unsigned int loadSkin;
  loadSkin = assetMan->getTexHandle("loading_bg");

  glEnable(GL_TEXTURE_2D);
 
   glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, loadSkin);
      fillRect(0.0f, 0.0f, 640.0f, 480.0f);

 glEnable(GL_BLEND);    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, myFont.handle);
      myFont.printStr(32,16,240-32, "Loading.. ");
 

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND); 
 

  xwin::xwinSwapBuffer(); 

}//drawloading








void
invokeGame::init(void)
{
  
  gameState = 0;  // -1 means not loaded yet  -- state 0 is default

  myFont.loadCharDef("data/atari16.xfnt");
  myFont.handle = assetMan->getTexHandle("atari16");

  drawLoading(0);


//todo -- also change this from settings
//i encountered some machines where you have shadow but no fbo -- not sure if to support those 
// (its possible but does it worth the effort)
  shadowMode = (extOgl::extShadow && extOgl::extDepthTex && extOgl::extFbo) ? 2 : 1;


//todo -- load tilemap before world reset (so we know the maps size)
  resetWorld(256*128,256*128);

  myRender.init(16384);


  

    myHud.assetMan = assetMan;
    myHud.init();
    myHud.pfont = &myFont;


  myPart.assetMan = assetMan;
  myPart.init();

  myPart.loadSprite("spark");
  myPart.loadAnim("spark");
  
  assetMan->initTexture("smoke",false, false, true);
  myPart.loadSprite("smoke");

  mySprite.assetMan = assetMan;


  //sprite anims for now are added manually 
  xPartSprite * ps;
  xSpriteMan * sm;
  sm = &(myPart.spriteMan);
  ps = new xPartSprite();
  storePartSpr.addData("smoke", ps);
    ps->addAnim(sm, "smoke", 0.2f, "smoke00,smoke01,smoke02,smoke03,smoke04,smoke05,smoke06,smoke07,smoke08,smoke09,smoke10,smoke11,smoke12");
    ps->addAnim(sm, "flame", 0.2f, "flame00,flame01,flame02,flame03,flame04,flame05,flame06,flame07,flame08,flame09,flame10,flame11,flame12,flame13,flame14,flame15,flame16,flame17,flame18");
  

  
//note: heightmap textures/images are special and its not using the asset manager (for now at least) 
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
    myHeight.mesh.makeVbo();


  myTile.initFromHeightmap(&myHeight);  



    
//preload all unit models and textures
//todo -- this part will need to use an xml or something

  xMdx3 * mesh;



  
  mesh = assetMan->getMesh("kunyho");  mesh->makeVbo();

  assetMan->initTexture("kunyho_textura_walpha", true, true, false);
  mesh = assetMan->getMesh("tree");  mesh->makeVbo();
  assetMan->initTexture("treeskin", true, true, false);
 

  mesh = assetMan->getMesh("wraith_foepulet"); 
  mesh->scaleMesh(4,4,4);
  mesh->makeVbo();
  assetMan->initTexture("wraith_foepulet", true, true, false);



//simple shadows are disabled for now
/*
  mesh = new xMdx3();
  mesh->initPlane(64.0f);
  myData.addMdxFromPtr(mesh, "shadow");  mesh->makeVbo();
  myData.addSkin("data/shadlarge.png", "shadow", true,true);
*/


  assetMan->initTexture("knight_skin2", true, true, false);
  assetMan->initTexture("mysmoke", true,false,false); 
   mySprite.addSpriteXms("mysmoke");

//mesh used for debugging
  debmesh.initBox(16);
 
//a big plane used for the water
 // waterSkin.loadTex("data/watertex.png");
  assetMan->initTexture("watertex", true, false, false);

  waterDeb.initPlaneXZ(32768, 0, 0, 32,32);



//world size
  float wwidth, wheight;
  wwidth = myHeight.mw * myHeight.cw;
  wheight = myHeight.mh * myHeight.ch;


//todo set scalefactor from local var (its 4.0f, 4.0f right now)

  xActor * a;
  xTmxRect * ta;
  tdVecTmxRect ::iterator wt;

  for (wt = tmx.vecRect.begin(); wt != tmx.vecRect.end(); wt++)
  {
    ta = (*wt);  

    a = 0;
    if (ta->type == "tree")    {      a = new xTree();    }
    else if (ta->type == "knight")    {      a = new xUnit();     }
    else if (ta->type == "slave")    {      a = new xSlave();    }
    else if (ta->type == "builder") {   a = new xBuilder();  }
    else if (ta->type == "mine")    {      a = new xMine();    } 
    else if (ta->type == "town")    {      a = new xTown();   } 
    else if (ta->type == "farm")    {      a = new xBuildTest();    }
    else if (ta->type == "tower")    {      a = new xTower();      }
    else if (ta->type == "smoke")    {      a = new xPartTest();    }
    else if (ta->type == "camstart")
    {
      camPos.set(ta->cx*4.0f, 2024, ta->cy*4.0f);
    }
    else if (ta->type == "decal")
    {
      a = new xGroundDecal();
      a->xrad = ta->rw * 4 * 0.5f;
      a->zrad = ta->rh * 4 * 0.5f;
      a->wstr = ta->name;
      a->ang = ta->ang;
      printf("decal angle %0.2f \n", a->ang);
    }
    else { printf("unknown actor type %s \n", ta->type.c_str());  } //warn about unknown actor


    if (a != 0)
    {
      //tmx map is in 32x32,  but the heightmap works in 128x128 tiles
      a->pos.set(ta->cx*4.0f, 0, ta->cy*4.0f);
      if (ta->name == "red") { a->team = 1; } else { a->team = 2;}

      addActor(a);
    }

  }//nextwt

}//init




void 
invokeGame::update(void)
{


  //cam speed
  float ms;
  ms = 32.0f;

//not using WASD for camera movement anymore as hotkeys are introduced
/*
  if (isKeyDown(KEY_W) || isKeyDown(KEY_UP)) { camPos.z -= ms;  } 
  if (isKeyDown(KEY_S) || isKeyDown(KEY_DOWN)) { camPos.z += ms;  }
  if (isKeyDown(KEY_A) || isKeyDown(KEY_LEFT)) { camPos.x -= ms; } 
  if (isKeyDown(KEY_D) || isKeyDown(KEY_RIGHT)) { camPos.x += ms; } 
*/


  if (isKeyDown(KEY_UP)) { camPos.z -= ms;  } 
  if (isKeyDown(KEY_DOWN)) { camPos.z += ms;  }
  if (isKeyDown(KEY_LEFT)) { camPos.x -= ms; } 
  if (isKeyDown(KEY_RIGHT)) { camPos.x += ms; } 


  if (isKeyDown(KEY_R)) { camPos.y += ms; }
  if (isKeyDown(KEY_F)) { camPos.y -= ms; }


  //todo use as parameter
  float maxHeight;
  float minHeight;

  maxHeight = 2000.0f;
  minHeight = 500.0f;

  if (camPos.y > maxHeight) { camPos.y = maxHeight; }
  if (camPos.y < minHeight) { camPos.y = minHeight; }
 
 //todo check map limits






  //transforming the universal mouse coordinates (0,1) to  (-1,1)
//todo -- move this fransformation into xCursor
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

  
  myPart.update();

  myWorld.preUpdate(gameTime);
  myWorld.update();

  myHud.viewMin = viewBox.min;
  myHud.viewMax = viewBox.max;
    myHud.childUpdate(this);


    xFrustum frust;
      frust.setPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
      frust.makeFrustum2(myCam.pos, myCam.ori);
      viewBox.genBox(&frust, 0,0,0, 0, 1, 0);

      
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
invokeGame::makeShadow(void)
{

  if (shadTex.handle == 0) { shadTex.init(1024); }


  //using opengl to generate matrices for the light (directional light)
    glMatrixMode(GL_MODELVIEW); 
    glPushMatrix();
      glLoadIdentity();
        //glOrtho(-512, 0, -512, 512, -9000, 9000);
        //glOrtho(-viewBox.size.x, viewBox.size.x, -viewBox.size.z, viewBox.size.z, -9000, 9000);
       // glOrtho(-viewBox.size.x*0.6f, viewBox.size.x*0.6f, -viewBox.size.z*0.5f, viewBox.size.z*0.5f, -9000, 9000);
        //glOrtho(-viewBox.size.z*0.5f, viewBox.size.z*0.5f, -viewBox.size.x*0.6f, viewBox.size.x*0.6f, -9000, 9000);
 
        glOrtho(-viewBox.size.x*0.5f, 0, -viewBox.size.z*0.6f, viewBox.size.z*0.6f, -4000, 4000);

          glGetFloatv(GL_MODELVIEW_MATRIX, lightProj);

      glLoadIdentity();
        //glRotatef(45, 0, 1, 0);  //looks good but is buggy for now
	      glRotatef(45, 1, 0, 0); //sun angle (90 is exactly down) (so far 90 works without bugs) (also try 45 and 135)
	      glRotatef(90, 0, 1, 0);
         glTranslatef(myCam.pos.x *-1 , 0, myCam.pos.z *-1 ); //todo -- needs tweaks
          glGetFloatv(GL_MODELVIEW_MATRIX, lightView);
    glPopMatrix();


//render scene from lights view point (into texture)

    glMatrixMode(GL_PROJECTION);  glLoadMatrixf(lightProj);
	  glMatrixMode(GL_MODELVIEW);  glLoadMatrixf(lightView);
 
    
    //set viewport to texture size
   // glViewport(0, 0, shadTex.mw,  shadTex.mh);
    glViewport(1, 1, shadTex.mw-2,  shadTex.mh-2); // "Hang back, buddy, and observe my magic"
    //(explanation: we dont render the edge pixels to avoid a rendering artifact)

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

}//makeshadow




//based on 
//http://www.paulsprojects.net/tutorials/smt/smt.html
// (aka the tutorial everyone is using for this)


void 
invokeGame::drawShadow(void)
{

     glDisable(GL_POLYGON_OFFSET_FILL);
     glEnable(GL_TEXTURE_2D);
     //glBindTexture(GL_TEXTURE_2D, shadTex.handle);
      glBindTexture(GL_TEXTURE_2D, shadTex.depth);

      glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );  //important
  
 
      static float row0[4];
      static float row1[4];
      static float row2[4];
      static float row3[4];
      static float texm[16];
      static float biasMatrix[16] = {0.5f, 0.0f, 0.0f, 0.0f,
								      0.0f, 0.5f, 0.0f, 0.0f,
								      0.0f, 0.0f, 0.5f, 0.0f,
                      0.5f, 0.5f, 0.5f, 1.0f};
      static float temp[16];

      gamex::cMat::multMatrix(biasMatrix, lightProj, temp);
      gamex::cMat::multMatrix(temp, lightView, texm);



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





      // glColor4f(1,1,1, 0.9f); //useful for testing
       //glColor4f(0,0,0, 0.33f);
       glColor4f(0.0f, 0.0f, 0.16f, 0.45f);


       glEnable(GL_BLEND);
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

       glEnable(GL_DEPTH_TEST); 
       glDepthFunc(GL_LEQUAL); 

      glDepthMask(GL_FALSE);


       glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

      //the classic approach is drawing the unshadowed scene here
      //and the shadows are a sort of mask that prevent us from drawing onto the shadowed areas
      //but our scheme uses a different approach where the shadows are rendered as a transparent texture
      //(at least for now)
       //glEnable(GL_ALPHA_TEST);
     //  glAlphaFunc(GL_LEQUAL, 0.99f);

     glEnable(GL_POLYGON_OFFSET_FILL);
      glPolygonOffset(-1.0f, -1.0f);

           myRender.simpRender(4);



//turn off texture coordinate generation
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_TEXTURE_GEN_S);
      glDisable(GL_TEXTURE_GEN_T);
      glDisable(GL_TEXTURE_GEN_R);
      glDisable(GL_TEXTURE_GEN_Q);
      
//reset other settings
      glDepthFunc(GL_LESS);
      glDisable(GL_BLEND);
      glDisable(GL_ALPHA_TEST);
      glDisable(GL_POLYGON_OFFSET_FILL);
      glDepthMask(GL_TRUE);
      glDisable(GL_DEPTH_TEST);  

}//drawshadow





void 
invokeGame::render(void) 
{
  //assuming zbuffer is cleared at this point

gamex::cMat view;

//update camera based on camera pos
  myCam.pos = camPos;
  myCam.pos.y += myHeight.getHeight(camPos.x, camPos.z);


//check if camera moved enough that we need to update the heightmap

    xFrustum frust;
    gamex::cVec3f dist;
    float mag;
    float diff;

      dist = lastPos - camPos;
      diff = myHeight.cw * 2.0f;

      mag = dist.getMag();

      //update heightmap mesh
        if (mag >= diff)
        {
          lastPos = camPos;

           diff *= 2.0f;

           frust.setPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
           frust.makeFrustum2(myCam.pos, myCam.ori);

          //todo -- make viewbox use the camera as parameter(?)          
            viewBox.genBox(&frust, 0,0,0, 0, 1, 0);

         
           //todo -- only update if box changes considerably
            //no need to check sides for difference as it seems to naturally be overestimated
            myHeight.updateMesh(viewBox.min.x, viewBox.min.z-diff, (viewBox.max.x-viewBox.min.x), (viewBox.max.z-viewBox.min.z)+diff*2.0f);
            myHeight.mesh.updateVboVertex();

          //printf("updatemesh %d %d \n", gameTime, myHeight.mesh.drawFace);
        }//endif
      







//3D rendering part

        myRender.resetFrame();

        myRender.setCam(&myCam); //needed for sorting transparent entities
  
               //todo -- for actors do a query based on the frustum 
                //so instead of testing each one against the frustum
                //we just pick ones that are potentially inside it 
              //    myWorld.render2(&myRender);

              drawActor(&myRender, mgrid, viewBox.min.x,  viewBox.min.z,  viewBox.size.x,viewBox.size.z);

              myPart.render(&myRender, myCam.ori);
  

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
                    e->skin = assetMan->getTexHandle("watertex"); // waterSkin.handle;
                    e->blend = 2; //0 solid   1 alpha test   2 transparent   3 additive
                    e->skinBlend = 3; //0 replace   1 blend  2 decal  3 modulate
                    e->useColor = 0; ///1 use color data of mesh or 0 for the e->color of xEnt
                    e->flags = 1;//draw if no flags are specified (dont cast or receive shadow)
              //todo -- water is problematic as it will need to be drawn AFTER the shadows are drawn
              // (this is true for the full second bucket)



if (shadowMode == 2)
{
  //render shadow from lights point of view
     makeShadow(); 
}


  //Render solids
    glMatrixMode(GL_PROJECTION); glLoadIdentity();  gluPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
	  glMatrixMode(GL_MODELVIEW);  view.setView(&myCam.pos, &myCam.ori);   glLoadMatrixf(view.m);
 
 if (shadowMode == 2) {   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); }
         myRender.renderBucket0(); //solid

         myRender.renderBucket2(); //decal

  //todo -- instead of extra pass use the 3rd texture channel (?)
 
   //draw shadow (duh)
 if (shadowMode == 2) {  drawShadow(); }


  //draw transparent objects
    glMatrixMode(GL_PROJECTION); glLoadIdentity();  gluPerspective(myCam.fov, myCam.aspect, myCam.neard, myCam.fard);
	  glMatrixMode(GL_MODELVIEW);  view.setView(&myCam.pos, &myCam.ori);   glLoadMatrixf(view.m);
    //render transparent
      myRender.renderBucket1();  //transparent



//draw (debug) cursor

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
 // myHand.mySelect.debRender(this);
    myHud.renderSelection3D();
  
//unit debug render
  myWorld.render();


//draw 2D hud (or ui) (or gui)
  myHud.childRender(this);



}//render



xBigPart * 
invokeGame::addPart(std::string wname)  {  return myPart.addPart(wname);}

void 
invokeGame::getDecal(xMdx3 * m, float x0, float y0, float w0, float h0){  myHeight.setDecal(m, x0, y0, w0, h0);}

float 
invokeGame::getHeight(float wx, float wz) { return myHeight.getHeight(wx, wz);}

xCam * 
invokeGame::getCamPtr(void){  return &myCam; }


//todo -- getanim, getskel

xMdx3 *
invokeGame::getMdx(std::string wname)  {  return assetMan->getMesh(wname);}

xTexture *
invokeGame::getTex(std::string wname) {   return assetMan->getTexture(wname);}  

unsigned int 
invokeGame::getSkin(std::string wname) { return  assetMan->getTexHandle(wname);}

xSprite * 
invokeGame::getSprite(std::string wname)  {  return mySprite.getSprite(wname);}

xPartSprite * 
invokeGame::getPartSprite(std::string wname) {  return storePartSpr.getData(wname);}



void 
invokeGame::gotCmd(int cmd, int arg0, int arg1)
{

// printf("invokegame gotcmd %d   %d %d \n", cmd, arg0, arg1);

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







bool 
invokeGame::canBuildRect(float wx, float wz, float ww, float wh)
{

  return myTile.hasRectZone(wx, wz, ww, wh, 2) == false;

}//canbuildrect


void 
invokeGame::setZoneType(float wx, float wz, int zt)
{
  myTile.setZoneRectWorld(wx, wz, 1.0f, 1.0f, zt);
}//setzonetype


void 
invokeGame::setZoneTypeRect(float wx, float wz, float ww, float wh, int zt)
{
  myTile.setZoneRectWorld(wx, wz, ww, wh, zt);
}//setzonetyprect

















