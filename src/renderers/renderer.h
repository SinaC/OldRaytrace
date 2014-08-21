#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "../math/vector3.h"
#include "../types/basics.h"
#include "../color.h"
#include "../parser/tag.h"

struct TWorld;
struct TCamera;

struct TRenderer {
  //----- Datas
  static const TTagList rendererKindTags [];
  static const enum ERendererKind {
    RENDERER_NORMAL,
    RENDERER_BILINEAR,
    RENDERER_QUINCUNX,
    RENDERER_FLIPQUAD,
    RENDERER_GRID,
    RENDERER_CORNER,
    RENDERER_DIAMOND,
    RENDERER_RGSS2x2,
    RENDERER_QUINCUNX_ADAPTATIVE_COLOR,
    RENDERER_QUINCUNX_ADAPTATIVE_PRIMITIVE,
    RENDERER_INTERPOLATED,
    RENDERER_MAX
  };
  u32 *surface;
  u32 width, height;

  //----- Constructor
  TRenderer();

  //----- Methods
  void setup( u32 *dest, const u32 width, const u32 height );
  void create( const u32 width, const u32 height );

  bool render( const TWorld &world );
  bool render( const u32 resolutionWidth, const u32 resolutionHeight,
	       const TWorld &world );
  
  //----- Output
  void print( const char *msg ) const;

  //***** Protected
  protected:
  TVector3 topLeft, rightStep, upStep;
};

// fill a square with a unique color
inline void fillSquareUniform( u32 *vs, const u32 width, const u32 height,
			       const u32 x, const u32 y, const u32 step, const TColor &c ) {
  u32 col = clampRGB32( c );
  u32 *dest = vs + x + y * width;
  for ( u32 yi = 0; yi < step; yi++ ) {
    for ( u32 xi = 0; xi < step; xi++ )
      *(dest++) = col;
    dest += width - step;
  }
}

// fill a square blending corner's color
// 1---3
// |   |
// 2---4
inline void drawBlock( u32 *surface, const u32 width, const u32 height,
		       const u32 x, const u32 y, const u32 step,
		       const u32 c1, const u32 c2, const u32 c3, const u32 c4 ) {
  switch( step ) {
  case 1: {
    u32 *dest = surface + x + y * width;
    (*dest) = blend4RGB( c1, c2, c3, c4 );
    break;
  }
  case 2: {
    u32 *dest = surface + x + y * width;
    for ( u32 v = 0; v < 2; v++ ) {
      u32 left = blend2( c1, c3, v );
      u32 right = blend2( c2, c4, v );
      
      for ( u32 u = 0; u < 2; u++ )
	*(dest++) = blend2( left, right, u );
      dest += width - 2;
    }
    break;
  }
  case 4: {
    u32 *dest = surface + x + y * width;
    for ( u32 v = 0; v < 4; v++ ) {
      u32 left = blend4( c1, c3, v );
      u32 right = blend4( c2, c4, v );
      
      for ( u32 u = 0; u < 4; u++ )
	*(dest++) = blend4( left, right, u );
      dest += width - 4;
    }
    break;
  }
  case 8: {
    u32 *dest = surface + x + y * width;
    for ( u32 v = 0; v < 8; v++ ) {
      u32 left = blend8( c1, c3, v );
      u32 right = blend8( c2, c4, v );
      
      for ( u32 u = 0; u < 8; u++ )
	*(dest++) = blend8( left, right, u );
      dest += width - 8;
    }
    break;
  }
  case 16: {
    u32 *dest = surface + x + y * width;
    for ( u32 v = 0; v < 16; v++ ) {
      u32 left = blend16( c1, c3, v );
      u32 right = blend16( c2, c4, v );
      
      for ( u32 u = 0; u < 16; u++ )
	*(dest++) = blend16( left, right, u );
      dest += width - 16;
    }
    break;
  }
  default: {
    u32 col = blend4RGB( c1, c2, c3, c4 );
    u32 *dest = surface + x + y * width;
    for ( u32 v = 0; v < step; v++ ) {
      for ( u32 u = 0; u < step; u++ )
	*(dest++) = col;
      dest += width - step;
    }
    break;
  }
  }
}

#endif
