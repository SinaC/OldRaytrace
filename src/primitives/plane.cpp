#include <stdio.h>

#include "../world.h"
#include "plane.h"
#include "../intersection.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../boundings/aabb.h"

const TTagList PrimitivePlane::primitivePlaneTags [] = {
  { "normal", TAG_primitivePlane_normal },
  { "distance", TAG_primitivePlane_distance },
  { NULL, -1 }
};

PrimitivePlane::PrimitivePlane() {
  TPrimitive::TPrimitive(); primitiveKind = TPrimitive::PRIMITIVE_PLANE;
  additionalInformation.setBit( PRIMITIVE_INFINITE );
}

void PrimitivePlane::setup() {
  m_Normal.normalize();
}

void PrimitivePlane::precalcFrame( const TWorld &a_World ) {
  m_PrecalcV0 = m_Distance - ( m_Normal | a_World.currentCamera->location );
}

bool PrimitivePlane::intersectionRayCamera( const TRay &a_Ray,
					    TIntersection &a_Inter ) const {
  // Plane equation: normal . point = dist
  // we must solve following equation:
  //  normal . ( ray.loc + ray.dir * t ) = dist
  // solution: t = -( dist - normal . ray.loc ) / ( normal . ray.dir )
  float vd = m_Normal | a_Ray.direction;
  if ( vd == 0.0f )
    return false;

  if ( ( m_PrecalcV0 > 0 ) ^ ( vd > 0 ) ) // if doesn't have same sign: normal is pointing away from the ray
    return false;

  a_Inter.distance = m_PrecalcV0 / vd;
  return true;
}

bool PrimitivePlane::intersectionRay( const TRay &a_Ray,
				       TIntersection &a_Inter ) const {
  // Plane equation: normal . point = dist
  // we must solve following equation:
  //  normal . ( ray.loc + ray.dir * t ) = dist
  // solution: t = -( dist - normal . ray.loc ) / ( normal . ray.dir )
  float vd = m_Normal | a_Ray.direction;
  if ( vd == 0.0f )
    return false;

  float v0 = m_Distance - ( m_Normal | a_Ray.origin );
  if ( ( v0 > 0 ) ^ ( vd > 0 ) ) // if doesn't have same sign: normal is pointing away from the ray
    return false;

  a_Inter.distance = v0 / vd;
  return true;
}

bool PrimitivePlane::intersectsRay( const TRay &a_Ray, const float a_DistMax2 ) const {
  //quick and dirty; usually infinite planes have no interesting shadows
  return false;
}

bool PrimitivePlane::allIntersectionRayCamera( const TRay &a_Ray,
					       TIntersectionStack &a_IStack ) const {
  TIntersection inter;
  if ( !PrimitivePlane::intersectionRayCamera( a_Ray, inter ) )
    return false;
  a_IStack.push( inter );
  return true;
}
bool PrimitivePlane::allIntersectionRay( const TRay &a_Ray,
					 TIntersectionStack &a_IStack ) const {
  TIntersection inter;
  if ( !PrimitivePlane::intersectionRay( a_Ray, inter ) )
    return false;
  a_IStack.push( inter );
  return true;
}

TVector3 PrimitivePlane::normalAt( const TIntersection &a_Inter ) const {
  return m_Normal;
}

bool PrimitivePlane::inside( const TPoint3 &a_P ) const {
  float t = a_P | m_Normal;
  return t - m_Distance > EPS;
}

bool PrimitivePlane::computeBoundingAABB( BoundingAABB &a_Box ) const {
  a_Box.m_Min = TPoint3( -INFINITE, -INFINITE, -INFINITE );
  a_Box.m_Max = TPoint3( INFINITE, INFINITE, INFINITE );
  return true;
}

bool PrimitivePlane::intersectsBox( const BoundingAABB &a_Box ) const {
  const TVector3 *v[2];
  v[0] = &a_Box.m_Min;
  v[1] = &a_Box.m_Max;
  int side1 = 0, side2 = 0, i = 0;
  for ( side1 = 0, side2 = 0, i = 0; i < 8; i++ ) {
      TVector3 p( v[i & 1]->x, v[(i >> 1) & 1]->y, v[(i >> 2) & 1]->z );
      if ( ( ( p | m_Normal ) + m_Distance ) < 0 ) side1++;
      else side2++;
    }
  if ( ( side1 == 0 ) || ( side2 == 0 ) ) return false;
  else return true;
}

void PrimitivePlane::print( const char *msg ) const {
  TPrimitive::print( msg );
  m_Normal.print("normal: ");
  printf("distance to origin: %5.5f\n", m_Distance );
}
