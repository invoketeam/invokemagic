#pragma warning (disable : 4786)

#include "xTree.h"


xTree::xTree(void) 
{
  skin = 0;

}//ctor


xTree::~xTree(void) 
{

}//dtor


void 
xTree::init(void)
{
  mesh = game->getMdx("tree");
  skin = game->getSkin("treeskin");

  pos.y = game->getHeight(pos.x, pos.z);

}//init



void 
xTree::update(void)
{

}//update


void 
xTree::render(void) 
{

}//render

  
unsigned int 
xTree::getSkin(std::string wname)
{
  return game->getSkin(wname);
}//getskin



void 
xTree::render2(xRender * r)
{
   xEnt * e;

    e = r->addFrame(0);
      e->pos = pos;
      e->blend = 0;
      e->fmesh = mesh;
      e->vmesh = mesh;
      e->alpha = 1;
      e->color = 1;
      e->twoSide = 0;
      e->skin = skin;
      //e->skin = getSkin("knight_skin") ;

    
    //mesh.render();

    /*
    float d;
    d =  (rend->camPos - pos).getMagSquared();

    if (d > 65384) { lodMesh = gameGetMdx("knight_lod01"); } 
    if (d > (65384+65384)) { lodMesh = gameGetMdx("knight_lod02"); } 
    if (d > (65384+65384+65384)) { lodMesh = gameGetMdx("knight_lod03"); } 


     skel.update();
////     skel.applySkin(&mesh, mesh.numVert);

     skel.applySkin(&mesh, lodMesh->numVert);
    */

}//render2
