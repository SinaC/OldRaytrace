#include <stdio.h>

#include "cylindrical.h"
#include "../bitmap/bitmap.h"
#include "../globals.h"

const TTagList PigmentCylindrical::pigmentCylindricalTags [] = {
  { "map", TAG_pigmentCylindrical_map },

  { NULL, -1 }
};

PigmentCylindrical::PigmentCylindrical() {
  TPigment::TPigment(); pigmentKind = PIGMENT_CYLINDRICAL;
  map = NULL;
}

PigmentCylindrical::~PigmentCylindrical() {
  map = NULL;
}

void PigmentCylindrical::setup() {
  if ( map != NULL )
    map->setup();
}

TColor PigmentCylindrical::getColor( const TPoint3 &p ) const {
  TVector3 vector = p - anchor;
  vector.normalize();
 
  float Vf = (float)(map->height-1) - (1.0f + vector[__Y]) * 0.5f * (float)(map->height-1);

  float a = atan2f( vector[__X], vector[__Z] );
  // a in [-pi, pi]

  float Uf = (a+M_PI) * 0.5f * M_ONE_OVER_PI * (float)(map->width-1);

  switch ( globalSettings::whichMapping ) {
  case globalSettings::MAPPING_NORMAL: return map->dotAt( (u32)Uf, (u32)Vf ); break;
  case globalSettings::MAPPING_BILINEAR: return map->dotAtBilinear( Uf, Vf ); break;
  case globalSettings::MAPPING_NORMAL_DISTANCE: return map->dotAtNormalDist( Uf, Vf ); break;
  default: return map->dotAt( (u32)Uf, (u32)Vf ); break;
  }
}

void PigmentCylindrical::print( const char *msg ) const {
  TPigment::print( msg );
  if ( map )
    map->print("map= ");
  else
    printf("NO MAP\n");
}
