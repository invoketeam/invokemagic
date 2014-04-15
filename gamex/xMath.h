#pragma once

#include <math.h>
#include <cstring>

//vector, quaternion and matrix math


namespace gamex
{

/* VECTOR 3D*/
  
class cVec3f
{
public:
	float x, y, z;

public:
	 cVec3f(void):x(0.0f),y(0.0f),z(0.0f) {} //ctor1
	 cVec3f(float x_, float y_, float z_):x(x_),y(y_),z(z_) {} //ctor2
	 cVec3f(float f_):x(f_), y(f_), z(f_) {}//ctor3

	inline float getMagSquared(void) 	{		return ((x*x) + (y*y) + (z*z));	}
	inline float getMag(void) {	return (1.0f / invSquareRoot( (x*x) + (y*y) + (z*z) ));	} //  sqrtf((x*x) + (y*y) + (z*z)); }

	inline void normalise(void) 
	{ 
		float invmag =  (x*x) + (y*y) + (z*z);  //1.0f / getMag();
		if (invmag == 1.0f) return;
		invmag = invSquareRoot(invmag);
		x *= invmag;	y *= invmag;	z *= invmag;
	}//normalise


	//precise (but slower) normalisation
	inline void normPrec(void)
	{
		float invmag =  (x*x) + (y*y) + (z*z);
		if (invmag == 1.0f) return;

		invmag = 1.0f / sqrtf(invmag);
		x *= invmag;	y *= invmag;	z *= invmag;
	}//normprec


	inline void set(float x_, float y_, float z_)
	{ x = x_; y = y_; z = z_; }

	//todo -- change these to static methods
	inline float dot(cVec3f &v_)
	{ return ( (x * v_.x) + (y * v_.y) + (z * v_.z) ); }

	inline cVec3f cross(cVec3f &v_)
	{ return cVec3f(
	(y * v_.z) - (z * v_.y),
	(z * v_.x) - (x * v_.z),
	(x * v_.y) - (y * v_.x)); }


	static inline float dot(cVec3f &va_, cVec3f &vb_)
	{ return ( (va_.x * vb_.x) + (va_.y * vb_.y) + (va_.z * vb_.z) ); }

	static inline cVec3f cross(cVec3f va_, cVec3f vb_)
	{ return cVec3f(
	(va_.y * vb_.z) - (va_.z * vb_.y),
	(va_.z * vb_.x) - (va_.x * vb_.z),
	(va_.x * vb_.y) - (va_.y * vb_.x)); }


	inline cVec3f operator -(void) {	return cVec3f(-x,-y,-z);  }

	inline void operator =(cVec3f v_) {      x = v_.x;  y = v_.y;    z = v_.z;	  }
	inline void operator =(float f_) {      x = y = z = f_;	   }

	inline void operator +=(cVec3f v_){     x += v_.x;   y += v_.y;    z += v_.z;	  }
	inline void operator +=(float f_) {      x += f_;    y += f_;      z += f_;	   }

	inline void operator -=(cVec3f&v_) {     x -= v_.x;   y -= v_.y;    z -= v_.z;	  }
	inline void operator -=(float f_) {      x -= f_;    y -= f_;      z -= f_;	   }

	inline void operator *=(cVec3f v_) {     x *= v_.x;   y *= v_.y;    z *= v_.z;	  }
	inline void operator *=(float f_) {      x *= f_;    y *= f_;      z *= f_;	   }

	inline void operator /=(cVec3f v_)
		{		if (v_.x == 0.0f || v_.y == 0.0f || v_.z == 0.0f) return; //protect from div by zero
				x /= v_.x;   y /= v_.y;    z /= v_.z;	 	}

	inline void operator /=(float f_)
		{ if (f_ == 0.0f) return; //protect from div by zero
			f_ = 1.0f / f_; //optimisation -- multiply by inverse rather than dividing 3 times
			x *= f_;    y *= f_;      z *= f_;	 }

	inline cVec3f operator +(cVec3f v_){	return cVec3f(x + v_.x,   y + v_.y,   z + v_.z);  }
	inline cVec3f operator +(float f_){	return cVec3f(x + f_,   y + f_,   z + f_);  }

	inline cVec3f operator -(const cVec3f &v_) {return cVec3f(x - v_.x,   y - v_.y,   z - v_.z);  }
	inline cVec3f operator -(float f_) {return cVec3f(x - f_,   y - f_,   z - f_);  }

	inline cVec3f operator *(cVec3f &v_){	return cVec3f(x * v_.x,   y * v_.y,   z * v_.z);  }
	inline cVec3f operator *(float f_) {	return cVec3f(x * f_,   y * f_,   z * f_);  }

	

	inline cVec3f operator /(cVec3f &v_)
	{	if (v_.x == 0.0f || v_.y == 0.0f || v_.z == 0.0f) return cVec3f(); //div by zero
		return cVec3f(x / v_.x,   y / v_.y,   z / v_.z); }

	inline cVec3f operator /(float f_)
	{	if (f_ == 0.0f) return cVec3f(); //div by zero
		return cVec3f(x / f_,   y / f_,   z / f_); }
	

	static inline float invSquareRoot(float x)
  {//source: //http://bits.stephan-brumme.com/invSquareRoot.html
		 float xHalf = 0.5f*x;
		 unsigned int *i = (unsigned int*) &x;
		 *i = 0x5F375A86 - (*i>>1);
		 return x * (1.5f - xHalf*x*x);
  }//invsq

};//classend (vec3f)





/* QUATERNION */

class cQuat
{
public:
	float x, y, z, w;

public:
	cQuat(void):x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}//ctor

	cQuat(float x_, float y_, float z_, float w_):
	x(x_), y(y_), z(z_), w(w_) {} //ctor2


	inline cQuat operator - (void) { return cQuat(-x,-y,-z, w); }

	inline cQuat getConj(void) { return cQuat(-x,-y,-z, w); } //conjugate

	inline void invert(void) { x = -x; y = -y; z = -z; }

	inline void reset(void) { x=0.0f; y=0.0f; z=0.0f; w = 1.0f; }

	inline void mul(cQuat &b) //could be the same as operator *= with other quaternion
	{
		float tx, ty, tz, tw;

		//source
		//http://glprogramming.com/codedump/godecho/quaternion.html

		tx =	(w*b.x + x*b.w + y*b.z - z*b.y),
		ty =	(w*b.y + y*b.w + z*b.x - x*b.z),
		tz =	(w*b.z + z*b.w + x*b.y - y*b.x),
		tw =	(w*b.w - x*b.x - y*b.y - z*b.z);

		x = tx; y = ty; z = tz; w = tw;
	
	}//multiply


	inline void normalise(void)
	{
		float mag = x*x+y*y+z*z+w*w;
		if (mag == 1.0f) return;

		//mag = cVec3f::invSquareRoot(mag);

		//can't use the fast square root for quaternions
		//not precise enough for it

		mag = 1.0f / sqrtf(mag);

		x *= mag; y *= mag; z *= mag; w *= mag; 
			
	}//normalise




	inline void setVecSide(cVec3f &v)
	{
		v.x = 1.0f - 2.0f * ( y*y + z*z );
		v.y = 2.0f * ( x*y + z*w );
		v.z = 2.0f * ( x*z - y*w );
	}//vecside


	inline void setVecUp(cVec3f &v)
	{
		v.x = 2.0f * ( x*y - z*w );
		v.y = 1.0f - 2.0f * ( x*x + z*z );
		v.z = 2.0f * ( y*z + x*w );
	}//vecside


	//get front vector (based on converting to matrix)
	inline void setVecFront(cVec3f &v)
	{
		v.x = 2.0f * ( x*z + y*w );
		v.y = 2.0f * ( y*z - x*w );
		v.z = 1.0f - 2.0f * ( x*x + y*y );
	}//vecfront

  inline cVec3f getFront()
  {
    return cVec3f( 2.0f * ( x*z + y*w ), 
            2.0f * ( y*z - x*w ),
            1.0f - 2.0f * ( x*x + y*y ));
            
  }//getfront








	//source
	//http://www.flipcode.com/documents/matrfaq.html#Q56
	//angle in radians
	inline void fromAxisAngle(float angle, float x_, float y_, float z_)
	{
		angle *= 0.5f;
		float sina = sinf(angle);

		x = x_ * sina;
		y = y_ * sina;
		z = z_ * sina;
		w = cosf(angle);
	
		normalise();
	}//fromaxangle


	//idea from
	//http://www.flipcode.com/documents/matrfaq.html
	//using the axis angle conversion
	//to turn degrees into quaternion
	//and use that to turn the quat

	inline void rotPitch(float ang)
	{
		cQuat q;

		ang *=0.5f;
	
		q.x = sinf(ang);
		q.y = 0;
		q.z = 0;
		q.w = cosf(ang);

		mul(q);
		normalise();

	}//rotpitch

	inline void rotYaw(float ang)
	{
		cQuat q;

		ang *=0.5f;
	
		q.x = 0;
		q.y = sinf(ang);
		q.z = 0;
		q.w = cosf(ang);

		mul(q);
		normalise();

	}//rotpitch


	inline void rotRoll(float ang)
	{
		cQuat q;

		ang *=0.5f;
	
		q.x = 0;
		q.y = 0;
		q.z = sinf(ang);
		q.w = cosf(ang);

		mul(q);
		normalise();

	}//rotroll



	//m should be the array in cMat (aka a float m[16])
	//note -- this only sets a rotation matrix -- no translate or scale
	inline void setMatrix(float * m)
	{
	
		//source:
		// http://www.flipcode.com/documents/matrfaq.html#Q54

			
		float xx      = x * x;
		float xy      = x * y;
		float xz      = x * z;
		float xw      = x * w;

		float yy      = y * y;
		float yz      = y * z;
		float yw      = y * w;

		float zz      = z * z;
		float zw      = z * w;

		m[0]  = 1.0f - 2.0f * ( yy + zz );
		m[4]  =     2.0f * ( xy - zw );
		m[8]  =     2.0f * ( xz + yw );

		m[1]  =     2.0f * ( xy + zw );
		m[5]  = 1.0f - 2.0f * ( xx + zz );
		m[9]  =     2.0f * ( yz - xw );

		m[2]  =     2.0f * ( xz - yw );
		m[6]  =     2.0f * ( yz + xw );
		m[10] = 1.0f - 2.0f * ( xx + yy );

		m[3]  = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
		m[15] = 1.0f;
	
	}//setmatrix


  
  inline void mulVec3(gamex::cVec3f &v)
  {
    //based on:
    //  http://wonderfl.net/c/6w8P
  
        float xx = x * x;
        float yy = y * y;
        float zz = z * z;
        float xy = x * y;
        float yz = y * z;
        float xz = x * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;
        float tx = v.x * (0.5f - yy - zz) + v.y * (xy - wz) + v.z * (xz + wy);
        float ty = v.x * (xy + wz) + v.y * (0.5f - xx - zz) + v.z * (yz - wx);
        float tz = v.x * (xz - wy) + v.y * (yz + wx) + v.z * (0.5f - xx - yy);
        
        v.x = tx * 2;
        v.y = ty * 2;
        v.z = tz * 2;
    
  }//mulvec
  
  


	//needs a m[16] sized matrix -- in opengl order
	//(just use m from a cMat)
	//based on http://www.flipcode.com/documents/matrfaq.html#Q55
	//(note -- changed the order of matrix they used to opengl one)

  //seldom used (probably not needed )
	inline void fromMatrix(float * m)
	{

		float t = m[0] + m[5] + m[10] + 1.0f;
		float s;

		if (t > 0.0f)
		{
			s = 0.5f / sqrtf(t); 
			
			w = 0.25f / s;
		  x = ( m[6] - m[9] ) * s;
      y = ( m[8] - m[2] ) * s;
      z = ( m[1] - m[4] ) * s;

			return;
		}//endif

		if (m[0] > m[5] && m[0] > m[10])
		{
				s = sqrtf( 1.0f + m[0] - m[5] - m[10] ) * 2.0f; 
		
				x = 0.5f / s;
        y = (m[4] + m[1] ) / s;
        z = (m[8] + m[2] ) / s;
        w = (m[9] + m[6] ) / s;
		}
		else
		{
			if (m[5] > m[10])
			{			
				s = sqrtf( 1.0f + m[5] - m[0] - m[10] ) * 2.0f; 

				x = (m[1] + m[4] ) / s;
				y = 0.5f / s;
				z = (m[6] + m[9] ) / s;
				w = (m[2] + m[8] ) / s;

			}
			else
			{
				s = sqrtf( 1.0f + m[10] - m[0] - m[5] ) * 2.0f; 

				x = (m[2] + m[8] ) / s;
				y = (m[6] + m[9] ) / s;
				z = 0.5f / s;
				w = (m[1] + m[4] ) / s;
			
			}//endif2
		}//endif


	}//frommatrix


//based on
//http://physicsforgames.blogspot.hu/2010/02/quaternions.html
 inline void nlerp(cQuat &q1, cQuat &q2, float t)
 {
  float dot, blend;
  
    dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z * q2.z;
    blend = 1.0f - t;

    if (dot < 0) { t = -t;}
    
      w = blend * q1.w + t * q2.w;
      x = blend * q1.x + t * q2.x;
      y = blend * q1.y + t * q2.y;
      z = blend * q1.z + t * q2.z;

     // not normalising by default 
 }//nlerp



//source
//http://irrlicht.sourceforge.net/docu/quaternion_8h-source.html

 inline void slerp(cQuat q1, cQuat q2, float t)
 {
		float scale;    float invscale;		float theta;
		float invsintheta;    float ang;

    ang = (q1.x*q2.x)+(q1.y*q2.y)+(q1.z*q2.z)+(q1.w*q2.w);


    if (ang < 0.0f)
		{		   
        q1.x = -q1.x;				 q1.y = -q1.y;		
   		  q1.z = -q1.z;				 q1.w = -q1.w;
        ang = -ang;
    }//endif

    if ((ang + 1.0f) > 0.05f)
    {
     if ((1.0f - ang) >= 0.05f) // spherical interpolation
     {
       theta = acosf(ang);
       invsintheta = 1.0f / sinf(theta);
       scale = sinf(theta * (1.0f-t)) * invsintheta;
       invscale = sinf(theta * t) * invsintheta;
     }
     else // linear interploation
     {
       scale = 1.0f - t;
       invscale = t;
     }//endif
     }
     else
     {
           
			q2.x = -q1.y;		q2.y = q1.x;		q2.z = -q1.w;			q2.w = q1.z;

       scale = sinf(3.1415f * (0.5f - t));
       invscale = sinf(3.1415f * t);
     }//endif


		 x = q1.x * scale + q2.x * invscale;
		 y = q1.y * scale + q2.y * invscale;
		 z = q1.z * scale + q2.z * invscale;
		 w = q1.w * scale + q2.w * invscale;

	}//slerp




 	void alignUpVector(gamex::cVec3f &newup)
	{
			 //found this code in this forum
			 //the guy doesnt write where he found it
			 //but ohmygod it works O_O (or at least it seem to be)
					 //http://forums.create.msdn.com/forums/t/31468.aspx

		gamex::cQuat q;

		gamex::cVec3f c;
 		gamex::cVec3f up;



 			setVecUp(up); //get old up vector from orientation

				c = up.cross(newup);

			float d = up.dot(newup);
			float s = sqrtf((1.0f+d)*2.0f);
			float rs = 1.0f / s;

				q.x = c.x * rs;	
				q.y = c.y * rs; 
				q.z = c.z * rs; 
				q.w = s * 0.5f;
				
				//q = q * ori;
					q.mul(*this);

					x = q.x;
					y = q.y;
					z = q.z;
					w = q.w;

					//	ori = q;  //no slerp yet

					//	ori.
					normalise();
				
				//Rotation = Quaternion.Slerp(Rotation, q, 0.1f * distanceScalar); 
	}//alignup

}; //classend









/* MATRIX 4x4 */

class cMat
{
public:
	float m[16];

public:

	cMat(void) 
		{
			memset(m, 0, 64); //16*4 -- 16 * 4byte
			m[0] = m[5] = m[10] = m[15] = 1.0f; 
		}//ctor

	inline void reset(void)
	{
		//reset to identity matrix

		memset(m, 0, 64); //16*4 -- 16 * 4byte
		m[0] = m[5] = m[10] = m[15] = 1.0f; 
	}//ident

  
  inline void copyMat(cMat * o)
  {
    memcpy(m, o->m, 64);  
  }//copymat

  inline void copyMat(cMat &o)
  {
    memcpy(m, o.m, 64);  
  }//copymat



	inline void quickInvert(void)
	{
		//quickly inverts matrix--
		//transpose rotation part -- negate translate part
		//src_transpose: http://www.mathwords.com/t/transpose_of_a_matrix.htm
		//note -- only correct if there is no scaling



		//transpose rotation part
		
			// 0 5 10 stays the same (check notes at the end)

			//note -- xor swap not works on floats
		
			//t = m[1];	//m[1] = m[4];	//m[4] = t;

			float t; //temp

			/*
			#define MSWAP(x, y) (t = m[x];  m[x] = m[y];  m[y] = t; )
				MSWAP(1,4)
				MSWAP(2,8)
				MSWAP(6,9)
			#undef MSWAP 
*/

			t = m[1]; m[1] = m[4];	m[4] = t;
			t = m[2]; m[2] = m[8];	m[8] = t;
			t = m[6]; m[6] = m[9];	m[9] = t;


		//negate translation part

			m[12] = -m[12];
			m[13] = -m[13];
			m[14] = -m[14];
	
	
	}//quickinvert




	inline static void multMatrix(cMat &ma, cMat &mb, cMat &mr )
	{
		
    float * a;    float * b;    float * r;

    a = ma.m;    b = mb.m;    r = mr.m;

    /*
//int i;
		for (i = 0; i < 16; i +=4)
		{
			r.m[i] = a.m[0]*b.m[i]  +  a.m[4]* b.m[i+1]+a.m[8]*b.m[i+2] +  a.m[12]*b.m[i+3];
			r.m[i+1] = a.m[1]*b.m[i] + a.m[5]*b.m[i+1] + a.m[9]*b.m[i+2] + a.m[13]*b.m[i+3];
			r.m[i+2] = a.m[2]*b.m[i] + a.m[6]*b.m[i+1] + a.m[10]*b.m[i+2] + a.m[14]*b.m[i+3];
			r.m[i+3] = a.m[3]*b.m[i] + a.m[7]*b.m[i+1] + a.m[11]*b.m[i+2] + a.m[15]*b.m[i+3];
		}//nexti
     */ 
        r[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
				r[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
				r[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
				r[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
        
        r[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
				r[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
				r[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
				r[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
        
        r[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
				r[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
				r[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
				r[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
        
        r[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
				r[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
				r[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
				r[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
		
	}//multmat



	//methods from Lighthouse3D's VSML
	//matrix functions that do the same as the opengl ones
	//namely gluPerspective
	//http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
	//http://www.lighthouse3d.com/very-simple-libs/vsl-downloads/

	inline void setPerspective(float fov, float aspect, float nearp, float farp)
	{
	
		reset();

		float f = 1.0f / tanf( (fov*(3.1415f/180.f))*0.5f );

			m[0] = f / aspect;
			m[5] = f;
			m[10] = (farp + nearp) / (nearp - farp);
			m[14] = (2.0f * farp * nearp) / (nearp - farp);
			m[11] = -1.0f;
			m[15] = 0.0f;
	
	}//setperspective


 inline void setView(cVec3f *pos, cQuat *ori)
 {

 
   // float * m;
 		gamex::cVec3f up;
		gamex::cVec3f forw; 
		gamex::cVec3f side;
  
   // m = mat.m;
  
    ori->setVecFront(forw);
    ori->setVecSide(side);
    ori->setVecUp(up);
  
    //identity matrix
			memset(m, 0, 64); //16*4 -- 16 * 4byte
			m[0] = m[5] = m[10] = m[15] = 1.0f; 

			m[0] = side.x;			  m[4] = side.y;			  m[8] = side.z;
			
			m[1] = up.x;			  m[5] = up.y;			  m[9] = up.z;

			m[2] = forw.x;			  m[6] = forw.y;			  m[10] = forw.z;


    //do the same as the result of 
		// glMultMatrixf(m);
		//    glTranslatef(-posx, -posy, -posz);

		//do the work of  glTranslatef(-posx, -posy, -posz);
			m[12] = (side.x *-pos->x) + (side.y * -pos->y) + (side.z * -pos->z);
			m[13] = (up.x *-pos->x) + (up.y * -pos->y) + (up.z * -pos->z);
			m[14] = (forw.x *-pos->x) + (forw.y * -pos->y) + (forw.z * -pos->z);

 }//setmodelview2

        
       



	//safe to use on self i guess //todo: test

	//returns 0 if unsuccesful
	int getInverted(gamex::cMat &inv)
	{

	//source
	//http://www.codng.com/2011/02/gluunproject-for-iphoneios.html

		float temp[16];
		float det;
		int i;

		temp[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
				 + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
		temp[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
				 - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
		temp[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
				 + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
		temp[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
				 - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
		temp[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
				 - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
		temp[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
				 + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
		temp[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
				 - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
		temp[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
				 + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
		temp[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
				 + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
		temp[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
				 - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
		temp[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
				 + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
		temp[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
				 - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
		temp[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
				 - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
		temp[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
				 + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
		temp[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
				 - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
		temp[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
				 + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

		det = m[0]*temp[0] + m[1]*temp[4] + m[2]*temp[8] + m[3]*temp[12];
  
		if (det == 0)   return 0;

		det = 1.0f / det;
 
		for (i = 0; i < 16; i++)
		{ 
			inv.m[i] = temp[i] * det; 
		}//nexti

		return 1;
	}//invert



}; //matrix




};//gamex




/*
       | 0  1  2  3  |            | 0  4  8  12 |
        |             |            |             |
        | 4  5  6  7  |            | 1  5  9  13 |
    M = |             |        M = |             |
        | 8  9  10 11 |            | 2  6  10 14 |
        |             |            |             |
        | 12 13 14 15 |            | 3  7  11 15 |

*/
