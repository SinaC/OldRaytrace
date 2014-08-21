#ifndef __MATH_VECTOR2_H__
#define __MATH_VECTOR2_H__

// 2-dimentional vector/point

#include "math.h"

// Complex may be considered as a vector2
#define TComplex      TVector2

struct TVector2 {
 public:
static const TVector2 zeroVector;
static const TVector2 oneVector;
static const TVector2 twoVector;

  float v[2];

  //  Constructors
  TVector2() {
    // NOP
  }
  TVector2( const float a, const float b ) {
    v[0] = a;
    v[1] = b;

  }
  TVector2( const float t ) {
    *this = t;
  }
  TVector2( const float *pt ) {
    *this = pt;
  }
  TVector2( const TVector2 &vec ) {
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
  inline TVector2  operator - () const {
    return TVector2(-v[0], -v[1]);
  }

  //  Equal and comparison operators
  inline void operator =( const float t ) {
    v[0] = v[1] = t;
  }
  inline void operator =( const float *pt ) {
    v[0] = pt[0]; v[1] = pt[1];
  }
  inline void operator =( const TVector2 &vec ) {
    v[0] = vec.v[0]; v[1] = vec.v[1];
  }
  
  //  Magnitude/normalize methods
  inline float magnitudeSquared() const {
    return v[0]*v[0] + v[1]*v[1];
  }
  inline float magnitude () const {
    return sqrtf( magnitudeSquared() );
  }
  inline void normalize() {
    float oneOverSqrt = fastInvSqrt(magnitudeSquared());
    v[0] *= oneOverSqrt;
    v[1] *= oneOverSqrt;
  }

  // TComplex may be considered as a TVector2
  static void exp( TComplex &target, const TComplex &source );
  static void log( TComplex &target, const TComplex &source );
  static void sin( TComplex &target, const TComplex &source );
  static void asin( TComplex &target, const TComplex &source );
  static void sinh( TComplex &target, const TComplex &source );
  static void asinh( TComplex &target, const TComplex &source );
  static void cos( TComplex &target, const TComplex &source );
  static void acos( TComplex &target, const TComplex &source );
  static void cosh( TComplex &target, const TComplex &source );
  static void acosh( TComplex &target, const TComplex &source );
  static void tan( TComplex &target, const TComplex &source );
  static void atan( TComplex &target, const TComplex &source );
  static void tanh( TComplex &target, const TComplex &source );
  static void atanh( TComplex &target, const TComplex &source );
  static void sqrt( TComplex &target, const TComplex &source );
  static void pwr( TComplex &target, const TComplex &source );
  static void mult( TComplex &target, const TComplex &source1, const TComplex &source2 );
  static void div( TComplex &target, const TComplex &source1, const TComplex &source2 );
  static void power( TComplex &target, const TComplex &source1, const TComplex &source2 );

  // Print method
  void print( const char *msg ) const;
};

extern TComplex complexExponent;

//  Arithmetic operators (vector with scalar)
inline TVector2  operator+ ( const TVector2 &v, const float t ) {
  return TVector2(v.v[0]+t, v.v[1]+t);
}
inline TVector2  operator- ( const TVector2 &v, const float t ) {
  return TVector2(v.v[0]-t, v.v[1]-t);
}
inline TVector2  operator* ( const TVector2 &v, const float t ) {
  return TVector2(v.v[0]*t, v.v[1]*t);
}
inline TVector2  operator/ ( const TVector2 &v, const float t ) {
  float oneOverT = 1.0f/t;
  return TVector2(v.v[0]*oneOverT, v.v[1]*oneOverT);
}
inline void operator+=( TVector2 &v, const float t ) {
  v.v[0] += t; v.v[1] += t;
}
inline void operator-=( TVector2 &v, const float t ) {
  v.v[0] -= t; v.v[1] -= t;
}
inline void operator*=( TVector2 &v, const float t ) {
  v.v[0] *= t; v.v[1] *= t;
}
inline void operator/=( TVector2 &v, const float t ) {
  float invT = 1.0f/t;
  v.v[0] *= invT; v.v[1] *= invT;
}

//  Arithmetic operators (vector with vector)
inline TVector2  operator+ ( const TVector2 &vec1, const TVector2 &vec2 ) {
  return TVector2(vec1.v[0]+vec2.v[0], vec1.v[1]+vec2.v[1]);
}
inline TVector2  operator- ( const TVector2 &vec1, const TVector2 &vec2 ) {
  return TVector2(vec1.v[0]-vec2.v[0], vec1.v[1]-vec2.v[1]);
}
inline TVector2  operator* ( const TVector2 &vec1, const TVector2 &vec2 ) {
  return TVector2(vec1.v[0]*vec2.v[0], vec1.v[1]*vec2.v[1]);
}
inline TVector2  operator/ ( const TVector2 &vec1, const TVector2 &vec2 ) {
  return TVector2(vec1.v[0]/vec2.v[0], vec1.v[1]/vec2.v[1]);
}
inline void operator+=( TVector2 &vec1, const TVector2 &vec2 ) {
  vec1.v[0] += vec2.v[0]; vec1.v[1] += vec2.v[1];
}
inline void operator-=( TVector2 &vec1, const TVector2 &vec2 ) {
  vec1.v[0] -= vec2.v[0]; vec1.v[1] -= vec2.v[1];
}
inline void operator*=( TVector2 &vec1, const TVector2 &vec2 ) {
  vec1.v[0] *= vec2.v[0]; vec1.v[1] *= vec2.v[1];
}
inline void operator/=( TVector2 &vec1, const TVector2 &vec2 ) {
  vec1.v[0] /= vec2.v[0]; vec1.v[1] /= vec2.v[1];
}

//  Dot product
inline float operator| ( const TVector2 &vec1, const TVector2 &vec2 ) {
  return vec1.v[0]*vec2.v[0] + vec1.v[1]*vec2.v[1];
}

#define TPoint2          TVector2

// FIXME
inline TVector2 crossProduct( const TVector2 &v1, const TVector2 &v2 ) {
  return TVector2(v1[0]*v2[1],v1[1]*v2[0]);
}

inline TVector2 inverseVector( const TVector2 &v ) {
  return TVector2( 1.0f/v[0], 1.0f/v[1] );
}


#endif  //  __VECTOR2_H__
