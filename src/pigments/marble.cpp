#include <stdio.h>

#include "marble.h"

const TTagList PigmentMarble::pigmentMarbleTags [] = {
  { "map", TAG_pigmentMarble_map },
  { "turbulence", TAG_pigmentMarble_turbulence },

  { NULL, -1 }
};

PigmentMarble::PigmentMarble() {
  TPigment::TPigment(); pigmentKind = PIGMENT_MARBLE;
  map = NULL;
  turbulence = NULL;
}

PigmentMarble::~PigmentMarble() {
  map = NULL;
  turbulence = NULL;
}

void PigmentMarble::setup() {
  if ( map )
    map->setup();
  if ( turbulence )
    turbulence->setup();
}

TColor PigmentMarble::getColor( const TPoint3 &p ) const {
  TVector3 t = ( p - anchor );
  t.normalize();

  float val;
  if ( turbulence )
    val = t[__X] + ( turbulence->turbulence[__X] * turbulence->getTurbulence1( t ) );
  else
    val = t[__X];

  if ( map )
    return map->getColor( val );
  return TColor( val, val, val );
}

void PigmentMarble::print( const char *msg ) const {
  TPigment::print( msg );
  if ( turbulence )
    turbulence->print("turbulence:");
  else
    printf("NO TURBULENCE!\n");
  if ( map )
    map->print( "colormap:" );
  else
    printf("no colormap\n");
}
