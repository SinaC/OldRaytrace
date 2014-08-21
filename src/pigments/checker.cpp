#include <stdio.h>

#include "checker.h"
#include "../math/math.h"

const float smallTolerance = 0.0001f;

const TTagList PigmentChecker::pigmentCheckerTags [] = {
  { "black", TAG_pigmentChecker_black },
  { "white", TAG_pigmentChecker_white },
  { "size", TAG_pigmentChecker_size },
  { NULL, -1 }
};

PigmentChecker::PigmentChecker() {
  TPigment::TPigment(); pigmentKind = PIGMENT_CHECKER;
  colorWhite = TColor::white;
  colorBlack = TColor::black;
  size = TVector3( 1, 1, 1 );
}

void PigmentChecker::setup() {
  invSize = inverseVector( size );
}

TColor PigmentChecker::getColor( const TPoint3 &p ) const {
  TPoint3 t = ( ( p - anchor ) * invSize ) + smallTolerance;
  return ( (int)( floorf(t[0]) +
  		  floorf(t[1]) +
  		  floorf(t[2]) )
	   & 1 )
    ? colorWhite : colorBlack;
}

void PigmentChecker::print( const char *msg ) const {
  TPigment::print( msg );
  colorWhite.print("color white: ");
  colorBlack.print("color black: ");
}
