#include <stdio.h>
#include <stdlib.h>

#include "superellipsoid.h"
#include "../intersection.h"
#include "../ray.h"

const TTagList PrimitiveSuperellipsoid::primitiveSuperellipsoidTags [] = {
  { "e", TAG_primitiveSuperellipsoid_e },
  { "n", TAG_primitiveSuperellipsoid_n },
  { NULL, -1 }
};


/****************************************************************************
*  Explanation:
*    Superellipsoids are defined by the implicit equation
*      f(x,y,z) = (|x|^(2/e) + |y|^(2/e))^(e/n) + |z|^(2/n) - 1
*    Where e is the east/west exponent and n is the north/south exponent.
*    NOTE: The exponents are precomputed and stored in the Power element.
*    NOTE: Values of e and n that are close to degenerate (e.g.,
*          below around 0.1) appear to give the root solver fits.
*          Not sure quite where the problem lays just yet.
*****************************************************************************/
// Minimal intersection depth for a valid intersection.
#define DEPTH_TOLERANCE (1.0e-4f)
// If |x| < ZERO_TOLERANCE it is regarded to be 0.
#define ZERO_TOLERANCE (1.0e-10f)
// This is not the signum function because SGNX(0) is 1.
#define SGNX(x) (((x) >= 0.0f) ? 1.0f : -1.0f)
#define MIN_VALUE (-1.0f)
#define MAX_VALUE  (1.0f)
#define MAX_ITERATIONS (20)
#define PLANECOUNT (9)
#define BOUND_HUGE (INFINITE)

static float planes[PLANECOUNT][4] =
  {{1, 1, 0, 0}, {1,-1, 0, 0},
   {1, 0, 1, 0}, {1, 0,-1, 0},
   {0, 1, 1, 0}, {0, 1,-1, 0},
   {1, 0, 0, 0}, 
   {0, 1, 0, 0}, 
   {0, 0, 1, 0}};

PrimitiveSuperellipsoid::PrimitiveSuperellipsoid() {
  TPrimitive::TPrimitive(); primitiveKind = TPrimitive::PRIMITIVE_SUPERELLIPSOID;
}

void PrimitiveSuperellipsoid::setup() {
  power = TVector3( 2.0f / e, e / n, 2.0f / n );
}

bool PrimitiveSuperellipsoid::intersectionRay( const TRay &ray,
					       TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create( 16 );
  if ( !PrimitiveSuperellipsoid::allIntersectionRay( ray, IStack ) )
    return false;
  bool found = false;
  inter.distance = INFINITE;
  for ( int i = 0; i < IStack.index; i++ )
    if ( IStack.stack[i].distance < inter.distance ) {
      found = true;
      inter.distance = IStack.stack[i].distance;
    }
  return found;
}

bool PrimitiveSuperellipsoid::intersectsRay( const TRay &ray, const float distMax2 ) const {
  TIntersection inter;
  if ( !PrimitiveSuperellipsoid::intersectionRay( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < distMax2 )
    return true;
  return false;
}

bool PrimitiveSuperellipsoid::allIntersectionRay( const TRay &ray,
						  TIntersectionStack &IStack ) const {
  //  int i, cnt, Found = FALSE;
  //  DBL dists[PLANECOUNT+2];
  //  DBL t, t1, t2, v0, v1, len;
  //  VECTOR P, D, P0, P1, P2, P3;
  bool found;

  // Intersect superellipsoid's bounding box.
  float t1, t2;
  if ( !intersectBox( ray, &t1, &t2) )
    return false;

  // Test if superellipsoid lies 'behind' the ray origin.
  if ( t2 < DEPTH_TOLERANCE )
    return false;

  int cnt = 0;

  if ( t1 < DEPTH_TOLERANCE )
    t1 = DEPTH_TOLERANCE;

  float dists[PLANECOUNT+2];
  dists[cnt++] = t1;
  dists[cnt++] = t2;

  // Intersect ray with planes cutting superellipsoids in pieces.
  cnt = findRayPlanePoints( ray, cnt, dists, t1, t2 );

  if ( cnt <= 1 )
    return false;

  TPoint3 P0 = ray.origin + ( ray.direction * dists[0] );

  float v0 = evaluateSuperellipsoid( P0 );

  if ( fabsf( v0 ) < ZERO_TOLERANCE ) {
    IStack.push( dists[0] );
    found = true;
  }
  for ( int i = 1; i < cnt; i++ ) {
    TPoint3 P1 = ray.origin + ( ray.direction * dists[i] );
    float v1 = evaluateSuperellipsoid( P1 );

    if ( fabsf(v1) < ZERO_TOLERANCE) {
      IStack.push( dists[i] );
      found = true;
    }
    else {
      TPoint3 P2;
      if ( v0 * v1 < 0.0f ) {
        // Opposite signs, there must be a root between
        solveHit1( v0, P0, v1, P1, P2 );
	
	TPoint3 P3 = P2 - ray.origin;
	float t = P3.magnitude();

	IStack.push( t );
	found = true;
      }
      else {
	// Although there was no sign change, we may actually be approaching
	// the surface. In this case, we are being fooled by the shape of the
	// surface into thinking there isn't a root between sample points. 
	float t;
        if ( checkHit2( ray, dists[i-1], P0, v0, dists[i], &t, P2 ) ) {
	  IStack.push( t );
	  found = true;
	}
      }
    }

    v0 = v1;
    P0 = P1;
  }

  return found;
}

TVector3 PrimitiveSuperellipsoid::normalAt( const TIntersection &inter ) const {
  float x = fabsf( inter.pointWorldSpace[__X] );
  float y = fabsf( inter.pointWorldSpace[__Y] );
  float z = fabsf( inter.pointWorldSpace[__Z] );

  float k = powerf( powerf( x, power[__X] ) + powerf( y, power[__X] ), power[__Y] - 1.0f );
  TVector3 N;
  
  N[__X] = k * SGNX(inter.pointWorldSpace[__X]) * powerf( x, power[__X] - 1.0f );
  N[__Y] = k * SGNX(inter.pointWorldSpace[__Y]) * powerf( y, power[__X] - 1.0f ); // FIXME ?
  N[__Z] =     SGNX(inter.pointWorldSpace[__Z]) * powerf( z, power[__Z] - 1.0f );
  
  N.normalize();
  return N;
}

bool PrimitiveSuperellipsoid::inside( const TPoint3 &p ) const {
  float val = evaluateSuperellipsoid( p );

  return val < EPS; // or inverse
}

void PrimitiveSuperellipsoid::print( const char *msg ) const {
  TPrimitive::print(msg);
  printf("e: %5.5f\n", e );
  printf("n: %5.5f\n", n );
}

// Intersect a ray with an axis aligned unit box.
bool PrimitiveSuperellipsoid::intersectBox( const TRay &ray, float *dmin, float *dmax ) const {
  float tmin = 0.0f, tmax = 0.0f;

  // Left/right.
  if ( fabsf(ray.direction[__X]) > EPS ) {
    if ( ray.direction[__X] > EPS ) {
      *dmin = (MIN_VALUE - ray.origin[__X]) * ray.reverseDirection[__X];
      *dmax = (MAX_VALUE - ray.origin[__X]) * ray.reverseDirection[__X];

      if ( *dmax < EPS ) return false;
    }
    else {
      *dmax = (MIN_VALUE - ray.origin[__X]) * ray.reverseDirection[__X];

      if (*dmax < EPS) return false;

      *dmin = (MAX_VALUE - ray.origin[__X]) * ray.reverseDirection[__X];
    }

    if (*dmin > *dmax) return false;
  }
  else {
    if ( (ray.origin[__X] < MIN_VALUE) || (ray.origin[__X] > MAX_VALUE) )
      return false;

    *dmin = -BOUND_HUGE;
    *dmax =  BOUND_HUGE;
  }

  // Top/bottom.
  if ( fabsf(ray.direction[__Y]) > EPS ) {
    if ( ray.direction[__Y] > EPS ) {
      tmin = (MIN_VALUE - ray.origin[__Y]) * ray.reverseDirection[__Y];
      tmax = (MAX_VALUE - ray.origin[__Y]) * ray.reverseDirection[__Y];
    }
    else {
      tmax = (MIN_VALUE - ray.origin[__Y]) * ray.reverseDirection[__Y];
      tmin = (MAX_VALUE - ray.origin[__Y]) * ray.reverseDirection[__Y];
    }

    if ( tmax < *dmax ) {
      if ( tmax < EPS ) return false;

      if ( tmin > *dmin ) {
        if ( tmin > tmax ) return false;

        *dmin = tmin;
      }
      else
        if ( *dmin > tmax ) return false;

      *dmax = tmax;
    }
    else {
      if ( tmin > *dmin ) {
        if ( tmin > *dmax ) return false;

        *dmin = tmin;
      }
    }
  }
  else
    if ((ray.origin[__Y] < MIN_VALUE) || (ray.origin[__Y] > MAX_VALUE))
      return false;

  // Front/back.
  if ( fabsf(ray.direction[__Z]) > EPS ) {
    if ( ray.direction[__Z] > EPS ) {
      tmin = (MIN_VALUE - ray.origin[__Z]) * ray.reverseDirection[__Z];
      tmax = (MAX_VALUE - ray.origin[__Z]) * ray.reverseDirection[__Z];
    }
    else {
      tmax = (MIN_VALUE - ray.origin[__Z]) * ray.reverseDirection[__Z];
      tmin = (MAX_VALUE - ray.origin[__Z]) * ray.reverseDirection[__Z];
    }

    if (tmax < *dmax) {
      if (tmax < EPS) return false;

      if (tmin > *dmin) {
        if (tmin > tmax) return false;

        *dmin = tmin;
      }
      else {
        if (*dmin > tmax) return false;
      }

      *dmax = tmax;
    }
    else {
      if (tmin > *dmin) {
        if (tmin > *dmax) return false;

        *dmin = tmin;
      }
    }
  }
  else
    if ((ray.origin[__Z] < MIN_VALUE) || (ray.origin[__Z] > MAX_VALUE))
      return false;

  *dmin = tmin;
  *dmax = tmax;

  return true;
}

// Get superellipsoid value in the given point.
float PrimitiveSuperellipsoid::evaluateSuperellipsoid( const TPoint3 &P ) const {

  float x = powerf( fabsf(P[__X]), power[__X] );
  float y = powerf( fabsf(P[__Y]), power[__X] );
  float z = powerf( fabsf(P[__Z]), power[__Z] );

  return powerf( x + y, power[__Y]) + z - 1.0f;
}

// Raise x to the power of e.
float PrimitiveSuperellipsoid::powerf( const float x, const float e ) const {
  register int i;
  register float b;

  b = x;

  i = (int)e;

  // Test if we have an integer power.

  if ( e == (float)i ) {
    switch (i){
      case 0: return 1.0f;

      case 1: return b;

      case 2: return SQR(b);

      case 3: return SQR(b) * b;

      case 4: b *= b; return SQR(b);

      case 5: b *= b; return SQR(b) * x;

      case 6: b *= b; return SQR(b) * b;

      default: return powf(x, e);
    }
  }
  else
    return powf(x, e);
}

// Compare two slabs. used by qsort
static int compdists( const void *in_a, const void *in_b ) {
  float a, b;
  a = *((float *)in_a);
  b = *((float *)in_b);
  if (a < b)
    return -1;
  if (a == b)
    return 0;
  else
    return 1;
}

//   Find all the places where the ray intersects the set of
//   subdividing planes through the superquadric.  Return the
//   number of valid hits (within the bounding box).
int PrimitiveSuperellipsoid::findRayPlanePoints( const TRay &ray, int cnt,
						 float *dists,
						 float mindist, float maxdist ) const {
  // Since min and max dist are the distance to two of the bounding planes
  // we are considering, there is a high probablity of missing them due to
  // round off error. Therefore we adjust min and max.
  float t = EPS * (maxdist - mindist);
  mindist -= t;
  maxdist += t;

  // Check the sets of planes that cut apart the superquadric.
  for ( int i = 0; i < PLANECOUNT; i++ ) {
    float d = ( ray.direction[0] * planes[i][0]
		+ ray.direction[1] * planes[i][1]
		+ ray.direction[2] * planes[i][2] );

    if ( fabsf(d) < EPS )
      // Can't possibly get a hit for this combination of ray and plane.
      continue;

    t = ( planes[i][3] - ( ray.origin[0] * planes[i][0]
			   + ray.origin[1] * planes[i][1]
			   + ray.origin[2] * planes[i][2] ) ) / d;

    if ( ( t >= mindist ) && ( t <= maxdist ) )
      dists[cnt++] = t;
  }

  // Sort the results for further processing.
  qsort( (void *)(dists), (size_t)cnt, sizeof(float), compdists );

  return cnt;
}

//   Home in on the root of a superquadric using a combination of
//   secant and bisection methods.  This routine requires that
//   the sign of the function be different at P0 and P1, it will
//   fail drastically if this isn't the case.
void PrimitiveSuperellipsoid::solveHit1( const float v0, const TVector3 &tP0,
					 const float v1, const TVector3 &tP1,
					 TVector3 &P ) const {
  //  int i;
  //  float x, v2, v3;
  //  TVector3 P0, P1, P2, P3;
  TVector3 P0 = tP0;
  TVector3 P1 = tP1;

  // The sign of v0 and v1 changes between P0 and P1, this
  // means there is an intersection point in there somewhere.

  int i = 0;
  for ( i = 0; i < MAX_ITERATIONS; i++ ) {
    if ( fabsf(v0) < ZERO_TOLERANCE ) {
      // Near point is close enough to an intersection - just use it.
      P = P0;
      break;
    }

    if ( fabsf(v1) < ZERO_TOLERANCE ) {
      // Far point is close enough to an intersection.
      P = P1;
      break;
    }

    // Look at the chord connecting P0 and P1.
    // Assume a line between the points.
    float x = fabsf(v0) / fabsf(v1 - v0);
    TVector3 P2 = P1 - P0;
    P2 = P0 + ( x * P2 );
    float v2 = evaluateSuperellipsoid( P2 );

    // Look at the midpoint between P0 and P1.
    TVector3 P3 = P1 - P0;
    P3 = P0 + 0.5f * P3;
    float v3 = evaluateSuperellipsoid( P3 );

    if ( v0 * v2 > 0.0f) {
      if ( v1 * v2 > 0.0f ) {
        // This should be impossible, since v0 and v1 were opposite signs,
	// v2 must be either 0 or opposite in sign to either v0 or v1. */
        //--error("internal failure in function solve_sq_hit1: %d, %g, %g, %g", i, v0, v1, v2);
	error("Internal failure in PrimitiveSuperellipsoid::solveHit1");
      }
      else {
        if ( v0 * v3 > 0.0f ) {
          if ( x < 0.5f)
	    P0 = P3;
          else
	    P0 = P2;
        }
        else {
          // We can move both ends.
	  P0 = P2;
	  P1 = P3;
        }
      }
    }
    else {
      if ( v0 * v3 > 0.0f ) {
        // We can move both ends.
	P0 = P3;
	P1 = P2;
      }
      else {
        if ( x < 0.5f )
	  P1 = P2;
        else
	  P1 = P3;
      }
    }
  }
  
  if ( i == MAX_ITERATIONS ) {
    // The loop never quite closed in on the result - just use the point
    //   closest to zero.  This really shouldn't happen since the max number
    //   of iterations is enough to converge with straight bisection.
    if ( fabsf(v0) < fabsf(v1) )
      P = P0;
    else
      P = P1;
  }
}

//   Try to find the root of a superquadric using Newtons method.
bool PrimitiveSuperellipsoid::checkHit2( const TRay &ray, const float t0,
					 TVector3 &P0, float v0,
					 const float t1,
					 float *t, TVector3 &Q ) const {
  float dt0 = t0;
  float dt1 = t0 + 0.0001f * (t1 - t0);
  float maxdelta = t1 - t0;

  for ( int i = 0; (dt0 < t1) && (i < MAX_ITERATIONS); i++ ) {
    float deltat = 0.0f;
    TPoint3 P1 = ray.origin + ( ray.direction * dt1 );
    float v1 = evaluateSuperellipsoid( P1 );
    if ( v0 * v1 < 0.0f ) {
      // Found a crossing point, go back and use normal root solving.
      solveHit1( v0, P0, v1, P1, Q );
      TVector3 P0 = Q - ray.origin;
      *t = P0.magnitude();
      return true;
    }
    else {
      if ( fabsf(v1) < ZERO_TOLERANCE) {
	Q = ray.origin + ( ray.direction * dt1 );
	*t = dt1;
	return true;
      }
      else {
        if (((v0 > 0.0f) && (v1 > v0)) || ((v0 < 0.0f) && (v1 < v0)))
          // We definitely failed
          break;
        else {
          if ( v1 == v0 )
            break;
          else
            deltat = v1 * (dt1 - dt0) / (v1 - v0);
        }
      }
    }

    if ( fabsf(deltat) > maxdelta )
       break;

    v0 = v1;
    dt0 = dt1;
    dt1 -= deltat;
  }

  return false;
}
