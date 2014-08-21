#ifndef __MATH_VECTOR4_H__
#define __MATH_VECTOR4_H__

// 4-dimentional vector/point

#include "math.h"

#include "vector3.h"

struct TVector4 {
 public:
static const TVector4 zeroVector;
static const TVector4 oneVector;
static const TVector4 twoVector;

  float v[4];

 public:
  //  Constructors
  TVector4(){}

  TVector4( const float a, const float b, const float c, const float d ){
    v[0] = a;
    v[1] = b;
    v[2] = c;
    v[3] = d;
  }

  TVector4( const float            t              ){ *this = t;   }
  TVector4( const float           *pt             ){ *this = pt;  }
  TVector4( const TVector4 &vec            ){ *this = vec; }
  TVector4( const TVector3 &vec            ) { *this = TVector4( vec.v[0], vec.v[1], vec.v[2], 1.0f ); }
  TVector4( const TVector3 &vec, const float f ) { *this = TVector4( vec.v[0], vec.v[1], vec.v[2], f ); }

  //  Casting and unary operators
  operator       float* ()                    { return v; }
  float           &operator       [] ( const int n )       { return v[n]; }
  operator const float* ()              const { return v; }
  float            operator       [] ( const int n ) const { return v[n]; }
  TVector4  operator        - ()              const { return TVector4(-v[0], -v[1], -v[2], -v[3]); }

  //  Equal and comparison operators
  void operator =( const float            t   )       { v[0] = v[1] = v[2] = v[3] = t; }
  void operator =( const float           *pt  )       { v[0] = pt[0]; v[1] = pt[1]; v[2] = pt[2]; v[3] = pt[3];}
  void operator =( const TVector4 &vec )       { v[0] = vec.v[0]; v[1] = vec.v[1]; v[2] = vec.v[2]; v[3] = vec.v[3]; }
  void operator =( const TVector3 &vec )       { v[0] = vec.v[0]; v[1] = vec.v[1]; v[2] = vec.v[2]; v[3] = 0.0f; }

//--  //  Arithmetic operators (vector with scalar)
//--  TVector4  operator+ ( const float t ) const { return TVector4(v[0]+t, v[1]+t, v[2]+t, v[3]+t); }
//--  TVector4  operator- ( const float t ) const { return TVector4(v[0]-t, v[1]-t, v[2]-t, v[3]-t); }
//--  TVector4  operator* ( const float t ) const { return TVector4(v[0]*t, v[1]*t, v[2]*t, v[3]*t); }
//--  TVector4  operator/ ( const float t ) const { const float it = 1.0f/t;return TVector4(v[0]*it, v[1]*it, v[2]*it, v[3]*it); }
//--  const TVector4 &operator+=( const float t )       { v[0] += t; v[1] += t; v[2] += t; v[3] += t; return *this; }
//--  const TVector4 &operator-=( const float t )       { v[0] -= t; v[1] -= t; v[2] -= t; v[3] -= t; return *this; }
//--  const TVector4 &operator*=( const float t )       { v[0] *= t; v[1] *= t; v[2] *= t; v[3] *= t; return *this; }
//--  const TVector4 &operator/=( const float t )       { float invT = 1/t; v[0] *= invT; v[1] *= invT; v[2] *= invT; v[3] *= invT; return *this; }
//--
//--  //  Arithmetic operators (vector with vector)
//--  TVector4  operator+ ( const TVector4 &vec ) const { return TVector4(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2], v[3]+vec.v[3]); }
//--  TVector4  operator- ( const TVector4 &vec ) const { return TVector4(v[0]-vec.v[0], v[1]-vec.v[1], v[2]-vec.v[2], v[3]-vec.v[3]); }
//--  TVector4  operator* ( const TVector4 &vec ) const { return TVector4(v[0]*vec.v[0], v[1]*vec.v[1], v[2]*vec.v[2], v[3]*vec.v[3]); }
//--  TVector4  operator/ ( const TVector4 &vec ) const { return TVector4(v[0]/vec.v[0], v[1]/vec.v[1], v[2]/vec.v[2], v[3]/vec.v[3]); }
//--  const TVector4 &operator+=( const TVector4 &vec )       { v[0] += vec.v[0]; v[1] += vec.v[1]; v[2] += vec.v[2]; v[3] += vec.v[3]; return *this; }
//--  const TVector4 &operator-=( const TVector4 &vec )       { v[0] -= vec.v[0]; v[1] -= vec.v[1]; v[2] -= vec.v[2]; v[3] -= vec.v[3]; return *this; }
//--  const TVector4 &operator*=( const TVector4 &vec )       { v[0] *= vec.v[0]; v[1] *= vec.v[1]; v[2] *= vec.v[2]; v[3] *= vec.v[3]; return *this; }
//--  const TVector4 &operator/=( const TVector4 &vec )       { v[0] /= vec.v[0]; v[1] /= vec.v[1]; v[2] /= vec.v[2]; v[3] /= vec.v[3]; return *this; }
//--
//--  //  Dot product
//--  inline float    operator| ( const TVector4 &vec ) const { return v[0]*vec[0] + v[1]*vec[1] + v[2]*vec[2] + v[3]*vec[3]; }

  //  Magnitude/normalize methods
  inline float    magnitudeSquared() const { return v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]; }
  inline float    magnitude       () const { return sqrtf( magnitudeSquared() ); }
  //inline void normalize       ()       { *this /= magnitude(); }
  inline void normalize() { 
    //--*this *= fastInvSqrt(magnitudeSquared());
    float oneOverSqrt = fastInvSqrt(magnitudeSquared());
    v[0] *= oneOverSqrt;
    v[1] *= oneOverSqrt;
    v[2] *= oneOverSqrt;
    v[3] *= oneOverSqrt;
  }
  inline void homogenize() { float divi = 1.0f/v[3]; v[0] *= divi; v[1] *= divi; v[2] *= divi; };

  // Print method
  void print( const char *msg ) const;
};

//  Arithmetic operators (vector with scalar)
inline TVector4  operator+ ( const TVector4 &v, const float t ) {
  return TVector4(v.v[0]+t, v.v[1]+t, v.v[2]+t, v.v[3]+t );
}
inline TVector4  operator- ( const TVector4 &v, const float t ) {
  return TVector4(v.v[0]-t, v.v[1]-t, v.v[2]-t, v.v[3]-t);
}
inline TVector4  operator* ( const TVector4 &v, const float t ) {
  return TVector4(v.v[0]*t, v.v[1]*t, v.v[2]*t, v.v[3]*t);
}
inline TVector4  operator/ ( const TVector4 &v, const float t ) {
  float oneOverT = 1.0f/t;
  return TVector4(v.v[0]*oneOverT, v.v[1]*oneOverT, v.v[2]*oneOverT, v.v[3]*oneOverT);
}
inline void operator+=( TVector4 &v, const float t ) {
  v.v[0] += t; v.v[1] += t; v.v[2] += t; v.v[3] += t;
}
inline void operator-=( TVector4 &v, const float t ) {
  v.v[0] -= t; v.v[1] -= t; v.v[2] -= t; v.v[3] -= t;
}
inline void operator*=( TVector4 &v, const float t ) {
  v.v[0] *= t; v.v[1] *= t; v.v[2] *= t; v.v[3] *= t;
}
inline void operator/=( TVector4 &v, const float t ) {
  float invT = 1.0f/t;
  v.v[0] *= invT; v.v[1] *= invT; v.v[2] *= invT; v.v[3] *= invT;
}

//  Arithmetic operators (vector with vector)
inline TVector4  operator+ ( const TVector4 &vec1, const TVector4 &vec2 ) {
  return TVector4(vec1.v[0]+vec2.v[0], vec1.v[1]+vec2.v[1], vec1.v[2]+vec2.v[2], vec1.v[3]+vec2.v[3]);
}
inline TVector4  operator- ( const TVector4 &vec1, const TVector4 &vec2 ) {
  return TVector4(vec1.v[0]-vec2.v[0], vec1.v[1]-vec2.v[1], vec1.v[2]-vec2.v[2], vec1.v[3]-vec2.v[3]);
}
inline TVector4  operator* ( const TVector4 &vec1, const TVector4 &vec2 ) {
  return TVector4(vec1.v[0]*vec2.v[0], vec1.v[1]*vec2.v[1], vec1.v[2]*vec2.v[2], vec1.v[3]*vec2.v[3]);
}
inline TVector4  operator/ ( const TVector4 &vec1, const TVector4 &vec2 ) {
  return TVector4(vec1.v[0]/vec2.v[0], vec1.v[1]/vec2.v[1], vec1.v[2]/vec2.v[2], vec1.v[3]/vec2.v[3]);
}
inline void operator+=( TVector4 &vec1, const TVector4 &vec2 ) {
  vec1.v[0] += vec2.v[0]; vec1.v[1] += vec2.v[1]; vec1.v[2] += vec2.v[2]; vec1.v[3] += vec2.v[3];
}
inline void operator-=( TVector4 &vec1, const TVector4 &vec2 ) {
  vec1.v[0] -= vec2.v[0]; vec1.v[1] -= vec2.v[1]; vec1.v[2] -= vec2.v[2]; vec1.v[3] -= vec2.v[3];
}
inline void operator*=( TVector4 &vec1, const TVector4 &vec2 ) {
  vec1.v[0] *= vec2.v[0]; vec1.v[1] *= vec2.v[1]; vec1.v[2] *= vec2.v[2]; vec1.v[3] *= vec2.v[3];
}
inline void operator/=( TVector4 &vec1, const TVector4 &vec2 ) {
  vec1.v[0] /= vec2.v[0]; vec1.v[1] /= vec2.v[1]; vec1.v[2] /= vec2.v[2]; vec1.v[3] /= vec2.v[3];
}

//  Dot product
inline float    operator| ( const TVector4 &vec1, const TVector4 &vec2 ) {
  return vec1.v[0]*vec2.v[0] + vec1.v[1]*vec2.v[1] + vec1.v[2]*vec2.v[2] + vec1.v[3]*vec2.v[3];
}


#define TPoint4          TVector4

inline TVector4 crossProduct( const TVector4 &r, const TVector4 &s, const TVector4 &t ) {
  return TVector4( r[1]*s[2]*t[3] + r[2]*s[3]*t[1] + r[3]*s[1]*t[2] - r[1]*s[3]*t[2] - r[2]*s[1]*t[3] - r[3]*s[2]*t[1],
		   r[0]*s[3]*t[2] + r[2]*s[0]*t[3] + r[3]*s[2]*t[0] - r[0]*s[2]*t[3] - r[2]*s[3]*t[0] - r[3]*s[0]*t[2],
		   r[0]*s[1]*t[3] + r[1]*s[3]*t[0] + r[3]*s[0]*t[1] - r[0]*s[3]*t[1] - r[1]*s[0]*t[3] - r[3]*s[1]*t[0],
		   r[0]*s[2]*t[1] + r[1]*s[0]*t[2] + r[2]*s[1]*t[0] - r[0]*s[1]*t[2] - r[1]*s[2]*t[0] - r[2]*s[0]*t[1] );
}

inline TVector4 crossProduct( const TVector4 &r, const TVector4 &s ) {
  return TVector4( r[1] * s[2] - r[2] * s[1],
		   r[2] * s[0] - r[0] * s[2],
		   r[0] * s[1] - r[1] * s[0],
		   0 );
}
#endif
