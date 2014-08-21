#include <stdio.h>

#include "../world.h"
#include "../math/math.h"
#include "../math/solver.h"
#include "../ray.h"
#include "torus.h"

const TTagList PrimitiveTorus::primitiveTorusTags [] = {
  { "radius1", TAG_primitiveTorus_radius1 },
  { "radius2", TAG_primitiveTorus_radius2 },
  { NULL, -1 }
};

PrimitiveTorus::PrimitiveTorus() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_TORUS;
  R1 = 1.0f;
  R2 = 0.5f;
}

static bool getSmallestPositive( const double v[], const int n, float &sol ) {
  bool found = false;
  sol = INFINITE;
  for ( int i = 0; i < n; i++ )
    if ( v[i] > 0.0f
	 && v[i] < sol ) {
      sol = (float)v[i];
      found = true;
    }
  return found;
}

bool PrimitiveTorus::intersectionRay( const TRay &ray, TIntersection &inter ) const {
  double c[5], r[4];
  
  double R12  = SQR(R1); // MAY BE PRECOMPUTED in SETUP
  double R22  = SQR(R2); // MAY BE PRECOMPUTED in SETUP
  
  double Py2  = SQR(ray.origin[__Z]);  // MAY BE PRECOMPUTED / FRAME
  double Dy2  = SQR(ray.direction[__Z]);
  double PDy2 = ray.origin[__Z] * ray.direction[__Z];

  double k1   = ray.origin[__X] * ray.origin[__X] + ray.origin[__Y] * ray.origin[__Y] + Py2 - R12 - R22;  // MAY BE PRECOMPUTED / FRAME
  double k2   = ray.origin[__X] * ray.direction[__X] + ray.origin[__Y] * ray.direction[__Y] + PDy2;

  c[0] = 1.0;
  c[1] = 4.0 *  k2;
  c[2] = 2.0 * (k1 + 2.0f * (k2 * k2 + R12 * Dy2));
  c[3] = 4.0 * (k2 * k1  + 2.0 * R12 * PDy2);
  c[4] = k1 *  k1 + 4.0 * R12 * (Py2 - R22);

  int n = solveQuartic(c, r);
  bool found = getSmallestPositive( r, n, inter.distance );

  if ( !found )
    return false;

  return true;
}

bool PrimitiveTorus::intersectsRay( const TRay &ray, const float maxDist2 ) const {
  TIntersection inter;
  if ( !PrimitiveTorus::intersectionRay( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < maxDist2 )
    return true;
  return false;
}

bool PrimitiveTorus::allIntersectionRay( const TRay &ray,
					 TIntersectionStack &IStack ) const {
  double c[5], r[4];
  
  double R12  = SQR(R1); // MAY BE PRECOMPUTED in SETUP
  double R22  = SQR(R2); // MAY BE PRECOMPUTED in SETUP
  
  double Py2  = SQR(ray.origin[__Z]);  // MAY BE PRECOMPUTED / FRAME
  double Dy2  = SQR(ray.direction[__Z]);
  double PDy2 = ray.origin[__Z] * ray.direction[__Z];

  double k1   = ray.origin[__X] * ray.origin[__X] + ray.origin[__Y] * ray.origin[__Y] + Py2 - R12 - R22;  // MAY BE PRECOMPUTED / FRAME
  double k2   = ray.origin[__X] * ray.direction[__X] + ray.origin[__Y] * ray.direction[__Y] + PDy2;

  c[0] = 1.0;
  c[1] = 4.0 *  k2;
  c[2] = 2.0 * (k1 + 2.0f * (k2 * k2 + R12 * Dy2));
  c[3] = 4.0 * (k2 * k1  + 2.0 * R12 * PDy2);
  c[4] = k1 *  k1 + 4.0 * R12 * (Py2 - R22);

  int n = solveQuartic(c, r);
  for ( int i = 0; i < n; i++ )
    if ( r[i] > 0 )
      IStack.push( r[i] );
  return IStack.index > 0;
}

TVector3 PrimitiveTorus::normalAt( const TIntersection &inter ) const {
//--RaVi Method  float v1 = ang(IP[0], IP[1]);
//--RaVi Method  TVector3 N1 = IP - TVector3(R1*cosf(v1), R1*sinf(v1), 0.0f);
//--RaVi Method  float v2 = v1+M_PI;
//--RaVi Method  TVector3 N2 = IP - TVector3(R1*cosf(v2), R1*sinf(v2), 0.0f);
//--RaVi Method
//--RaVi Method  TVector3 resultNormal;
//--RaVi Method  if ( fabsf( N1.magnitude() - R2 ) < fabsf( N2.magnitude() - R2) ) {
//--RaVi Method    resultNormal = N1;
//--RaVi Method    resultNormal.normalize();
//--RaVi Method  }
//--RaVi Method  else {
//--RaVi Method    resultNormal = N2;
//--RaVi Method    resultNormal.normalize();
//--RaVi Method  }
//--RaVi Method  return resultNormal;
//--RaVi Method  1 div  1 arctan  2 cos  2 sin  7 mul  9 add  2 sqrt  2 fastInvSqrt

//--POVRAY    float dist = sqrtf( sqr(inter.pointWorldSpace[__X]) + sqr(inter.pointWorldSpace[__Y]) );
//--POVRAY    TVector3 M = TVector3::zeroVector;
//--POVRAY    if ( dist > EPS ) {
//--POVRAY      float invDist = 1.0f / dist;
//--POVRAY      M = TVector3( R1 * inter.pointWorldSpace[__X] * invDist,
//--POVRAY   		  R1 * inter.pointWorldSpace[__Y] * invDist,
//--POVRAY   		  0.0f );
//--POVRAY    }
//--POVRAY    TVector3 result = inter.pointWorldSpace - M;
//--POVRAY    result.normalize();
//--POVRAY    return result;
//--POVRAY Method  2 div  9 mul  1 sqrt  6 add  1 fastInvSqrt

  // Optimizations
  //---------------
  // using
  //   cos arctan X = 1 / sqrt( 1 + SQR(X) )
  //   sin arctan X = X / sqrt( 1 + SQR(X) )
  // we find
  //   dYdX = IP[1]/IP[0];
  //   coeff = 1/sqrt( 1 + SQR(dYdX)
  //   N1 = IP - TVector3( R1*coeff, R1*dYdX*coeff, 0.0f );
  // and (with no need to calculate N1)
  //   N1Magnitude = SQR(R1) - 2*R1*(IP[0]+IP[1]*dYdX)*coeffInv + IP.magnitudeSquared();
  // using
  //   cos(x+PI) = -cos(x)
  //   sin(x+PI) = -sin(x)
  // we find
  //   N2 = IP + TVector3( R1*coeff, R1*dYdX*coeff, 0.0f );
  // and
  //   N2Magnitude2 = SQR(R1) + 2*R1*(IP[0]+IP[1]*dYdX)*coeffInv + IP.magnitudeSquared()
  float dYdX = inter.pointWorldSpace[__Y]/inter.pointWorldSpace[__X];
  float coeffInv = fastInvSqrt( 1 + SQR(dYdX) );
  float X = R1*coeffInv;
  float Y = R1*dYdX*coeffInv;
  TVector3 N1Opt = inter.pointWorldSpace - TVector3( X, Y, 0.0f );
  float b = 2*R1*(inter.pointWorldSpace[__X]+inter.pointWorldSpace[__Y]*dYdX)*coeffInv;
  float N1magn2 = N1Opt.magnitudeSquared();
  float N1magn = sqrtf( N1magn2 );

  TVector3 resultNormal;
  if ( -2*b + 4*SQR(R2) + 2*R2*N1magn < 0 ) { // OPTIMIZE: SQR(R2) may be precomputed
    resultNormal = N1Opt;
    resultNormal /= N1magn;
  }
  else {
    resultNormal = inter.pointWorldSpace + TVector3( X, Y, 0.0f );
    resultNormal.normalize();
  }
  return resultNormal;

  // div: 1  fastInvSqrt: 1  mul: 16  add: 5  sqrt: 1
  // best: +1 div
  // worst: +3 add, +3 mul, +1 fastInvSqrt
  // total best:
  //  2 div  16 mul  5 add  1 sqrt  1 fastInvSqrt
  // total worst:
  //  1 div  19 mul  8 add  1 sqrt  2 fastInvSqrt
}

bool PrimitiveTorus::inside( const TPoint3 &p ) const {
  float r = sqrtf( sqr(p[__X]) + sqr(p[__Y]) );
  float r2 = sqr(p[__Z]) + sqr( r - R1 );
  return r2 > sqr(R2);
}

void PrimitiveTorus::print( const char *msg ) const {
  TPrimitive::print( msg );
  printf("radii: %5.5f  %5.5f\n", R1, R2 );
}

