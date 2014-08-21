#include <stdio.h>

#include "../world.h"
#include "cylinder.h"
#include "../math/math.h"
#include "../ray.h"
#include "../cameras/camera.h"

const TTagList PrimitiveCylinder::primitiveCylinderTags [] = {
  { "center", TAG_primitiveCylinder_center },
  { "axis", TAG_primitiveCylinder_axis },
  { "radius", TAG_primitiveCylinder_radius },
  { "height", TAG_primitiveCylinder_height },
  { NULL, -1 }
};

static const enum EIntersectionKind {
  INTERSECTION_BOTTOM_CAP,
  INTERSECTION_TOP_CAP,
  INTERSECTION_CYLINDER_WALL
};

PrimitiveCylinder::PrimitiveCylinder() {
  TPrimitive::TPrimitive(); primitiveKind = TPrimitive::PRIMITIVE_CYLINDER;
  capped = false;
}

void PrimitiveCylinder::setup() {
  radius2 = radius*radius;
  invRadius = 1.0f/radius;
  axis.normalize();
  U = axis; V = axis; W = axis;
  generateOrthonormalBasis( U, V, W, true );
  if ( !capped )
    additionalInformation.setBit( PRIMITIVE_INFINITE );
}

void PrimitiveCylinder::precalcFrame( const TWorld &world ) {
  if ( !capped ) {
    precalcRC = world.currentCamera->location - center;
    precalcO = crossProduct( precalcRC, axis );
  }
  else {
    TVector3 Diff = world.currentCamera->location - center;
    precalcP = TVector3( U | Diff, V | Diff, W | Diff );
    precalcC = precalcP[0]*precalcP[0] + precalcP[1]*precalcP[1] - radius2;
  }
}

bool PrimitiveCylinder::intersectionRayCamera( const TRay &ray,
					       TIntersection &inter ) const {
  if ( capped )
    return PrimitiveCylinder::intersectionRayCappedCamera( ray, inter );
  else
    return PrimitiveCylinder::intersectionRayNonCappedCamera( ray, inter );
}
bool PrimitiveCylinder::intersectionRay( const TRay &ray,
					 TIntersection &inter ) const {
  if ( capped )
    return PrimitiveCylinder::intersectionRayCapped( ray, inter );
  else
    return PrimitiveCylinder::intersectionRayNonCapped( ray, inter );
}
bool PrimitiveCylinder::intersectsRay( const TRay &ray, const float maxDist2 ) const {
  if ( capped )
    return PrimitiveCylinder::intersectsRayCapped( ray, maxDist2 );
  else
    return PrimitiveCylinder::intersectsRayNonCapped( ray, maxDist2 );
}
bool PrimitiveCylinder::allIntersectionRayCamera( const TRay &ray,
						  TIntersectionStack &IStack ) const {
  if ( capped )
    return PrimitiveCylinder::allIntersectionRayCappedCamera( ray, IStack );
  else
    return PrimitiveCylinder::allIntersectionRayNonCappedCamera( ray, IStack );
}
bool PrimitiveCylinder::allIntersectionRay( const TRay &ray,
					    TIntersectionStack &IStack ) const {
  if ( capped )
    return PrimitiveCylinder::allIntersectionRayCapped( ray, IStack );
  else
    return PrimitiveCylinder::allIntersectionRayNonCapped( ray, IStack );
}
bool PrimitiveCylinder::inside( const TPoint3 &p ) const {
  if ( capped )
    return PrimitiveCylinder::insideCapped( p );
  else
    return PrimitiveCylinder::insideNonCapped( p );
}

//------------------
//--- Non-Capped ---
bool PrimitiveCylinder::intersectionRayNonCappedCamera( const TRay &ray,
							TIntersection &inter ) const {
  TVector3 n = crossProduct( ray.direction, axis ); // 6 mul  3 add

  float ln = n.magnitudeSquared(); // 3 mul  2 add
  if ( ln == 0.0f ) // ray parallel to cylinder
    return false;

  float oneOverSqrtLn = fastInvSqrt( ln ); // 8 mul  3 add
  n *= oneOverSqrtLn; // 1 mul
  float d = fabsf( precalcRC | n );		// shortest distance

  if  ( d <= radius ) {				// if ray hits cylinder
    float t = - (( precalcO | n ) * oneOverSqrtLn); // 3 mul  2 add
    TVector3 O = crossProduct( n, axis ); // 6 mul  3 add
    float s = fabsf( sqrtf(radius2 - d*d) / ( ray.direction | O ) ); // 1 sqrt  1 div  5 mul  4 add
    inter.distance = t - s;
    if ( inter.distance > 0 )
      return true;
    inter.distance = t + s;
    if ( inter.distance > 0 )
      return true;
    return false;
  }
  return false;
}
bool PrimitiveCylinder::intersectionRayNonCapped( const TRay &ray,
						  TIntersection &inter ) const {
  TVector3 RC;		// Ray base to cylinder base
  float	d;		// Shortest distance between the ray and the cylinder
  float	t, s;		// Distances along the ray
  TVector3 n, D, O;
  float	ln;

  RC = ray.origin - center; // 3 add  //  MAY BE PRECOMPUTED/FRAME
  n = crossProduct( ray.direction, axis ); // 6 mul  3 add

  ln = n.magnitudeSquared(); // 3 mul  2 add
  if ( ln == 0.0f ) // ray parallel to cylinder
    return false;
  
  float oneOverSqrtLn = fastInvSqrt( ln ); // 8 mul  3 add
  n *= oneOverSqrtLn; // 1 mul
  d = fabsf( RC | n );		// shortest distance

  if  ( d <= radius ) {				// if ray hits cylinder
    O = crossProduct( RC, axis ); // 6 mul  3 add  //  MAY BE PRECOMPUTED/FRAME
    t = - (( O | n ) * oneOverSqrtLn); // 3 mul  2 add
    O = crossProduct( n, axis ); // 6 mul  3 add
    s = fabsf( sqrtf(radius2 - d*d) / ( ray.direction | O ) ); // 1 sqrt  1 div  5 mul  4 add
    // *in = t - s;			// entering distance
    // *out = t + s;			// exiting  distance
    //--return t-s; // 1 add
    inter.distance = t - s;
    if ( inter.distance > 0 )
      return true;
    inter.distance = t + s;
    if ( inter.distance > 0 )
      return true;
    return false;
  }
  return false;
}
bool PrimitiveCylinder::intersectsRayNonCapped( const TRay &ray, const float maxDist2 ) const {
  TIntersection inter;
  if ( !PrimitiveCylinder::intersectionRayNonCapped( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < maxDist2 )
    return true;
  return false;
}
bool PrimitiveCylinder::allIntersectionRayNonCappedCamera( const TRay &ray,
							   TIntersectionStack &IStack ) const {
  
  TVector3 n = crossProduct( ray.direction, axis ); // 6 mul  3 add

  float ln = n.magnitudeSquared(); // 3 mul  2 add
  if ( ln == 0.0f ) // ray parallel to cylinder
    return false;
  
  float oneOverSqrtLn = fastInvSqrt( ln ); // 8 mul  3 add
  n *= oneOverSqrtLn; // 1 mul
  float d = fabsf( precalcRC | n );		// shortest distance

  if  ( d <= radius ) {				// if ray hits cylinder
    float t = - (( precalcO | n ) * oneOverSqrtLn); // 3 mul  2 add
    TVector3 O = crossProduct( n, axis ); // 6 mul  3 add
    float s = fabsf( sqrtf(radius2 - d*d) / ( ray.direction | O ) ); // 1 sqrt  1 div  5 mul  4 add
    float dist = t - s;
    int iCount = 0;
    if ( dist > 0 ) {
      IStack.push( dist ); iCount++;
    }
    dist = t + s;
    if ( dist > 0 ) {
      IStack.push( dist ); iCount++;
    }
    return iCount > 0;
  }
  return false;
}
bool PrimitiveCylinder::allIntersectionRayNonCapped( const TRay &ray,
						     TIntersectionStack &IStack ) const {
  
  TVector3 RC;		// Ray base to cylinder base
  float	d;		// Shortest distance between the ray and the cylinder
  float	t, s;		// Distances along the ray
  TVector3 n, D, O;
  float	ln;
  int iCount = 0;

  RC = ray.origin - center; // 3 add  //  MAY BE PRECOMPUTED/FRAME
  n = crossProduct( ray.direction, axis ); // 6 mul  3 add

  ln = n.magnitudeSquared(); // 3 mul  2 add
  if ( ln == 0.0f ) // ray parallel to cylinder
    return false;
  
  float oneOverSqrtLn = fastInvSqrt( ln ); // 8 mul  3 add
  n *= oneOverSqrtLn; // 1 mul
  d = fabsf( RC | n );		// shortest distance

  if  ( d <= radius ) {				// if ray hits cylinder
    O = crossProduct( RC, axis ); // 6 mul  3 add  //  MAY BE PRECOMPUTED/FRAME
    t = - (( O | n ) * oneOverSqrtLn); // 3 mul  2 add
    O = crossProduct( n, axis ); // 6 mul  3 add
    s = fabsf( sqrtf(radius2 - d*d) / ( ray.direction | O ) ); // 1 sqrt  1 div  5 mul  4 add
    float dist = t - s;
    if ( dist > 0 ) {
      IStack.push( dist ); iCount++;
    }
    dist = t + s;
    if ( dist > 0 ) {
      IStack.push( dist ); iCount++;
    }
    return iCount > 0;
  }
  return false;
}
bool PrimitiveCylinder::insideNonCapped( const TPoint3 &p ) const {
  // compute distance from pt to cylinder axis
  // and compare it to radius
  // distance on cylinder axis from center to tested point projection on cylinder axis
  TVector3 p2center = p-center;
  float d = ( p2center | axis );
  // distance from tested point to tested point projection on cylinder axis
  float dist2 = ( p2center - (axis*d) ).magnitudeSquared();
  return dist2 < radius2;
}

//--------------
//--- Capped ---
bool PrimitiveCylinder::intersectionRayCappedCamera( const TRay &ray,
						     TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create( 2 );
  if ( !PrimitiveCylinder::allIntersectionRayCappedCamera( ray, IStack ) )
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
bool PrimitiveCylinder::intersectionRayCapped( const TRay &ray,
					       TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create( 2 );
  if ( !PrimitiveCylinder::allIntersectionRayCapped( ray, IStack ) )
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
  }
  return false;
}
bool PrimitiveCylinder::intersectsRayCapped( const TRay &ray, const float maxDist2 ) const {
  TIntersection inter;
  if ( !PrimitiveCylinder::intersectionRayCapped( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < maxDist2 )
    return true;
  return false;
}
bool PrimitiveCylinder::allIntersectionRayCappedCamera( const TRay &ray,
							TIntersectionStack &IStack ) const {
  // set up quadratic Q(t) = a*t^2 + 2*b*t + c
  TVector3 D( U | ray.direction, V | ray.direction, W | ray.direction );
  //-- DLength is always equal to 1 because U, V, W and ray.direction are normalized

  float halfHeight = 0.5f * height;
  float inv, A, B, Discr, Root, T, T0, T1, Tmp0, Tmp1;
  int iCount = 0;
  if ( fabsf(D[2]) >= 1.0f - EPS ) {
#ifdef __DEBUG__
      printf("parallel to cylinder axis\n");
#endif
    // line is parallel to cylinder axis
    if ( precalcC <= 0.0f ) {
      //--Tmp0 = invDLength / D[2];
      Tmp0 = 1.0f / D[2];
      IStack.push( (+halfHeight - precalcP[2])*Tmp0, INTERSECTION_TOP_CAP ); iCount++;
      IStack.push( (-halfHeight - precalcP[2])*Tmp0, INTERSECTION_BOTTOM_CAP ); iCount++;
#ifdef __DEBUG__
      printf("intersection found\n");
#endif
      return true;
    }
    else {
#ifdef __DEBUG__
      printf("parallel to axis but no intersection with caps\n");
#endif
      return false;
    }
  }

  if ( fabsf(D[2]) <= EPS ) {
#ifdef __DEBUG__
    printf("perpendicular to cylinder axis\n");
#endif
    // line is perpendicular to axis of cylinder
    if ( fabsf(precalcP[2]) > halfHeight ) {
      // line is outside the planar caps of cylinder
#ifdef __DEBUG__
      printf("outside plane caps\n");
#endif
      return false;
    }
#ifdef __DEBUG__
      printf("inside planar caps\n");
#endif
    A = D[0]*D[0] + D[1]*D[1];
    B = precalcP[0]*D[0] + precalcP[1]*D[1];
    Discr = B*B - A*precalcC;
    if ( Discr < 0.0f ) {
      // line does not intersect cylinder wall
#ifdef __DEBUG__
      printf("no intersection with cylinder wall\n");
#endif
      return false;
    }
    else if ( Discr > 0.0f ) {
#ifdef __DEBUG__
      printf("intersect cylinder wall  2 inter\n");
#endif
      Root = sqrtf(Discr);
      //--Tmp0 = invDLength / A;
      Tmp0 = 1.0f / A;
      IStack.push( ( -B - Root ) * Tmp0, INTERSECTION_CYLINDER_WALL ); iCount++;
      IStack.push( ( -B + Root ) * Tmp0, INTERSECTION_CYLINDER_WALL ); iCount++;
      return true;
    }
    else {
#ifdef __DEBUG__
      printf("intersect cylinder wall  1 inter\n");
#endif
      //--intersections[0] = -B * invDLength / A;
      IStack.push( -B / A,  INTERSECTION_CYLINDER_WALL ); iCount++;
      return true;
    }
  }
  // test plane intersections first
#ifdef __DEBUG__
  printf("test plane intersection\n");
#endif
  inv = 1.0f / D[2];

  T0 = (+halfHeight - precalcP[2]) * inv;
  Tmp0 = precalcP[0] + T0 * D[0];
  Tmp1 = precalcP[1] + T0 * D[1];
  if ( SQR(Tmp0) + SQR(Tmp1) <= radius2 ) {
    IStack.push( T0, INTERSECTION_TOP_CAP ); iCount++;
  }

  T1 = (-halfHeight - precalcP[2]) * inv;
  Tmp0 = precalcP[0] + T1 * D[0];
  Tmp1 = precalcP[1] + T1 * D[1];
  if ( SQR(Tmp0) + SQR(Tmp1) <= radius2 ) {
    IStack.push( T1, INTERSECTION_BOTTOM_CAP ); iCount++;
  }

  if ( iCount == 2 ) {
#ifdef __DEBUG__
  printf("intersect top & bottom\n");
#endif
    // line intersects both top and bottom
    return true;
  }
  // If only 1 intersection, then line must intersect cylinder wall
  // somewhere between caps in a single point.  This case is detected
  // in the following code that tests for intersection between line and
  // cylinder wall.
  
  A = D[0]*D[0] + D[1]*D[1];
  B = precalcP[0]*D[0] + precalcP[1]*D[1];
  Discr = B*B - A*precalcC;
  if ( Discr < 0.0f ) {
    // line does not intersect cylinder wall
#ifdef __DEBUG__
  printf("no cylinder wall intersection\n");
#endif
    //--assert( iQuantity == 0 ); TODO
    return false;
  }
  else if ( Discr > 0.0f ) {
#ifdef __DEBUG__
  printf("intersect cylinder wall  2 inter\n");
#endif
    Root = sqrtf(Discr);
    inv = 1.0f / A;
    T = ( -B - Root ) * inv;
#ifdef __DEBUG__
    printf("negative root: %5.5f   t0: %5.5f  t1: %5.5f\n", T, T0, T1 );
#endif
    if ( T0 <= T1 ) {
      if ( T0 <= T && T <= T1 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
    else {
      if ( T1 <= T && T <= T0 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
    if ( iCount == 2 ) {
      // Line intersects one of top/bottom of cylinder and once on
      // cylinder wall.
#ifdef __DEBUG__
      printf("intersect one top/bottom and cylinder wall\n");
#endif
      return true;
    }
    T = ( -B + Root ) * inv;
#ifdef __DEBUG__
    printf("positive root: %5.5f   t0: %5.5f  t1: %5.5f\n", T, T0, T1 );
#endif
    if ( T0 <= T1 ) {
      if ( T0 <= T && T <= T1 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
    else {
      if ( T1 <= T && T <= T0 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
  }
  else {
#ifdef __DEBUG__
  printf("intersect cylinder wall  1 inter\n");
#endif
    T = -B / A;
    if ( T0 <= T1 ) {
      if ( T0 <= T && T <= T1 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
    else {
      if ( T1 <= T && T <= T0 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
  }
  return iCount > 0;

}
bool PrimitiveCylinder::allIntersectionRayCapped( const TRay &ray,
						  TIntersectionStack &IStack ) const {
  // set up quadratic Q(t) = a*t^2 + 2*b*t + c
  TVector3 D( U | ray.direction, V | ray.direction, W | ray.direction );
  //-- DLength is always equal to 1 because U, V, W and ray.direction are normalized
  //--  float DLength = D.magnitude(); // OPTIMIZE: is this always equal to 1 ?
  //--  float invDLength = 1/DLength;
  //--  D *= invDLength;

  TVector3 Diff = ray.origin - center; // OPTIMIZE for camera rays
  TVector3 P( U | Diff, V | Diff, W | Diff ); // OPTIMIZE for camera rays
  float halfHeight = 0.5f * height;
  float inv, A, B, C, Discr, Root, T, T0, T1, Tmp0, Tmp1;
  int iCount = 0;
  
  if ( fabsf(D[2]) >= 1.0f - EPS ) {
#ifdef __DEBUG__
      printf("parallel to cylinder axis\n");
#endif
    // line is parallel to cylinder axis
    if ( P[0]*P[0] + P[1] * P[1] <= radius2 ) {
      //--Tmp0 = invDLength / D[2];
      Tmp0 = 1.0f / D[2];
      IStack.push( (+halfHeight - P[2])*Tmp0, INTERSECTION_TOP_CAP ); iCount++;
      IStack.push( (-halfHeight - P[2])*Tmp0, INTERSECTION_BOTTOM_CAP ); iCount++;
#ifdef __DEBUG__
      printf("intersection found\n");
#endif
      return true;
    }
    else {
#ifdef __DEBUG__
      printf("parallel to axis but no intersection with caps\n");
#endif
      return false;
    }
  }

  if ( fabsf(D[2]) <= EPS ) {
#ifdef __DEBUG__
    printf("perpendicular to cylinder axis\n");
#endif
    // line is perpendicular to axis of cylinder
    if ( fabsf(P[2]) > halfHeight ) {
      // line is outside the planar caps of cylinder
#ifdef __DEBUG__
      printf("outside plane caps\n");
#endif
      return false;
    }
#ifdef __DEBUG__
      printf("inside planar caps\n");
#endif
    A = D[0]*D[0] + D[1]*D[1];
    B = P[0]*D[0] + P[1]*D[1];
    C = P[0]*P[0] + P[1]*P[1] - radius2;
    Discr = B*B - A*C;
    if ( Discr < 0.0f ) {
      // line does not intersect cylinder wall
#ifdef __DEBUG__
      printf("no intersection with cylinder wall\n");
#endif
      return false;
    }
    else if ( Discr > 0.0f ) {
#ifdef __DEBUG__
      printf("intersect cylinder wall  2 inter\n");
#endif
      Root = sqrtf(Discr);
      //--Tmp0 = invDLength / A;
      Tmp0 = 1.0f / A;
      IStack.push( ( -B - Root ) * Tmp0, INTERSECTION_CYLINDER_WALL ); iCount++;
      IStack.push( ( -B + Root ) * Tmp0, INTERSECTION_CYLINDER_WALL ); iCount++;
      return true;
    }
    else {
#ifdef __DEBUG__
      printf("intersect cylinder wall  1 inter\n");
#endif
      //--intersections[0] = -B * invDLength / A;
      IStack.push( -B / A,  INTERSECTION_CYLINDER_WALL ); iCount++;
      return true;
    }
  }
  // test plane intersections first
#ifdef __DEBUG__
  printf("test plane intersection\n");
#endif
  inv = 1.0f / D[2];

  T0 = (+halfHeight - P[2]) * inv;
  Tmp0 = P[0] + T0 * D[0];
  Tmp1 = P[1] + T0 * D[1];
  if ( Tmp0*Tmp0 + Tmp1*Tmp1 <= radius2 ) {
    IStack.push( T0, INTERSECTION_TOP_CAP ); iCount++;
  }

  T1 = (-halfHeight - P[2]) * inv;
  Tmp0 = P[0] + T1 * D[0];
  Tmp1 = P[1] + T1 * D[1];
  if ( Tmp0*Tmp0 + Tmp1*Tmp1 <= radius2 ) {
    IStack.push( T1, INTERSECTION_BOTTOM_CAP ); iCount++;
  }

  if ( iCount == 2 ) {
#ifdef __DEBUG__
  printf("intersect top & bottom\n");
#endif
    // line intersects both top and bottom
    return true;
  }
  // If only 1 intersection, then line must intersect cylinder wall
  // somewhere between caps in a single point.  This case is detected
  // in the following code that tests for intersection between line and
  // cylinder wall.

  A = D[0]*D[0] + D[1]*D[1];
  B = P[0]*D[0] + P[1]*D[1];
  C = P[0]*P[0] + P[1]*P[1] - radius2;
  Discr = B*B - A*C;
  if ( Discr < 0.0f ) {
    // line does not intersect cylinder wall
#ifdef __DEBUG__
  printf("no cylinder wall intersection\n");
#endif
    //--assert( iQuantity == 0 ); TODO
    return false;
  }
  else if ( Discr > 0.0f ) {
#ifdef __DEBUG__
  printf("intersect cylinder wall  2 inter\n");
#endif
    Root = sqrtf(Discr);
    inv = 1.0f / A;
    T = ( -B - Root ) * inv;
#ifdef __DEBUG__
    printf("negative root: %5.5f   t0: %5.5f  t1: %5.5f\n", T, T0, T1 );
#endif
    if ( T0 <= T1 ) {
      if ( T0 <= T && T <= T1 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
    else {
      if ( T1 <= T && T <= T0 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
    if ( iCount == 2 ) {
      // Line intersects one of top/bottom of cylinder and once on
      // cylinder wall.
#ifdef __DEBUG__
      printf("intersect one top/bottom and cylinder wall\n");
#endif
      return true;
    }
    T = ( -B + Root ) * inv;
#ifdef __DEBUG__
    printf("positive root: %5.5f   t0: %5.5f  t1: %5.5f\n", T, T0, T1 );
#endif
    if ( T0 <= T1 ) {
      if ( T0 <= T && T <= T1 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
    else {
      if ( T1 <= T && T <= T0 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
  }
  else {
#ifdef __DEBUG__
  printf("intersect cylinder wall  1 inter\n");
#endif
    T = -B / A;
    if ( T0 <= T1 ) {
      if ( T0 <= T && T <= T1 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
    else {
      if ( T1 <= T && T <= T0 ) {
	IStack.push( T, INTERSECTION_CYLINDER_WALL ); iCount++;
      }
    }
  }
  return iCount > 0;
}

bool PrimitiveCylinder::insideCapped( const TPoint3 &p ) const {
  // distance on cylinder axis from center to tested point projection on cylinder axis
  TVector3 p2center = p-center;
  float d = ( p2center | axis );
  // compare d to cylinder height
  if ( fabsf(d) > height * 0.5f )
    return false;
  // distance from tested point to tested point projection on cylinder axis
  float dist2 = ( p2center - (axis*d) ).magnitudeSquared();
  // compare this distance to radius
  return dist2 < radius2;
}

// compute the normal to primitive at intersection point
TVector3 PrimitiveCylinder::normalAt( const TIntersection &inter ) const {
  if ( capped ) {
    if ( inter.additionalInformation.side == INTERSECTION_TOP_CAP )
      return axis;
    else if ( inter.additionalInformation.side == INTERSECTION_BOTTOM_CAP )
      return -axis;
    // else fall through
  }
  TVector3 v = inter.pointWorldSpace-center;
  float p = ( v | axis );
  // loc+dir*p is the projection of hit on cylinder axis
  // hit - (loc+dir*p) is the normal (vector from projection on axis to hit)
  return ( v - axis*p ) * invRadius;
}

void PrimitiveCylinder::print( const char *msg ) const {
  TPrimitive::print( msg );
  center.print("center: ");
  axis.print("axis: ");
  printf("radius: %5.5f\n", radius );
  if ( capped )
    printf("capped: height: %5.5f\n", height );
}
