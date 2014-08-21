#include <stdio.h>

#include "volumetric.h"
#include "../turbulences/turbulence.h"
#include "../ray.h"
#include "../primitives/primitive.h"

const TTagList AtmosphericVolumetric::atmosphericVolumetricTags [] = {
  { "volume", TAG_atmosphericVolumetric_volume },

  { NULL, -1 }
};

AtmosphericVolumetric::AtmosphericVolumetric() {
  AtmosphericFog::AtmosphericFog(); atmosphericKind = ATMOSPHERIC_VOLUMETRIC;
  volume = NULL;
}

void AtmosphericVolumetric::setup() {
  if ( volume != NULL )
    volume->setup();
}

float AtmosphericVolumetric::getAttenuation( const TIntersection &intersection ) const {
  if ( volume == NULL )
    return AtmosphericFog::getAttenuation( intersection );
  else {
    TIntersectionStack IStack;
    IStack.create(16);
    const TRay *ray = intersection.ray;
    if ( !volume->allIntersectionRay( *ray, IStack ) )
      return -1.0f; // no intersection
    TPoint3 P1, P2;
    float width;
    if ( IStack.index == 1 ) { // only 1 intersection: first point is ray origin
      P1 = ray->origin;
      P2 = ray->origin + ( IStack.stack[0].distance * ray->direction );
      width = IStack.stack[0].distance;
    }
    else { // more than 1 intersection: get first and last point and create fog between these 2 points
      float dMin = INFINITE, dMax = -INFINITE;
      for ( int i = 0; i < IStack.index; i++ ) {
	if ( IStack.stack[i].distance < dMin )
	  dMin = IStack.stack[i].distance;
	if ( IStack.stack[i].distance > dMax )
	  dMax = IStack.stack[i].distance;
      }
      P1 = ray->origin + ( dMin * ray->direction );
      P2 = ray->origin + ( dMax * ray->direction );
      width = ( dMin + dMax ) * 0.5f;
    }
    // TODO: for each pair of intersection, compute integral along y-axis
    // add these integrals  ( integral of 1/(1+x2) between a and b  is atan(b)-atan(a) )

    float localDensity = 1.0f;

    if ( turbulence != NULL ) {
      TPoint3 P = ( P1 + P2 ) * 0.5f;
      P = P * turbulence->turbulence;
      float k = 1.0f / ( 1.0f + intersection.distance * density );
      width = width * 
	( 1.0f - k * MIN( 1.0f, turbulence->getTurbulence1( P ) * turbulenceDepth ) );
    }
    return 1.0f / ( 1.0f + width * density * localDensity );
  }
  return -1.0f;
//--TODO--  // depth = 0.0
//--TODO--  // width = intersection.distance
//--TODO--  float width = 1.0f;
//--TODO--  float localDensity = 1.0f;
//--TODO--  if ( intersection.distance >= INFINITE
//--TODO--       || intersection.ray == NULL )
//--TODO--    return -1.0f; // FIXME: no fog
//--TODO--  else {
//--TODO--    TPoint3 P1 = intersection.ray->origin; // start point
//--TODO--    TPoint3 P2 = intersection.pointWorldSpace; // end point
//--TODO--    width = intersection.distance;
//--TODO--    float y1 = up | P1;
//--TODO--    float y2 = up | P2;
//--TODO--    float start = ( y1 - offset ) / altitude;
//--TODO--    float end = ( y2 - offset ) / altitude;
//--TODO--    // get integral along y-axis from start to end.
//--TODO--    if ( start <= 0.0f )
//--TODO--      if ( end <= 0.0f )
//--TODO--	localDensity = 1.0f;
//--TODO--      else
//--TODO--	localDensity = ( atanf( end ) - start ) / ( end - start );
//--TODO--    else
//--TODO--      if ( end <= 0.0f )
//--TODO--	localDensity = ( atanf( start ) - end ) / ( start - end );
//--TODO--      else {
//--TODO--	float delta = start - end;
//--TODO--	if ( fabsf(delta) > EPS )
//--TODO--	  localDensity = ( atanf( start ) - atanf( end ) ) / delta;
//--TODO--	else
//--TODO--	  localDensity = 1.0f / ( SQR(start) + 1.0f );
//--TODO--      }
//--TODO--    if ( turbulence != NULL ) {
//--TODO--      TPoint3 P = ( P1 + P2 ) * 0.5f;
//--TODO--      P = P * turbulence->turbulence;
//--TODO--      //--float k = expf( -intersection.distance * density );
//--TODO--      float k = 1.0f / ( 1.0f + intersection.distance * density );
//--TODO--      width = intersection.distance * 
//--TODO--	( 1.0f - k * MIN( 1.0f, turbulence->getTurbulence1( P ) * turbulenceDepth ) );
//--TODO--    }
//--TODO--  }
//--TODO--  //--  return expf( -width * density * localDensity );
//--TODO--  return 1.0f / ( 1.0f + width*density*localDensity );
  return -1.0f;
}

void AtmosphericVolumetric::print( const char *msg ) const {
  AtmosphericFog::print( msg );
  if ( volume )
    volume->print("volume: ");
  else
    printf("NO VOLUME\n");
}
