#include <stdio.h>

#include "bumps.h"
#include "../turbulences/noise.h"

const TTagList NormalBumps::normalBumpsTags [] = {
  { "bumps", TAG_normalBumps_bumps },
  { NULL, -1 }
};

NormalBumps::NormalBumps() {
  TNormal::TNormal(); normalKind = NORMAL_BUMPS;
  bumps = 0.0f;
}

bool NormalBumps::check() const {
  return true;
}

void NormalBumps::perturbNormal( TIntersection &inter ) const {
  if ( bumps > 0.0f ) {
    inter.normal = inter.rawNormal + ( TurbulenceNoise::getNoise3( inter.pointWorldSpace ) * bumps );
    inter.normal.normalize();
  }
  else
    inter.normal = inter.rawNormal;
}

void NormalBumps::print( const char *msg ) const {
  TNormal::print(msg);
  printf("bumps: %5.2f\n", bumps );
}
