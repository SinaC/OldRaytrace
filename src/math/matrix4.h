#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "vector3.h"
#include "vector4.h"

struct TMatrix4 {
  //----- Datas
  static const TMatrix4 Identity;
  float m[4][4];

  //----- Methods
  operator float*()                                    { return &m[0][0]; }
  float &operator() ( const int i, const int j )       { return m[i][j]; }
  operator const float*() const                        { return &m[0][0]; }
  float  operator() ( const int i, const int j ) const { return m[i][j]; }

  TVector4 getRow( const int i ) const;
  TVector4 getColumn( const int i ) const;
  void setRow( const int i, const TVector4 &v );
  void setColumn( const int i, const TVector4 &v );

  void operator=( const TMatrix4 &mat ) {
    const float *from = &(mat.m[0][0]);
    float *to = &(m[0][0]);
    for ( register int i=0; i<16; i++ ) *(to++) = *(from++);
  }
  void operator=( const float *pf ) {
    const float *from = pf;
    float *to = &(m[0][0]);
    for ( register int i=0; i<16; i++ ) *(to++) = *(pf++);
  }
  void operator=( const float f ) {
    float *to = &(m[0][0]);
    for ( register int i=0; i<16; i++ ) *(to++) = f;
  }

  TMatrix4 operator* ( const TMatrix4  &mat ) const;
  const TMatrix4  &operator*=( const TMatrix4  &mat ) { *this = *this * mat; return *this; }

  void transpose();
  void transpose( const TMatrix4 &mat );

  bool invert();
  bool invert( const TMatrix4 &mat );

  void print( const char *msg ) const;
};

#endif
