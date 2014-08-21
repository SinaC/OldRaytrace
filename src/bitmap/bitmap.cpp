#include <stdlib.h>
#include <stdio.h>

#include "bitmap.h"
#include "../color.h"
#include "../error.h"

//-- bitmap 32 bits
TBitmap::TBitmap() {
  data = NULL;
  palette = NULL;
}

TBitmap::~TBitmap() {
  if ( data )
    delete [] data;
  data = NULL;
  if ( palette )
    delete [] palette;
  palette = NULL;
}

u32 TBitmap::getPaletteColor( const u8 palIndex ) const {
  u8 r = palette[palIndex*3+0];
  u8 g = palette[palIndex*3+1];
  u8 b = palette[palIndex*3+2];
  return packRGB32( r, g, b );
}

void TBitmap::print( const char *msg ) const {
  printf("%s\n", msg );
  printf("width: %4d  height: %4d\n", width, height );
  printf("data: %x\n", (int)data );
  printf("palette: %x\n", (int)palette );
}

//-- bitmap float
const TTagList TBitmapColor::bitmapTags [] = {
  { "pcx", TAG_bitmap_pcx },
  { "tga", TAG_bitmap_tga },
  { "bmp", TAG_bitmap_bmp },
  { "width", TAG_bitmap_width },
  { "height", TAG_bitmap_height },

  { NULL, -1 }
};

TBitmapColor::TBitmapColor() {
  THandle::THandle(); handleKind = HANDLE_BITMAP;
  TBitmap::TBitmap();
  dataColor = NULL;
}

TBitmapColor::~TBitmapColor() {
  if ( dataColor )
    delete [] dataColor;
  dataColor = NULL;
}

void TBitmapColor::setup() {
  if ( dataColor != NULL )
    return;
  if ( data == NULL )
    g_error("TBitmapColor::setup: data is NULL\n");
  dataColor = new TColor [ width*height ];
  for ( u32 i = 0; i < width * height; i++ )
    dataColor[i] = packRGBColor( data[i] );
  delete [] data;
  data = NULL;
}

//--OLD WAY--TColor *TBitmapColor::getCorner( int x, int y ) const {
//--OLD WAY--  if ( x < 0 ) {
//--OLD WAY--#ifdef __DEBUG__
//--OLD WAY--    printf("x < 0   %d\n", x );
//--OLD WAY--#endif
//--OLD WAY--    x += width;
//--OLD WAY--  }
//--OLD WAY--  else if ( x >= width ) {
//--OLD WAY--#ifdef __DEBUG__
//--OLD WAY--    printf("x >= width   %d\n", x );
//--OLD WAY--#endif
//--OLD WAY--    x -= width;
//--OLD WAY--  }
//--OLD WAY--  if ( y < 0 ) {
//--OLD WAY--#ifdef __DEBUG__
//--OLD WAY--    printf("y < 0   %d\n", y );
//--OLD WAY--#endif
//--OLD WAY--    y += height;
//--OLD WAY--  }
//--OLD WAY--  else if ( y >= height ) {
//--OLD WAY--#ifdef __DEBUG__
//--OLD WAY--    printf("y >= height   %d\n", height );
//--OLD WAY--#endif
//--OLD WAY--    y -= height;
//--OLD WAY--  }
//--OLD WAY--  return dataColor+x+y*width;
//--OLD WAY--}

// we never call getCorner with x-1, x+1 and y+1
//  so we can remove some tests
TColor *TBitmapColor::getCorner( int x, int y ) const {
  if ( y < 0 ) {
#ifdef __DEBUG__
    printf("y < 0   %d\n", y );
#endif
    //y += height;
	y = 0;
  }
  return dataColor+x+y*width;
}

TColor TBitmapColor::dotAtBilinear( const float x, const float y ) const {
  TColor *corners[4];
  // get
  //     x+1, y    -> corners[0]     (!!don't go out of texture)
  //     x  , y    -> corners[1]
  //     x+1, y-1  -> corners[2]
  //     x  , y-1  -> corners[3]
  int xi = (u32) x;
  int yi = (u32) y;

  corners[0] = getCorner( xi+1, yi );
  corners[1] = getCorner( xi, yi );
  corners[2] = getCorner( xi+1, yi-1 );
  corners[3] = getCorner( xi, yi-1 );

  float p = x - (u32)x;
  float q = y - (u32)y;
  return
    ( ( *corners[0] * ( p * q ) ) +
      ( *corners[1] * ( q * ( 1 - p ) ) ) +
      ( *corners[2] * ( p * ( 1 - q ) ) ) +
      ( *corners[3] * ( ( 1 - q ) * ( 1 - p ) ) ) );
}

#define PYTHAGOREAN_SQ(a,b)  ( (a)*(a) + (b)*(b) )
TColor TBitmapColor::dotAtNormalDist( const float x, const float y ) const {
  TColor *corners[4];
  // get
  //     x  , y-1  -> corners[0]     (!!don't go out of texture)
  //     x+1, y-1  -> corners[1]
  //     x  , y    -> corners[2]
  //     x+1, y    -> corners[3]
  int xi = (int) x;
  int yi = (int) y;
  corners[0] = getCorner( xi, yi-1 );
  corners[1] = getCorner( xi+1, yi-1 );
  corners[2] = getCorner( xi, yi );
  corners[3] = getCorner( xi+1, yi );

  float p = x - (int)x;
  float q = y - (int)y;

  // ( a/b + c/d + e/f + g/h ) / ( 1/b + 1/d + 1/f + 1/h)
  // = ( a dfh + c bfh + e bdh + g bdf ) / ( dfh + bfh + bdh + bdf )
  float wts[4];
  wts[0] = PYTHAGOREAN_SQ( p, q );
  wts[1] = PYTHAGOREAN_SQ( 1-p, q );
  wts[2] = PYTHAGOREAN_SQ( p, 1-q );
  wts[3] = PYTHAGOREAN_SQ( 1-p, 1-q );
  float mulWts[4];
  mulWts[0] = wts[1] * wts[2] * wts[3];
  mulWts[1] = wts[0] * wts[2] * wts[3];
  mulWts[2] = wts[0] * wts[1] * wts[3];
  mulWts[3] = wts[0] * wts[1] * wts[2];
  TColor res = TColor::black;
  float sumMulWts = 0.0f;
  for ( int i = 0; i < 4; i++ ) {
    sumMulWts += mulWts[i];
    res += *corners[i] * mulWts[i];
  }
  return res * ( 1.0f / sumMulWts );
//--OLD WAY--  float invWts[4];
//--OLD WAY--  invWts[0] = 1.0f / PYTHAGOREAN_SQ( p, q );
//--OLD WAY--  invWts[1] = 1.0f / PYTHAGOREAN_SQ( 1-p, q );
//--OLD WAY--  invWts[2] = 1.0f / PYTHAGOREAN_SQ( p, 1-q );
//--OLD WAY--  invWts[3] = 1.0f / PYTHAGOREAN_SQ( 1-p, 1-q );
//--OLD WAY--  TColor res = TColor::black;
//--OLD WAY--  float sumInvWts = 0.0f;
//--OLD WAY--  for ( int i = 0; i < 4; i++ ) {
//--OLD WAY--    sumInvWts += invWts[i];
//--OLD WAY--    res += *corners[i] * invWts[i];
//--OLD WAY--  }
//--OLD WAY--  return res * ( 1.0f / sumInvWts );
}

void TBitmapColor::print( const char *msg ) const {
  THandle::print( msg );
  TBitmap::print( "bitmap:");
  printf("data color: %x\n", (int)dataColor );
}
