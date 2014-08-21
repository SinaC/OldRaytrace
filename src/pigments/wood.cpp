
#include <stdio.h>

#include "wood.h"
#include "../math/cycloidal.h"

const TTagList PigmentWood::pigmentWoodTags [] = {
  { "map", TAG_pigmentWood_map },
  { "turbulence", TAG_pigmentWood_turbulence },

  { NULL, -1 }
};

PigmentWood::PigmentWood() {
  TPigment::TPigment(); pigmentKind = PIGMENT_WOOD;
  map = NULL;
  turbulence = NULL;
}

PigmentWood::~PigmentWood() {
  map = NULL;
  turbulence = NULL;
}

void PigmentWood::setup() {
  if ( map )
    map->setup();
  if ( turbulence )
    turbulence->setup();
}

TColor PigmentWood::getColor( const TPoint3 &p ) const {
  TVector3 t = ( p - anchor );
  t.normalize();

  float val;
  if ( turbulence ) {
    TPoint3 woodTurbulence = turbulence->getTurbulence3( t );
    float x = t[__X] + cycloidal( t[__X] + woodTurbulence[__X] ) * turbulence->turbulence[__X];
    float y = t[__Y] + cycloidal( t[__Y] + woodTurbulence[__Y] ) * turbulence->turbulence[__Y];
    val = sqrtf( SQR(x) + SQR(y) );
  }
  else
    val = sqrtf( SQR(t[__X]) + SQR(t[__Y]) );

  if ( map )
    return map->getColor( val );
  return TColor( val, val, val );
}

void PigmentWood::print( const char *msg ) const {
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
