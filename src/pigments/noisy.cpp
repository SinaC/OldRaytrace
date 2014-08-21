#include <stdio.h>

#include "noisy.h"
#include "../turbulences/noise.h"

const TTagList PigmentNoisy::pigmentNoisyTags [] = {
  { NULL, -1 }
};

PigmentNoisy::PigmentNoisy() {
  TPigment::TPigment(); pigmentKind = PIGMENT_NOISY;
}

TColor PigmentNoisy::getColor( const TPoint3 &hitPoint ) const {
  TVector3 p = TurbulenceNoise::getNoise3( hitPoint - anchor );
  p.normalize();
  return TColor( p[0], p[1], p[2]);
}

void PigmentNoisy::print( const char *msg ) const {
  TPigment::print( msg );
}
