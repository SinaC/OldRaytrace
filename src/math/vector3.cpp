#include <stdio.h>

#include "vector3.h"

const TVector3 TVector3::zeroVector( 0, 0, 0 );
const TVector3 TVector3::oneVector( 1, 1, 1 );
const TVector3 TVector3::twoVector( 2, 2, 2 );

void TVector3::print( const char *msg ) const {
  printf("%s ( %5.5f, %5.5f, %5.5f )\n", msg, v[0], v[1], v[2] ); fflush(stdout);
}
