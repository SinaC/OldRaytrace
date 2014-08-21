#include <stdio.h>

#include "cone.h"

#include "../intersection.h"
#include "../ray.h"

const TTagList PrimitiveCone::primitiveConeTags [] = {
  { "center", TAG_primitiveCone_center },
  { "axis", TAG_primitiveCone_axis },
  { "angle", TAG_primitiveCone_angle },
  { NULL, -1 }
};

PrimitiveCone::PrimitiveCone() {
  TPrimitive::TPrimitive(); primitiveKind = TPrimitive::PRIMITIVE_CONE;
  additionalInformation.setBit( PRIMITIVE_INFINITE );
}

void PrimitiveCone::setup() {
  cosAngle = cosf(angle);
  sinAngle = sinf(angle);
  axis.normalize();
}

bool PrimitiveCone::intersectionRay( const TRay &ray,
				     TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create(2);
  if ( !PrimitiveCone::allIntersectionRay( ray, IStack ) )
    return false;
  int closestId = -1;
  float closestDistance = INFINITE;
  for ( int i = 1; i < IStack.index; i++ )
    if ( IStack.stack[i].distance > 0.0f
	 && IStack.stack[i].distance < closestDistance ) {
      closestId = i;
      closestDistance = IStack.stack[i].distance;
    }
  if ( closestId == -1 )
    return false;
  inter.copyIntersection( IStack.stack[closestId] );
  return true;
}

bool PrimitiveCone::intersectsRay( const TRay &ray, const float distMax2 ) const {
  TIntersection inter;
  if ( !PrimitiveCone::intersectionRay( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < distMax2 )
    return true;
  return false;
}

bool PrimitiveCone::allIntersectionRay( const TRay &ray,
					  TIntersectionStack &IStack ) const {
  // set up quadratic Q(t) = c2*t^2 + 2*c1*t + c0
  float fAdD = axis | ray.direction;
  float fDdD = ray.direction | ray.direction; // OPTIMIZE: always 1.0f
  float fCosSqr = cosAngle * cosAngle; // OPTIMIZE: may be precomputed
  TVector3 kE = ray.origin - center;
  float fAdE = axis | kE;
  float fDdE = ray.direction | kE;
  float fEdE = kE | kE;
  float fC2 = fAdD*fAdD - fCosSqr*fDdD;
  float fC1 = fAdD*fAdE - fCosSqr*fDdE;
  float fC0 = fAdE*fAdE - fCosSqr*fEdE;
  
  // Solve the quadratic.  Keep only those X for which Dot(A,X-V) > 0.
  if ( fabsf(fC2) >= EPS ) {
    // c2 != 0
    float fDiscr = fC1*fC1 - fC0*fC2;
    if ( fDiscr < 0.0f )
      // no real roots or only 1 root
      return false;
    else if ( fDiscr > 0.0f ) {
      // two distinct real roots
      float fRoot = sqrtf(fDiscr);
      float fInvC2 = 1.0f/fC2;
      float fT = ( -fC1 - fRoot ) * fInvC2;
      kE = ( ray.origin + ( fT * ray.direction ) ) - center; // OPTIMIZE: kE -= fT * ray.direction;
      if ( ( kE | axis ) > 0.0f )
	IStack.push( fT );
      
      fT = ( -fC1 + fRoot ) * fInvC2;
      kE = ( ray.origin + ( fT * ray.direction ) ) - center;
      if ( ( kE | axis ) > 0.0f )
	IStack.push( fT );
      return true;
    }
    else {
      // one repeated real root
      float fT = fC1 / fC2;
      kE = ( ray.origin + ( fT * ray.direction ) ) - center;
      if ( ( kE | axis ) > 0.0f )
	IStack.push( fT );
      return true;
    }
  }
  else if ( fabsf(fC1) >= EPS ) {
    // c2 = 0, c1 != 0
    float fT = 0.5f * fC0 / fC1;
    kE = ( ray.origin + ( fT * ray.direction ) ) - center;
    if ( ( kE | axis ) > 0.0f )
      IStack.push( fT );
    return true;
  }
  return false;
}

TVector3 PrimitiveCone::normalAt( const TIntersection &inter ) const {
  return TVector3( 0, 1, 0 ); // FIXME
}

bool PrimitiveCone::inside( const TPoint3 &p ) const {
  return false;
}

void PrimitiveCone::print( const char *msg ) const {
  TPrimitive::print( msg );
  center.print("center: ");
  axis.print("axis: ");
  printf("angle: %5.5f\n", angle );
}
