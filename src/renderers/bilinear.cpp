#include <stdio.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "bilinear.h"

//--#define __DEBUG__

RendererBilinear::RendererBilinear() {
  buffer = NULL;
}

RendererBilinear::~RendererBilinear() {
  if ( buffer != NULL )
    delete [] buffer;
}

void RendererBilinear::setup( u32 *dest, const u32 _width, const u32 _height ) {
  surface = dest;
  width = _width;
  height = _height;
  resolutionWidth = width / 2;
  resolutionHeight = height / 2;
  if ( buffer != NULL )
    delete [] buffer;
  buffer = new u32 [resolutionHeight * resolutionWidth ];
}

bool RendererBilinear::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;

  camera->computeDirectionVector( resolutionWidth, resolutionHeight,
				  topLeft, rightStep, upStep );
  u32 x, y;
  u32 *dest = buffer;
  for ( y = 0; y < resolutionHeight; y++ ) {
    TVector3 yScanline = topLeft - ( upStep * ((float)y+0.5f) ) + ( rightStep * 0.5f );
    for ( x = 0; x < resolutionWidth; x++ ) {
      //--      ray.clear();
      //--      ray.direction = yScanline; ray.direction.normalize();
      yScanline += rightStep;

#ifdef __DEBUG__
      if ( !( x == resolutionWidth / 2
	      &&  y == resolutionHeight / 2 ) ) {
	*(dest++) = 0x000000FF;
	continue;
      }
      printf("middle:   %d\n", (int)(dest-buffer) );
#endif

      //--      TColor dotColor = world.traceRay( ray, 1.0f );
      TColor dotColor = world.renderRay( camera->location, yScanline );

      dotColor.print("dotColor: ");
      printf("clamped color: %x\n", clampRGB32( dotColor ) );

//--      for ( int j = 0; j < 10; j++ )
//--	for ( int i = 0; i < 10; i++ )
//--	  *(dest+i+j*resolutionWidth) = 0x00FF00FF;

      *(dest++) = clampRGB32( dotColor );
    }
  }

  // from data to buf
  // data 
  //  1 2 3
  //  4 5 6
  //  7 8 9
  // buf
  //  1  1     12  2     23  3
  //  1  1     12  2     23  3
  //
  // 14 14   1245 25   2356 36
  //  4  4     45  5     56  6
  //
  // 47 47   4578 58   5689 69
  //  7  7     78  8     89  9

  u32 c, averageX, averageY, c1, c2, c3;
  u32 *sourcePreviousLine;
  u32 *source = buffer;
  u32 diffWidth = ( width - resolutionWidth );
  dest = surface;
  u32 *destNextLine = dest + width;
  diffWidth *= 2;
  // FIRST LINE
  // FIRST DOT
  c = *source;
  *dest = c;         *(dest+1) = c;
  *destNextLine = c; *(destNextLine+1) = c;
  sourcePreviousLine = source;
  source++;
  dest+=2; destNextLine+=2;
  // REST OF FIRST LINE
  for ( x = 1; x < resolutionWidth; x++ ) {
    c = *source;
    c1 = *(source-1);
    averageX = blend2RGB( c, c1 );
    *dest = averageX;         *(dest+1) = c;
    *destNextLine = averageX; *(destNextLine+1) = c;
    source++;
    dest+=2; destNextLine+=2;
  }
  dest+=diffWidth; destNextLine+=diffWidth;
  // REST OF SCREEN
  for ( y = 1; y < resolutionHeight; y++ ) {
    // FIRST DOT OF EACH LINE
    c = *source;
    c1 = *sourcePreviousLine;
    averageY = blend2RGB( c, c1 );
    *dest = averageY; *(dest+1) = averageY;
    *destNextLine = c; *(destNextLine+1) = c;
    sourcePreviousLine++;
    source++;
    dest+=2; destNextLine+=2;
    // REST OF LINE
    for ( x = 1; x < resolutionWidth; x++ ) {
      c = *source; // DotI
      c1 = *(source-1); // PreviousDotI
      c2 = *(sourcePreviousLine); // PreviousLine
      c3 = *(sourcePreviousLine-1); // PreviousLine-1
      *dest = blend4RGB( c, c1, c2, c3 ); *(dest+1) = blend2RGB( c, c2 );
      *destNextLine = blend2RGB( c, c1 ); *(destNextLine+1) = c;
      
      sourcePreviousLine++;
      source++;
      dest+=2; destNextLine+=2;
    }
    dest+=diffWidth; destNextLine+=diffWidth;
  }

  return true;
}
