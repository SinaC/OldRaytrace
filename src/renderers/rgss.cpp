#include <stdio.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "rgss.h"

// RGSS2x2
// _______
//| | |X| |  sample weight: 0.25
// -------
//|X| | | |
// -------
//| | | |X|
// -------
//| |X| | |
// -------
bool RendererRGSS::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  u32 *dest = surface;
  for ( u32 y = 0; y < height; y++ ) {
    //   1     0.6, 0.2
    // 2       0.2, 0.4
    //    3    0.8, 0.6
    //  4      0.4, 0.8
    TVector3 yScanline1 = topLeft - ( upStep * ((float)y+0.2f) ) + ( rightStep * 0.6f );
    TVector3 yScanline2 = topLeft - ( upStep * ((float)y+0.4f) ) + ( rightStep * 0.2f );
    TVector3 yScanline3 = topLeft - ( upStep * ((float)y+0.6f) ) + ( rightStep * 0.8f );
    TVector3 yScanline4 = topLeft - ( upStep * ((float)y+0.8f) ) + ( rightStep * 0.4f );
    for ( u32 x = 0; x < width; x++ ) {

      TColor dotColor = TColor::black;
      ray.clear(); ray.direction = yScanline1; ray.direction.normalize();
      dotColor += world.traceRay( ray, 1.0f ) * 0.25f;
      ray.clear(); ray.direction = yScanline2; ray.direction.normalize();
      dotColor += world.traceRay( ray, 1.0f ) * 0.25f;
      ray.clear(); ray.direction = yScanline3; ray.direction.normalize();
      dotColor += world.traceRay( ray, 1.0f ) * 0.25f;
      ray.clear(); ray.direction = yScanline4; ray.direction.normalize();
      dotColor += world.traceRay( ray, 1.0f ) * 0.25f;

      *(dest++) = clampRGB32( dotColor );

      yScanline1 += rightStep;
      yScanline2 += rightStep;
      yScanline3 += rightStep;
      yScanline4 += rightStep;
    }
  }

  return true;
}
