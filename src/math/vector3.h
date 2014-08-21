#ifndef __MATH_VECTOR3_H__
#define __MATH_VECTOR3_H__

// 3-dimentional vector/point

#include "math.h"

struct TVector3 {
 public:
static const TVector3 zeroVector;
static const TVector3 oneVector;
static const TVector3 twoVector;

  union {
    struct { float v[3]; };
    struct { float x, y, z; };
  };

  //  Constructors
  TVector3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {
    // NOP
  }
  TVector3( const float a, const float b, const float c ) {
    v[0] = a;
    v[1] = b;
    v[2] = c;
  }
  TVector3( const float t ) {
    *this = t;
  }
  TVector3( const float *pt ) {
    *this = pt;
  }
  TVector3( const TVector3 &vec ) {
    *this = vec;
  }

  //  Casting and unary operators
  inline operator float* () {
    return v;
  }
  inline float &operator [] ( const int n ) {
    return v[n];
  }
  inline operator const float* () const {
    return v;
  }
  inline float operator [] ( const int n ) const {
    return v[n];
  }
  inline TVector3  operator - () const {
    return TVector3(-v[0], -v[1], -v[2]);
  }

  //  Equal and comparison operators
  inline void operator =( const float t ) {
    v[0] = v[1] = v[2] = t;
  }
  inline void operator =( const float *pt ) {
    v[0] = pt[0]; v[1] = pt[1]; v[2] = pt[2];
  }
  inline void operator =( const TVector3 &vec ) {
    v[0] = vec.v[0]; v[1] = vec.v[1]; v[2] = vec.v[2];
  }
  
  //  Magnitude/normalize methods
  inline float magnitudeSquared() const {
    return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
  }
  inline float magnitude () const {
    return sqrtf( magnitudeSquared() );
  }
  inline void normalize() {
    float oneOverSqrt = fastInvSqrt(magnitudeSquared());
    v[0] *= oneOverSqrt;
    v[1] *= oneOverSqrt;
    v[2] *= oneOverSqrt;
  }

  // Print method
  void print( const char *msg ) const;
};

//  Arithmetic operators (vector with scalar)
inline TVector3  operator+ ( const TVector3 &v, const float t ) {
  return TVector3(v.v[0]+t, v.v[1]+t, v.v[2]+t);
}
inline TVector3  operator- ( const TVector3 &v, const float t ) {
  return TVector3(v.v[0]-t, v.v[1]-t, v.v[2]-t);
}
inline TVector3  operator* ( const TVector3 &v, const float t ) {
  return TVector3(v.v[0]*t, v.v[1]*t, v.v[2]*t);
}
inline TVector3  operator/ ( const TVector3 &v, const float t ) {
  float oneOverT = 1.0f/t;
  return TVector3(v.v[0]*oneOverT, v.v[1]*oneOverT, v.v[2]*oneOverT);
}
inline void operator+=( TVector3 &v, const float t ) {
  v.v[0] += t; v.v[1] += t; v.v[2] += t;
}
inline void operator-=( TVector3 &v, const float t ) {
  v.v[0] -= t; v.v[1] -= t; v.v[2] -= t;
}
inline void operator*=( TVector3 &v, const float t ) {
  v.v[0] *= t; v.v[1] *= t; v.v[2] *= t;
}
inline void operator/=( TVector3 &v, const float t ) {
  float invT = 1.0f/t;
  v.v[0] *= invT; v.v[1] *= invT; v.v[2] *= invT;
}

//  Arithmetic operators (vector with vector)
inline TVector3  operator+ ( const TVector3 &vec1, const TVector3 &vec2 ) {
  return TVector3(vec1.v[0]+vec2.v[0], vec1.v[1]+vec2.v[1], vec1.v[2]+vec2.v[2]);
}
inline TVector3  operator- ( const TVector3 &vec1, const TVector3 &vec2 ) {
  return TVector3(vec1.v[0]-vec2.v[0], vec1.v[1]-vec2.v[1], vec1.v[2]-vec2.v[2]);
}
inline TVector3  operator* ( const TVector3 &vec1, const TVector3 &vec2 ) {
  return TVector3(vec1.v[0]*vec2.v[0], vec1.v[1]*vec2.v[1], vec1.v[2]*vec2.v[2]);
}
inline TVector3  operator/ ( const TVector3 &vec1, const TVector3 &vec2 ) {
  return TVector3(vec1.v[0]/vec2.v[0], vec1.v[1]/vec2.v[1], vec1.v[2]/vec2.v[2]);
}
inline void operator+=( TVector3 &vec1, const TVector3 &vec2 ) {
  vec1.v[0] += vec2.v[0]; vec1.v[1] += vec2.v[1]; vec1.v[2] += vec2.v[2];
}
inline void operator-=( TVector3 &vec1, const TVector3 &vec2 ) {
  vec1.v[0] -= vec2.v[0]; vec1.v[1] -= vec2.v[1]; vec1.v[2] -= vec2.v[2];
}
inline void operator*=( TVector3 &vec1, const TVector3 &vec2 ) {
  vec1.v[0] *= vec2.v[0]; vec1.v[1] *= vec2.v[1]; vec1.v[2] *= vec2.v[2];
}
inline void operator/=( TVector3 &vec1, const TVector3 &vec2 ) {
  vec1.v[0] /= vec2.v[0]; vec1.v[1] /= vec2.v[1]; vec1.v[2] /= vec2.v[2];
}

//  Dot product
inline float operator| ( const TVector3 &vec1, const TVector3 &vec2 ) {
  return vec1.v[0]*vec2.v[0] + vec1.v[1]*vec2.v[1] + vec1.v[2]*vec2.v[2];
}

#define TPoint3          TVector3

inline TVector3 crossProduct( const TVector3 &v1, const TVector3 &v2 ) {
  return TVector3(
		 v1[__Y] * v2[__Z] - v1[__Z] * v2[__Y],
		 v1[__Z] * v2[__X] - v1[__X] * v2[__Z],
		 v1[__X] * v2[__Y] - v1[__Y] * v2[__X] );
}

inline TVector3 reflectVector( const TVector3 &v, const TVector3 &normal ) {
  // reflection direction is   dir - 2*normal * ( normal . dir )
  //                           vector - vector * scalar
  return v - ( normal * ( ( ( normal | v ) * 2.0f ) ) );
  //  vector -   vector *           scalar
}

inline TVector3 inverseVector( const TVector3 &v ) {
  return TVector3( 1.0f/v[0], 1.0f/v[1], 1.0f/v[2] );
}

inline void generateOrthonormalBasis( TVector3 &U, TVector3 &V, TVector3 &W, const bool unitLengthW ) {
  if ( !unitLengthW )
    W.normalize();
  if ( fabsf(W[0]) >= fabsf(W[1]) ) {
    // W.x or W.z is the largest magnitude component, swap them
    float invLength = fastInvSqrt(W[0]*W[0]+W[2]*W[2]);
    U[0] = -W[2] * invLength;
    U[1] = 0.0f;
    U[2] = +W[0] * invLength;
  }
  else {
    // W.y or W.z is the largest magnitude component, swap them
    float invLength = fastInvSqrt(W[1]*W[1]+W[2]*W[2]);
    U[0] = 0.0f;
    U[1] = +W[2] * invLength;
    U[2] = -W[1] * invLength;
  }
  V = crossProduct( W, U );
}

#endif  //  __VECTOR3_H__
