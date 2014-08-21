#include <stdio.h>

#include "math\math.h"
#include "colormap.h"

const TTagList TColorMap::colormapTags [] = {
  { "map", TAG_colormap_map },
  //  { "mapping", TAG_colormap_function },
  { NULL, -1 }
};

TColorMap::TColorMap() {
  THandle::THandle(); handleKind = HANDLE_COLORMAP;
  map = NULL;
}

void TColorMap::setup() {
  if ( nbItems == 0
       || map == NULL )
    error("Colormap without any elements!!\n");
  else {
    map[0].oneOverDiffPrevious = 0.0f;
    for ( int i = 1; i < nbItems; i++ )
      map[i].oneOverDiffPrevious = 1.0f / ( map[i].startValue - map[i-1].startValue );
  }
}

TColor TColorMap::getColor( const float value ) const {
  // find interval
  int current = -1;
  float valueLoop = 0.0f;
  for ( int i = 0; i < nbItems; i++ ) {
    if ( value >= valueLoop && value < map[i].startValue+EPS ) {
      current = i;
      break;
    }
    valueLoop = map[i].startValue;
  }

  // if current == -1: return map[nbItems-1].color
  // else if current == 0: return map[0].color;
  // else return interp map[current-1].color & map[current].color
  if ( current == 0 ) return map[0].color;
  else if ( current == -1 ) return map[nbItems-1].color;
  else {
    TColor diffC = map[current].color - map[current-1].color;
    float startV = value-map[current-1].startValue;
    float dV = startV * map[current].oneOverDiffPrevious;
    TColor resultColor = map[current-1].color + ( diffC * dV );
    return resultColor;
  }
}

void TColorMap::print( const char *msg ) const {
  THandle::print( msg );
  printf("colormap [%d]:\n", nbItems );
  for ( int i = 0; i < nbItems; i++ ) {
    printf("%5.2f)\n", map[i].startValue );
    map[i].color.print("color: ");
  }
}
