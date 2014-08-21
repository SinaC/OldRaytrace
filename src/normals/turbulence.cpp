#include <stdio.h>

#include "turbulence.h"
#include "../turbulences/noise.h"

const TTagList NormalTurbulence::normalTurbulenceTags [] = {
  { "turbulence", TAG_normalTurbulence_turbulence },
  { "factor", TAG_normalTurbulence_factor },
  { NULL, -1 }
};

NormalTurbulence::NormalTurbulence() {
  TNormal::TNormal(); normalKind = NORMAL_TURBULENCE;
  turbulence = NULL;
  factor = 0.0f;
}

bool NormalTurbulence::check() const {
  return true;
}

void NormalTurbulence::perturbNormal( TIntersection &inter ) const {
  if ( turbulence != NULL
       && factor > 0.0f ) {
    inter.normal = inter.rawNormal + ( turbulence->getTurbulence3( inter.pointWorldSpace ) * factor );
    inter.normal.normalize();
  }
  else
    inter.normal = inter.rawNormal;
}

void NormalTurbulence::print( const char *msg ) const {
  TNormal::print(msg);
  if ( turbulence )
    turbulence->print("turbulence:");
  printf("factor: %5.5f\n", factor );
}
