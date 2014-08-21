#include <stdio.h>

#include "../world.h"
#include "box.h"
#include "../intersection.h"
#include "../ray.h"
#include "../poly3d/rendersimple.h"
#include "../cameras/camera.h"
#include "../boundings/aabb.h"

const TTagList PrimitiveBox::primitiveBoxTags [] = {
  { "min", TAG_primitiveBox_min },
  { "max", TAG_primitiveBox_max },
  { NULL, -1 }
};

// Side ID
static const enum BoxSide {
  SIDE_X_MIN,     // x < 0
  SIDE_X_MAX,     // x > 0
  SIDE_Y_MIN,     // y < 0
  SIDE_Y_MAX,     // y > 0
  SIDE_Z_MIN,     // z < 0
  SIDE_Z_MAX      // z > 0
};

PrimitiveBox::PrimitiveBox() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_BOX;
  m_Min = TPoint3( 0, 0, 0 );
  m_Max = TPoint3( 0, 0, 0 );
}

void PrimitiveBox::precalcFrame( const TWorld &a_World ) {
  precalcRayToMin = m_Min - a_World.currentCamera->location;
  precalcRayToMax = m_Max - a_World.currentCamera->location;
}

// Minimal intersection depth.
#define DEPTH_TOLERANCE (1.0e-6f)
// Two values are equal if their difference is small than CLOSE_TOLERANCE.
#define CLOSE_TOLERANCE (1.0e-6f)

bool PrimitiveBox::intersectionRayCamera( const TRay &a_Ray,
					  TIntersection &a_Inter ) const {
  int smin = -1, smax = -1; // Side hit for min/max intersection.
  float t, tmin, tmax;
  tmin = 0.0f;
  tmax = INFINITE;

  // Sides first.
  if ( a_Ray.direction[__X] < -EPS) {
    t = precalcRayToMin[__X] * a_Ray.reverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MIN;
      tmax = t;
    }

    t = precalcRayToMax[__X] * a_Ray.reverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MAX;
      tmin = t;
    }
  }
  else if ( a_Ray.direction[__X] > EPS ) {
    t = precalcRayToMax[__X] * a_Ray.reverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MAX;
      tmax = t;
    }
    
    t = precalcRayToMin[__X] * a_Ray.reverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MIN;
      tmin = t;
    }
  }
  else if ( precalcRayToMin[__X] > 0 || precalcRayToMax[__X] < 0 )
    return false;

  // Check Top/Bottom.
  if ( a_Ray.direction[__Y] < -EPS ) {
    t = precalcRayToMin[__Y] * a_Ray.reverseDirection[__Y];
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
        if ( -a_Ray.direction[__Y] > fabsf( a_Ray.direction[__X] ) )
	  smax = SIDE_Y_MIN;
    
    t = precalcRayToMax[__Y] * a_Ray.reverseDirection[__Y];
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
        if ( -a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smin = SIDE_Y_MAX;
  }
  else if ( a_Ray.direction[__Y] > EPS ) {
    t = precalcRayToMax[__Y] * a_Ray.reverseDirection[__Y];
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
	if ( a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smax = SIDE_Y_MAX;
      
    t = precalcRayToMin[__Y] * a_Ray.reverseDirection[__Y];
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
	if ( a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smin = SIDE_Y_MIN;
  }
  else if ( precalcRayToMin[__Y] > 0 || precalcRayToMax[__Y] < 0 )
    return false;

  // Now front/back
  if ( a_Ray.direction[__Z] < -EPS ) {
    t = precalcRayToMin[__Z] * a_Ray.reverseDirection[__Z];
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
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smax = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smax = SIDE_Z_MIN;
	  break;
	}
    
    t = precalcRayToMax[__Z] * a_Ray.reverseDirection[__Z];
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
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smin = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smin = SIDE_Z_MAX;
	  break;
        }
  }
  else if ( a_Ray.direction[__Z] > EPS ) {
    t = precalcRayToMax[__Z] * a_Ray.reverseDirection[__Z];
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
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smax = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smax = SIDE_Z_MAX;
	  break;
	}

    t = precalcRayToMin[__Z] * a_Ray.reverseDirection[__Z];
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
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smin = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smin = SIDE_Z_MIN;
	  break;
	}
  }
  else if ( precalcRayToMin[__Z] > 0 || precalcRayToMax[__Z] < 0 )
    return false;

  if ( tmin < DEPTH_TOLERANCE ) { // a_Ray.origin was inside box
    if ( tmax < DEPTH_TOLERANCE )
      return false;
    tmin = tmax;
    smin = smax;
  }
  
  a_Inter.distance = tmin;
  a_Inter.additionalInformation.side = smin;
  
  return true;
}

bool PrimitiveBox::intersectionRay( const TRay &a_Ray,
				    TIntersection &a_Inter ) const {
  int smin = -1, smax = -1; // Side hit for min/max intersection.
  float t, tmin, tmax;
  tmin = 0.0f;
  tmax = INFINITE;
  
  // Sides first.
  if ( a_Ray.direction[__X] < -EPS ) {
    t = ( m_Min[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MIN;
      tmax = t;
    }
    
    t = ( m_Max[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MAX;
      tmin = t;
    }
  }
  else if ( a_Ray.direction[__X] > EPS ) {
    t = ( m_Max[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MAX;
      tmax = t;
    }
    
    t = (m_Min[__X] - a_Ray.origin[__X]) * a_Ray.reverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MIN;
      tmin = t;
    }
  }
  else if ( ( a_Ray.origin[__X] < m_Min[__X] ) || ( a_Ray.origin[__X] > m_Max[__X] ) )
    return false;

  // Check Top/Bottom.
  if ( a_Ray.direction[__Y] < -EPS ) {
    t = ( m_Min[__Y] - a_Ray.origin[__Y] ) * a_Ray.reverseDirection[__Y];
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
	if ( -a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X] ) )
	  smax = SIDE_Y_MIN;
    
    t = (m_Max[__Y] - a_Ray.origin[__Y]) * a_Ray.reverseDirection[__Y];
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
	if ( -a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smin = SIDE_Y_MAX;
  }
  else if (a_Ray.direction[__Y] > EPS ) {
    t = (m_Max[__Y] - a_Ray.origin[__Y]) * a_Ray.reverseDirection[__Y];
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
	if ( a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smax = SIDE_Y_MAX;
      
    t = ( m_Min[__Y] - a_Ray.origin[__Y] ) * a_Ray.reverseDirection[__Y];
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
	if ( a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smin = SIDE_Y_MIN;
  }
  else if ( ( a_Ray.origin[__Y] < m_Min[__Y] ) || ( a_Ray.origin[__Y] > m_Max[__Y] ) )
    return false;

  // Now front/back
  if ( a_Ray.direction[__Z] < -EPS ) {
    t = ( m_Min[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
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
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smax = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smax = SIDE_Z_MIN;
	  break;
	}
    
    t = ( m_Max[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
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
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smin = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smin = SIDE_Z_MAX;
	  break;
	}
  }
  else if ( a_Ray.direction[__Z] > EPS ) {
    t = ( m_Max[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
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
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smax = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smax = SIDE_Z_MAX;
	  break;
	}

    t = ( m_Min[__Z] - a_Ray.origin[__Z]) * a_Ray.reverseDirection[__Z];
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
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smin = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smin = SIDE_Z_MIN;
	  break;
	}
  }
  else if ( ( a_Ray.origin[__Z] < m_Min[__Z]) || (a_Ray.origin[__Z] > m_Max[__Z]) )
    return false;

  if ( tmin < DEPTH_TOLERANCE ) { // a_Ray.origin was inside box
    if ( tmax < DEPTH_TOLERANCE )
      return false;
    tmin = tmax;
    smin = smax;
  }

  a_Inter.distance = tmin;
  a_Inter.additionalInformation.side = smin;

  return true;
}

bool PrimitiveBox::intersectsRay( const TRay &a_Ray, const float distMax2 ) const {
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
  if ( tzmax < tmax )
    tmax = tzmax;
  return  tmin > 0 && ( tmax*tmax < distMax2 );
}

bool PrimitiveBox::allIntersectionRayCamera( const TRay &a_Ray,
					       TIntersectionStack &a_IStack ) const {
  int smin = -1, smax = -1; // Side hit for min/max intersection.
  float t, tmin, tmax;
  tmin = 0.0f;
  tmax = INFINITE;

  // Sides first.
  if ( a_Ray.direction[__X] < -EPS) {
    t = precalcRayToMin[__X] * a_Ray.reverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MIN;
      tmax = t;
    }

    t = precalcRayToMax[__X] * a_Ray.reverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MAX;
      tmin = t;
    }
  }
  else if ( a_Ray.direction[__X] > EPS ) {
    t = precalcRayToMax[__X] * a_Ray.reverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MAX;
      tmax = t;
    }
    
    t = precalcRayToMin[__X] * a_Ray.reverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MIN;
      tmin = t;
    }
  }
  else if ( precalcRayToMin[__X] > 0 || precalcRayToMax[__X] < 0 )
    return false;

  // Check Top/Bottom.
  if ( a_Ray.direction[__Y] < -EPS ) {
    t = precalcRayToMin[__Y] * a_Ray.reverseDirection[__Y];
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
	if ( -a_Ray.direction[__Y] > fabsf( a_Ray.direction[__X] ) )
	  smax = SIDE_Y_MIN;
    
    t = precalcRayToMax[__Y] * a_Ray.reverseDirection[__Y];
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
	if ( -a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smin = SIDE_Y_MAX;
  }
  else if ( a_Ray.direction[__Y] > EPS ) {
    t = precalcRayToMax[__Y] * a_Ray.reverseDirection[__Y];
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
	if ( a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smax = SIDE_Y_MAX;
      
    t = precalcRayToMin[__Y] * a_Ray.reverseDirection[__Y];
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
	if ( a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smin = SIDE_Y_MIN;
  }
  else if ( precalcRayToMin[__Y] > 0 || precalcRayToMax[__Y] < 0 )
    return false;

  // Now front/back
  if ( a_Ray.direction[__Z] < -EPS ) {
    t = precalcRayToMin[__Z] * a_Ray.reverseDirection[__Z];
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
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smax = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smax = SIDE_Z_MIN;
	  break;
	}
    
    t = precalcRayToMax[__Z] * a_Ray.reverseDirection[__Z];
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
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smin = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smin = SIDE_Z_MAX;
	  break;
	}
  }
  else if ( a_Ray.direction[__Z] > EPS ) {
    t = precalcRayToMax[__Z] * a_Ray.reverseDirection[__Z];
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
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smax = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smax = SIDE_Z_MAX;
	  break;
	}

    t = precalcRayToMin[__Z] * a_Ray.reverseDirection[__Z];
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
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smin = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smin = SIDE_Z_MIN;
	  break;
	}
  }
  else if ( precalcRayToMin[__Z] > 0 || precalcRayToMax[__Z] < 0 )
    return false;

  if ( tmin < DEPTH_TOLERANCE ) { // a_Ray.origin was inside box
    if ( tmax < DEPTH_TOLERANCE )
      return false;
    a_IStack.push( tmax, smax );
    return true;
  }
  a_IStack.push( tmin, smin );
  a_IStack.push( tmax, smax );
  return true;
}

bool PrimitiveBox::allIntersectionRay( const TRay &a_Ray,
				       TIntersectionStack &a_IStack ) const {
  int smin = -1, smax = -1; // Side hit for min/max intersection.
  float t, tmin, tmax;
  tmin = 0.0f;
  tmax = INFINITE;

  // Sides first.
  if ( a_Ray.direction[__X] < -EPS) {
    t = ( m_Min[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MIN;
      tmax = t;
    }

    t = ( m_Max[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MAX;
      tmin = t;
    }
  }
  else if ( a_Ray.direction[__X] > EPS ) {
    t = ( m_Max[__X] - a_Ray.origin[__X] ) * a_Ray.reverseDirection[__X];
    if ( t < tmin )
      return false;
    if ( t <= tmax ) {
      smax = SIDE_X_MAX;
      tmax = t;
    }
    
    t = (m_Min[__X] - a_Ray.origin[__X]) * a_Ray.reverseDirection[__X];
    if ( t >= tmin ) {
      if ( t > tmax )
	return false;
      smin = SIDE_X_MIN;
      tmin = t;
    }
  }
  else if ( ( a_Ray.origin[__X] < m_Min[__X] ) || ( a_Ray.origin[__X] > m_Max[__X] ) )
    return false;

  // Check Top/Bottom.
  if ( a_Ray.direction[__Y] < -EPS ) {
    t = ( m_Min[__Y] - a_Ray.origin[__Y] ) * a_Ray.reverseDirection[__Y];
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
	if ( -a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X] ) )
	  smax = SIDE_Y_MIN;
    
    t = (m_Max[__Y] - a_Ray.origin[__Y]) * a_Ray.reverseDirection[__Y];
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
	if ( -a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smin = SIDE_Y_MAX;
  }
  else if (a_Ray.direction[__Y] > EPS ) {
    t = (m_Max[__Y] - a_Ray.origin[__Y]) * a_Ray.reverseDirection[__Y];
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
	if ( a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smax = SIDE_Y_MAX;
      
    t = ( m_Min[__Y] - a_Ray.origin[__Y] ) * a_Ray.reverseDirection[__Y];
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
	if ( a_Ray.direction[__Y] > fabsf(a_Ray.direction[__X]) )
	  smin = SIDE_Y_MIN;
  }
  else if ( ( a_Ray.origin[__Y] < m_Min[__Y] ) || ( a_Ray.origin[__Y] > m_Max[__Y] ) )
    return false;

  // Now front/back
  if ( a_Ray.direction[__Z] < -EPS ) {
    t = ( m_Min[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
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
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smax = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smax = SIDE_Z_MIN;
	  break;
	}
    
    t = ( m_Max[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
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
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smin = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( -a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smin = SIDE_Z_MAX;
	  break;
	}
  }
  else if ( a_Ray.direction[__Z] > EPS ) {
    t = ( m_Max[__Z] - a_Ray.origin[__Z] ) * a_Ray.reverseDirection[__Z];
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
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smax = SIDE_Z_MAX;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smax = SIDE_Z_MAX;
	  break;
	}

    t = ( m_Min[__Z] - a_Ray.origin[__Z]) * a_Ray.reverseDirection[__Z];
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
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__X]) )
	    smin = SIDE_Z_MIN;
	  break;
	case SIDE_Y_MIN :
	case SIDE_Y_MAX :
	  if ( a_Ray.direction[__Z] > fabsf(a_Ray.direction[__Y]) )
	    smin = SIDE_Z_MIN;
	  break;
	}
  }
  else if ( ( a_Ray.origin[__Z] < m_Min[__Z]) || (a_Ray.origin[__Z] > m_Max[__Z]) )
    return false;

  if ( tmin < DEPTH_TOLERANCE ) { // a_Ray.origin was inside box
    if ( tmax < DEPTH_TOLERANCE )
      return false;
    a_IStack.push( tmax, smax );
    return true;
  }
  a_IStack.push( tmin, smin );
  a_IStack.push( tmax, smax );
  return true;
}

TVector3 PrimitiveBox::normalAt( const TIntersection &inter ) const {
  TVector3 resultVector = TVector3::zeroVector;
  switch ( inter.additionalInformation.side ) {
  case SIDE_X_MIN: resultVector = TVector3( -1, 0, 0 ); break;
  case SIDE_X_MAX: resultVector = TVector3( +1, 0, 0 ); break;
  case SIDE_Y_MIN: resultVector = TVector3( 0, -1, 0 ); break;
  case SIDE_Y_MAX: resultVector = TVector3( 0, +1, 0 ); break;
  case SIDE_Z_MIN: resultVector = TVector3( 0, 0, -1 ); break;
  case SIDE_Z_MAX: resultVector = TVector3( 0, 0, +1 ); break;
  }
  return resultVector;
}

bool PrimitiveBox::inside( const TPoint3 &a_P ) const {
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

bool PrimitiveBox::intersectsBox( const BoundingAABB &a_Box ) const {
  return ( ( m_Max.x > a_Box.m_Min.x ) && ( m_Min.x < a_Box.m_Max.x ) && // x-axis overlap
	   ( m_Max.y > a_Box.m_Min.y ) && ( m_Min.y < a_Box.m_Max.y ) && // y-axis overlap
	   ( m_Max.z > a_Box.m_Min.z ) && ( m_Min.z < a_Box.m_Max.z ) ); // z-axis overlap
}

bool PrimitiveBox::computeBoundingAABB( BoundingAABB &a_Box ) const {
  a_Box.m_Min = m_Min;
  a_Box.m_Max = m_Max;
  return true;
}

void PrimitiveBox::print( const char *msg ) const {
  TPrimitive::print( msg );
  m_Min.print("min: "); m_Max.print("max: ");
}

void PrimitiveBox::renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const {
  renderBox( screen, width, height, m_Min, m_Max, color, camera );
}
