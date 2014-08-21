#include <stdio.h>

#include "linearf.h"

const TTagList MoverLinearF::moverLinearFTags [] = {
  { "zero", TAG_moverLinearF_zero },
  { "factor", TAG_moverLinearF_factor },
  { NULL, -1 }
};

MoverLinearF::MoverLinearF() {
  MoverFloat(); moverKind = MOVER_LINEAR;
}

float MoverLinearF::fmove( const float time ) {
  return time*fact+zero;
}

void MoverLinearF::print( const char *msg ) const {
  TMover::print(msg);
  printf("zero: %5.2f\n"
	 "fact: %5.2f\n",
	 zero, fact );
}
