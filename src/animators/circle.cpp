#include <stdio.h>

#include "circle.h"
#include "../math/matrix3.h"

const TTagList MoverCircle::moverCircleTags [] = {
  { "source", TAG_moverCircle_source },
  { "axis", TAG_moverCircle_axis },
  { "speed", TAG_moverCircle_speed },
  { NULL, -1 }
};

MoverCircle::MoverCircle() {
  MoverVector(); moverKind = MOVER_CIRCLE;
}

TVector3 MoverCircle::vmove( const float time ) {
  TMatrix3 rot = rotateMatrix( time*speed, axis );
  return rot * source;
}

void MoverCircle::print( const char *msg ) const {
  TMover::print(msg);
  source.print("source: ");
  axis.print("axis: ");
  printf("speed: %5.2f\n", speed );
}
