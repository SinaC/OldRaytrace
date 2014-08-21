#include <stdio.h>

#include "hemispherical.h"
#include "../math/math.h"

const TTagList AmbientHemispherical::ambientHemisphericalTags [] = {
  { "center", TAG_ambientHemispherical_center },
  { "up", TAG_ambientHemispherical_up },
  { "colorTop", TAG_ambientHemispherical_colorTop },
  { "colorBottom", TAG_ambientHemispherical_colorBottom },
  { NULL, -1 }
};

AmbientHemispherical::AmbientHemispherical() {
  TAmbient::TAmbient(); ambientKind = AMBIENT_HEMISPHERICAL;
  center = TVector3::zeroVector;
  up = TVector3( 0, 1, 0 );
  colorTop = TColor::white;
  colorBottom = TColor::black;
}

void AmbientHemispherical::setup() {
  up.normalize();
}

// OPTIMIZE: use cubic spline to simulate halfSinAngle
TColor AmbientHemispherical::getColor( const TPoint3 &point ) const {
  // OPTIMIZED VERSION:
  // sin arccos X = sqrt( 1 - X*X )
  // test on angle < pi/2 may be done on cosAngle > 0
  TVector3 v = point - center;
  v.normalize();
  float cosAngle = v | up;
  float halfSinAngle = sqrtf( 1 - cosAngle*cosAngle ) * 0.5f;
  
  TColor resultColor;
  if ( cosAngle > 0.0f )
    resultColor = ( colorTop * ( 1.0f - halfSinAngle ) ) + ( colorBottom * ( halfSinAngle ) );
  else
    resultColor = ( colorTop * ( halfSinAngle ) )        + ( colorBottom * ( 1.0f - halfSinAngle ) );

#ifdef __DEBUG__  
  v.print("v: ");
  printf("cosAngle: %5.5f  halfSinAngle: %5.5f\n", cosAngle, halfSinAngle );
  resultColor.print("hemispherical color: ");
#endif

  return resultColor;

//--UNOPTIMIZED VERSION  // v = point - center
//--UNOPTIMIZED VERSION  // angle = acosf( v . up )
//--UNOPTIMIZED VERSION  // if ( angle < pi/2 )   color = (1-sin(angle)/2) * top  +  (sin(angle)/2)*bottom
//--UNOPTIMIZED VERSION  // else                  color = (sin(angle)/2) * top  +  (1-sin(angle)/2)*bottom
//--UNOPTIMIZED VERSION  TVector3 v = point - center;
//--UNOPTIMIZED VERSION  v.normalize();
//--UNOPTIMIZED VERSION  float cosAngle = v | up;
//--UNOPTIMIZED VERSION  float angle = acosf(cosAngle);
//--UNOPTIMIZED VERSION
//--UNOPTIMIZED VERSION  TColor resultColor;
//--UNOPTIMIZED VERSION  if ( angle < M_HALF_PI )
//--UNOPTIMIZED VERSION    resultColor = ( colorTop * ( 1 - sinf(angle)*0.5f ) ) + ( colorBottom * ( sinf(angle)*0.5f ) );
//--UNOPTIMIZED VERSION  else
//--UNOPTIMIZED VERSION    resultColor = ( colorTop * ( sinf(angle)*0.5f ) ) + ( colorBottom * ( 1 - sinf(angle)*0.5f ) );
//--UNOPTIMIZED VERSION
//--UNOPTIMIZED VERSION  point.print("POINT: ");
//--UNOPTIMIZED VERSION  center.print("CENTER: ");
//--UNOPTIMIZED VERSION  v.print("V: ");
//--UNOPTIMIZED VERSION  printf("cosAngle: %5.5f  angle: %5.5f  sin(angle): %5.5f\n",
//--UNOPTIMIZED VERSION	 cosAngle, angle, sinf(angle) );
//--UNOPTIMIZED VERSION  resultColor.print("hemispherical color: ");
//--UNOPTIMIZED VERSION
//--UNOPTIMIZED VERSION  return resultColor;
}

void AmbientHemispherical::print( const char *msg ) const {
  TAmbient::print( msg );
  colorTop.print("top color: ");
  colorBottom.print("bottom color: ");
  center.print("center: ");
  up.print("up: ");
}
