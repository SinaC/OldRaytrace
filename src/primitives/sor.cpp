#include <stdio.h>
#include <stdlib.h>

#include "sor.h"
#include "../partitions/bcylinder.h"
#include "../math/matrix4.h"
#include "../math/solver.h"
#include "../intersection.h"
#include "../ray.h"

const TTagList PrimitiveSor::primitiveSorTags [] = {
  { "points", TAG_primitiveSor_points },
  { "open", TAG_primitiveSor_open },
  { NULL, -1 }
};

#define INTERSECTION_BASE_PLANE (0)
#define INTERSECTION_CAP_PLANE (1)
#define INTERSECTION_CURVE (2)

PrimitiveSor::PrimitiveSor() {
  TPrimitive::TPrimitive(); primitiveKind = TPrimitive::PRIMITIVE_SOR;
  spline = NULL;
}

PrimitiveSor::~PrimitiveSor() {
  if ( spline != NULL ) {
    if ( spline->BCyl != NULL )
      spline->BCyl->destroy();
    if ( spline->entry != NULL )
      delete [] spline->entry;
    delete spline;
  }
}

void PrimitiveSor::setup() {
}

void PrimitiveSor::create( TVector2 *p ) { // create the sor using a list of 2D points
  int i;

  // number of points is reduced by 3
  number -= 3;

  // Allocate number segments.
  if ( spline == NULL) {
    spline = new SorSpline;
    spline->entry = new PrimitiveSor::SorSpline::SorSplineEntry [number];
  }
  else
    error("PrimitiveSor::create: Surface of revolution segments are already defined.\n");

  // Allocate temporary lists.
  float *tmp_r1 = new float [number];
  float *tmp_r2 = new float [number];
  float *tmp_h1 = new float [number];
  float *tmp_h2 = new float [number];

  // We want to know the size of the overall bounding cylinder.
  float A, B, C, D, w, k[4];
  float x[4], xmax, xmin;
  float y[2], ymax, ymin;
  double c[3], r[2];

  xmax = ymax = -INFINITE;
  xmin = ymin =  INFINITE;

  // Calculate segments, i.e. cubic patches.
  for ( i = 0; i < number; i++ ) {
    if ( fabsf( p[i+2][__Y] - p[i][__Y] ) < EPS ||
	 fabsf( p[i+3][__Y] - p[i+1][__Y] ) < EPS )
      error("PrimitiveSor::create: Incorrect point in surface of revolution.\n");

    // Use cubic interpolation.
    k[0] = p[i+1][__X] * p[i+1][__X];
    k[1] = p[i+2][__X] * p[i+2][__X];
    k[2] = (p[i+2][__X] - p[i][__X]) / (p[i+2][__Y] - p[i][__Y]);
    k[3] = (p[i+3][__X] - p[i+1][__X]) / (p[i+3][__Y] - p[i+1][__Y]);

    k[2] *= 2.0f * p[i+1][__X];
    k[3] *= 2.0f * p[i+2][__X];

    w = p[i+1][__Y];

    TMatrix4 mat;

    mat.m[0][0] = w * w * w;
    mat.m[0][1] = w * w;
    mat.m[0][2] = w;
    mat.m[0][3] = 1.0f;

    mat.m[2][0] = 3.0f * w * w;
    mat.m[2][1] = 2.0f * w;
    mat.m[2][2] = 1.0f;
    mat.m[2][3] = 0.0f;

    w = p[i+2][__Y];

    mat.m[1][0] = w * w * w;
    mat.m[1][1] = w * w;
    mat.m[1][2] = w;
    mat.m[1][3] = 1.0f;

    mat.m[3][0] = 3.0f * w * w;
    mat.m[3][1] = 2.0f * w;
    mat.m[3][2] = 1.0f;
    mat.m[3][3] = 0.0f;

    mat.invert();

    // Calculate coefficients of cubic patch.
    A = k[0] * mat.m[0][0] + k[1] * mat.m[0][1] + k[2] * mat.m[0][2] + k[3] * mat.m[0][3];
    B = k[0] * mat.m[1][0] + k[1] * mat.m[1][1] + k[2] * mat.m[1][2] + k[3] * mat.m[1][3];
    C = k[0] * mat.m[2][0] + k[1] * mat.m[2][1] + k[2] * mat.m[2][2] + k[3] * mat.m[2][3];
    D = k[0] * mat.m[3][0] + k[1] * mat.m[3][1] + k[2] * mat.m[3][2] + k[3] * mat.m[3][3];

    if ( fabsf(A) < EPS ) A = 0.0f;
    if ( fabsf(B) < EPS ) B = 0.0f;
    if ( fabsf(C) < EPS ) C = 0.0f;
    if ( fabsf(D) < EPS ) D = 0.0f;

    spline->entry[i].A = A;
    spline->entry[i].B = B;
    spline->entry[i].C = C;
    spline->entry[i].D = D;

    // Get minimum and maximum radius**2 in current segment.
    y[0] = p[i+1][__Y];
    y[1] = p[i+2][__Y];

    x[0] = x[2] = p[i+1][__X];
    x[1] = x[3] = p[i+2][__X];

    c[0] = 3.0 * (double)A;
    c[1] = 2.0 * (double)B;
    c[2] = C;

    int n = solveQuadric( c, r );

    while (n--)
      if ( r[n] >= y[0] && r[n] <= y[1] )
        x[n] = (float)sqrt( r[n] * ( r[n] * ( r[n] * A + B ) + C ) + D );

    // Set current segment's bounding cylinder.
    tmp_r1[i] = MIN( MIN( x[0], x[1] ), MIN( x[2], x[3] ) );
    tmp_r2[i] = MAX( MAX( x[0], x[1] ), MAX( x[2], x[3] ) );

    tmp_h1[i] = y[0];
    tmp_h2[i] = y[1];

    // Keep track of overall bounding cylinder.
    xmin = MIN( xmin, tmp_r1[i] );
    xmax = MAX( xmax, tmp_r2[i] );

    ymin = MIN( ymin, tmp_h1[i] );
    ymax = MAX( ymax, tmp_h2[i] );
  }

  // Set overall bounding cylinder.
  radius1 = ymax;
  radius2 = xmax;

  height1 = ymin;
  height2 = ymax;

  // Get cap radius.
  w = tmp_h1[number-1];

  A = spline->entry[number-1].A;
  B = spline->entry[number-1].B;
  C = spline->entry[number-1].C;
  D = spline->entry[number-1].D;

  if ( ( capRadius2 = w * (w * (A * w + B) + C) + D) < 0.0f )
    capRadius2 = 0.0f;

  // Get base radius.
  w = tmp_h1[0];

  A = spline->entry[0].A;
  B = spline->entry[0].B;
  C = spline->entry[0].C;
  D = spline->entry[0].D;

  if ( ( baseRadius2 = w * (w * (A * w + B) + C) + D) < 0.0f )
    baseRadius2 = 0.0f;

  // Get bounding cylinder.
  spline->BCyl = BCylinder::create( number, tmp_r1, tmp_r2, tmp_h1, tmp_h2 );

  delete [] tmp_h2;
  delete [] tmp_h1;
  delete [] tmp_r2;
  delete [] tmp_r1;
}

bool PrimitiveSor::intersectionRay( const TRay &ray,
				       TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create( 16 );
  if ( !PrimitiveSor::allIntersectionRay( ray, IStack ) )
    return false;
  bool found = false;
  inter.distance = INFINITE;
  for ( int i = 0; i < IStack.index; i++ )
    if ( IStack.stack[i].distance < inter.distance
	 && IStack.stack[i].distance > 0.0f ) {
      found = true;
      inter.distance = IStack.stack[i].distance;
      inter.additionalInformation = IStack.stack[i].additionalInformation;
    }
  return found;
}

bool PrimitiveSor::intersectsRay( const TRay &ray, const float distMax2 ) const {
  TIntersection inter;
  if ( !PrimitiveSor::intersectionRay( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < distMax2 )
    return true;
  return false;
}

bool PrimitiveSor::allIntersectionRay( const TRay &ray,
				       TIntersectionStack &IStack ) const {
  // Test if ray misses object's bounds.
  if ( ( ray.direction[__Y] >= 0.0f && ray.origin[__Y] >  height2 ) ||
       ( ray.direction[__Y] <= 0.0f && ray.origin[__Y] <  height1 ) ||
       ( ray.direction[__X] >= 0.0f && ray.origin[__X] >  radius2 ) ||
       ( ray.direction[__X] <= 0.0f && ray.origin[__X] < -radius2 ) )
    return false;

  // Get distance of the ray from rotation axis (= y axis).
  float r0 = ray.origin[__X] * ray.direction[__Z] - ray.origin[__Z] * ray.direction[__X];

  float a;
  if ( ( a = SQR(ray.direction[__X]) + SQR(ray.direction[__Z]) ) > 0.0f )
    r0 *= fastInvSqrt(a);

  // Test if ray misses object's bounds.
  if ( r0 > radius2 )
    return false;

  // Intersect all cylindrical bounds.
  int cnt;
  if ( ( cnt = spline->BCyl->intersect( ray.origin, ray.direction ) ) == 0 )
    return false;

  // Test base/cap plane.
  bool found = false;
  float best = INFINITE;
  float k, u, v, b, h;

  if ( additionalInformation.isSet(TPrimitive::PRIMITIVE_CLOSED)
       && fabsf( ray.direction[__Y] ) > EPS ) {
    // Test base plane.
    if ( baseRadius2 > EPS ) {
      k = ( height1 - ray.origin[__Y] ) / ray.direction[__Y];

      u = ray.origin[__X] + k * ray.direction[__X];
      v = ray.origin[__Z] + k * ray.direction[__Z];

      b = u * u + v * v;

      if ( b <= baseRadius2 ) {
	IStack.push( k, INTERSECTION_BASE_PLANE );
	found = true;
          if ( k < best )
            best = k;
      }
    }

    // Test cap plane.
    if ( capRadius2 > EPS ) {
      k = ( height1 - ray.origin[__Y] ) / ray.direction[__Y];

      u = ray.origin[__X] + k * ray.direction[__X];
      v = ray.origin[__Z] + k * ray.direction[__Z];

      b = u * u + v * v;

      if ( b <= capRadius2 ) {
	IStack.push( k, INTERSECTION_CAP_PLANE );
	found = true;
	if ( k < best )
	  best = k;
      }
    }
  }

  // Step through the list of intersections.
  BCylinderIntersection *intervals = spline->BCyl->intervals;

  for ( int j = 0; j < cnt; j++ ) {
    // Get current segment.
    PrimitiveSor::SorSpline::SorSplineEntry *Entry = &(spline->entry[intervals[j].n]);

    // If we already have the best intersection we may exit.
    if ( intervals[j].d[0] > best )
      break;

    // Cubic curve.
    double x[4];
    x[0] = Entry->A * ray.direction[__Y] * ray.direction[__Y] * ray.direction[__Y];
//    x[1] = ray.direction[__Y] * ray.direction[__Y] * (3.0 * Entry->A * ray.origin[__Y] + Entry->B) - ray.direction[__X] * ray.direction[__X] - ray.direction[__Z] * ray.direction[__Z];
    x[1] = ray.direction[__Y] * ray.direction[__Y] * (3.0f * Entry->A * ray.origin[__Y] + Entry->B) - a;

    x[2] = ray.direction[__Y] * (ray.origin[__Y] * (3.0f * Entry->A * ray.origin[__Y] + 2.0f * Entry->B) + Entry->C) - 2.0f * (ray.origin[__X] * ray.direction[__X] + ray.origin[__Z] * ray.direction[__Z]);

    x[3] = ray.origin[__Y] * (ray.origin[__Y] * (Entry->A * ray.origin[__Y] + Entry->B) + Entry->C) + Entry->D - ray.origin[__X] * ray.origin[__X] - ray.origin[__Z] * ray.origin[__Z];

    double y[3];
    int n = solveCubic( x, y );

    while ( n-- ) {
      k = (float)y[n];
      h = ray.origin[__Y] + k * ray.direction[__Y];
      if ( h >= spline->BCyl->height[spline->BCyl->entry[intervals[j].n].h1] &&
	   h <= spline->BCyl->height[spline->BCyl->entry[intervals[j].n].h2] ) {
	IStack.push( k, INTERSECTION_CURVE + (intervals[j].n<<2) );
	found = true;
	if ( y[n] < best )
	  best = k;
      }
    }
  }

  return found;
}

TVector3 PrimitiveSor::normalAt( const TIntersection &inter ) const {
  TVector3 result;
  int side = (inter.additionalInformation.side & 2);
  switch ( side ) {
  case INTERSECTION_CURVE: {
    if ( SQR(inter.pointWorldSpace[__X]) + SQR(inter.pointWorldSpace[__Z]) > EPS ) {
      int interval = inter.additionalInformation.side >> 2;
      PrimitiveSor::SorSpline::SorSplineEntry *Entry = &(spline->entry[interval]);
      float k = 0.5f * (inter.pointWorldSpace[__Y] * (3.0f * Entry->A * inter.pointWorldSpace[__Y] + 2.0f * Entry->B) + Entry->C );
      result = TVector3( inter.pointWorldSpace[__X],
			 -k,
			 inter.pointWorldSpace[__Z] );
      result.normalize();
    }
    break;
  }
  case INTERSECTION_BASE_PLANE: result = TVector3( 0.0f, -1.0f, 0.0f ); break;
  case INTERSECTION_CAP_PLANE: result = TVector3( 0.0f, 1.0f, 0.0f ); break;
  default: result = TVector3::zeroVector; break;
  }
  return result;
}

bool PrimitiveSor::inside( const TPoint3 &p ) const {
  int i;
  // Test if we are inside the cylindrical bound.
  if ( p[__Y] >= height1
       && p[__Y] <= height2 ) {
    float r0 = SQR(p[__X]) + SQR(p[__Z]);

    // Test if we are inside the cylindrical bound.
    if ( r0 <= SQR( radius2 ) ) {
      // Now find the segment the point is in.
      PrimitiveSor::SorSpline::SorSplineEntry *Entry;
      for ( i = 0; i < number; i++ ) {
        Entry = &(spline->entry[i]);

        if ( p[__Y] >= spline->BCyl->height[spline->BCyl->entry[i].h1]
	     && p[__Y] <= spline->BCyl->height[spline->BCyl->entry[i].h2] )
          break;
      }

      // Have we found any segment?
      if ( i < number ) {
        float r = p[__Y] * ( p[__Y] * ( p[__Y] * Entry->A + Entry->B ) + Entry->C ) + Entry->D;

        if ( r0 <= r )
          // We're inside.
          return true;
      }
    }
  }

  // We're outside.
  return false;
}

void PrimitiveSor::print( const char *msg ) const {
  TPrimitive::print( msg );
  printf("number: %d\n", number );
  printf("height: %5.5f  %5.5f\n", height1, height2 );
  printf("radius: %5.5f  %5.5f\n", radius1, radius2 );
  printf("base radius2: %5.5f\n", baseRadius2 );
  printf("cap radius2: %5.5f\n", capRadius2 );
}
