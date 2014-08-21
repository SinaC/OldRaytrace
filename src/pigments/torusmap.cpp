#include <stdio.h>

#include "torusmap.h"
#include "../bitmap/bitmap.h"
#include "../globals.h"

//--#define __DEBUG__

const TTagList PigmentTorusmap::pigmentTorusmapTags [] = {
  { "map", TAG_pigmentTorusmap_map },
  { "gradient", TAG_pigmentTorusmap_gradient },

  { NULL, -1 }
};

PigmentTorusmap::PigmentTorusmap() {
  TPigment::TPigment(); pigmentKind = PIGMENT_TORUSMAP;
  map = NULL;
  gradient = TVector3( 1, 0, -1 );
}

PigmentTorusmap::~PigmentTorusmap() {
  map = NULL;
}

void PigmentTorusmap::setup() {
  if ( map != NULL )
    map->setup();
}

TColor PigmentTorusmap::getColor( const TPoint3 &p ) const {
  TVector3 vector = ( p - anchor );
  float Uf, Vf;

#ifdef __DEBUG__
  vector.print("vector:");
#endif

  float theta;
  float x = vector[__X];
  float y = vector[__Y];
  float z = vector[__Z];
  float r0 = gradient[__X];
  // Determine its angle from the x-axis.
  float len = sqrtf( SQR(x) + SQR(y) );
  if ( len == 0.0f )
    return TColor::black;
  else
    if ( y == 0.0f )
      if ( x > 0.0f )
	theta = 0.0f;
      else
	theta = M_PI;
    else {
      theta = acosf( x / len );
      if ( y < 0.0f )
	theta = M_2_PI - theta;
    }
  theta = 0.0f - theta;

  // Now rotate about the y-axis to get the point (x, y, z) into the x-y plane.
  x = len - r0;
  len = sqrtf( SQR(x) + SQR(z) );
  float phi = acosf( -x / len );
  if ( z > 0.0f )
    phi = M_2_PI - phi;

  // Determine the parametric coordinates.
  theta *= M_ONE_OVER_2_PI;
  phi *= M_ONE_OVER_2_PI;

  Uf = ( -theta * (float)(map->width-1) );

  Vf = ( phi * (float)(map->height-1) );

#ifdef __DEBUG__
  printf("Uf: %5.5f    Vf: %5.5f\n", Uf, Vf );
#endif

  switch ( globalSettings::whichMapping ) {
  case globalSettings::MAPPING_NORMAL: return map->dotAt( (u32)Uf, (u32)Vf ); break;
  case globalSettings::MAPPING_BILINEAR: return map->dotAtBilinear( Uf, Vf ); break;
  case globalSettings::MAPPING_NORMAL_DISTANCE: return map->dotAtNormalDist( Uf, Vf ); break;
  default: return map->dotAt( (u32)Uf, (u32)Vf ); break;
  }
}

void PigmentTorusmap::print( const char *msg ) const {
  TPigment::print( msg );
  gradient.print("gradient:");
  if ( map )
    map->print("map= ");
  else
    printf("NO MAP\n");
}
