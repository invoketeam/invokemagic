#pragma warning (disable : 4786)

#include "xBigPart.h"


#include "xGLCommon.h"



xBigPart::xBigPart(void)
{
  hp = -1;
  skin = 0;
  anim = 0; 
  curFrame = 0.0f;
  endFrame = 0.0f;
  startFrame = 0.0f;
  animSpeed = 0.0f;
  repeat = 0; 
  alpha = 1.0f; 
  fade = 0.0f;
  lastFrame = -1;
  mesh.initPlaneXY(1.0f);

  scale = 1.0f;
  grow = 0.0f;

  roll = 0.0f;
  angSpeed = 0.0f;

}//ctor


xBigPartSys::xBigPartSys(void)
{
  assetMan = 0;
  it = 0;
}//ctor

xBigPartSys::~xBigPartSys(void)
{

}//dtor


void 
xBigPartSys::clear(void)
{
  spriteMan.clear();
}//clear



void 
xBigPartSys::addSprite(std::string wname)
{
  spriteMan.assetMan = assetMan;
  spriteMan.addSpriteXms(wname);
}//addsprite


void 
xBigPartSys::addAnim(std::string wname)
{
  spriteMan.assetMan = assetMan;
  spriteMan.addAnimXma(wname);
}//addanim




xBigPart * 
xBigPartSys::addPart(std::string wanim)
{
  xBigPart * a;
  
  a = &(vecPart[it]);
  a->hp = 50;
  a->scale = 1.0f;
  a->grow = 0.0f;
  a->roll = 0.0f;
  a->angSpeed = 0.0f,
  a->rgb = 1.0f;
  a->alpha = 1.0f;
  a->fade = 0.0f;
  a->lastFrame = -1;


  //a->anim = spriteMan.getAnim("exploa");
  a->anim = spriteMan.getAnim(wanim);
  if (a->anim != 0)
  { 
    a->curFrame = 0;
    a->endFrame = a->anim->vecFrame.size();
    a->animSpeed = a->anim->speed;
  }//endif



  it += 1;
  if (it >= XBIGPART_MAXPART) { it = 0; }
  
  return a;
}//addpart



void 
xBigPartSys::update(void)
{
  int i;
  int num;
  xBigPart * a;

  num = XBIGPART_MAXPART;
  
  for (i = 0; i < num; i++)
  {
    a = &(vecPart[i]);
    
    if (a->hp <= 0) { continue; }
  
    a->pos += a->vel;
    
    a->hp -= 1;

    a->scale += a->grow; if (a->scale <= 0) { a->hp = -1; continue; }

    a->alpha -= a->fade; if (a->alpha <= 0) { a->hp = -1; continue; }

      
    //todo -- for now if there is no valid anim the particle gets destroyed
    if (a->anim == 0) { a->hp = -1; }


    //destroy at end of animation (todo -- loop)
    a->curFrame += a->animSpeed; if (a->curFrame >= a->endFrame) { a->hp = -1; continue; }


    a->roll += a->angSpeed;  

  }//nexti
  
}//update


void 
xBigPartSys::debRender(gamex::cQuat camori)
{
  int i;  int num;  xBigPart * a;  
  float rot[16];
  
  camori.setMatrix(rot);

   num = XBIGPART_MAXPART;

  for (i = 0; i < num; i++)
  {
    a = &(vecPart[i]);
    if (a->hp <= 0) { continue; }

    glPushMatrix();
      glTranslatef(a->pos.x, a->pos.y, a->pos.z);
      glMultMatrixf(rot);
      glScalef(a->scale, a->scale, a->scale);
        a->mesh.render();
    glPopMatrix();
  }//nexti

}//bigpartsys

  
void 
xBigPartSys::render(xRender * rend, gamex::cQuat camori)
{
  int i;
  int num;
  xBigPart * a;
  xEnt * e;

  xSprite * sp;
  int f;
  float hh, hw;

  num = XBIGPART_MAXPART;
  
  gamex::cQuat ori;
  
  ori = camori;
  //ori.invert();
  
  for (i = 0; i < num; i++)
  {
    a = &(vecPart[i]);
    if (a->hp <= 0) { continue; }


    //only update mesh if the frame changed

    f = (int) a->curFrame;
    if (a->lastFrame != f)
    {
      a->lastFrame = f;
      
      sp = spriteMan.getSprite( a->anim->vecFrame[f] );

      a->skin = sp->handle;

      //todo -- change size based on sprite width and height (?)
        hh = sp->h * 0.5f;
        hw = sp->w * 0.5f;
        a->mesh.vecVert[0].pos.set(-hw,-hh, 0.0f);
        a->mesh.vecVert[1].pos.set(hw,-hh, 0.0f);
        a->mesh.vecVert[2].pos.set(-hw,hh, 0.0f);
        a->mesh.vecVert[3].pos.set(hw,hh, 0.0f);

      //set uv
        a->mesh.vecVert[0].u = sp->u0;
        a->mesh.vecVert[0].v = sp->v1;
        a->mesh.vecVert[1].u = sp->u1;
        a->mesh.vecVert[1].v = sp->v1;
        a->mesh.vecVert[2].u = sp->u0;
        a->mesh.vecVert[2].v = sp->v0;
        a->mesh.vecVert[3].u = sp->u1;
        a->mesh.vecVert[3].v = sp->v0;
    }//endif
    
    
    e = rend->addFrame(1);
     e->sortpos = a->pos;
     e->blend = 2; //2;

     e->vmesh = &(a->mesh);
     e->fmesh = &(a->mesh);

     e->skin = a->skin;
     e->skinBlend = 3; //3;
     e->color = a->rgb;
     e->alpha = a->alpha;

     e->pos = a->pos;
     e->ori = ori;  e->ori.rotRoll(a->roll);
     e->scale = a->scale;

     e->useTransMat = false;
     
     //e->twoSide = 1;
  
  }//nexti
}//render
  



