#include <stdio.h>

#include "fog.h"
#include "../turbulences/turbulence.h"
#include "../ray.h"

const TTagList AtmosphericFog::atmosphericFogTags [] = {
  { "density", TAG_atmosphericFog_density },
  { "color", TAG_atmosphericFog_color },
  { "filter", TAG_atmosphericFog_filter },
  { "turbulence", TAG_atmosphericFog_turbulence },
  { "depth", TAG_atmosphericFog_turbulenceDepth },

  { NULL, -1 }
};

AtmosphericFog::AtmosphericFog() {
  TAtmospheric::TAtmospheric(); atmosphericKind = ATMOSPHERIC_FOG;
  turbulence = NULL;
}

AtmosphericFog::~AtmosphericFog() {
  turbulence = NULL;
}

void AtmosphericFog::setup() {
  if ( turbulence != NULL )
    turbulence->setup();
}

TColor AtmosphericFog::addColor( const TColor &currentColor,
				 const TIntersection &intersection ) const {
  float att = getAttenuation( intersection );
  if (att < 0.0f )
    return currentColor;
  TColor attColor;
  attColor.RED   = att * ( ( 1.0f - filter ) + ( filter * color.RED   ) );
  attColor.GREEN = att * ( ( 1.0f - filter ) + ( filter * color.GREEN ) );
  attColor.BLUE  = att * ( ( 1.0f - filter ) + ( filter * color.BLUE  ) );
  return
    ( currentColor * attColor )
    + ( color * ( 1.0f - att ) );
}

float AtmosphericFog::getAttenuation( const TIntersection &intersection ) const {
  float width = 1.0f;
  if ( turbulence == NULL
       || intersection.distance >= INFINITE
       || intersection.ray == NULL )
    width = intersection.distance;
  else {
    float depth = intersection.distance * 0.5f;
    TPoint3 P = intersection.ray->origin + ( depth * intersection.ray->direction );
    P = P * turbulence->turbulence;
    //--float k = expf( -intersection.distance*density );
    float k = 1.0f / ( 1.0f + intersection.distance*density );
    width = intersection.distance *
      ( 1.0f - k * MIN( 1.0f, turbulence->getTurbulence1( P ) * turbulenceDepth ) );
  }
  //--return expf( -width * density );
  return 1.0f / ( 1.0f + width*density );
}

void AtmosphericFog::print( const char *msg ) const {
  TAtmospheric::print(msg);
  printf("density: %5.5f\n", density );
  color.print("color: ");
  if ( turbulence != NULL )
    turbulence->print("turbulence: ");
}
