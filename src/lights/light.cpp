#include <stdio.h>

#include "../world.h"
#include "../ray.h"
#include "../intersection.h"
#include "../lighting.h"
#include "../primitives/primitive.h"
#include "light.h"

const TTagList TLight::lightTags [] = {
  { "color", TAG_light_color },
  { "brightness", TAG_light_brightness },
  //--  { "powerOn", TAG_light_powerOn },
  //--  { "looksLike", TAG_light_looksLike },
  { "volume", TAG_light_volume },
  { NULL, -1 }
};

const TTagList TLight::lightSubClassTags [] = {
  { "dot", LIGHT_DOT },
  { "square", LIGHT_SQUARE },
  //--  {  "spherical", LIGHT_SPHERICAL },
  {  NULL, -1 }
};

TLight::TLight() {
  THandle::THandle(); handleKind = HANDLE_LIGHT;
  lightKind = LIGHT_NOTHING;
  color = TColor::white;
  brightness = 1.0f;
  volume = NULL;
}

TLight::~TLight() {
  volume = NULL;
}

void TLight::setup() {
  if ( volume ) {
    volume->setup();
    if ( !volume->additionalInformation.isSet( TPrimitive::PRIMITIVE_INFINITE ) )
      error("TLight::setup: light volume must be INFINITE!!\n");
  }
  shadowCache = NULL;
}

void TLight::precalcFrame( const TWorld &world ) {
  if ( volume )
    volume->precalcFrame( world );
}

bool TLight::computeLighting( const TWorld &world, const TRay &ray,
			      TLighting &lighting ) {
  lighting.light = this;

  lighting.visible = isLighten( ray.intersection.pointWorldSpace );
  // If point not in light volume, return false (no diffuse component)
  if ( !lighting.visible )
    return false;

  lighting.color = color * brightness;
  return true;
}


// check if a point is in light volume
bool TLight::isLighten( const TPoint3 &point ) const {
  if ( volume )
    return volume->inside( point );
  else
    return true;
}

// compute light ray and amount of light intersection received from this light
//  but don't compute any shading
bool TLight::getLight( const TWorld &world, const TRay &ray, TRay &lightRay ) {
  return false;
}
// compute lighting using ray.shadowRay
TColor TLight::getColor( const TWorld &world, const TRay &ray, const TRay &lightRay ) {
  return TColor::black;
}

void TLight::print( const char *msg ) const {
  THandle::print( msg );
  printf("KIND: [%s]\n", lightName() );
  color.print("color:");
  printf("brightness: %5.5f\n", brightness );
  if ( volume )
    volume->print("volume: ");
}

char *TLight::lightName() const {
  for ( int i = 0; lightSubClassTags[i].tagName != NULL; i++ )
    if ( lightSubClassTags[i].tagId == lightKind )
      return lightSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", lightKind );
  return buf;
}
