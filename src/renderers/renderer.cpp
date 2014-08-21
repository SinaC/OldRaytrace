#include <stdio.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "renderer.h"

const TTagList TRenderer::rendererKindTags [] = {
  { "normal", RENDERER_NORMAL },
  { "bilinear", RENDERER_BILINEAR },
  { "quincunx", RENDERER_QUINCUNX },
  { "flipquad", RENDERER_FLIPQUAD },
  { "grid", RENDERER_GRID },
  { "corner", RENDERER_CORNER },
  { "diamond", RENDERER_DIAMOND },
  { "rgss2x2", RENDERER_RGSS2x2 },
  { "adaptative_color", RENDERER_QUINCUNX_ADAPTATIVE_COLOR },
  { "adaptative_primitive", RENDERER_QUINCUNX_ADAPTATIVE_PRIMITIVE },
  { "interpolated", RENDERER_INTERPOLATED },
  { NULL, -1 }
};


TRenderer::TRenderer() {
  surface = NULL;
  width = 0;
  height = 0;
}

void TRenderer::setup( u32 *dest, const u32 _width, const u32 _height ) {
  surface = dest;
  width = _width;
  height = _height;
}

void TRenderer::create( const u32 _width, const u32 _height ) {
  if ( surface != NULL )
    delete [] surface;
  width = _width;
  height = _height;
  surface = new u32 [width*height];
}

bool TRenderer::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  u32 *dest = surface;
  for ( u32 y = 0; y < height; y++ ) {
    TVector3 yScanline = topLeft - ( upStep * ((float)y+0.5f) ) + ( rightStep * 0.5f );
    for ( u32 x = 0; x < width; x++ ) {

      ray.clear();
      ray.direction = yScanline; ray.direction.normalize();
      yScanline += rightStep;

      TColor dotColor = world.traceRay( ray, 1.0f );

      *(dest++) = clampRGB32( dotColor );
	  //--*(dest++) = clampRGB32ExposureGamma( dotColor );
    }
  }

  return true;
}

bool TRenderer::render( const u32 resolutionWidth, const u32 resolutionHeight,
			const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( resolutionWidth, resolutionHeight, topLeft, rightStep, upStep );

  u32 *dest = surface;
  for ( u32 y = 0; y < resolutionHeight; y++ ) {
    TVector3 yScanline = topLeft - ( upStep * ((float)y+0.5f) ) + ( rightStep * 0.5f );
    for ( u32 x = 0; x < resolutionWidth; x++ ) {

      ray.clear();
      ray.direction = yScanline; ray.direction.normalize();
      yScanline += rightStep;

      TColor dotColor = world.traceRay( ray, 1.0f );

      *(dest++) = clampRGB32( dotColor );
    }
    dest += ( width - resolutionWidth );
  }

  return true;
}

void TRenderer::print( const char *msg ) const {
  printf("%s\n", msg );
  printf("width: %d\n"
	 "height: %d\n"
	 "surface: %x\n", width, height, (int)surface );
}
