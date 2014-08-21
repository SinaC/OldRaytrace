#ifndef __MATH_MATRIX4_H__
#define __MATH_MATRIX4_H__


#if defined(_MSC_VER)
# pragma warning(disable:4786)
#endif


#include "math.h"
#include "vector4.h"

/*!
  This class implements a 4x4 matrix. Several functions and operators are
  defined to make working with matrices easier. The values are kept in column-
  major order to make it easier to use with OpenGL. For performance reasons,
  most of the functions assume that all matrices are orthogonal, which means the
  bottom row is [ 0 0 0 1 ]. Since I plan to use the GL_PROJECTION matrix to
  handle the projection matrix, I should never need to use any other kind of
  matrix, and I get a decent performance boost by ignoring the bottom row.
	
  Note: This class is not templatized because integral data types don't make sense
  and there's no need for double-precision.
*/
class TMatrix4 {
 public:
  static const TMatrix4 IDENTITY;
  static const TMatrix4 SWAP_XY;
  static const TMatrix4 SWAP_XZ;
  static const TMatrix4 SWAP_YZ;
  static const TMatrix4 MIRROR_X;
  static const TMatrix4 MIRROR_Y;
  static const TMatrix4 MIRROR_Z;
 public:
  // This class uses column-major order, as used by OpenGL
  // | m[0][0] m[1][0] m[2][0] m[3][0] |
  // | m[0][1] m[1][1] m[2][1] m[3][1] | 
  // | m[0][2] m[1][2] m[2][2] m[3][2] | 
  // | m[0][3] m[1][3] m[2][3] m[3][3] | 
  float m[4][4];

  TMatrix4(){}
  TMatrix4( const float       f  ){ *this = f;  }
  TMatrix4( const float      *pf ){ *this = pf; }
  //  TMatrix4( const Quaternion &q  ){ *this = q;  }

  // Init functions
  void zeroMatrix(){
    m[0][0] = m[0][1] = m[0][2] = m[0][3] =
      m[1][0] = m[1][1] = m[1][2] = m[1][3] =
      m[2][0] = m[2][1] = m[2][2] = m[2][3] =
      m[3][0] = m[3][1] = m[3][2] = m[3][3] = 0;
  }

  operator     float* ()                                    { return &m[0][0]; }
  float &operator            () ( const int i, const int j )       { return m[i][j]; }
  operator const float*  ()                           const { return &m[0][0]; }
  float  operator            () ( const int i, const int j ) const { return m[i][j]; }

  void operator=( const float k ){
    float *f;

    for( f = &m[0][0]; f != (float *)m+16; f++ ){
      *f = k;
    }
  }

  void operator=( const float *pf ){
    float *to = &m[0][0];

    for( register int i=0; i<16; i++ ){
      to[i] = pf[i];
    }
  }

  //  void   operator=( const Quaternion &q );

  TMatrix4 operator-() const {
    TMatrix4       mat;
    const float *from = &    m[0][0];
    float       *to   = &mat.m[0][0];
    
    for( register int i=0; i<16; i++ ){
      to[i] = -from[i];
    }
    return mat;
  }

  TMatrix4   operator* ( const TMatrix4  &mat ) const;
  const TMatrix4  &operator*=( const TMatrix4  &mat ){ *this = *this * mat; return *this; }
  TVector4  operator* ( const TVector4 &vec ) const { return transformVector(vec); }
  TVector3  operator* ( const TVector3 &vec ) const { return transformPoint3(vec); }

  TVector4 transformVector( const TVector4 &vec ) const {
    return TVector4(
		   (m[0][0]*vec.v[0] + m[1][0]*vec.v[1] + m[2][0]*vec.v[2] + m[3][0]*vec.v[3]),
		   (m[0][1]*vec.v[0] + m[1][1]*vec.v[1] + m[2][1]*vec.v[2] + m[3][1]*vec.v[3]),
		   (m[0][2]*vec.v[0] + m[1][2]*vec.v[1] + m[2][2]*vec.v[2] + m[3][2]*vec.v[3]),
		   (m[0][3]*vec.v[0] + m[1][3]*vec.v[1] + m[2][3]*vec.v[2] + m[3][3]*vec.v[3])
		   );
  }
  TVector3 transformPoint3( const TVector3 &vec ) const {
    return TVector3(
		   (m[0][0]*vec.v[0] + m[1][0]*vec.v[1] + m[2][0]*vec.v[2] + m[3][0] ),
		   (m[0][1]*vec.v[0] + m[1][1]*vec.v[1] + m[2][1]*vec.v[2] + m[3][1] ),
		   (m[0][2]*vec.v[0] + m[1][2]*vec.v[1] + m[2][2]*vec.v[2] + m[3][2] )
		   );
  }
  TVector3 transformVector3( const TVector3 &vec ) const {
    return TVector3(
		   (m[0][0]*vec.v[0] + m[1][0]*vec.v[1] + m[2][0]*vec.v[2]),
		   (m[0][1]*vec.v[0] + m[1][1]*vec.v[1] + m[2][1]*vec.v[2]),
		   (m[0][2]*vec.v[0] + m[1][2]*vec.v[1] + m[2][2]*vec.v[2])
		   );
  }
  TVector3 inverseTransformVector3( const TVector3 &vec ) const {
    return TVector3(
		   (m[0][0]*vec.v[0] + m[0][1]*vec.v[1] + m[0][2]*vec.v[2]),
		   (m[1][0]*vec.v[0] + m[1][1]*vec.v[1] + m[1][2]*vec.v[2]),
		   (m[2][0]*vec.v[0] + m[2][1]*vec.v[1] + m[2][2]*vec.v[2])
		   );
  }
  TVector3 inverseTransformPoint3( const TVector3 &vec ) const {
    return TVector3(
		   (m[0][0]*vec.v[0] + m[0][1]*vec.v[1] + m[0][2]*vec.v[2] + m[3][0] ),
		   (m[1][0]*vec.v[0] + m[1][1]*vec.v[1] + m[1][2]*vec.v[2] + m[3][1] ),
		   (m[2][0]*vec.v[0] + m[2][1]*vec.v[1] + m[2][2]*vec.v[2] + m[3][2] )
		   );
  }

  TVector4 getRow    ( const int i                   ) const { return TVector4( m[i][0], m[i][1], m[i][2], m[i][3] ); }
  TVector4 getColumn ( const int i                   ) const { return TVector4( m[0][i], m[1][i], m[2][i], m[3][i] ); }
  void    setRow    ( const int i, const TVector4 &v )       { m[i][0] = (float)v[0]; m[i][1] = (float)v[1]; m[i][2] = (float)v[2]; m[i][3] = (float)v[3]; }
  void    setColumn ( const int i, const TVector4 &v )       { m[0][i] = (float)v[0]; m[1][i] = (float)v[1]; m[2][i] = (float)v[2]; m[3][i] = (float)v[3]; }

  void setViewDistance( const float distance ) {
    float inverseDistance = 1.0f / distance;
    for( int counter = 0; counter < 4; counter++ )
      m[counter][3] = m[counter][2] * inverseDistance;
  }

  //  void modelMatrix( const Quaternion &q, const TVector4 &vFrom ){
  //    *this = q;
  //    m[3][0] = (float)vFrom.v[0];
  //    m[3][1] = (float)vFrom.v[1];
  //    m[3][2] = (float)vFrom.v[2];
  //  }
 
  void translateMatrix( const TVector4 &v ){
    // | 1  0  0  x |
    // | 0  1  0  y |
    // | 0  0  1  z |
    // | 0  0  0  1 |
    m[0][1] = m[0][2] = m[0][3] =
      m[1][0] = m[1][2] = m[1][3] =
      m[2][0] = m[2][1] = m[2][3] =
      0;
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
    m[3][0] = (float)v[0]; 
    m[3][1] = (float)v[1]; 
    m[3][2] = (float)v[2]; 
  }

  // Translate functions
  void translateMatrix( const float x, const float y, const float z ){
    // | 1  0  0  x |
    // | 0  1  0  y |
    // | 0  0  1  z |
    // | 0  0  0  1 |
    m[0][1] = m[0][2] = m[0][3] =
      m[1][0] = m[1][2] = m[1][3] =
      m[2][0] = m[2][1] = m[2][3] = 0;
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;
  }

  // Translate functions
  void translateMatrix( const TVector3 &v ){
    // | 1  0  0  x |
    // | 0  1  0  y |
    // | 0  0  1  z |
    // | 0  0  0  1 |
    m[0][1] = m[0][2] = m[0][3] =
      m[1][0] = m[1][2] = m[1][3] =
      m[2][0] = m[2][1] = m[2][3] = 0;
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
    m[3][0] = v[0];
    m[3][1] = v[1];
    m[3][2] = v[2];
  }

  //void translateMatrix( const float *pf ){ translateTMatrix4( pf[0], pf[1], pf[2] ); }

  void translate( const float x, const float y, const float z ){
//--    // 9 muls, 9 adds
//--    // | m[0][0] m[1][0] m[2][0] m[3][0] |   | 1  0  0  x |   | m[0][0] m[1][0] m[2][0] m[0][0]*x+m[1][0]*y+m[2][0]*z+m[3][0] |
//--    // | m[0][1] m[1][1] m[2][1] m[3][1] |   | 0  1  0  y |   | m[0][1] m[1][1] m[2][1] m[0][1]*x+m[1][1]*y+m[2][1]*z+m[3][1] |
//--    // | m[0][2] m[1][2] m[2][2] m[3][2] | * | 0  0  1  z | = | m[0][2] m[1][2] m[2][2] m[0][2]*x+m[1][2]*y+m[2][2]*z+m[3][2] |
//--    // | 0   0   0   1   |   | 0  0  0  1 |   | 0   0   0   1                     |
    m[3][0] = m[0][0]*x + m[1][0]*y + m[2][0]*z + m[3][0];
    m[3][1] = m[0][1]*x + m[1][1]*y + m[2][1]*z + m[3][1];
    m[3][2] = m[0][2]*x + m[1][2]*y + m[2][2]*z + m[3][2];
  }

  void translate( const TVector3 &v ) {
    // 9 muls, 9 adds
    // | m[0][0] m[1][0] m[2][0] m[3][0] |   | 1  0  0  x |   | m[0][0] m[1][0] m[2][0] m[0][0]*x+m[1][0]*y+m[2][0]*z+m[3][0] |
    // | m[0][1] m[1][1] m[2][1] m[3][1] |   | 0  1  0  y |   | m[0][1] m[1][1] m[2][1] m[0][1]*x+m[1][1]*y+m[2][1]*z+m[3][1] |
    // | m[0][2] m[1][2] m[2][2] m[3][2] | * | 0  0  1  z | = | m[0][2] m[1][2] m[2][2] m[0][2]*x+m[1][2]*y+m[2][2]*z+m[3][2] |
    // | 0   0   0   1   |   | 0  0  0  1 |   | 0   0   0   1                     |
    m[3][0] = m[0][0]*v[0] + m[1][0]*v[1] + m[2][0]*v[2] + m[3][0];
    m[3][1] = m[0][1]*v[0] + m[1][1]*v[1] + m[2][1]*v[2] + m[3][1];
    m[3][2] = m[0][2]*v[0] + m[1][2]*v[1] + m[2][2]*v[2] + m[3][2];
  }

  void translate( const float *pf ){
    translate( pf[0], pf[1], pf[2] );
  }

  // Scale functions
  void scaleMatrix( const float x, const float y, const float z ){
    // | x  0  0  0 |
    // | 0  y  0  0 |
    // | 0  0  z  0 |
    // | 0  0  0  1 |
    m[0][1] = m[0][2] = m[0][3] =
      m[1][0] = m[1][2] = m[1][3] =
      m[2][0] = m[2][1] = m[2][3] =
      m[3][0] = m[3][1] = m[3][2] = 0;
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    m[3][3] = 1;
  }

  void scaleMatrix( const float *pf ){
    scaleMatrix( pf[0], pf[1], pf[2] );
  }

  // Scale functions
  void scaleMatrix( const TVector3 &v ){
    // | x  0  0  0 |
    // | 0  y  0  0 |
    // | 0  0  z  0 |
    // | 0  0  0  1 |
    m[0][1] = m[0][2] = m[0][3] =
      m[1][0] = m[1][2] = m[1][3] =
      m[2][0] = m[2][1] = m[2][3] =
      m[3][0] = m[3][1] = m[3][2] = 0;
    m[0][0] = v[0];
    m[1][1] = v[1];
    m[2][2] = v[2];
    m[3][3] = 1;
  }

  void scale( const float x, const float y, const float z ){
    // 9 muls
    // | m[0][0] m[1][0] m[2][0] m[3][0] |   | x  0  0  0 |   | m[0][0]*x m[1][0]*y m[2][0]*z m[3][0] |
    // | m[0][1] m[1][1] m[2][1] m[3][1] |   | 0  y  0  0 |   | m[0][1]*x m[1][1]*y m[2][1]*z m[3][1] |
    // | m[0][2] m[1][2] m[2][2] m[3][2] | * | 0  0  z  0 | = | m[0][2]*x m[1][2]*y m[2][2]*z m[3][2] |
    // | 0   0   0   1   |   | 0  0  0  1 |   | 0     0     0     1   |
    m[0][0] *= x; m[1][0] *= y; m[2][0] *= z;
    m[0][1] *= x; m[1][1] *= y; m[2][1] *= z;
    m[0][2] *= x; m[1][2] *= y; m[2][2] *= z;
  }

  void scale( const TVector3 &v ) {
    // 9 muls
    // | m[0][0] m[1][0] m[2][0] m[3][0] |   | x  0  0  0 |   | m[0][0]*x m[1][0]*y m[2][0]*z m[3][0] |
    // | m[0][1] m[1][1] m[2][1] m[3][1] |   | 0  y  0  0 |   | m[0][1]*x m[1][1]*y m[2][1]*z m[3][1] |
    // | m[0][2] m[1][2] m[2][2] m[3][2] | * | 0  0  z  0 | = | m[0][2]*x m[1][2]*y m[2][2]*z m[3][2] |
    // | 0   0   0   1   |   | 0  0  0  1 |   | 0     0     0     1   |
    m[0][0] *= v[0]; m[1][0] *= v[1]; m[2][0] *= v[2];
    m[0][1] *= v[0]; m[1][1] *= v[1]; m[2][1] *= v[2];
    m[0][2] *= v[0]; m[1][2] *= v[1]; m[2][2] *= v[2];
  }

  void scale( const float *pf ){
    scale( pf[0], pf[1], pf[2] );
  }

  // Rotate functions
  void rotateMatrix( const TVector3 &angle ) {
    zeroMatrix();
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
    float cx = cosf( angle[0] ); float cy = cosf( angle[1] ); float cz = cosf( angle[2] );
    float sx = sinf( angle[0] ); float sy = sinf( angle[1] ); float sz = sinf( angle[2] );
    m[0][0] = cy*cz;            m[1][0] = -cy*sz;           m[2][0] = -sy;
    m[0][1] = cx*sz - sx*sy*cz; m[1][1] = cx*cz + sx*sy*sz; m[2][1] = -sx*cy;
    m[0][2] = sx*sz + cx*sy*cz; m[1][2] = sx*cz - cx*sy*sz; m[2][2] = cx*cy;
  }

  void rotateXMatrix( const float radians ){
    // | 1 0    0     0 |
    // | 0 fCos -fSin 0 |
    // | 0 fSin fCos  0 |
    // | 0 0    0     1 |
    m[0][1] = m[0][2] = m[0][3] = 
      m[1][0] = m[1][3] =
      m[2][0] = m[2][3] =
      m[3][0] = m[3][1] = m[3][2] = 0;
    m[0][0] = m[3][3] = 1;

    float fCos = cosf( radians );
    float fSin = sinf( radians );
    m[1][1] = m[2][2] = fCos;
    m[1][2] =  fSin;
    m[2][1] = -fSin;
  }

  void rotateX( const float radians ){
    // 12 muls, 6 adds, 2 trig function calls
    // | m[0][0] m[1][0] m[2][0] m[3][0] |   | 1 0    0     0 |   | m[0][0] m[1][0]*fCos+m[2][0]*fSin m[2][0]*fCos-m[1][0]*fSin m[3][0] |
    // | m[0][1] m[1][1] m[2][1] m[3][1] |   | 0 fCos -fSin 0 |   | m[0][1] m[1][1]*fCos+m[2][1]*fSin m[2][1]*fCos-m[1][1]*fSin m[3][1] |
    // | m[0][2] m[1][2] m[2][2] m[3][2] | * | 0 fSin fCos  0 | = | m[0][2] m[1][2]*fCos+m[2][2]*fSin m[2][2]*fCos-m[1][2]*fSin m[3][2] |
    // | 0   0   0   1   |   | 0 0    0     1 |   | 0   0                 0                 1   |
    float fTemp;
    float fCos = cosf( radians );
    float fSin = sinf( radians );

    fTemp   = m[1][0]*fCos + m[2][0]*fSin;
    m[2][0] = m[2][0]*fCos - m[1][0]*fSin;
    m[1][0] = fTemp;
    fTemp   = m[1][1]*fCos + m[2][1]*fSin;
    m[2][1] = m[2][1]*fCos - m[1][1]*fSin;
    m[1][1] = fTemp;
    fTemp   = m[1][2]*fCos + m[2][2]*fSin;
    m[2][2] = m[2][2]*fCos - m[1][2]*fSin;
    m[1][2] = fTemp;
  }

  void rotateYMatrix( const float radians ){
    // | fCos  0 fSin  0 |
    // | 0     1 0     0 |
    // | -fSin 0 fCos  0 |
    // | 0     0 0     1 |
    m[0][1] = m[0][3] = 
      m[1][0] = m[1][2] = m[1][3] = 
      m[2][1] = m[2][3] = 
      m[3][0] = m[3][1] = m[3][2] = 0;
    m[1][1] = m[3][3] = 1;

    float fCos = cosf( radians );
    float fSin = sinf( radians );
    m[0][0] = m[2][2] = fCos;
    m[0][2] = -fSin;
    m[2][0] =  fSin;
  }
  void rotateY( const float radians ){
    // 12 muls, 6 adds, 2 trig function calls
    // | m[0][0] m[1][0] m[2][0] m[3][0] |   | fCos  0 fSin  0 |   | m[0][0]*fCos-m[2][0]*fSin m[1][0] m[0][0]*fSin+m[2][0]*fCos m[3][0] |
    // | m[0][1] m[1][1] m[2][1] m[3][1] |   | 0     1 0     0 |   | m[0][1]*fCos-m[2][1]*fSin m[1][1] m[0][1]*fSin+m[2][1]*fCos m[3][1] |
    // | m[0][2] m[1][2] m[2][2] m[3][2] | * | -fSin 0 fCos  0 | = | m[0][2]*fCos-m[2][2]*fSin m[1][2] m[0][2]*fSin+m[2][2]*fCos m[3][2] |
    // | 0   0   0   1   |   | 0     0 0     1 |   | 0                 0   0                 1   |
    float fTemp;
    float fCos = cosf( radians );
    float fSin = sinf( radians );
    fTemp   = m[0][0]*fCos - m[2][0]*fSin;
    m[2][0] = m[0][0]*fSin + m[2][0]*fCos;
    m[0][0] = fTemp;
    fTemp   = m[0][1]*fCos - m[2][1]*fSin;
    m[2][1] = m[0][1]*fSin + m[2][1]*fCos;
    m[0][1] = fTemp;
    fTemp   = m[0][2]*fCos - m[2][2]*fSin;
    m[2][2] = m[0][2]*fSin + m[2][2]*fCos;
    m[0][2] = fTemp;
  }
  void rotateZMatrix( const float radians ){
    // | fCos -fSin 0 0 |
    // | fSin fCos  0 0 |
    // | 0    0     1 0 |
    // | 0    0     0 1 |
    m[0][2] = m[0][3] =
      m[1][2] = m[1][3] =
      m[2][0] = m[2][1] = m[2][3] =
      m[3][0] = m[3][1] = m[3][2] = 0;
    m[2][2] = m[3][3] = 1;

    float fCos = cosf( radians );
    float fSin = sinf( radians );
    m[0][0] = m[1][1] = fCos;
    m[0][1] =  fSin;
    m[1][0] = -fSin;
  }

  void rotateZ( const float radians ){
    // 12 muls, 6 adds, 2 trig function calls
    // | m[0][0] m[1][0] m[2][0] m[3][0] |   | fCos -fSin 0 0 |   | m[0][0]*fCos+m[1][0]*fSin m[1][0]*fCos-m[0][0]*fSin m[2][0] m[3][0] |
    // | m[0][1] m[1][1] m[2][1] m[3][1] |   | fSin fCos  0 0 |   | m[0][1]*fCos+m[1][1]*fSin m[1][1]*fCos-m[0][1]*fSin m[2][1] m[3][1] |
    // | m[0][2] m[1][2] m[2][2] m[3][2] | * | 0    0     1 0 | = | m[0][2]*fCos+m[1][2]*fSin m[1][2]*fCos-m[0][2]*fSin m[2][2] m[3][2] |
    // | 0   0   0   1   |   | 0    0     0 1 |   | 0                 0                 0   1   |
    float fTemp;
    float fCos = cosf( radians );
    float fSin = sinf( radians );
    fTemp   = m[0][0]*fCos + m[1][0]*fSin;
    m[1][0] = m[1][0]*fCos - m[0][0]*fSin;
    m[0][0] = fTemp;
    fTemp   = m[0][1]*fCos + m[1][1]*fSin;
    m[1][1] = m[1][1]*fCos - m[0][1]*fSin;
    m[0][1] = fTemp;
    fTemp   = m[0][2]*fCos + m[1][2]*fSin;
    m[1][2] = m[1][2]*fCos - m[0][2]*fSin;
    m[0][2] = fTemp;
  }

  // Rotate functions
  void rotate( const TVector3 &angle ) {
    TMatrix4 m1;
    m1.rotateMatrix( angle );
    *this = *this * m1;
  }

  bool invert(){
    TVector4 x  = getColumn( 0 );
    TVector4 y  = getColumn( 1 );
    TVector4 z  = getColumn( 2 );
    TVector4 w  = getColumn( 3 );
    TVector4 x1 = crossProduct( y, z, w );
    TVector4 y1 = crossProduct( z, w, x );
    TVector4 z1 = crossProduct( w, x, y );
    TVector4 w1 = crossProduct( x, y, z );
    float   lx = x | x1;
    float   ly = y | y1;
    float   lz = z | z1;
    float   lw = w | w1;

    if( !lx || !ly || !lz || !lw )
      return false;

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

  // For orthogonal matrices, I believe this also gives you the inverse.
  void transpose(){
#		define SWAP(a,b,c) c=a; a=b; b=c
    float f;
    SWAP(m[0][1], m[1][0], f);
    SWAP(m[0][2], m[2][0], f);
    SWAP(m[0][3], m[3][0], f);
    SWAP(m[1][2], m[2][1], f);
    SWAP(m[1][3], m[3][1], f);
    SWAP(m[2][3], m[3][2], f);
#		undef SWAP
  }

  void print( const char *msg ) const;
};

#endif
