#include <stdio.h>

#include "spherical.h"
#include "../bitmap/bitmap.h"
#include "../globals.h"

const TTagList PigmentSpherical::pigmentSphericalTags [] = {
  { "map", TAG_pigmentSpherical_map },

  { NULL, -1 }
};

PigmentSpherical::PigmentSpherical() {
  TPigment::TPigment(); pigmentKind = PIGMENT_SPHERICAL;
  map = NULL;
}

PigmentSpherical::~PigmentSpherical() {
  map = NULL;
}

void PigmentSpherical::setup() {
  if ( map != NULL )
    map->setup();
}

TColor PigmentSpherical::getColor( const TPoint3 &p ) const {
//--NEW WAY  TVector3 v = anchor - p;
//--NEW WAY  float northAngle = acosf( -v[__Y] ); // northPoint( 0, 1, 0 )
//--NEW WAY  float equatorAngle = acosf( v[__X] / sinf( northAngle ) ) * M_2_PI; // equatorPoint( 1, 0, 0 )
//--NEW WAY  float Uf = ( cross( TVector3( 0, 1, 0 ), TVector3( 1, 0, 0 ) ) | direction ) < 0 ? equatorAngle : 1 - equatorAngle;
//--NEW WAY  float Vf = northAngle * M_ONE_OVER_PI;

//  float X = p[0] - anchor[0],
//    Y = p[1] - anchor[1],
//    Z = p[2] - anchor[2];
//   float len = sqrtf( X * X
// 		     + Y * Y
// 		     + Z * Z );
//   if ( len == 0.0f )
//     return TColor::black;
//   float invLen = 1/len;
//   X *= invLen;
//   Y *= invLen;
//   Z *= invLen;
  TVector3 vector = p - anchor;
  vector.normalize();

  float phi = 1.0f - ( 0.5f + asinf( vector[__Y] ) * M_ONE_OVER_PI ); // 0 -> 1.0f

  //  len = sqrtf( vector[__X] * vector[__X]
  //	       + vector[__Z] * vector[__Z] );
  float len = vector[__X] * vector[__X]
    + vector[__Z] * vector[__Z];

  float theta = 0.0f;
  if ( len != 0.0f )
    if ( vector[__Z] == 0.0f ) {
      if ( vector[__X] < 0.0f )
	theta = 1.0f;
    }
    else {
      //theta = acosf( X / len );
      theta = acosf( vector[__X] * fastInvSqrt(len) );
      if ( vector[__Z] < 0.0f )
	theta = M_2_PI - theta;
      theta *= M_ONE_OVER_PI; // 0 -> 1.0f
    }

  //--return TColor( theta, 0, phi );

  //--int U = (int)( theta * (float)(map->getWidth()-1) );
  //--int V = (int)( phi * (float)(map->getHeight()-1) );
  float Uf =  theta * (map->width-1.0f);
  float Vf =  phi * (map->height-1.0f);

  switch ( globalSettings::whichMapping ) {
  case globalSettings::MAPPING_NORMAL: return map->dotAt( (u32)Uf, (u32)Vf ); break;
  case globalSettings::MAPPING_BILINEAR: return map->dotAtBilinear( Uf, Vf ); break;
  case globalSettings::MAPPING_NORMAL_DISTANCE: return map->dotAtNormalDist( Uf, Vf ); break;
  default: return map->dotAt( (u32)Uf, (u32)Vf ); break;
  }
}

void PigmentSpherical::print( const char *msg ) const {
  TPigment::print( msg );
  if ( map )
    map->print("map= ");
  else
    printf("NO MAP\n");
}
