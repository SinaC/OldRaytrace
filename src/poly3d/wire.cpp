#include <stdlib.h>

#include "wire.h"

void bresen( u32 *bitmap, const u32 width, const u32 height,
	     const TVector4 &p1, const TVector4 &p2, const TColor &c ) {
  int x1 = (int)p1[__X],
    y1 = (int)p1[__Y],
    x2 = (int)p2[__X],
    y2 = (int)p2[__Y];
  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;
  u32 col = clampRGB32( c );

  if ( abs( x2-x1 ) < abs( y2-y1 ) ) {
    if( y1 > y2 ) {
      swap( x1, x2 );
      swap( y1, y2 );
    }

    xincr = ( x2 > x1 ) ? 1 : -1;

    dy = y2-y1;
    dx = abs( x2-x1 );
    d = 2 * dx - dy;
    aincr = 2 * ( dx-dy );
    bincr = 2 * dx;

    x = x1;
    for( y = y1; y <= y2; ++y ) {
      if ( x >= 0 && x < (int)width
	   && y >= 0 && y < (int)height )
	bitmap[x+y*width] = col;
      if ( d >= 0 ) {
	x += xincr;
	d += aincr;
      }
      else
	d += bincr;
    }
  }
  else {
    if ( x1 > x2 ) {
      swap( x1, x2 );
      swap( y1, y2 );
    }
    yincr = ( y2 > y1 ) ? 1 : -1;

    dx = x2-x1;
    dy = abs( y2-y1 );
    d = 2 * dy - dx;
    aincr = 2 * ( dy-dx );
    bincr = 2 * dy;

    y=y1;
    for( x = x1; x <= x2; ++x ) {
      if ( x >= 0 && x < (int)width
	   && y >= 0 && y < (int)height )
	bitmap[x+y*width] = col;
      if ( d >= 0 ) {
	y += yincr;
	d += aincr;
      }
      else
	d += bincr;
    }
  }
}

void bresen( u32 *bitmap, const u32 width, const u32 height,
	     const u32 _x1, const u32 _y1, const u32 _x2, const u32 _y2, const u32 color ) {
  int x1 = _x1,
    y1 = _y1,
    x2 = _x2,
    y2 = _y2;
  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;

  if ( abs( x2-x1 ) < abs( y2-y1 ) ) {
    if( y1 > y2 ) {
      swap( x1, x2 );
      swap( y1, y2 );
    }

    xincr = ( x2 > x1 ) ? 1 : -1;

    dy = y2-y1;
    dx = abs( x2-x1 );
    d = 2 * dx - dy;
    aincr = 2 * ( dx-dy );
    bincr = 2 * dx;

    x = x1;
    for( y = y1; y <= y2; ++y ) {
      if ( x >= 0 && x < (int)width
	   && y >= 0 && y < (int)height )
	bitmap[x+y*width] = color;
      if ( d >= 0 ) {
	x += xincr;
	d += aincr;
      }
      else
	d += bincr;
    }
  }
  else {
    if ( x1 > x2 ) {
      swap( x1, x2 );
      swap( y1, y2 );
    }
    yincr = ( y2 > y1 ) ? 1 : -1;

    dx = x2-x1;
    dy = abs( y2-y1 );
    d = 2 * dy - dx;
    aincr = 2 * ( dy-dx );
    bincr = 2 * dy;

    y=y1;
    for( x = x1; x <= x2; ++x ) {
      if ( x >= 0 && x < (int)width
	   && y >= 0 && y < (int)height )
	bitmap[x+y*width] = color;
      if ( d >= 0 ) {
	y += yincr;
	d += aincr;
      }
      else
	d += bincr;
    }
  }
}
