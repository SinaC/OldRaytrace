#include "cycloidal.h"
#include "math.h"

#define SINTABSIZE (1000)

static float sintab[SINTABSIZE];

void cycloidalInit() {
  for ( int i = 0; i < SINTABSIZE; i++ )
    sintab[i] = sinf( (float)i * M_2_PI / (float)SINTABSIZE );
}

float cycloidal( const float value ) {
  register int indx;

  if ( value >= 0.0f ) {
    indx = (int)((value - floor(value)) * SINTABSIZE);
    return (sintab [indx]);
  }
  else
  {
    indx = (int)((0.0f - (value + floor(0.0f - value))) * SINTABSIZE);
    return (0.0f - sintab [indx]);
  }
}
