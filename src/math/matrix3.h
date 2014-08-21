#ifndef __MATH_MATRIX3_H__
#define __MATH_MATRIX3_H__

#include "math.h"
#include "vector3.h"

/*!
  This class implements a 3x3 matrix. Several functions and operators are
  defined to make working with matrices easier. The values are kept in column-
  major order to make it easier to use with OpenGL.
	
  Note: This class is not templatized because integral data types don't make sense
  and there's no need for double-precision.
*/
class TMatrix3 {
 public:
  static const TMatrix3 IDENTITY;
  static const TMatrix3 SWAP_XY;
  static const TMatrix3 SWAP_XZ;
  static const TMatrix3 SWAP_YZ;
  static const TMatrix3 MIRROR_X;
  static const TMatrix3 MIRROR_Y;
  static const TMatrix3 MIRROR_Z;
 public:
  // This class uses column-major order, as used by OpenGL
  // | m[0][0] m[1][0] m[2][0]
  // | m[0][1] m[1][1] m[2][1]
  // | m[0][2] m[1][2] m[2][2]
  float m[3][3];

  TMatrix3(){}
  TMatrix3( const float       f  ){ *this = f;  }
  TMatrix3( const float      *pf ){ *this = pf; }

  // Init functions
  void zeroMatrix(){
    m[0][0] = m[0][1] = m[0][2] =
      m[1][0] = m[1][1] = m[1][2] =
      m[2][0] = m[2][1] = m[2][2] = 0;
  }

  operator     float* ()                                    { return &m[0][0]; }
  float &operator            () ( const int i, const int j )       { return m[i][j]; }
  operator const float*  ()                           const { return &m[0][0]; }
  float  operator            () ( const int i, const int j ) const { return m[i][j]; }

  void operator=( const float k ){
    float *f;

    for( f = &m[0][0]; f != (float *)m+9; f++ ){
      *f = k;
    }
  }

  void operator=( const float *pf ){
    float *to = &m[0][0];

    for( register int i=0; i<9; i++ ){
      to[i] = pf[i];
    }
  }

  TMatrix3 operator-() const {
    TMatrix3       mat;
    const float *from = &    m[0][0];
    float       *to   = &mat.m[0][0];
    
    for( register int i=0; i<9; i++ ){
      to[i] = -from[i];
    }
    return mat;
  }

  TMatrix3   operator* ( const TMatrix3  &mat ) const;
  const TMatrix3  &operator*=( const TMatrix3  &mat ){ *this = *this * mat; return *this; }
  TVector3  operator* ( const TVector3 &vec ) const {
    return TVector3( (m[0][0]*vec.v[0] + m[1][0]*vec.v[1] + m[2][0]*vec.v[2]),
		     (m[0][1]*vec.v[0] + m[1][1]*vec.v[1] + m[2][1]*vec.v[2]),
		     (m[0][2]*vec.v[0] + m[1][2]*vec.v[1] + m[2][2]*vec.v[2]) ); }
  
  TVector3 getRow    ( const int i                   ) const { return TVector3( m[i][0], m[i][1], m[i][2] ); }
  TVector3 getColumn ( const int i                   ) const { return TVector3( m[0][i], m[1][i], m[2][i] ); }
  void    setRow    ( const int i, const TVector3 &v )       { m[i][0] = (float)v[0]; m[i][1] = (float)v[1]; m[i][2] = (float)v[2]; }
  void    setColumn ( const int i, const TVector3 &v )       { m[0][i] = (float)v[0]; m[1][i] = (float)v[1]; m[2][i] = (float)v[2]; }

  void print( const char *msg ) const;
};

TMatrix3 rotateMatrix( const float th, const TVector3 & ax );

#endif
