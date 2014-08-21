#include <stdio.h>

#include "sphere.h"
#include "../ray.h"
#include "../intersection.h"

const TTagList BoundingSphere::boundingSphereTags [] = {
  { "center", TAG_boundingSphere_center },
  { "radius2", TAG_boundingSphere_radius2 },
  { NULL, -1 }
};

// Axis-aligned bounding box
BoundingSphere::BoundingSphere() {
  TBounding::TBounding(); boundingKind = TBounding::BOUNDING_SPHERE;
  m_Center = TVector3::zeroVector;
  m_Radius2 = 0;
}

void BoundingSphere::create( const int numP, const TPoint3 *pts ) {
  int i;
  // compute gravity center
  m_Center = TVector3::zeroVector;
  for ( i = 0; i < numP; i++ )
    m_Center += pts[i];
  m_Center = m_Center * ( 1.0f / (float)numP );
  // compute radius
  m_Radius2 = INFINITE;
  for ( i = 0; i < numP; i++ ) {
    float r2 = (m_Center - pts[i]).magnitudeSquared();
    if ( r2 < m_Radius2 )
      m_Radius2 = r2;
  }
}

bool BoundingSphere::intersectsBox( const BoundingAABB &a_Box ) const {
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

bool BoundingSphere::intersectsRay( const TRay &ray ) const {
  TVector3 diff = m_Center - ray.origin;
  float t = diff | ray.direction;
  if ( t < 0 )
    return false;
  diff -= (ray.direction * t);
  float dist2 = diff.magnitudeSquared();
  return dist2 <= m_Radius2;
}

// Calculate a sphere that encloses two given spheres.
void BoundingSphere::merge( const BoundingSphere &a_Sphere ) {
  TVector3 D = m_Center - a_Sphere.m_Center;
  float l = D.magnitude();

  // Check if one sphere encloses the other.
  float r1r = sqrtf( m_Radius2 );
  float r2r = sqrtf( a_Sphere.m_Radius2 );

  if ( l + r1r <= r2r ) {
    // the other sphere is enclosing 'this'
    m_Center = a_Sphere.m_Center;
    m_Radius2 = a_Sphere.m_Radius2;
    return;
  }

  if ( l + r2r <= r1r )
    // no modification, 'this' is enclosing the other sphere
    return;
  
  float oneOverL = 1.0f / l;
  float k1 = ( 1.0f + ( r1r - r2r ) * oneOverL ) * 0.5f;
  float k2 = ( 1.0f + ( r2r - r1r ) * oneOverL ) * 0.5f;

  m_Center = ( m_Center * k1 ) + ( a_Sphere.m_Center * k2 );
  m_Radius2 = SQR( ( l + r1r + r2r ) * 0.5f );
}

bool BoundingSphere::inside( const TPoint3 &a_P ) const {
  TVector3 pointToCenter = m_Center - a_P;
  float dist2 = pointToCenter.magnitudeSquared();

  return dist2 < m_Radius2;
}

void BoundingSphere::print( const char *msg ) const {
  TBounding::print( msg );
  m_Center.print("center: ");
  printf("radius2: %5.5f\n", m_Radius2 );
}

void BoundingSphere::renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const {
}
