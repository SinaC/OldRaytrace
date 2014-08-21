#include <string.h>
#include <stdio.h>

#include "atmospheric.h"

const TTagList TAtmospheric::atmosphericTags [] = {
  { NULL, -1 }
};

const TTagList TAtmospheric::atmosphericSubClassTags [] = {
  {  "fog", ATMOSPHERIC_FOG },
  {  "ground", ATMOSPHERIC_GROUND },
  {  "volumetric", ATMOSPHERIC_VOLUMETRIC },
  {  NULL, -1 }
};

TAtmospheric::TAtmospheric() {
  THandle::THandle(); handleKind = HANDLE_ATMOSPHERIC;
  atmosphericKind = ATMOSPHERIC_NOTHING;
}

void TAtmospheric::setup() {
}

TColor TAtmospheric::addColor( const TColor &currentColor,
			       const TIntersection &intersection ) const {
  return currentColor;
}

float TAtmospheric::getAttenuation( const TIntersection &intersection ) const {
  return 0.0f;
}

void TAtmospheric::print( const char *msg ) const {
  THandle::print(msg);
  printf("KIND: %s\n", atmosphericName() );
}

char *TAtmospheric::atmosphericName() const {
  for ( int i = 0; atmosphericSubClassTags[i].tagName != NULL; i++ )
    if ( atmosphericSubClassTags[i].tagId == atmosphericKind )
      return atmosphericSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", atmosphericKind );
  return buf;
}
