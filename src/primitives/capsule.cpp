#include <stdio.h>

#include "../world.h"
#include "capsule.h"
#include "../ray.h"
#include "../cameras/camera.h"

const TTagList PrimitiveCapsule::primitiveCapsuleTags [] = {
  { "p1", TAG_primitiveCapsule_p1 },
  { "p2", TAG_primitiveCapsule_p2 },
  { "radius", TAG_primitiveCapsule_radius },
  { NULL, -1 }
};

static const enum EIntersectionKind {
  INTERSECTION_BOTTOM_HEMISPHERE,
  INTERSECTION_TOP_HEMISPHERE,
  INTERSECTION_CYLINDER_WALL
};

PrimitiveCapsule::PrimitiveCapsule() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_CAPSULE;
}

void PrimitiveCapsule::setup() {
  radius2 = radius*radius;
  invRadius = 1.0f/radius;
  direction = p2 - p1;
  height = direction.magnitude();
  direction.normalize();
  U = direction; V = direction; W = direction;
  generateOrthonormalBasis( U, V, W, true );
}

void PrimitiveCapsule::precalcFrame( const TWorld &world ) {
  TVector3 Diff = world.currentCamera->location - p1;
  precalcP = TVector3( U | Diff, V | Diff, W | Diff );
  precalcC = precalcP[0]*precalcP[0] + precalcP[1]*precalcP[1] - radius2;
}

bool PrimitiveCapsule::intersectionRayCamera( const TRay &ray,
					      TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create( 2 );
  if ( !PrimitiveCapsule::allIntersectionRayCamera( ray, IStack ) )
    return false;
#ifdef __DEBUG__
  printf("numIntersections: %d\n", IStack.index );
#endif
  if ( IStack.index == 1 ) {
    inter.copyIntersection( IStack.stack[0] );
    return true;
  }
  else if ( IStack.index == 2 ) {
#ifdef __DEBUG__
    printf("intersections: %5.5f   %5.5f\n", IStack.stack[0].distance, IStack.stack[1].distance );
    printf("additional: %d  %d\n", IStack.stack[0].additionalInformation.side, IStack.stack[1].additionalInformation.side );
#endif
    float closestDistance = INFINITE;
    int closestIndex = -1;
    for ( int i = 0; i < IStack.index; i++ )
      if ( IStack.stack[i].distance > 0.0f
	   && IStack.stack[i].distance < closestDistance ) {
	closestDistance = IStack.stack[i].distance;
	closestIndex = i;
      }
    if ( closestIndex == -1 ) {
#ifdef __DEBUG__
      printf("no positive intersection found\n");
#endif
      return false;
    }
    inter.copyIntersection( IStack.stack[closestIndex] );
    return true;
  }
  return false;
}

bool PrimitiveCapsule::intersectionRay( const TRay &ray,
					TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create( 2 );
  if ( !PrimitiveCapsule::allIntersectionRay( ray, IStack ) )
    return false;
#ifdef __DEBUG__
  printf("numIntersections: %d\n", IStack.index );
#endif
  if ( IStack.index == 1 ) {
    inter.copyIntersection( IStack.stack[0] );
    return true;
  }
  else if ( IStack.index == 2 ) {
#ifdef __DEBUG__
    printf("intersections: %5.5f   %5.5f\n", IStack.stack[0].distance, IStack.stack[1].distance );
    printf("additional: %d  %d\n", IStack.stack[0].additionalInformation.side, IStack.stack[1].additionalInformation.side );
#endif
    float closestDistance = INFINITE;
    int closestIndex = -1;
    for ( int i = 0; i < IStack.index; i++ )
      if ( IStack.stack[i].distance > 0.0f
	   && IStack.stack[i].distance < closestDistance ) {
	closestDistance = IStack.stack[i].distance;
	closestIndex = i;
      }
    if ( closestIndex == -1 ) {
#ifdef __DEBUG__
      printf("no positive intersection found\n");
#endif
      return false;
    }
    inter.copyIntersection( IStack.stack[closestIndex] );
    return true;
  }
  return false;
}

bool PrimitiveCapsule::allIntersectionRayCamera( const TRay &ray,
						 TIntersectionStack &IStack ) const {
  // set up quadratic Q(t) = a*t^2 + 2*b*t + c
  TVector3 D( U | ray.direction, V | ray.direction, W | ray.direction );

  // Is the velocity parallel to the capsule direction? (or zero)
  if ( fabsf(D[2]) >= 1.0f - EPS ) {
    float AxisDir = ray.direction | direction;
    if ( AxisDir < 0.0f && precalcC <= 0.0f ) {
      // Velocity anti-parallel to the capsule direction
      float Root = sqrtf(-precalcC);
      IStack.push( precalcP[2] + Root, INTERSECTION_TOP_HEMISPHERE );
      IStack.push( -( height - precalcP[2] + Root ), INTERSECTION_BOTTOM_HEMISPHERE );
      return true;
    }
    else if ( AxisDir > 0.0f && precalcC <= 0.0f ) {
      // Velocity parallel to the capsule direction
      float Root = sqrtf(-precalcC);
      IStack.push( -precalcP[2] + Root, INTERSECTION_TOP_HEMISPHERE );
      IStack.push( ( height - precalcP[2] + Root ), INTERSECTION_BOTTOM_HEMISPHERE );
      return true;
    }
    else
      // sphere heading wrong direction, or no velocity at all
      return false;
  }
  // test intersection with infinite cylinder
  float A = D[0]*D[0] + D[1]*D[1];
  float B = precalcP[0]*D[0] + precalcP[1]*D[1];
  float Discr = B*B-A*precalcC;
  if ( Discr < 0.0f )
    // line does not intersect infinite cylinder
    return false;

  if ( Discr > 0.0f ) {
    // line intersects infinite cylinder in two places
    float Root = sqrtf(Discr);
    float Inv = 1.0f / A;
    float T = (-B - Root ) * Inv;
    float Tmp = precalcP[2] + T*D[2];
    if ( 0.0f <= Tmp && Tmp <= height )
      IStack.push( T, INTERSECTION_CYLINDER_WALL );
    T = (-B + Root ) * Inv;
    Tmp = precalcP[2] + T*D[2];
    if ( 0.0f <= Tmp && Tmp <= height )
      IStack.push( T, INTERSECTION_CYLINDER_WALL );
    if ( IStack.index == 2 )
      // line intersects capsule wall in two places
      return true;
  }
  else {
    // line is tangent to infinite cylinder
    float T = -B/A;
    float Tmp = precalcP[2] + T*D[2];
    if ( 0.0f <= Tmp && Tmp <= height ) {
      IStack.push( T, INTERSECTION_CYLINDER_WALL );
      return true;
    }
  }

  // test intersection with bottom hemisphere
  // A = 1.0f
  B += precalcP[2]*D[2];
  float C = precalcC + precalcP[2]*precalcP[2];
  Discr = B*B-C;
  if ( Discr > 0.0f ) {
    float Root = sqrtf(Discr);
    float T = -B - Root;
    float Tmp = precalcP[2] + T*D[2];
    if ( Tmp <= 0.0f ) {
      IStack.push( T, INTERSECTION_BOTTOM_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
    T = -B + Root;
    Tmp = precalcP[2] + T*D[2];
    if ( Tmp <= 0.0f ) {
      IStack.push( T, INTERSECTION_BOTTOM_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
  }
  else if ( Discr == 0.0f ) {
    float T = -B;
    float Tmp = precalcP[2] + T*D[2];
    if ( Tmp <= 0.0f ) {
      IStack.push( T, INTERSECTION_BOTTOM_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
  }

  // test intersection with top hemisphere
  // A = 1
  B -= D[2] * height;
  C += height * ( height - 2.0f*precalcP[2]);
  Discr = B*B-C;
  if ( Discr > 0.0f ) {
    float Root = sqrtf(Discr);
    float T = -B - Root;
    float Tmp = precalcP[2] + T*D[2];
    if ( Tmp >= height ) {
      IStack.push( T, INTERSECTION_TOP_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
    T = -B + Root;
    Tmp = precalcP[2] + T*D[2];
    if ( Tmp >= height ) {
      IStack.push( T, INTERSECTION_TOP_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    } 
  }
  else if ( Discr == 0.0f ) {
    float T = -B;
    float Tmp = precalcP[2] + T*D[2];
    if ( Tmp >= height ) {
      IStack.push( T, INTERSECTION_TOP_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
  }
  return true;
}

bool PrimitiveCapsule::intersectsRay( const TRay &ray, const float maxDist2 ) const {
  TIntersection inter;
  if ( !PrimitiveCapsule::intersectionRay( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < maxDist2 )
    return true;
  return false;
}

bool PrimitiveCapsule::allIntersectionRay( const TRay &ray,
					   TIntersectionStack &IStack ) const {
  // set up quadratic Q(t) = a*t^2 + 2*b*t + c
  TVector3 D( U | ray.direction, V | ray.direction, W | ray.direction );
  TVector3 Diff = ray.origin - p1;
  TVector3 P( U | Diff, V | Diff, W | Diff );

  float Inv, A, B, C, Discr, Root, T, Tmp;
  // Is the velocity parallel to the capsule direction? (or zero)
  if ( fabsf(D[2]) >= 1.0f - EPS ) {
    float AxisDir = ray.direction | direction;
    Discr = radius2 - P[0]*P[0] - P[1] * P[1];
    if ( AxisDir < 0.0f && Discr >= 0.0f ) {
      // Velocity anti-parallel to the capsule direction
      Root = sqrtf(Discr);
      IStack.push( P[2] + Root, INTERSECTION_TOP_HEMISPHERE );
      IStack.push( -( height - P[2] + Root ), INTERSECTION_BOTTOM_HEMISPHERE );
      return true;
    }
    else if ( AxisDir > 0.0f && Discr >= 0.0f ) {
      // Velocity parallel to the capsule direction
      Root = sqrtf(Discr);
      IStack.push( -P[2] + Root, INTERSECTION_TOP_HEMISPHERE );
      IStack.push( ( height - P[2] + Root ), INTERSECTION_BOTTOM_HEMISPHERE );
      return true;
    }
    else
      // sphere heading wrong direction, or no velocity at all
      return false;
  }
  // test intersection with infinite cylinder
  A = D[0]*D[0] + D[1]*D[1];
  B = P[0]*D[0] + P[1]*D[1];
  C = P[0]*P[0] + P[1]*P[1] - radius2;
  Discr = B*B-A*C;
  if ( Discr < 0.0f )
    // line does not intersect infinite cylinder
    return false;

  if ( Discr > 0.0f ) {
    // line intersects infinite cylinder in two places
    Root = sqrtf(Discr);
    Inv = 1.0f / A;
    T = (-B - Root ) * Inv;
    Tmp = P[2] + T*D[2];
    if ( 0.0f <= Tmp && Tmp <= height )
      IStack.push( T, INTERSECTION_CYLINDER_WALL );
    T = (-B + Root ) * Inv;
    Tmp = P[2] + T*D[2];
    if ( 0.0f <= Tmp && Tmp <= height )
      IStack.push( T, INTERSECTION_CYLINDER_WALL );
    if ( IStack.index == 2 )
      // line intersects capsule wall in two places
      return true;
  }
  else {
    // line is tangent to infinite cylinder
    T = -B/A;
    Tmp = P[2] + T*D[2];
    if ( 0.0f <= Tmp && Tmp <= height ) {
      IStack.push( T, INTERSECTION_CYLINDER_WALL );
      return true;
    }
  }

  // test intersection with bottom hemisphere
  // A = 1.0f
  B += P[2]*D[2];
  C += P[2]*P[2];
  Discr = B*B-C;
  if ( Discr > 0.0f ) {
    Root = sqrtf(Discr);
    T = -B - Root;
    Tmp = P[2] + T*D[2];
    if ( Tmp <= 0.0f ) {
      IStack.push( T, INTERSECTION_BOTTOM_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
    T = -B + Root;
    Tmp = P[2] + T*D[2];
    if ( Tmp <= 0.0f ) {
      IStack.push( T, INTERSECTION_BOTTOM_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
  }
  else if ( Discr == 0.0f ) {
    T = -B;
    Tmp = P[2] + T*D[2];
    if ( Tmp <= 0.0f ) {
      IStack.push( T, INTERSECTION_BOTTOM_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
  }

  // test intersection with top hemisphere
  // A = 1
  B -= D[2] * height;
  C += height * ( height - 2.0f*P[2]);
  Discr = B*B-C;
  if ( Discr > 0.0f ) {
    Root = sqrtf(Discr);
    T = -B - Root;
    Tmp = P[2] + T*D[2];
    if ( Tmp >= height ) {
      IStack.push( T, INTERSECTION_TOP_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
    T = -B + Root;
    Tmp = P[2] + T*D[2];
    if ( Tmp >= height ) {
      IStack.push( T, INTERSECTION_TOP_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    } 
  }
  else if ( Discr == 0.0f ) {
    T = -B;
    Tmp = P[2] + T*D[2];
    if ( Tmp >= height ) {
      IStack.push( T, INTERSECTION_TOP_HEMISPHERE );
      if ( IStack.index == 2 )
	return true;
    }
  }
  return true;
}

TVector3 PrimitiveCapsule::normalAt( const TIntersection &inter ) const {
  switch( inter.additionalInformation.side ) {
  case INTERSECTION_CYLINDER_WALL: {
    TVector3 v = inter.pointWorldSpace-p1;
    float p = ( v | direction );
    return ( v - direction*p ) * invRadius;
    break;
  }
  case INTERSECTION_TOP_HEMISPHERE: {
    return ( inter.pointWorldSpace - p2 ) * invRadius;
    break;
  }
  case INTERSECTION_BOTTOM_HEMISPHERE: {
    return ( inter.pointWorldSpace - p1 ) * invRadius;
    break;
  }
  }
  return TVector3::zeroVector;
}

bool PrimitiveCapsule::inside( const TPoint3 &p ) const {
  // distance on cylinder axis from center to tested point projection on cylinder axis
  TPoint3 center = ( p1 + p2 ) * 0.5f;
  TVector3 p2center = p-center;
  float d = ( p2center | direction );
  float dAbs = fabsf(d);
  // compare d to cylinder height + radius
  if ( dAbs > height * 0.5f + radius ) // outside augmented capsule (axis direction)
    return false;
  // distance from tested point to tested point projection on cylinder axis
  float dist2 = ( p2center - (direction*d) ).magnitudeSquared();
  // compare this distance to radius
  if ( dist2 > radius2 ) // outside cylinder
    return false;
  if ( dAbs < height * 0.5f ) // inside capped cylinder: capsule without hemispheres
    return true;
  if ( d > 0 ) { // p is p2 hemisphere ( direction = p2-p1 )
    if ( ( p - p2 ).magnitudeSquared() > radius2 ) // p is outside p2 hemisphere
      return false;
  }
  else {
    if ( ( p - p1 ).magnitudeSquared() > radius2 ) // p is outside p1 hemisphere
      return false;
  }
  return true;
}

void PrimitiveCapsule::print( const char *msg ) const {
  TPrimitive::print( msg );
  p1.print("point1: ");
  p2.print("point2: ");
  printf("radius: ");
}

