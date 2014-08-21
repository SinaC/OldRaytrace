#include <stdio.h>

#include "solid.h"

const TTagList AmbientSolid::ambientSolidTags [] = {
  { "color", TAG_ambientSolid_color },
  { NULL, -1 }
};

AmbientSolid::AmbientSolid() {
  TAmbient::TAmbient(); ambientKind = AMBIENT_SOLID;
  color = TColor::black;
}

TColor AmbientSolid::getColor( const TPoint3 &point ) const {
  return color;
}

void AmbientSolid::print( const char *msg ) const {
  TAmbient::print(msg);
  color.print("color");
}
