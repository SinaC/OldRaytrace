#include <stdio.h>

#include "../world.h"
#include "sphere.h"
#include "../intersection.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../boundings/aabb.h"

//--#define __DEBUG__

const TTagList PrimitiveSphere::primitiveSphereTags [] = {
  { "center", TAG_primitiveSphere_center },
  { "radius", TAG_primitiveSphere_radius },
  { NULL, -1 }
};

PrimitiveSphere::PrimitiveSphere() {
  TPrimitive::TPrimitive(); primitiveKind = TPrimitive::PRIMITIVE_SPHERE;
}

void PrimitiveSphere::setup() {
  m_InvRadius = 1.0f / m_Radius;
  m_Radius2 = SQR(m_Radius);
}

void PrimitiveSphere::precalcFrame( const TWorld &world ) {
  m_PrecalcRayToCenter = m_Center - world.currentCamera->location;
  m_PrecalcC = m_PrecalcRayToCenter.magnitudeSquared() - m_Radius2;
}

bool PrimitiveSphere::intersectionRayCamera( const TRay &ray, TIntersection &inter ) const {
  // same as intersectionRay but C and rayToCenter are already known
  float B = ( m_PrecalcRayToCenter | ray.direction );
  float discriminant = B*B - m_PrecalcC;

  if ( discriminant < 0.0f )
    return false;

  float sqrtDiscr = sqrtf(discriminant);
  inter.distance = B-sqrtDiscr;
  if ( inter.distance > 0.0f )
    return true;
  inter.distance = B+sqrtDiscr;
  if ( inter.distance > 0.0f )
    return true;
  return false;
}

bool PrimitiveSphere::intersectionRay( const TRay &ray,
				       TIntersection &inter ) const {
  // We have to solve
  // A t^2 + 2*B t + C = 0
  // with A = square norm of ray.dir
  //      B = ray.dir . ( ray.loc - sphere.loc )
  //  and C = square norm of ( sphere.loc - ray.loc ) - radius^2
  //                -B + (or -) sqrt( B^2 - A * C )
  // solution: t = -----------------------------------
  //                           A
  // if discriminant (B^2- A * C) is negative: no solution
  // A is always strictly positive (= 1 if ray is normalized)
  // if we could assume C > 0, then we could stop if B < 0 and always get the positive solution

  // OPTIMIZATION 1: We assume A = 1.0

  TVector3 rayToCenter = m_Center - ray.origin;
  float B = ( ray.direction | rayToCenter );
  float C = rayToCenter.magnitudeSquared() - m_Radius2;
  float discriminant = B*B-C;

#ifdef __DEBUG__
  printf("PrimitiveSphere::intersectionRay\n");
  ray.origin.print("ray origin: ");
  ray.direction.print("ray direction: ");
  rayToCenter.print("ray to center: ");
  printf("B: %5.5f   C: %5.5f   discr: %5.5f\n",
	 B, C, discriminant );
#endif

  if ( discriminant < 0.0f )
    return false;

  float sqrtDiscr = sqrtf(discriminant);
  inter.distance = B-sqrtDiscr;
  // return smallest positive solution
  if ( inter.distance > 0.0f )
    return true;
  inter.distance = B+sqrtDiscr;
  if ( inter.distance > 0.0f )
    return true;
  return false;
}

bool PrimitiveSphere::intersectsRay( const TRay &ray, const float distMax2 ) const {
  TVector3 diff = m_Center - ray.origin;
  float t = diff | ray.direction;
  if ( t < 0 )
    return false;
  diff -= ray.direction*t;
  float dist2 = diff.magnitudeSquared();
  return dist2 <= m_Radius2;
}

bool PrimitiveSphere::allIntersectionRayCamera( const TRay &ray,
						TIntersectionStack &IStack ) const {
  // same as allIntersectionRay but C and rayToCenter are already known
  float B = ( m_PrecalcRayToCenter | ray.direction );
  float discriminant = B*B - m_PrecalcC;

  if ( discriminant < 0.0f )
    return false;

  float sqrtDiscr = sqrtf(discriminant);
  float dist = B-sqrtDiscr;
  if ( dist > 0 )
    IStack.push( dist );
  dist = B+sqrtDiscr;
  if ( dist > 0 )
    IStack.push( dist );
  return IStack.index > 0;
}

bool PrimitiveSphere::allIntersectionRay( const TRay &ray,
					  TIntersectionStack &IStack ) const {
  // We have to solve
  // A t^2 + 2*B t + C = 0
  // with A = square norm of ray.dir
  //      B = ray.dir . ( ray.loc - sphere.loc )
  //  and C = square norm of ( sphere.loc - ray.loc ) - radius^2
  //                -B + (or -) sqrt( B^2 - A * C )
  // solution: t = -----------------------------------
  //                           A
  // if discriminant (B^2- A * C) is negative: no solution
  // A is always strictly positive (= 1 if ray is normalized)
  // if we could assume C > 0, then we could stop if B < 0 and always get the positive solution

  // OPTIMIZATION 1: We assume A = 1.0

  TVector3 rayToCenter = m_Center - ray.origin;
  float B = ( ray.direction | rayToCenter );
  float C = rayToCenter.magnitudeSquared() - m_Radius2;
  float discriminant = B*B-C;

  if ( discriminant < 0.0f )
    return false;

  float sqrtDiscr = sqrtf(discriminant);
  float dist = B-sqrtDiscr;
  if ( dist > 0 )
    IStack.push( dist );
  dist = B+sqrtDiscr;
  if ( dist > 0 )
    IStack.push( dist );
  return IStack.index > 0;
}

TVector3 PrimitiveSphere::normalAt( const TIntersection &inter ) const {
  return ( inter.pointWorldSpace - m_Center) * m_InvRadius;
}

bool PrimitiveSphere::inside( const TPoint3 &p ) const {
  TVector3 pointToCenter = m_Center - p;
  float dist2 = pointToCenter.magnitudeSquared();

  return dist2 < m_Radius2;
}

bool PrimitiveSphere::computeBoundingAABB( BoundingAABB &a_Box ) const {
  TVector3 size( m_Radius, m_Radius, m_Radius );
  a_Box.m_Min = m_Center - size;
  a_Box.m_Max = m_Center + size;
  return true;
}

bool PrimitiveSphere::intersectsBox( const BoundingAABB &a_Box ) const {
  float dmin = 0;
  if ( m_Center.x < a_Box.m_Min.x )
    dmin = dmin + (m_Center.x - a_Box.m_Min.x) * (m_Center.x - a_Box.m_Min.x);
  else if ( m_Center.x > a_Box.m_Max.x )
    dmin = dmin + (m_Center.x - a_Box.m_Max.x) * (m_Center.x - a_Box.m_Max.x);
  if ( m_Center.y < a_Box.m_Min.y )
    dmin = dmin + (m_Center.y - a_Box.m_Min.y) * (m_Center.y - a_Box.m_Min.y);
  else if (m_Center.y > a_Box.m_Max.y)
    dmin = dmin + (m_Center.y - a_Box.m_Max.y) * (m_Center.y - a_Box.m_Max.y);
  if (m_Center.z < a_Box.m_Min.z)
    dmin = dmin + (m_Center.z - a_Box.m_Min.z) * (m_Center.z - a_Box.m_Min.z);
  else if (m_Center.z > a_Box.m_Max.z)
    dmin = dmin + (m_Center.z - a_Box.m_Max.z) * (m_Center.z - a_Box.m_Max.z);
  return ( dmin <= m_Radius2 );
}

void PrimitiveSphere::print( const char *msg ) const {
  TPrimitive::print( msg );
  m_Center.print("center: ");
  printf("radius: %5.5f\n", m_Radius );
}
