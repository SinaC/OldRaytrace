#include <stdio.h>

#include "transform.h"

void TTransform::print( const char *msg ) const {
  printf("%s\n", msg );
  matrix.print("transformation matrix: ");
  inverse.print("inverse transformation matrix: ");
}

TTransform::TTransform() {
  matrix = TMatrix4::Identity;
  inverse = TMatrix4::Identity;
}

TPoint3 TTransform::transformPoint( const TPoint3 &p ) const {
  return TPoint3( p[__X] * matrix.m[0][0]
		  + p[__Y] * matrix.m[1][0]
		  + p[__Z] * matrix.m[2][0]
		  + matrix.m[3][0],
		  p[__X] * matrix.m[0][1]
		  + p[__Y] * matrix.m[1][1]
		  + p[__Z] * matrix.m[2][1]
		  + matrix.m[3][1],
		  p[__X] * matrix.m[0][2]
		  + p[__Y] * matrix.m[1][2]
		  + p[__Z] * matrix.m[2][2]
		  + matrix.m[3][2] );
}
TPoint3 TTransform::inverseTransformPoint( const TPoint3 &p ) const {
  return TPoint3( p[__X] * inverse.m[0][0]
		  + p[__Y] * inverse.m[1][0]
		  + p[__Z] * inverse.m[2][0]
		  + inverse.m[3][0],
		  p[__X] * inverse.m[0][1]
		  + p[__Y] * inverse.m[1][1]
		  + p[__Z] * inverse.m[2][1]
		  + inverse.m[3][1],
		  p[__X] * inverse.m[0][2]
		  + p[__Y] * inverse.m[1][2]
		  + p[__Z] * inverse.m[2][2]
		  + inverse.m[3][2] );
}
TVector3 TTransform::transformDirection( const TVector3 &v ) const {
  return TVector3( v[__X] * matrix.m[0][0]
		  + v[__Y] * matrix.m[1][0]
		  + v[__Z] * matrix.m[2][0],
		  v[__X] * matrix.m[0][1]
		  + v[__Y] * matrix.m[1][1]
		  + v[__Z] * matrix.m[2][1],
		  v[__X] * matrix.m[0][2]
		  + v[__Y] * matrix.m[1][2]
		  + v[__Z] * matrix.m[2][2] );
}
TVector3 TTransform::inverseTransformDirection( const TVector3 &v ) const {
  return TVector3( v[__X] * inverse.m[0][0]
		  + v[__Y] * inverse.m[1][0]
		  + v[__Z] * inverse.m[2][0],
		  v[__X] * inverse.m[0][1]
		  + v[__Y] * inverse.m[1][1]
		  + v[__Z] * inverse.m[2][1],
		  v[__X] * inverse.m[0][2]
		  + v[__Y] * inverse.m[1][2]
		  + v[__Z] * inverse.m[2][2] );
}
TVector3 TTransform::transformNormal( const TVector3 &v ) const {
  return TVector3( v[__X] * inverse.m[0][0]
		  + v[__Y] * inverse.m[0][1]
		  + v[__Z] * inverse.m[0][2],
		  v[__X] * inverse.m[1][0]
		  + v[__Y] * inverse.m[1][1]
		  + v[__Z] * inverse.m[1][2],
		  v[__X] * inverse.m[2][0]
		  + v[__Y] * inverse.m[2][1]
		  + v[__Z] * inverse.m[2][2] );
}
TVector3 TTransform::inverseTransformNormal( const TVector3 &v ) const {
  return TVector3( v[__X] * matrix.m[0][0]
		  + v[__Y] * matrix.m[0][1]
		  + v[__Z] * matrix.m[0][2],
		  v[__X] * matrix.m[1][0]
		  + v[__Y] * matrix.m[1][1]
		  + v[__Z] * matrix.m[1][2],
		  v[__X] * matrix.m[2][0]
		  + v[__Y] * matrix.m[2][1]
		  + v[__Z] * matrix.m[2][2] );
}

// Builds a complete transform from a matrix.
void TTransform::computeMatrixTransform( const TMatrix4 &mat ) {
  for ( register int i = 0; i < 4; i++ ) {
    matrix.m[i][0] = mat.m[i][0];
    matrix.m[i][1] = mat.m[i][1];
    matrix.m[i][2] = mat.m[i][2];
    matrix.m[i][3] = mat.m[i][3];
  }
  inverse.invert( matrix );
}

void TTransform::computeScalingTransform( const TVector3 &v ) {
  matrix = TMatrix4::Identity;
  matrix.m[0][0] = v[__X];
  matrix.m[1][1] = v[__Y];
  matrix.m[2][2] = v[__Z];

  inverse = TMatrix4::Identity;
  inverse.m[0][0] = 1.0f / v[__X];
  inverse.m[1][1] = 1.0f / v[__Y];
  inverse.m[2][2] = 1.0f / v[__Z];
}

void TTransform::computeTranslationTransform( const TVector3 &v ) {
  matrix = TMatrix4::Identity;
  matrix.m[3][0] = v[__X];
  matrix.m[3][1] = v[__Y];
  matrix.m[3][2] = v[__Z];

  inverse = TMatrix4::Identity;
  inverse.m[3][0] = -v[__X];
  inverse.m[3][1] = -v[__Y];
  inverse.m[3][2] = -v[__Z];
}

void TTransform::computeRotationTransform( const TVector3 &v ) {
  TVector3 radianVector = v * M_PI_180;

  matrix = TMatrix4::Identity;

  float cosx = cosf( radianVector[__X] );
  float sinx = sinf( radianVector[__X] );
  float cosy = cosf( radianVector[__Y] );
  float siny = sinf( radianVector[__Y] );
  float cosz = cosf( radianVector[__Z] );
  float sinz = sinf( radianVector[__Z] );

  matrix.m[1][1] = cosx;
  matrix.m[2][2] = cosx;
  matrix.m[1][2] = sinx;
  matrix.m[2][1] = -sinx;

  inverse.transpose( matrix );
  
  TMatrix4 mat = TMatrix4::Identity;
  mat.m[0][0] = cosy;
  mat.m[2][2] = cosy;
  mat.m[0][2] = -siny;
  mat.m[2][0] = siny;

  matrix *= mat;

  mat.transpose();
  inverse = mat * inverse;

  mat = TMatrix4::Identity;

  mat.m[0][0] = cosz;
  mat.m[1][1] = cosz;
  mat.m[0][1] = sinz;
  mat.m[1][0] = -sinz;

  matrix *= mat;

  mat.transpose();
  inverse = mat * inverse;
}

void TTransform::composeTransforms( const TTransform &newTransform ) {
  matrix *= newTransform.matrix;
  inverse = newTransform.inverse * inverse;
}

// Rotation about an arbitrary axis - formula from:
//  "Computational Geometry for Design and Manufacture", Faux & Pratt
// The angles for this transform are specified in radians.
void TTransform::computeAxisRotationTransform( const TVector3 &V1, const float angle ) {
  matrix = TMatrix4::Identity;
  
  float cosx = cosf(angle);
  float sinx = sinf(angle);

  matrix.m[0][0] = V1[__X] * V1[__X] + cosx * ( 1.0f - V1[__X] * V1[__X]);
  matrix.m[0][1] = V1[__X] * V1[__Y] * ( 1.0f - cosx) + V1[__Z] * sinx;
  matrix.m[0][2] = V1[__X] * V1[__Z] * ( 1.0f - cosx) - V1[__Y] * sinx;

  matrix.m[1][0] = V1[__X] * V1[__Y] * ( 1.0f - cosx) - V1[__Z] * sinx;
  matrix.m[1][1] = V1[__Y] * V1[__Y] + cosx * ( 1.0f - V1[__Y] * V1[__Y]);
  matrix.m[1][2] = V1[__Y] * V1[__Z] * ( 1.0f - cosx) + V1[__X] * sinx;

  matrix.m[2][0] = V1[__X] * V1[__Z] * ( 1.0f - cosx) + V1[__Y] * sinx;
  matrix.m[2][1] = V1[__Y] * V1[__Z] * ( 1.0f - cosx) - V1[__X] * sinx;
  matrix.m[2][2] = V1[__Z] * V1[__Z] + cosx * ( 1.0f - V1[__Z] * V1[__Z]);

  inverse.transpose( matrix );
}

// Given a point and a direction and a radius, find the transform
//  that brings these into a canonical coordinate system
void TTransform::computeCoordinateTransform( const TPoint3 &origin, const TVector3 &up,
					     const float radius, const float length ) {
  TTransform trans2;
  TVector3 tmpV = TVector3( radius, radius, length );
  TVector3 localUp = up;

  computeScalingTransform( tmpV );

  if ( fabsf(localUp[__Z]) > 1.0f - EPS ) {
    tmpV = TVector3( 1.0f, 0.0f, 0.0f );
    localUp[__Z] = localUp[__Z] < 0.0f ? -1.0f : 1.0f;
  }
  else
    tmpV = TVector3( -localUp[__Y], localUp[__X], 0.0f );

  trans2.computeAxisRotationTransform( tmpV, acosf( up[__Z] ) );
  composeTransforms( trans2 );
  trans2.computeTranslationTransform( origin );
  composeTransforms( trans2 );
}
