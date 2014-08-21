#include <stdio.h>

#include "skymap.h"
#include "../bitmap/bitmap.h"
#include "../globals.h"

const TTagList PigmentSkymap::pigmentSkymapTags [] = {
  { "map", TAG_pigmentSkymap_map },

  { NULL, -1 }
};

PigmentSkymap::PigmentSkymap() {
  TPigment::TPigment(); pigmentKind = PIGMENT_SKYMAP;
  map = NULL;
}

PigmentSkymap::~PigmentSkymap() {
  map = NULL;
}

void PigmentSkymap::setup() {
  if ( map != NULL )
    map->setup();
}

TColor PigmentSkymap::getColor( const TVector3 &p ) const {
  if ( map != NULL ) {
    TVector3 vector = p - anchor;
    vector.normalize();

    float Vf = (1.0f-vector[__Y]) * (float)(map->height-1);
    Vf = RANGE( 0.0f, Vf, (float)(map->height-1) );
    //--int y = (int)((1.0f-X[1]) * map->getHeight());

    //--y = RANGE( 0, y, map->getHeight()-1 );

    // [-1/scale, 1/scale] * scale + 1.0f ==> [0, 2]
    float a = atan2f( vector[__X], vector[__Z] );

    // a in [-pi, pi]
    //--int x = (int)(( a + M_PI ) / ( 2.0f * M_PI )* map->getWidth() );
    float Uf = ( a + M_PI ) * 0.5f * M_ONE_OVER_PI * (float)(map->width-1);
    
    switch ( globalSettings::whichMapping ) {
    case globalSettings::MAPPING_NORMAL: return map->dotAt( (u32)Uf, (u32)Vf ); break;
    case globalSettings::MAPPING_BILINEAR: return map->dotAtBilinear( Uf, Vf ); break;
    case globalSettings::MAPPING_NORMAL_DISTANCE: return map->dotAtNormalDist( Uf, Vf ); break;
    default: return map->dotAt( (u32)Uf, (u32)Vf ); break;
    }
  }
  else {
    float r,g,b;
    b = 0.4f*(1.0f-p[__Y])*1.3f+0.2f;
    r = b*(1.0f-p[__Y])*0.5f;
    g = 0.05f*(1.0f-p[__Y]);
    
    return TColor( r, g, b );
  }
  return TColor::black;
}

void PigmentSkymap::print( const char *msg ) const {
  TPigment::print( msg );
  if ( map )
    map->print("map= ");
  else
    printf("NO MAP\n");
}
