#include <stdio.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "interpolated.h"

RendererInterpolated::RendererInterpolated() {
  vertex = NULL;
  block = NULL;
}

RendererInterpolated::~RendererInterpolated() {
  if ( vertex != NULL )
    delete [] vertex;
  vertex = NULL;
  if ( block != NULL )
    delete [] block;
  block = NULL;
}


void RendererInterpolated::setup( u32 *dest, const u32 _width, const u32 _height, const u32 _step ) {
  surface = dest;
  width = _width;
  height = _height;
  step = _step;
  xBlocks = width / step;
  yBlocks = height / step;
  if ( vertex != NULL )
    delete [] vertex;
  if ( block != NULL )
    delete [] block;
  u32 x, y;
  vertex = new TVertex[ (yBlocks + 1) * (xBlocks + 1) ];
  block = new TBlock[ yBlocks * xBlocks ];
  for ( y = 0; y < yBlocks; y++)
    for ( x = 0; x < xBlocks; x++) {
      block[y * xBlocks + x].vertex[0] = &vertex[y * (xBlocks + 1) + x];
      block[y * xBlocks + x].vertex[1] = &vertex[y * (xBlocks + 1) + x + 1];
      block[y * xBlocks + x].vertex[2] = &vertex[(y + 1) * (xBlocks + 1) + x];
      block[y * xBlocks + x].vertex[3] = &vertex[(y + 1) * (xBlocks + 1) + x + 1];
    }
}

bool RendererInterpolated::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  TVector3 bigRightStep = rightStep * (float)step;
  TVector3 bigUpStep = upStep * (float)step;

  u32 x, y;
  // compute color/id at block corners
  for ( y = 0; y < yBlocks + 1; y++ ) {
      TVector3 yScanline = topLeft - ( bigUpStep * (float)y );
      for ( x = 0; x < xBlocks + 1; x++ ) {
	ray.clear();
	ray.direction = yScanline; ray.direction.normalize();
	yScanline += bigRightStep;
	
	world.traceRayNoShading( ray, 1.0f );
	vertex[ y * (xBlocks + 1) + x ].id = ray.id;
	vertex[ y * (xBlocks + 1) + x ].color = clampRGB32( world.shadeRay( ray ) );
      }
  }

  // use color and id to fill block
  u32 *dest = surface;
  for ( y = 0; y < yBlocks; y++ ) {
    for ( x = 0; x < xBlocks; x++ ) {
      u32 i = x * step,
	j = y * step;
      TBlock *b = &block[y * xBlocks + x];
      
      if ((b->vertex[0]->id == b->vertex[1]->id) &
	  (b->vertex[1]->id == b->vertex[2]->id) &
	  (b->vertex[2]->id == b->vertex[3]->id)) {
	drawBlock( dest, width, height,
		   i, j, step,
		   b->vertex[0]->color, b->vertex[1]->color,
		   b->vertex[2]->color, b->vertex[3]->color );
      }
      else {
	TVector3 startPoint = topLeft - ( upStep * (float)j ) + ( rightStep * (float)i );
	for ( u32 v = j; v < j + step; v++ ) {
	  TVector3 scanLine = startPoint;
	  for ( u32 u = i; u < i + step; u++ ) {
	    ray.clear();
	    ray.direction = scanLine; ray.direction.normalize();
	    scanLine += rightStep;
	    *(dest+v*width+u) = clampRGB32( world.traceRay( ray, 1.0f ) );
	  }
	  startPoint -= upStep;
	}
      }
    }
  }
  return true;
}
