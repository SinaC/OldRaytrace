#include <stdio.h>

#include "vector4.h"
#include "vector3.h"

const TVector4 TVector4::zeroVector( 0, 0, 0, 0 );
const TVector4 TVector4::oneVector( 1, 1, 1, 1 );
const TVector4 TVector4::twoVector( 2, 2, 2, 2 );

void TVector4::print( const char *msg ) const {
  printf("%s ( %5.5f, %5.5f, %5.5f, %5.5f )\n", msg, v[0], v[1], v[2], v[3] );  fflush(stdout);
}
