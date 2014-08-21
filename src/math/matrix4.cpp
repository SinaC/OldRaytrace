#include <stdio.h>

#include "matrix4.h"
#include "../error.h"

const TMatrix4 TMatrix4::Identity = { 1, 0, 0, 0,
				      0, 1, 0, 0,
				      0, 0, 1, 0,
				      0, 0, 0, 1 };

TVector4 TMatrix4::getRow( const int i ) const {
  return TVector4( m[i][0], m[i][1], m[i][2], m[i][3] );
}

TVector4 TMatrix4::getColumn( const int i ) const {
  return TVector4( m[0][i], m[1][i], m[2][i], m[3][i] );
}

void TMatrix4::setRow( const int i, const TVector4 &v ) {
  m[i][0] = (float)v[0]; m[i][1] = (float)v[1]; m[i][2] = (float)v[2]; m[i][3] = (float)v[3];
}

void TMatrix4::setColumn( const int i, const TVector4 &v ) {
  m[0][i] = (float)v[0]; m[1][i] = (float)v[1]; m[2][i] = (float)v[2]; m[3][i] = (float)v[3];
}

TMatrix4 TMatrix4::operator* ( const TMatrix4 &mat ) const {
  TMatrix4 mRet;
  for ( int i = 0 ; i < 4 ; i++ )
    for ( int j = 0 ; j < 4 ; j++ ) {
      mRet.m[i][j] = 0.0f;
      for ( int k = 0 ; k < 4 ; k++ )
        mRet.m[i][j] += m[i][k] * mat.m[k][j];
    }
  return mRet;
}

void TMatrix4::transpose() {
#define SWAP(a,b,c) c=a; a=b; b=c
  float f;
  SWAP( m[0][1], m[1][0], f );
  SWAP( m[0][2], m[2][0], f );
  SWAP( m[0][3], m[3][0], f );
  SWAP( m[1][2], m[2][1], f );
  SWAP( m[1][3], m[3][1], f );
  SWAP( m[2][3], m[3][2], f );
#undef SWAP
}

void TMatrix4::transpose( const TMatrix4 &mat ) {
  for ( int i = 0 ; i < 4 ; i++ )
    for ( int j = 0 ; j < 4 ; j++ )
      m[i][j] = mat.m[j][i];
}

bool TMatrix4::invert() {
  TVector4 x  = getColumn( 0 );
  TVector4 y  = getColumn( 1 );
  TVector4 z  = getColumn( 2 );
  TVector4 w  = getColumn( 3 );
  TVector4 x1 = crossProduct( y, z, w );
  TVector4 y1 = crossProduct( z, w, x );
  TVector4 z1 = crossProduct( w, x, y );
  TVector4 w1 = crossProduct( x, y, z );
  float lx = x | x1;
  float ly = y | y1;
  float lz = z | z1;
  float lw = w | w1;
  
  if ( !lx || !ly || !lz || !lw ) {
    g_error("TMatrix4::invert(): Singular matrix.\n");
    return false;
  }
  
  x1 *= 1.0f/lx;
  y1 *= 1.0f/ly;
  z1 *= 1.0f/lz;
  w1 *= 1.0f/lw;
  
  setRow( 0, x1 );
  setRow( 1, y1 );
  setRow( 2, z1 );
  setRow( 3, w1 );

  return true;
}

bool TMatrix4::invert( const TMatrix4 &mat ) {
  float d00, d01, d02, d03;
  float d10, d11, d12, d13;
  float d20, d21, d22, d23;
  float d30, d31, d32, d33;
  float m00, m01, m02, m03;
  float m10, m11, m12, m13;
  float m20, m21, m22, m23;
  float m30, m31, m32, m33;
  float D;

  m00 = mat.m[0][0];  m01 = mat.m[0][1];  m02 = mat.m[0][2];  m03 = mat.m[0][3];
  m10 = mat.m[1][0];  m11 = mat.m[1][1];  m12 = mat.m[1][2];  m13 = mat.m[1][3];
  m20 = mat.m[2][0];  m21 = mat.m[2][1];  m22 = mat.m[2][2];  m23 = mat.m[2][3];
  m30 = mat.m[3][0];  m31 = mat.m[3][1];  m32 = mat.m[3][2];  m33 = mat.m[3][3];

  d00 = m11*m22*m33 + m12*m23*m31 + m13*m21*m32 - m31*m22*m13 - m32*m23*m11 - m33*m21*m12;
  d01 = m10*m22*m33 + m12*m23*m30 + m13*m20*m32 - m30*m22*m13 - m32*m23*m10 - m33*m20*m12;
  d02 = m10*m21*m33 + m11*m23*m30 + m13*m20*m31 - m30*m21*m13 - m31*m23*m10 - m33*m20*m11;
  d03 = m10*m21*m32 + m11*m22*m30 + m12*m20*m31 - m30*m21*m12 - m31*m22*m10 - m32*m20*m11;

  d10 = m01*m22*m33 + m02*m23*m31 + m03*m21*m32 - m31*m22*m03 - m32*m23*m01 - m33*m21*m02;
  d11 = m00*m22*m33 + m02*m23*m30 + m03*m20*m32 - m30*m22*m03 - m32*m23*m00 - m33*m20*m02;
  d12 = m00*m21*m33 + m01*m23*m30 + m03*m20*m31 - m30*m21*m03 - m31*m23*m00 - m33*m20*m01;
  d13 = m00*m21*m32 + m01*m22*m30 + m02*m20*m31 - m30*m21*m02 - m31*m22*m00 - m32*m20*m01;

  d20 = m01*m12*m33 + m02*m13*m31 + m03*m11*m32 - m31*m12*m03 - m32*m13*m01 - m33*m11*m02;
  d21 = m00*m12*m33 + m02*m13*m30 + m03*m10*m32 - m30*m12*m03 - m32*m13*m00 - m33*m10*m02;
  d22 = m00*m11*m33 + m01*m13*m30 + m03*m10*m31 - m30*m11*m03 - m31*m13*m00 - m33*m10*m01;
  d23 = m00*m11*m32 + m01*m12*m30 + m02*m10*m31 - m30*m11*m02 - m31*m12*m00 - m32*m10*m01;

  d30 = m01*m12*m23 + m02*m13*m21 + m03*m11*m22 - m21*m12*m03 - m22*m13*m01 - m23*m11*m02;
  d31 = m00*m12*m23 + m02*m13*m20 + m03*m10*m22 - m20*m12*m03 - m22*m13*m00 - m23*m10*m02;
  d32 = m00*m11*m23 + m01*m13*m20 + m03*m10*m21 - m20*m11*m03 - m21*m13*m00 - m23*m10*m01;
  d33 = m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m20*m11*m02 - m21*m12*m00 - m22*m10*m01;

  D = m00*d00 - m01*d01 + m02*d02 - m03*d03;

  if ( D == 0.0f ) {
    g_error("TMatrix4::invert( TMatrix4 & ): Singular matrix.\n");
    return false;
  }

  float invD = 1.0f / D;

  m[0][0] =  d00*invD; m[0][1] = -d10*invD;  m[0][2] =  d20*invD; m[0][3] = -d30*invD;
  m[1][0] = -d01*invD; m[1][1] =  d11*invD;  m[1][2] = -d21*invD; m[1][3] =  d31*invD;
  m[2][0] =  d02*invD; m[2][1] = -d12*invD;  m[2][2] =  d22*invD; m[2][3] = -d32*invD;
  m[3][0] = -d03*invD; m[3][1] =  d13*invD;  m[3][2] = -d23*invD; m[3][3] =  d33*invD;
  
  return true;
}

void TMatrix4::print( const char *msg ) const {
  printf("%s\n", msg );
  for ( int j = 0; j < 4; j++ ) {
    for ( int i = 0; i < 4; i++ )
      printf("%10.5f  ", m[i][j] );
    printf("\n");
  }
}
