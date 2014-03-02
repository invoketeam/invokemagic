#pragma warning (disable : 4786)
#include "xFrustum.h"


//#include "madGlInc.h"


void 
xFrustum::setPerspective(float fov_, float aspect_, float nearDist_, float farDist_)
	{

		fov = fov_;
		aspect = aspect_;
		nearDist = nearDist_;
		farDist = farDist_;
	
		float tang;
		fov_ *= 3.1415f / 180.0f; //torad
		tang = tanf(fov_ * 0.5f);

		nearHeight = nearDist * tang;
		nearWidth = nearHeight * aspect;

		farHeight = farDist * tang;
		farWidth = farHeight * aspect;
	
	}//setpersp
  
  
  
float 
xFrustum::getPlaneInter(gamex::cVec3f a, gamex::cVec3f b, gamex::cVec3f p, gamex::cVec3f n)
	{
		gamex::cVec3f v, d;
		float ndotv;

		v = b - a;
		d = p - a;

		ndotv = (v.x * n.x) + (v.y * n.y) + (v.z * n.z);
		if (ndotv == 0) { return -99999; } //line and plane parallel, no inters

			
		return ( ( ( n.x * d.x) + (n.y * d.y) + (n.z * d.z) ) / ndotv );

	
	}//getplaneinter
  
  

  
  
  	//sets the frustum points and planes
	//based on a position and orientation
	void 
  xFrustum::setPoints(gamex::cVec3f pos, gamex::cQuat ori,float umx, float umy)
	{

		gamex::cVec3f side, up, front;

		ori.setVecFront(front);
		ori.setVecSide(side);
		ori.setVecUp(up);

  //FIX
    side = -side;
    front = -front;

    //moved to member
	//	gamex::cVec3f fc, nc;

		//dividing by two doesnt seem to work out ok
		//not sure why they put that in the lighthouse3d tut

		  float nh, nw, fh, fw; 
			nh = nearHeight;// * 0.5f;
			nw = nearWidth; //* 0.5f;

			fh = farHeight; //* 0.5f;
			fw = farWidth; //* 0.5f;

	//	gamex::cVec3f farUp, farSide;
//		gamex::cVec3f nearUp, nearSide;

			farUp = up * fh;
			farSide = side * fw;

			nearUp = up * nh;
			nearSide = side * nw;

		
		fc = pos + front * farDist;
		
			ftl = fc + farUp - farSide;
			ftr = fc + farUp + farSide;
			fbl = fc - farUp - farSide;
			fbr = fc - farUp + farSide;

		nc = pos + front * nearDist;

			ntl = nc + nearUp - nearSide;
			ntr = nc + nearUp + nearSide;
			nbl = nc - nearUp - nearSide;
			nbr = nc - nearUp + nearSide;

      mclose = nc;
      mclose += nearUp * umy;
      mclose += nearSide * umx;

      mfar = fc;
      mfar += farUp * umy;
      mfar += farSide * umx;
/*
ma.copyVec(fc); 
ma.addVec( vVec.getVecMulNum(farUp, umy) );
ma.addVec( vVec.getVecMulNum(farSide, umx) );
			//sa = fc + (farUp * umy) + (farSide * umx);


			mb.copyVec(nc);
 mb.addVec( vVec.getVecMulNum(nearUp, umy) );
 mb.addVec( vVec.getVecMulNum(nearSide, umx) );
			//sb = nc + (nearUp * umy) + (nearSide * umx)
*/
      
    //now look elsewhere, because this is going to be ugly
      #define FMIN(x,y) ((x)<(y)?(x):(y))
      #define FMAX(x,y) ((x)>(y)?(x):(y))


      aabb_min.x = FMIN(FMIN(FMIN(FMIN(FMIN(FMIN(FMIN(ftl.x, ftr.x), fbl.x),fbr.x),ntl.x),ntr.x),nbl.x),nbr.x);
      aabb_min.y = FMIN(FMIN(FMIN(FMIN(FMIN(FMIN(FMIN(ftl.y, ftr.y), fbl.y),fbr.y),ntl.y),ntr.y),nbl.y),nbr.y);
      aabb_min.z = FMIN(FMIN(FMIN(FMIN(FMIN(FMIN(FMIN(ftl.z, ftr.z), fbl.z),fbr.z),ntl.z),ntr.z),nbl.z),nbr.z);

      aabb_max.x = FMAX(FMAX(FMAX(FMAX(FMAX(FMAX(FMAX(ftl.x, ftr.x), fbl.x),fbr.x),ntl.x),ntr.x),nbl.x),nbr.x);
      aabb_max.y = FMAX(FMAX(FMAX(FMAX(FMAX(FMAX(FMAX(ftl.y, ftr.y), fbl.y),fbr.y),ntl.y),ntr.y),nbl.y),nbr.y);
      aabb_max.z = FMAX(FMAX(FMAX(FMAX(FMAX(FMAX(FMAX(ftl.z, ftr.z), fbl.z),fbr.z),ntl.z),ntr.z),nbl.z),nbr.z);


      #undef FMIN
      #undef FMAX


		setNormals();
	}//setpoints
  
  
  
  
bool 
xFrustum::isPointInside(gamex::cVec3f &p, float rad)
	{

		//todo -- opt
		//dist variable is not really needed here
		//so can jut put the calculations in the if

		float dist;

      rad = -rad;

		// check distance to planes
		// distance = (object position - cam position) dot (plane normal)

		//check point distance to near and far planes first
		//if its behind any of them we know its outside the frustum

			dist = (p.x-pa.x) * norma.x + (p.y-pa.y)*norma.y + (p.z-pa.z)*norma.z;
			if (dist < rad) return false;
		
			dist = (p.x-pb.x) * normb.x + (p.y-pb.y)*normb.y + (p.z-pb.z)*normb.z;
			if (dist < rad) return false;
	
			dist = (p.x-pc.x) * normc.x + (p.y-pc.y)*normc.y + (p.z-pc.z)*normc.z;
			if (dist < rad) return false;

			dist = (p.x-pd.x) * normd.x + (p.y-pd.y)*normd.y + (p.z-pd.z)*normd.z;
			if (dist < rad) return false;

			dist = (p.x-pe.x) * norme.x + (p.y-pe.y)*norme.y + (p.z-pe.z)*norme.z;
			if (dist < rad) return false;

			dist = (p.x-pf.x) * normf.x + (p.y-pf.y)*normf.y + (p.z-pf.z)*normf.z;
			if (dist < rad) return false;
	
		return true;
	}//ispinside
  
  
  
  
  
void 
xFrustum::setNormals()
	{
		//near
		pa = (ntr + nbl) *0.5f;
		norma = gamex::cVec3f::cross( (ntr-nbr), (nbl-ntr) );
		norma.normPrec();
	
		//far
		pb = (ftr + fbl) * 0.5f;
		normb = gamex::cVec3f::cross( (ftr-fbr), (ftr-fbl) );
		normb.normPrec();

		//left
		pc = (ftl + fbl + ntl + nbl) / 4 ;
		normc = gamex::cVec3f::cross( (ftl-fbl), (ftl-ntl) );
		normc.normPrec();

		//right
		pd = (ftr + fbr + ntr + nbr) / 4;
		normd = gamex::cVec3f::cross( (ftr-ntr), (ftr-fbr) );
		normd.normPrec();

		//top
		pe = (ftr + ftl + ntr + ntl) / 4;
		norme = gamex::cVec3f::cross( (ftr-ftl), (ftr - ntr) );
		norme.normPrec();

		//bottom
		pf = (fbr + fbl + nbr + nbl) / 4;
		normf = gamex::cVec3f::cross( (fbl-nbl), (fbr-fbl) );
		normf.normPrec();

	}//setnormals
  
  
  
  
  /*
  void 
  xFrustum::drawMouse(float umx, float umy)
	{

		gamex::cVec3f side;
		gamex::cVec3f side2;
		gamex::cVec3f up;


		side = ftr + (ftl - ftr) * umx;
		side2 = fbr + (fbl - fbr) * umx; 

		up = side + (side2 - side) * umy;


		gamex::cVec3f nside;
		gamex::cVec3f nside2;
		gamex::cVec3f nup;


		nside = ntr + (ntl - ntr) * umx;
		nside2 = nbr + (nbl - nbr) * umx; 

		nup = nside + (nside2 - nside) * umy;

		//up = ftr + (fbr - ftr) * umy;



		glBegin(GL_POINTS);
			glVertex3f(side.x, side.y, side.z);
			glVertex3f(side2.x, side2.y, side2.z);
			glVertex3f(up.x, up.y, up.z);

			glVertex3f(nside.x, nside.y, nside.z);
			glVertex3f(nside2.x, nside2.y, nside2.z);
			glVertex3f(nup.x, nup.y, nup.z);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(up.x, up.y, up.z);
			glVertex3f(nup.x, nup.y, nup.z);
		glEnd();
	
	}//drawmouse
  */
  
  
  
  /*
  void render()//debugrender
	{

		gamex::cVec3f p;

	

		glBegin(GL_LINES);

			glColor3f(0,0,1);
				p = pa;
					glVertex3f(p.x, p.y, p.z);
				p += norma;
					glVertex3f(p.x, p.y, p.z);

			glColor3f(1, 0, 0);
				p = pb;
					glVertex3f(p.x, p.y, p.z);
				p += normb;
					glVertex3f(p.x, p.y, p.z);

			glColor3f(0,0.5,1);
				p = pc;
					glVertex3f(p.x, p.y, p.z);
				p += normc;
					glVertex3f(p.x, p.y, p.z);

			glColor3f(1,0.5,0);
				p = pd;
					glVertex3f(p.x, p.y, p.z);
				p += normd;
					glVertex3f(p.x, p.y, p.z);


			glColor3f(1,0,0.5);
				p = pe;
					glVertex3f(p.x, p.y, p.z);
				p += norme;
					glVertex3f(p.x, p.y, p.z);

			glColor3f(0.5,0,1);
				p = pf;
					glVertex3f(p.x, p.y, p.z);
				p += normf;
					glVertex3f(p.x, p.y, p.z);

		glEnd();

		glBegin(GL_POINTS);

			glColor3f(1,0,0);
				glVertex3f(ftl.x, ftl.y, ftl.z);
				glVertex3f(ftr.x, ftr.y, ftr.z);
				glVertex3f(fbl.x, fbl.y, fbl.z);
				glVertex3f(fbr.x, fbr.y, fbr.z);

			glColor3f(0,0,1);
				glVertex3f(ntl.x, ntl.y, ntl.z);
				glVertex3f(ntr.x, ntr.y, ntr.z);
				glVertex3f(nbl.x, nbl.y, nbl.z);
				glVertex3f(nbr.x, nbr.y, nbr.z);

		glEnd();

		glBegin(GL_LINE_LOOP);
			glColor3f(1,0,0);
				glVertex3f(ftl.x, ftl.y, ftl.z);
				glVertex3f(ftr.x, ftr.y, ftr.z);
				glVertex3f(fbr.x, fbr.y, fbr.z);
				glVertex3f(fbl.x, fbl.y, fbl.z);
			
			
			
		glEnd();

		glBegin(GL_LINE_LOOP);
			glColor3f(0,0,1);
				glVertex3f(ntl.x, ntl.y, ntl.z);
				glVertex3f(ntr.x, ntr.y, ntr.z);
				glVertex3f(nbr.x, nbr.y, nbr.z);
				glVertex3f(nbl.x, nbl.y, nbl.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glColor3f(0,1,0);
				glVertex3f(ntl.x, ntl.y, ntl.z);
				glVertex3f(ftl.x, ftl.y, ftl.z);
				glVertex3f(fbl.x, fbl.y, fbl.z);
				glVertex3f(nbl.x, nbl.y, nbl.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glColor3f(0,1,1);
				glVertex3f(ntr.x, ntr.y, ntr.z);
				glVertex3f(ftr.x, ftr.y, ftr.z);
				glVertex3f(fbr.x, fbr.y, fbr.z);
				glVertex3f(nbr.x, nbr.y, nbr.z);
		glEnd();
	}//render
  */
  
    /*
	void
  xFrustum::drawMouse2(float umx, float umy,gamex::cVec3f pos, gamex::cQuat ori)
	{
		gamex::cVec3f fc, nc;
		gamex::cVec3f side, up, front;

		ori.setVecFront(front);
		ori.setVecSide(side);
		ori.setVecUp(up);

		gamex::cVec3f farUp, farSide;
		gamex::cVec3f nearUp, nearSide;
	
		float nh, nw, fh, fw; 
			nh = nearHeight;// * 0.5f;
			nw = nearWidth; //* 0.5f;

			fh = farHeight; //* 0.5f;
			fw = farWidth; //* 0.5f;

			farUp = up * fh;
			farSide = side * fw;

			nearUp = up * nh;
			nearSide = side * nw;

		
		gamex::cVec3f sa, sb;
	

		fc = pos + front * farDist;
		
			sa = fc + (farUp * umy) + (farSide * umx);

		nc = pos + front * nearDist;

			sb = nc + (nearUp * umy) + (nearSide * umx);


		glBegin(GL_POINTS);
			glVertex3f(sa.x, sa.y, sa.z);
			glVertex3f(sb.x, sb.y, sb.z);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(sa.x, sa.y, sa.z);
			glVertex3f(sb.x, sb.y, sb.z);
		glEnd();

		float t;
		gamex::cVec3f in;

		t = getPlaneInter(sa, sb, gamex::cVec3f(), gamex::cVec3f(0,1,0) );

		in = sa + (sb - sa) * t;
		
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
			glVertex3f(in.x, in.y, in.z);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(in.x, in.y, in.z);
			glVertex3f(in.x, in.y+1, in.z);
			glVertex3f(in.x, in.y, in.z);
			glVertex3f(in.x+1, in.y, in.z);
			glVertex3f(in.x, in.y, in.z);
			glVertex3f(in.x, in.y, in.z+1);
		glEnd();

		glPushMatrix();
			glTranslatef(in.x, in.y, in.z);
			glutWireCube(1);
		glPopMatrix();	



	}//drawmouse
    */
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  