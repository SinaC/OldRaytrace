#include <stdio.h>

#include "planar.h"
#include "../bitmap/bitmap.h"
#include "../globals.h"

//--#define __DEBUG__

const TTagList PigmentPlanar::pigmentPlanarTags [] = {
  { "map", TAG_pigmentPlanar_map },
  { "gradient", TAG_pigmentPlanar_gradient },
  { "size", TAG_pigmentPlanar_size },

  { NULL, -1 }
};

PigmentPlanar::PigmentPlanar() {
  TPigment::TPigment(); pigmentKind = PIGMENT_PLANAR;
  map = NULL;
  gradient = TVector3( 1, 0, -1 );
  size = 128.0f;
}

PigmentPlanar::~PigmentPlanar() {
  map = NULL;
}

void PigmentPlanar::setup() {
  if ( map != NULL )
    map->setup();
  invSize = 1.0f / size;
}

TColor PigmentPlanar::getColor( const TPoint3 &p ) const {
  TVector3 vector = ( p - anchor ) * invSize;
  float Uf, Vf;
  const float widthF = (float)(map->width-1);
  const float heightF = (float)(map->height-1);

#ifdef __DEBUG__
  vector.print("vector:");
  printf("widthF: %5.5f   heightF: %5.5f\n", widthF, heightF );
#endif

   if ( gradient[__X] != 0.0f )
     if ( gradient[__X] > 0.0f )
       Uf = (vector[__X] - (int)vector[__X]) * widthF;
     else
       Vf = (vector[__X] - (int)vector[__X]) * heightF;
   if ( gradient[__Y] != 0.0f )
     if ( gradient[__Y] > 0.0f )
       Uf = (vector[__Y] - (int)vector[__Y]) * widthF;
     else
       Vf = (vector[__Y] - (int)vector[__Y]) * heightF;
   if ( gradient[__Z] != 0.0f )
     if ( gradient[__Z] > 0.0f )
       Uf = (vector[__Z] - (int)vector[__Z]) * widthF;
     else
       Vf = (vector[__Z] - (int)vector[__Z]) * heightF;

#ifdef __DEBUG__
  if ( Uf < 0.0f || Uf >= widthF
       || Vf < 0.0f || Vf >= heightF )
    printf("==========================>\n");
  printf("Uf: %5.5f    Vf: %5.5f\n", Uf, Vf );
#endif

  // >= tests are not needed because fmodf assures we have a number below widthF(heightF)
  //                                 -(int)
  if ( Uf < 0.0f )
    Uf += widthF;
  //--   else if ( Uf >= widthF )
  //--     Uf -= widthF;
  if ( Vf < 0.0f )
    Vf += heightF;
  //--   else if ( Vf >= heightF )
  //--     Vf -= heightF;

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

void PigmentPlanar::print( const char *msg ) const {
  TPigment::print( msg );
  gradient.print("gradient:");
  printf("size: %5.5f\n", size );
  if ( map )
    map->print("map= ");
  else
    printf("NO MAP\n");
}
