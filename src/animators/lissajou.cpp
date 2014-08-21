#include <stdio.h>

#include "lissajou.h"

// LISSAJOU
const TTagList MoverLissajou::moverLissajouTags [] = {
  { "pulse", TAG_moverLissajou_pulse },
  { "phase", TAG_moverLissajou_phase },
  { "amplitude", TAG_moverLissajou_amplitude },
  { "shift", TAG_moverLissajou_shift },
  { NULL, -1 }
};

MoverLissajou::MoverLissajou() {
  MoverVector::MoverVector(); moverKind = MOVER_LISSAJOU;
}

TVector3 MoverLissajou::vmove( const float time ) {
  TVector3 res = pulse*time+phase;

  res[0] = sinf(res[0]);
  res[1] = sinf(res[1]);
  res[2] = sinf(res[2]);

  res = shift + ( res * ampl );
  return res;
}

void MoverLissajou::print( const char *msg ) const {
  TMover::print(msg);
  pulse.print("pulse: ");
  phase.print("phase: ");
  ampl.print("amplitude: ");
  shift.print("shift: ");
}

