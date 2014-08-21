#include <stdio.h>

#include "aabb.h"
#include "../ray.h"
#include "../poly3d/rendersimple.h"
#include "../intersection.h"

const TTagList BoundingAABB::boundingAABBTags [] = {
  { "min", TAG_boundingAABB_min },
  { "max", TAG_boundingAABB_max },
  { NULL, -1 }
};

// Axis-aligned bounding box
BoundingAABB::BoundingAABB() {
  TBounding::TBounding(); boundingKind = TBounding::BOUNDING_AABB;
  m_Min = TVector3::zeroVector;
  m_Max = TVector3::zeroVector;
}

void BoundingAABB::create( const int a_NumP, const TPoint3 *a_Pts ) {
  m_Min = TPoint3( INFINITE, INFINITE, INFINITE );
  m_Max = -m_Min;
  for ( int i = 0; i < a_NumP; i++ ) {
    if ( a_Pts[i][__X] < m_Min[__X] ) m_Min[__X] = a_Pts[i][__X];
    if ( a_Pts[i][__Y] < m_Min[__Y] ) m_Min[__Y] = a_Pts[i][__Y];
    if ( a_Pts[i][__Z] < m_Min[__Z] ) m_Min[__Z] = a_Pts[i][__Z];

    if ( a_Pts[i][__X] > m_Max[__X] ) m_Max[__X] = a_Pts[i][__X];
    if ( a_Pts[i][__Y] > m_Max[__Y] ) m_Max[__Y] = a_Pts[i][__Y];
    if ( a_Pts[i][__Z] > m_Max[__Z] ) m_Max[__Z] = a_Pts[i][__Z];
  }
}

bool BoundingAABB::intersectsBox( const BoundingAABB &a_Box ) const {
  return ( ( m_Max.x > a_Box.m_Min.x ) && ( m_Min.x < a_Box.m_Max.x ) && // x-axis overlap
	   ( m_Max.y > a_Box.m_Min.y ) && ( m_Min.y < a_Box.m_Max.y ) && // y-axis overlap
	   ( m_Max.z > a_Box.m_Min.z ) && ( m_Min.z < a_Box.m_Max.z ) ); // z-axis overlap
}

bool BoundingAABB::intersectsRay( const TRay &a_Ray ) const {
  float tmin, tmax, tymin, tymax, tzmin, tzmax;
  if ( a_Ray.direction[__X] >= 0 ) {
    tmin = ( m_Min[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
    tmax = ( m_Max[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
  }
  else {
    tmin = ( m_Max[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
    tmax = ( m_Min[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
  }
  if ( a_Ray.direction[__Y] >= 0 ) {
    tymin = ( m_Min[__Y] - a_Ray.origin[__Y] ) * a_Ray.reverseDirection[__Y];
    tymax = ( m_Max[__Y] - a_Ray.origin[__Y] ) * a_Ray.reverseDirection[__Y];
  }
  else {
    tymin = ( m_Max[__Y] - a_Ray.origin[__Y] ) * a_Ray.reverseDirection[__Y];
    tymax = ( m_Min[__Y] - a_Ray.origin[__Y] ) * a_Ray.reverseDirection[__Y];
  }
  if ( ( tmin > tymax ) || ( tymin > tmax ) )
    return false;
  if ( tymin > tmin )
    tmin = tymin;
  if ( tymax < tmax )
    tmax = tymax;
  if ( a_Ray.direction[__Z] >= 0 ) {
    tzmin = ( m_Min[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
    tzmax = ( m_Max[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
  }
  else {
    tzmin = ( m_Max[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
    tzmax = ( m_Min[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
  }
  if ( ( tmin > tzmax ) || ( tzmin > tmax ) )
    return false;
  if ( tzmin > tmin )
    tmin = tzmin;
  if ( tzmax > tmax )
    tmax = tzmax;
  // return tmin > t0 && tmax < t1      // test distance range [t0, t1]
  return tmin > 0;
}

bool BoundingAABB::inside( const TPoint3 &a_P ) const {
  if ( a_P[__X] < m_Min[__X]
       || a_P[__X] > m_Max[__X] )
    return false;
  if ( a_P[__Y] < m_Min[__Y]
       || a_P[__Y] > m_Max[__Y] )
    return false;
  if ( a_P[__Z] < m_Min[__Z]
       || a_P[__Z] > m_Max[__Z] )
    return false;
  return true;
}

bool BoundingAABB::intersectionRay( const TPoint3 &a_Origin, const TVector3 &a_Direction, const TVector3 &a_ReverseDirection,
				    float &a_Distance ) const {
  // Side ID
  static const enum BoxSide {
    SIDE_X_MIN,     // x < 0
    SIDE_X_MAX,     // x > 0
    SIDE_Y_MIN,     // y < 0
    SIDE_Y_MAX,     // y > 0
    SIDE_Z_MIN,     // z < 0
    SIDE_Z_MAX      // z > 0
  };
// Minimal intersection depth.
#define DEPTH_TOLERANCE (1.0e-6f)
// Two values are equal if their difference is small than CLOSE_TOLERANCE.
#define CLOSE_TOLERANCE (1.0e-6f)
  int smin = -1, smax = -1; // Side hit for min/max intersection.
  float t, tmin, tmax;
  tmin = 0.0f;
  tmax = INFINITE;
  
  // Sides first.
  if ( a_Direction[__X] < -EPS ) {
    t = ( m_Min[__X] - a_Origin[__X] ) * a_ReverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MIN;
      tmax = t;
    }
    
    t = ( m_Max[__X] - a_Origin[__X] ) * a_ReverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MAX;
      tmin = t;
    }
  }
  else if ( a_Direction[__X] > EPS ) {
    t = ( m_Max[__X] - a_Origin[__X] ) * a_ReverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MAX;
      tmax = t;
    }
    
    t = (m_Min[__X] - a_Origin[__X]) * a_ReverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MIN;
      tmin = t;
    }
  }
  else if ( ( a_Origin[__X] < m_Min[__X] ) || ( a_Origin[__X] > m_Max[__X] ) )
    return false;

  // Check Top/Bottom.
  if ( a_Direction[__Y] < -EPS ) {
    t = ( m_Min[__Y] - a_Origin[__Y] ) * a_ReverseDirection[__Y];
    if ( t < tmin )
      return false;

    if ( t <= tmax - CLOSE_TOLERANCE ) {
      smax = SIDE_Y_MIN;
      tmax = t;
    }
    else
      // If intersection points are close to each other find out
      // which side to use, i.e. is most probably hit. [DB 9/94]
      if ( t <= tmax + CLOSE_TOLERANCE )
	if ( -a_Direction[__Y] > fabsf(a_Direction[__X] ) )
	  smax = SIDE_Y_MIN;
    
    t = (m_Max[__Y] - a_Origin[__Y]) * a_ReverseDirection[__Y];
    if ( t >= tmin + CLOSE_TOLERANCE ) {
      if (t > tmax)
	return false;
      smin = SIDE_Y_MAX;
      tmin = t;
    }
    else
      // If intersection points are close to each other find out
      // which side to use, i.e. is most probably hit. [DB 9/94]
      if ( t >= tmin - CLOSE_TOLERANCE )
	if ( -a_Direction[__Y] > fabsf(a_Direction[__X]) )
	  smin = SIDE_Y_MAX;
  }
  else if (a_Direction[__Y] > EPS ) {
    t = (m_Max[__Y] - a_Origin[__Y]) * a_ReverseDirection[__Y];
    if ( t < tmin )
      return false;
    if ( t <= tmax - CLOSE_TOLERANCE ) {
      smax = SIDE_Y_MAX;
      tmax = t;
    }
    else
      // If intersection points are close to each other find out
      // which side to use, i.e. is most probably hit. [DB 9/94]
      if ( t <= tmax + CLOSE_TOLERANCE )
	if ( a_Direction[__Y] > fabsf(a_Direction[__X]) )
	  smax = SIDE_Y_MAX;
      
    t = ( m_Min[__Y] - a_Origin[__Y] ) * a_ReverseDirection[__Y];
    if ( t >= tmin + CLOSE_TOLERANCE ) {
      if ( t > tmax )
	return false;
      smin = SIDE_Y_MIN;
      tmin = t;
    }
    else
      // If intersection points are close to each other find out
      // which side to use, i.e. is most probably hit. [DB 9/94]
      if ( t >= tmin - CLOSE_TOLERANCE )
	if ( a_Direction[__Y] > fabsf(a_Direction[__X]) )
	  smin = SIDE_Y_MIN;
  }
  else if ( ( a_Origin[__Y] < m_Min[__Y] ) || ( a_Origin[__Y] > m_Max[__Y] ) )
    return false;

  // Now front/back
  if ( a_Direction[__Z] < -EPS ) {
    t = ( m_Min[__Z] - a_Origin[__Z] ) * a_ReverseDirection[__Z];
    if ( t < tmin )
      return false;
    if ( t <= tmax - CLOSE_TOLERANCE ) {
      smax = SIDE_Z_MIN;
      tmax = t;
    }
    else
      // If intersection points are close to each other find out
      // which side to use, i.e. is most probably hit. [DB 9/94]
      if ( t <= tmax + CLOSE_TOLERANCE )
	switch (smax) {
	case SIDE_X_MIN :
	case SIDE_X_MAX :
	  if ( -a_Direction[__Z] > fabsf(a_Direction[__X]) )
	    smax = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Direction[__Z] > fabsf(a_Direction[__Y]) )
	    smax = SIDE_Z_MIN;
	  break;
	}
    
    t = ( m_Max[__Z] - a_Origin[__Z] ) * a_ReverseDirection[__Z];
    if ( t >= tmin + CLOSE_TOLERANCE )  {
      if ( t > tmax )
	return false;
      smin = SIDE_Z_MAX;
      tmin = t;
    }
    else
      // If intersection points are close to each other find out
      // which side to use, i.e. is most probably hit. [DB 9/94]
      if ( t >= tmin - CLOSE_TOLERANCE )
	switch (smin)  {
	case SIDE_X_MIN :
	case SIDE_X_MAX :
	  if ( -a_Direction[__Z] > fabsf(a_Direction[__X]) )
	    smin = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Direction[__Z] > fabsf(a_Direction[__Y]) )
	    smin = SIDE_Z_MAX;
	  break;
	}
  }
  else if ( a_Direction[__Z] > EPS ) {
    t = ( m_Max[__Z] - a_Origin[__Z] ) * a_ReverseDirection[__Z];
    if ( t < tmin )
      return false;
    if ( t <= tmax - CLOSE_TOLERANCE ) {
      smax = SIDE_Z_MAX;
      tmax = t;
    }
    else
      // If intersection points are close to each other find out
      // which side to use, i.e. is most probably hit. [DB 9/94]
      if ( t <= tmax + CLOSE_TOLERANCE )
	switch ( smax ) {
	case SIDE_X_MIN :
	case SIDE_X_MAX :
	  if ( a_Direction[__Z] > fabsf(a_Direction[__X]) )
	    smax = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Direction[__Z] > fabsf(a_Direction[__Y]) )
	    smax = SIDE_Z_MAX;
	  break;
	}

    t = ( m_Min[__Z] - a_Origin[__Z]) * a_ReverseDirection[__Z];
    if ( t >= tmin + CLOSE_TOLERANCE ) {
      if ( t > tmax )
	return false;
      smin = SIDE_Z_MIN;
      tmin = t;
    }
    else
      // If intersection points are close to each other find out
      // which side to use, i.e. is most probably hit. [DB 9/94]
      if ( t >= tmin - CLOSE_TOLERANCE )
	switch (smin) {
	case SIDE_X_MIN :
	case SIDE_X_MAX :
	  if ( a_Direction[__Z] > fabsf(a_Direction[__X]) )
	    smin = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Direction[__Z] > fabsf(a_Direction[__Y]) )
	    smin = SIDE_Z_MIN;
	  break;
	}
  }
  else if ( ( a_Origin[__Z] < m_Min[__Z]) || (a_Origin[__Z] > m_Max[__Z]) )
    return false;

  if ( tmin < DEPTH_TOLERANCE ) { // a_Origin was inside box
    if ( tmax < DEPTH_TOLERANCE )
      return false;
    tmin = tmax;
  }

  a_Distance = tmin;

  return true;
}

void BoundingAABB::print( const char *msg ) const {
  TBounding::print( msg );
  m_Min.print("min: ");
  m_Max.print("max: ");
}

void BoundingAABB::renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const {
  renderBox( screen, width, height, m_Min, m_Max, color, camera );
}
