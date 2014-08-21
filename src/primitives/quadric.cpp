#include <stdio.h>

#include "../world.h"
#include "quadric.h"
#include "../intersection.h"
#include "../ray.h"
#include "../cameras/camera.h"

//--#define __DEBUG__

const TTagList PrimitiveQuadric::primitiveQuadricTags [] = {
  { "center", TAG_primitiveQuadric_center },
  { "radius", TAG_primitiveQuadric_radius },
  { "matrix", TAG_primitiveQuadric_A },
  { NULL, -1 }
};

PrimitiveQuadric::PrimitiveQuadric() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_QUADRIC;
}

void PrimitiveQuadric::setup() {
  radius2 = radius * radius;
  invRadius = 1.0f / radius;
  bool ellipsoid = true;
  for ( int i = 0; i < 3; i++ )
    for ( int j = 0; j < 3; j++ ) {
      if ( A( i, j ) < 0 )
	printf("Matrix[%d,%d] is negative!!\n", i, j );
      // ellipsoids are diagonal matrix with each diagonal element different from 0
      if ( ( i != j && fabsf(A( i, j )) > EPS )
	   || ( i == j && fabsf(A( i, j )) < EPS ) )
	ellipsoid = false;
    }
  if ( !ellipsoid )
    additionalInformation.setBit( PRIMITIVE_INFINITE );
}

void PrimitiveQuadric::precalcFrame( const TWorld &world ) {
  TVector3 kDiff = center - world.currentCamera->location;
  precalcKMatDiff = A * kDiff;
  precalcC = ( kDiff | precalcKMatDiff ) - radius2;
}

bool PrimitiveQuadric::intersectionRayCamera( const TRay &ray,
					      TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create(2);
  if ( !PrimitiveQuadric::allIntersectionRayCamera( ray, IStack ) )
    return false;
  inter.copyIntersection( IStack.stack[0] );
  return true;
}

bool PrimitiveQuadric::intersectionRay( const TRay &ray,
					TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create(2);
  if ( !PrimitiveQuadric::allIntersectionRay( ray, IStack ) )
    return false;
  inter.copyIntersection( IStack.stack[0] );
  return true;
}

bool PrimitiveQuadric::intersectsRay( const TRay &ray, const float distMax2 ) const {
  // set up quadratic Q(t) = a*t^2 + 2*b*t + c
  TVector3 kDiff = ray.origin - center;
  TVector3 kMatDir = A * ray.direction;
  TVector3 kMatDiff = A * kDiff;
  float fA = ray.direction | kMatDir; // fA > 0 is necessary
  float fB = ray.direction | kMatDiff;
  float fC = ( kDiff | kMatDiff ) - radius2;
  
  float fDiscr = fB*fB - fA*fC;
  // no intersection if Q(t) has no real roots
  if ( fDiscr < 0.0f )
    return false;

  // test if ray origin is inside quadric
  if ( fC < 0.0f )
    return true;

  // At this point, fC > 0 and Q(t) has real roots.  Intersection occurs
  // if Q'(0) < 0.
  return fB < 0.0f;
}

bool PrimitiveQuadric::allIntersectionRayCamera( const TRay &ray,
						 TIntersectionStack &IStack ) const {
  // set up quadratic Q(t) = a*t^2 + 2*b*t + c
  TVector3 kMatDir = A * ray.direction;
  float fA = ray.direction | kMatDir; // fA > 0 is necessary
  float fB = ray.direction | precalcKMatDiff;

  float fDiscr = fB*fB - fA*precalcC;
  // no intersection if Q(t) has no real roots
  if ( fDiscr < 0.0f )
    return false;
  else if ( fDiscr > 0.0f ) {
    // 2 real roots
    // assert: t0 < t1 since A > 0
    float fRoot = sqrtf( fDiscr );
    float fInvA = 1.0f / fA;
    float t0 = ( fB - fRoot ) * fInvA;
    if ( t0 >= 0.0f )
      IStack.push( t0 );
    float t1 = ( fB + fRoot ) * fInvA;
    if ( t1 >= 0.0f )
      IStack.push( t1 );
  }
  else {
    // one real root
    float t0 = fB / fA;
    if ( t0 >= 0.0f )
      IStack.push( t0 );
  }
  return IStack.index > 0;
}

bool PrimitiveQuadric::allIntersectionRay( const TRay &ray,
					   TIntersectionStack &IStack ) const {
  // set up quadratic Q(t) = a*t^2 + 2*b*t + c
  TVector3 kDiff = center - ray.origin;
  TVector3 kMatDir = A * ray.direction;
  TVector3 kMatDiff = A * kDiff;
  float fA = ray.direction | kMatDir; // fA > 0 is necessary
  float fB = ray.direction | kMatDiff;
  float fC = ( kDiff | kMatDiff ) - radius2;

  float fDiscr = fB*fB - fA*fC;
  // no intersection if Q(t) has no real roots
  if ( fDiscr < 0.0f )
    return false;
  else if ( fDiscr > 0.0f ) {
    // 2 real roots
    // assert: t0 < t1 since A > 0
    float fRoot = sqrtf( fDiscr );
    float fInvA = 1.0f / fA;
    float t0 = ( fB - fRoot ) * fInvA;
    if ( t0 >= 0.0f )
      IStack.push( t0 );
    float t1 = ( fB + fRoot ) * fInvA;
    if ( t1 >= 0.0f )
      IStack.push( t1 );
  }
  else {
    // one real root
    float t0 = fB / fA;
    if ( t0 >= 0.0f )
      IStack.push( t0 );
  }
  return IStack.index > 0;
}

TVector3 PrimitiveQuadric::normalAt( const TIntersection &inter ) const {
  TVector3 kDiff = inter.pointWorldSpace - center;
  TVector3 kMatDiff = A * kDiff;

  return kMatDiff * invRadius;
}

bool PrimitiveQuadric::inside( const TPoint3 &p ) const {
  TVector3 kDiff = center - p;
  TVector3 kMatDiff = A * kDiff;
  float dist2 = kMatDiff.magnitudeSquared();

  return dist2 < radius2;
}

void PrimitiveQuadric::print( const char *msg ) const {
  TPrimitive::print( msg );
  center.print("center: ");
  printf("radius: %5.5f\n", radius );
  A.print("matrix A: ");
}
