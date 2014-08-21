#include <stdio.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "grid.h"

RendererGrid::RendererGrid() {
  n = 2;
  weight = 1.0f/(float)(n*n);
  yScanline = new TVector3[n*n];
}

RendererGrid::~RendererGrid() {
  if ( yScanline != NULL )
    delete [] yScanline;
}

void RendererGrid::setup( u32 *data, const u32 _width, const u32 _height, const u32 _n ) {
  surface = data;
  width = _width;
  height = _height;
  n = _n;
  weight = 1.0f/(float)(n);
  if ( yScanline != NULL )
    delete [] yScanline;
  yScanline = new TVector3[n*n];
}

// Grid NxN
// _____
//|X|X|X|   sample weight: 1/(n*n)
// -----
//|X|X|X|   example with N = 3
// -----
//|X|X|X|
// -----
bool RendererGrid::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  u32 *dest = surface;
  int i, j;
  for ( u32 y = 0; y < height; y++ ) {
    for ( j = 0; j < n; j++ )
      for ( i = 0; i < n; i++ )
	yScanline[i+j*n] =
	  topLeft
	  - ( upStep * ((float)y+(float)(j+1)/(float)(n+1)) )
	  + ( rightStep * ((float)(i+1)/(float)(n+1)) );
    for ( u32 x = 0; x < width; x++ ) {
      TColor dotColor = TColor::black;
      for ( i = 0; i < n*n; i++ ) {
	ray.clear(); ray.direction = yScanline[i]; ray.direction.normalize();
	dotColor += world.traceRay( ray, 1.0f );
      }

      *(dest++) = clampRGB32( dotColor * weight );

      for ( i = 0; i < n*n; i++ )
	yScanline[i] += rightStep;
    }
  }

  return true;
}
