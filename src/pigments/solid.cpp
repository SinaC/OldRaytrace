#include <stdio.h>

#include "solid.h"

const TTagList PigmentSolid::pigmentSolidTags [] = {
  { "color", TAG_pigmentSolid_color },

  { NULL, -1 }
};

PigmentSolid::PigmentSolid() {
  TPigment::TPigment(); pigmentKind = PIGMENT_SOLID;
  color = TColor::black;
}

void PigmentSolid::setup() {
  simple = color;
}

TColor PigmentSolid::getColor( const TPoint3 &p ) const {
  return color;
}

void PigmentSolid::print( const char *msg ) const {
  TPigment::print( msg );
  color.print("color: ");
}
