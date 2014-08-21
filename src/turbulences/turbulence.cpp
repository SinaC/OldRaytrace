#include <stdio.h>

#include "turbulence.h"

const TTagList TTurbulence::turbulenceTags [] = {
  { "turbulence", TAG_turbulence_turbulence },
  { "octaves", TAG_turbulence_octaves },
  { "lambda", TAG_turbulence_lambda },
  { "omega", TAG_turbulence_omega },

  { NULL, -1 }
};

const TTagList TTurbulence::turbulenceSubClassTags [] = {
  {  "perlin", TURBULENCE_PERLIN },
  {  "noise", TURBULENCE_NOISE },
  {  NULL, -1 }
};

TTurbulence::TTurbulence() {
  THandle::THandle(); handleKind = HANDLE_TURBULENCE;
  turbulenceKind = TURBULENCE_NOTHING;
  turbulence = TVector3( 1, 1, 1 );
  octaves = 3;
  lambda = 0.5f;
  omega = 0.5f;
}

void TTurbulence::setup() {
  turbulence.normalize();
}

float TTurbulence::getTurbulence1( const TPoint3 &p ) const {
  return 0.0f;
}

TVector3 TTurbulence::getTurbulence3( const TPoint3 &p ) const {
  return TVector3::zeroVector;
}

void TTurbulence::print( const char *msg ) const {
  THandle::print( msg );
  printf("KIND: [%s]\n", turbulenceName() );
  turbulence.print("turbulence:");
  printf("#octaves: %d\n"
	 "lambda: %5.5f\n"
	 "omega: %5.5f\n", octaves, lambda, omega );
}

char* TTurbulence::turbulenceName() const {
  for ( int i = 0; turbulenceSubClassTags[i].tagName != NULL; i++ )
    if ( turbulenceSubClassTags[i].tagId == turbulenceKind )
      return turbulenceSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", turbulenceKind );
  return buf;
}
