#include <stdio.h>

#include "heart.h"
#include "../ray.h"
#include "../intersection.h"

const TTagList PrimitiveHeart::primitiveHeartTags [] = {
  { "center", TAG_primitiveHeart_center },
  { "radius", TAG_primitiveHeart_radius },
  { "step", TAG_primitiveHeart_step },
  { "refinement", TAG_primitiveHeart_refinement },
  { NULL, -1 }
};

PrimitiveHeart::PrimitiveHeart() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_HEART;
  center = TVector3::zeroVector;
  radius = 1.0f;
  tStep = 2.0f;
  refinementSteps = 8;
}

void PrimitiveHeart::setup() {
  radius2 = radius * radius;
  invRadius = 1.0f / radius;
  //--  origin = c->p - centre;  //-- FIXME: should be in precalcFrame
  //--  cTerm = origin * origin - radius2; //-- same as above
}

float PrimitiveHeart::fieldEquation( const TVector3 &v ) const {
  const TVector3 nv = v * 0.1f;
  const float a = ( 2.0f * nv[__X] * nv[__X] +
		    nv[__Z] * nv[__Z] +
		    nv[__Y] * nv[__Y]
		    - 1.0f );
  const float yCube = nv[__Y] * nv[__Y] * nv[__Y];
  return a * a * a - 0.1f * nv[__X] * nv[__X] * yCube - nv[__Z] * nv[__Z] * yCube;
}

// this is currently done with less precision than the primary ray intersection
bool PrimitiveHeart::intersectionRay( const TRay &ray,
				      TIntersection &inter ) const {
  TVector3 originToCenter = ray.origin - center;

  float b = originToCenter | ray.direction;
  float c = originToCenter.magnitudeSquared() - radius2;
  
  const float d = b * b - c;
  
  if ( d < 0.0f )
    return false;
  
  const float sqrtd = sqrtf(d);
  float t1 = -b - sqrtd;
  float t2 = -b + sqrtd;
  
  if ( t2 < 0.0f ) // no intersection between ray and "bounding sphere"
    return false;

  if ( t1 < 0.0f )
    t1 = 0.0f;
  
  float t = t1;
  float f = fieldEquation( ray.origin + ( ray.direction * t ) );
  bool intersected = false;
  
  // iterate through volume looking for intersections
  while ( t < t2 ) {
    t += tStep;
    if ( f * fieldEquation( ray.origin + ( ray.direction * t ) ) < 0.0f ) {
      intersected = true;
      break;
    }
  }
  
  if ( !intersected )
    return false;
  
  float start = t - tStep;
  float end = t;
  
  for ( int i = 0; i < refinementSteps; i++ ) {
    float midpoint = (start + end) * 0.5f;
    TVector3 middle = ray.origin + ( ray.direction * midpoint );

    if ( f * fieldEquation( middle ) < 0.0f )
      end = midpoint;
    else
      start = midpoint;
  }

  inter.distance = ( start + end ) * 0.5f - 0.08f;
  return true;
}

bool PrimitiveHeart::allIntersectionRay( const TRay &ray,
					 TIntersectionStack &IStack ) const {
  return false;
}

bool PrimitiveHeart::intersectsRay( const TRay &ray, const float maxDist2 ) const {
  TIntersection inter;
  if ( !intersectionRay( ray, inter ) )
    return false;
  if ( sqr(inter.distance) < maxDist2 )
    return true;
  return false;
//   TVector3 originToCenter = ray.origin - center;
//   float b = originToCenter | ray.direction;
//   float c = originToCenter.magnitudeSquared() - radius2;
  
//   float d = b * b - c;
  
//   if ( d < 0.0f )
//     return false;
  
//   return true;
}

TVector3 PrimitiveHeart::normalAt( const TIntersection &inter ) const {
  const TPoint3 &v = inter.pointWorldSpace;

  float e0 = fieldEquation( v );
  float e1x = fieldEquation( v + TVector3( 0.01f, 0.0f,  0.0f  ) );
  float e1y = fieldEquation( v + TVector3(  0.0f, 0.01f, 0.0f  ) );
  float e1z = fieldEquation( v + TVector3(  0.0f,  0.0f, 0.01f ) );

  TVector3 result = TVector3( e1x - e0, e1y - e0, e1z - e0 );
  result.normalize();

  return result;
}

bool PrimitiveHeart::inside( const TPoint3 &p ) const {
  return false;
}

void PrimitiveHeart::print( const char *msg ) const {
  TPrimitive::print( msg );
  center.print("center");
  printf("radius: %5.5f\n"
	 "tStep: %5.5f\n"
	 "refinementSteps: %d\n", radius, tStep, refinementSteps );
}
