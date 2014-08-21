#include <stdio.h>

#include "ground.h"
#include "../turbulences/turbulence.h"
#include "../ray.h"

const TTagList AtmosphericGround::atmosphericGroundTags [] = {
  { "offset", TAG_atmosphericGround_offset },
  { "altitude", TAG_atmosphericGround_altitude },
  { "up", TAG_atmosphericGround_up },

  { NULL, -1 }
};

AtmosphericGround::AtmosphericGround() {
  AtmosphericFog::AtmosphericFog(); atmosphericKind = ATMOSPHERIC_GROUND;
  up = TVector3( 0, 1, 0 );
  offset = 0.0f;
  altitude = 1.0f;
}

void AtmosphericGround::setup() {
	up.normalize();
}

float AtmosphericGround::getAttenuation( const TIntersection &intersection ) const {
  // depth = 0.0
  // width = intersection.distance
  float width = 1.0f;
  float localDensity = 1.0f;
  TPoint3 P1;
  TPoint3 P2;
  if ( intersection.distance >= INFINITE
	  || intersection.ray == NULL ) {
	  //return -1.0f; // FIXME: no fog

	  float distTop = ( intersection.ray->origin | up ) - altitude - offset;
	  float distBottom = ( intersection.ray->origin | up ) - offset;
	  // Plane equation: normal . point = dist
	  // we must solve following equation:
      //  normal . ( ray.loc + ray.dir * t ) = dist
      // solution: t = -( dist - normal . ray.loc ) / ( normal . ray.dir )
      float vd = up | intersection.ray->direction;
      if ( vd == 0.0f )
		  return 1.0f;

      if ( ( -distTop > 0 ) ^ ( vd > 0 ) ) // if doesn't have same sign: normal is pointing away from the ray
        return -1.0f;

	  float width = -distTop / vd;

	  if ( width < 0.0f ) {
		P2 = intersection.ray->origin;
		P1 = intersection.ray->origin + ( width * intersection.ray->direction );
	  }
	  else {
		P1 = intersection.ray->origin;
		P2 = intersection.ray->origin + ( width * intersection.ray->direction );
	  }
	  /*
	  // if origin is in the fog
	  //   compute intersection with virtual plane (up,altitude+offset)
	  // else
	  //   if direction is thru fog, choose arbitrary width
	  //   else no fog
	  if (  distTop < 0.0f && distBottom > 0.0f ) {
		// Plane equation: normal . point = dist
		// we must solve following equation:
		//  normal . ( ray.loc + ray.dir * t ) = dist
		// solution: t = -( dist - normal . ray.loc ) / ( normal . ray.dir )
			float vd = up | intersection.ray->direction;
			if ( vd == 0.0f ) // parallel to planes
				return fabsf( distTop - distBottom ) / altitude;

			float distTop2 = -distTop / vd;
			float distBottom2 = -distBottom / vd;

			// distTop and distBottom cannot be both negative
			if ( distTop2 < 0.0f )
				width = distBottom2;
			else if ( distBottom2 < 0.0f )
				width = distTop2;
			else if ( distBottom2 < distTop2 )
				width = distBottom2;
			else
				width = distTop2;
			P1 = intersection.ray->origin;
			P2 = intersection.ray->origin + ( width * intersection.ray->direction );
	  }
	  // if direction points to fog
	  else if ( ( intersection.ray->direction | up ) < 0.0f ) {
		  float vd = up | intersection.ray->direction;
		  float distTop2 = -distTop / vd;
		  float distBottom2 = -distBottom / vd;
			// distTop and distBottom cannot be both negative
			if ( distTop2 < 0.0f )
				width = distBottom2;
			else if ( distBottom2 < 0.0f )
				width = distTop2;
			else if ( distBottom2 < distTop2 )
				width = distBottom2;
			else
				width = distTop2;
			P1 = intersection.ray->origin;
			P2 = intersection.ray->origin + ( width * intersection.ray->direction );

		  //width = 100000.0f; // TODO: P2 is equal to intersection with top or bottom fog plane
		  //P1 = intersection.ray->origin;
		  //P2 = intersection.ray->origin + ( width * intersection.ray->direction );
	  }
	  else {
		  printf("GROUND FOG: %8.8f  %8.8f    %8.8f\n", distTop, distBottom, (intersection.ray->direction | up) );
		  return -1.0f;
	  }
	  */
  }
  // fog only if P2 is inside/behind fog
  // P1 is the first intersection between ray and fog
  else
  {
    P1 = intersection.ray->origin; // start point
    P2 = intersection.pointWorldSpace; // end point
	width = intersection.distance;
  }
  {
    float y1 = up | P1;
    float y2 = up | P2;
    float start = ( y1 - offset ) / altitude;
    float end = ( y2 - offset ) / altitude;
    // get integral along y-axis from start to end.
    if ( start <= 0.0f )
      if ( end <= 0.0f )
	localDensity = 1.0f;
      else
	localDensity = ( atanf( end ) - start ) / ( end - start );
    else
      if ( end <= 0.0f )
	localDensity = ( atanf( start ) - end ) / ( start - end );
      else {
	float delta = start - end;
	if ( fabsf(delta) > EPS )
	  localDensity = ( atanf( start ) - atanf( end ) ) / delta;
	else
	  localDensity = 1.0f / ( SQR(start) + 1.0f );
      }
    if ( turbulence != NULL ) {
      TPoint3 P = ( P1 + P2 ) * 0.5f;
      P = P * turbulence->turbulence;
      //--float k = expf( -intersection.distance * density );
      float k = 1.0f / ( 1.0f + intersection.distance * density );
      width = intersection.distance *
	( 1.0f - k * MIN( 1.0f, turbulence->getTurbulence1( P ) * turbulenceDepth ) );
    }
  }
  //--  return expf( -width * density * localDensity );
  return 1.0f / ( 1.0f + width*density*localDensity );
}

void AtmosphericGround::print( const char *msg ) const {
  AtmosphericFog::print( msg );
  printf("altitude: %5.5f\n"
	 "offset: %5.5f\n",
	 altitude, offset );
  up.print("up:");
}
