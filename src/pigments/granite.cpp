#include <stdio.h>

#include "granite.h"
#include "../turbulences/noise.h"

const TTagList PigmentGranite::pigmentGraniteTags [] = {
  { "map", TAG_pigmentGranite_map },

  { NULL, -1 }
};

PigmentGranite::PigmentGranite() {
  TPigment::TPigment(); pigmentKind = PIGMENT_GRANITE;
  map = NULL;
}

PigmentGranite::~PigmentGranite() {
  map = NULL;
}

void PigmentGranite::setup() {
  if ( map )
    map->setup();
}

TColor PigmentGranite::getColor( const TPoint3 &p ) const {
  TVector3 tv1 = ( p - anchor ) * 4.0f;
  tv1.normalize();

  float freq = 1.0f;
  float result = 0.0f;
  for ( int i = 0; i < 6; freq *= 2.0f, i++ ) {
    TVector3 tv2 = tv1 * freq;
    float temp = fabsf( 0.5f - TurbulenceNoise::getNoise1( tv2 ) );
    result += temp / freq;
  }

  if ( map )
    return map->getColor( result );
  return TColor( result, result, result );
}

void PigmentGranite::print( const char *msg ) const {
  TPigment::print( msg );
  if ( map )
    map->print( "colormap:" );
  else
    printf("no colormap\n");
}
