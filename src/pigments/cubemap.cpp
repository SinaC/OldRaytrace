#include <stdio.h>

#include "cubemap.h"
#include "../bitmap/bitmap.h"
#include "../globals.h"

const TTagList PigmentCubemap::pigmentCubemapTags [] = {
  { "maps", TAG_pigmentCubemap_maps },

  { NULL, -1 }
};

const u8 PigmentCubemap::right = 0;
const u8 PigmentCubemap::left = 1;
const u8 PigmentCubemap::up = 2;
const u8 PigmentCubemap::down = 3;
const u8 PigmentCubemap::front = 4;
const u8 PigmentCubemap::back = 5;

PigmentCubemap::PigmentCubemap() {
  TPigment::TPigment(); pigmentKind = PIGMENT_CUBEMAP;
  for ( int i = 0; i < 6; i++ )
    maps[i] = NULL;
}

PigmentCubemap::~PigmentCubemap() {
  for ( int i = 0; i < 6; i++ )
    maps[i] = NULL;
}

void PigmentCubemap::setup() {
  for ( int i = 0; i < 6; i++ )
    if ( maps[i] != NULL )
      maps[i]->setup();
}

TColor PigmentCubemap::getColor( const TPoint3 &p ) const {
  const TVector3 v = p - anchor;
  // projected on highest component
  const float Vx = fabsf(v[__X]),
    Vy = fabsf(v[__Y]),
    Vz = fabsf(v[__Z]);
  const u32 highestComponent = MAX3i( Vx, Vy, Vz );
  const float oneOverHighest = 0.5f/v.v[highestComponent];
  float Uf, Vf;
  TBitmapColor *whichMap;
  switch (highestComponent) {
  case __X:
    if ( v.v[highestComponent] > 0 ) {
      whichMap = maps[right];
      Uf = 1.0f - ( v.v[__Z] * oneOverHighest + 0.5f );
      Vf = 1.0f - ( v.v[__Y] * oneOverHighest + 0.5f );
    }
    else {
      whichMap = maps[left];
      Uf = v.v[__Z] * oneOverHighest + 0.5f;
      Vf = v.v[__Y] * oneOverHighest + 0.5f;
    }
    break;
  case __Y:
    if ( v.v[highestComponent] > 0 ) {
      whichMap = maps[up];
      Uf = v.v[__X] * oneOverHighest + 0.5f;
      Vf = v.v[__Z] * oneOverHighest + 0.5f;
    }
    else {
      whichMap = maps[down];
      Uf = 1.0f - ( v.v[__X] * oneOverHighest + 0.5f );
      Vf = 1.0f - ( v.v[__Z] * oneOverHighest + 0.5f );
    }
    break;
  case __Z:
    if ( v.v[highestComponent] > 0 ) {
      whichMap = maps[front];
      Uf = 1.0f - ( v.v[__X] * oneOverHighest + 0.5f );
      Vf = 1.0f - ( v.v[__Y] * oneOverHighest + 0.5f );
    }
    else {
      whichMap = maps[back];
      Uf = v.v[__X] * oneOverHighest + 0.5f;
      Vf = v.v[__Y] * oneOverHighest + 0.5f;
    }
    break;
  }
  Uf *= (float)( whichMap->width - 1 );
  Vf *= (float)( whichMap->height - 1 );

  switch ( globalSettings::whichMapping ) {
  case globalSettings::MAPPING_NORMAL: return whichMap->dotAt( (u32)Uf, (u32)Vf ); break;
  case globalSettings::MAPPING_BILINEAR: return whichMap->dotAtBilinear( Uf, Vf ); break;
  case globalSettings::MAPPING_NORMAL_DISTANCE: return whichMap->dotAtNormalDist( Uf, Vf ); break;
  default: return whichMap->dotAt( (u32)Uf, (u32)Vf ); break;
  }
}

void PigmentCubemap::print( const char *msg ) const {
  TPigment::print( msg );
  for ( int i = 0; i < 6; i++ )
    if ( maps[i] )
      maps[i]->print("map= ");
    else
      printf("NO MAP [%d]\n", i );
}
