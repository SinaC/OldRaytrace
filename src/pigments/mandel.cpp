
#include <stdio.h>

#include "mandel.h"

const TTagList PigmentMandel::pigmentMandelTags [] = {
  { "map", TAG_pigmentMandel_map },
  { "iterations", TAG_pigmentMandel_iterations },

  { NULL, -1 }
};

PigmentMandel::PigmentMandel() {
  TPigment::TPigment(); pigmentKind = PIGMENT_MANDEL;
  iterations = 2;
  map = NULL;
}

PigmentMandel::~PigmentMandel() {
  map = NULL;
}

void PigmentMandel::setup() {
  if ( map )
    map->setup();
}

TColor PigmentMandel::getColor( const TPoint3 &p ) const {
  TPoint3 t = ( p - anchor );
  t.normalize();

  int col;
  float a, b, cf, a2, b2, x, y;

  a = x = t[__X]; a2 = sqr(a);
  b = y = t[__Y]; b2 = sqr(b);

  for ( col = 0; col < iterations; col++ ) {
    b  = 2.0f * a * b + y;
    a  = a2 - b2 + x;

    a2 = sqr(a);
    b2 = sqr(b);

    if ( (a2 + b2) > 4.0f )
      break;
  }

  cf = (float)col / (float)iterations;

  if ( map )
    return map->getColor( cf );
  return TColor( cf, cf, cf );
}

void PigmentMandel::print( const char *msg ) const {
  TPigment::print( msg );
  if ( map )
    map->print( "colormap:" );
  else
    printf("no colormap\n");
  printf("#iterations: %d\n", iterations );
}
