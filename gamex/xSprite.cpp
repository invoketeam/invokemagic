#pragma warning (disable : 4786)

#include "xSprite.h"
#include "xGLCommon.h"

#include "pugixml/pugixml.hpp"


xSprite::xSprite(void)  
  { 
    x = 0.0f; y = 0.0f; 
    w= 0.0f; h = 0.0f; 
    u0 = 0.0f; v0 = 0.0f; 
    u1 = 1.0f; v1 = 1.0f;
    handle = 0;
  }//ctor






xSpriteMan::xSpriteMan(void) 
  {
    assetMan = 0;
  }//ctor


xSpriteMan::~xSpriteMan(void) 
  { 
    clear();
  }//dtor


void 
xSpriteMan::clear(void) 
  {
    storeSprite.clear();
  }//clear



 void 
 xSpriteMan::addSpriteTex(std::string wname)
 {
  xTexture * skin;
  xSprite * a;

  skin = assetMan->getTexture(wname);

   a = new xSprite();
    a->wname = wname;
    a->skin = wname;
    a->handle = skin->handle;
    a->w = (float) skin->mw;
    a->h = (float) skin->mh;
    storeSprite.addData(wname, a);

 }//addspritetex


 void 
 xSpriteMan::addSpriteXms(std::string wname)
  {
    pugi::xml_document xms;
 	  pugi::xml_node tex;
 	  pugi::xml_node img;
    std::string fname;
    xSprite * a;
    xTexture * skin;
    unsigned int handle;

    xAsset * w;
    w = assetMan->getAsset(wname, ASSETGRP_XSPRITE);
    if (w == 0) { printf("xSpriteMan -- asset doesnt exist %s \n", wname.c_str()); return; }

    //todo -- check if file is zipped
    fname = w->fname;

    //dont clear, adds new sprite
    if  (!(xms.load_file(fname.c_str(), 0) ))	{		printf("xSpriteMan: Couldn't load xms [%s] \n", fname);		return;	}//endif

    tex = xms.child("texture");
	    if (tex.empty() ) { return; } //todo error invalid file

    float picw;
    float pich;
    std::string picname;

    picw = (float) tex.attribute("width").as_int();
    pich = (float) tex.attribute("height").as_int();
    picname = tex.attribute("name").value(); 

    handle = 0;
    picname = stripName(picname);
    skin = assetMan->getTexture(picname);
    //skin = storeSkin.getData(picname);
    if (skin == 0) { printf("xSpriteMan: image [%s] not loaded before sprites \n", picname.c_str()); }
    else { handle = skin->handle; }
    //todo -- refreshsprites instead? (go through all loaded sprites in a method or something)

   for (img = tex.child("image"); img; img = img.next_sibling("image")) 
   {
      a = new xSprite();
      a->wname = img.attribute("name").value();
      a->skin = picname;
      a->handle = handle;

      a->x = (float) img.attribute("x").as_int();
      a->y = (float) img.attribute("y").as_int();
      a->w = (float) img.attribute("width").as_int();
      a->h = (float) img.attribute("height").as_int();

      a->u0 = (float)(a->x) / picw;
      a->v0 = (float)(a->y) / pich;
      a->u1 = (float)(a->x + a->w) / picw;
      a->v1 = (float)(a->y + a->h) / pich;

      //printf("addsprite %s %0.2f %0.2f %u \n", a->wname.c_str(), a->x, a->y, a->handle);

      storeSprite.addData(a->wname, a);
   }//nextimg

  }//addsprite




xSprite * 
xSpriteMan::getSprite(std::string wname) 
  {
    return storeSprite.getData(wname);
  }//getsprite


void 
xSpriteMan::drawSprite(xSprite * a, float ax, float ay, float ang)
  {
   float aw, ah;
   aw = a->w * 0.5f;
   ah = a->h * 0.5f;


   glPushMatrix();
   glTranslatef(ax, ay, 0);
   glRotatef(ang*57.29f, 0, 0, 1); //convert radian to degrees
     glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(a->u0, a->v0);
      glVertex2f(-aw, -ah); //0

      glTexCoord2f(a->u0, a->v1);
      glVertex2f(-aw, ah); //1

      glTexCoord2f(a->v1, a->v0);
      glVertex2f(aw, -ah); //2

      glTexCoord2f(a->u1, a->v1);
      glVertex2f(aw, ah);//3
     glEnd();
   glPopMatrix();
  
  }//drawsprite2


void 
xSpriteMan::drawImage(xSprite * a, float ax, float ay)
  {
   glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(a->u0, a->v0);
    glVertex2f(ax, ay); //0

    glTexCoord2f(a->u0, a->v1);
    glVertex2f(ax, ay+a->w); //1

    glTexCoord2f(a->v1, a->v0);
    glVertex2f(ax+a->h, ay); //2

    glTexCoord2f(a->u1, a->v1);
    glVertex2f(ax+a->w, ay+a->h);//3
   glEnd();
  
  }//drawsprite