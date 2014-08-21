#include <stdio.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "flipquad.h"

// Flipquad
//  X- -X X-   sample weight: 0.25
// |  X  |  X
// X  |  X  |
//  -X X- -X
// X  |  X  |
// |  X  |  X
//  X- -X X-
bool RendererFlipquad::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  u32 *dest = surface;

  u32 x = 0, y = 0;
  TColor upC, leftC;
  TColor *previousDown, previousRight;
  previousDown = new TColor [width];
  // FIRST DOT, FIRST LINE
  // we know nothing
  //  compute upC, leftC, rightC(->previousRight), downC(->previousDown[0])
  // up
  computeDirectionUP( x, y, ray.direction ); ray.direction.normalize();
  upC = world.traceRay( ray, 1.0f ); ray.clear();
  // left
  computeDirectionLEFT( x, y, ray.direction ); ray.direction.normalize();
  leftC = world.traceRay( ray, 1.0f ); ray.clear();
  // right
  computeDirectionRIGHT( x, y, ray.direction ); ray.direction.normalize();
  previousRight = world.traceRay( ray, 1.0f ); ray.clear();
  // down
  computeDirectionDOWN( x, y, ray.direction ); ray.direction.normalize();
  previousDown[0] = world.traceRay( ray, 1.0f ); ray.clear();
  // write dest
  *(dest++) = clampRGB32( ( upC + leftC + previousRight + previousDown[0] ) * 0.25f );
  // FIRST LINE, MINUS FIRST DOT
  for ( x = 1; x < width; x++ ) {
    // we know leftC (previousRight)
    //  compute upC, rightC(->previousRight), downC(->previousDown[x])
    // up
    computeDirectionUP( x, y, ray.direction ); ray.direction.normalize();
    upC = world.traceRay( ray, 1.0f ); ray.clear();
    // left
    leftC = previousRight;
    // right
    computeDirectionRIGHT( x, y, ray.direction ); ray.direction.normalize();
    previousRight = world.traceRay( ray, 1.0f ); ray.clear();
    // down
    computeDirectionDOWN( x, y, ray.direction ); ray.direction.normalize();
    previousDown[x] = world.traceRay( ray, 1.0f ); ray.clear();
    // write dest
    *(dest++) = clampRGB32( ( upC + leftC + previousRight + previousDown[x] ) * 0.25f );
  }
  // REST OF SCREEN, MINUS FIRST LINE
  for ( y = 1; y < height; y++ ) {
    // FIRST DOT OF LINE
    // we know upC (previousDown[0])
    //  compute leftC, rightC(->previousRight), downC(->previousDown[0])
    // up
    upC = previousDown[0];
    // left
    computeDirectionLEFT( x, y, ray.direction ); ray.direction.normalize();
    leftC = world.traceRay( ray, 1.0f ); ray.clear();
    // right
    computeDirectionRIGHT( x, y, ray.direction ); ray.direction.normalize();
    previousRight = world.traceRay( ray, 1.0f ); ray.clear();
    // down
    computeDirectionDOWN( x, y, ray.direction ); ray.direction.normalize();
    previousDown[0] = world.traceRay( ray, 1.0f ); ray.clear();
    // write dest
    *(dest++) = clampRGB32( ( upC + leftC + previousRight + previousDown[0] ) * 0.25f );
    // LINE, MINUS FIRST DOT
    for ( x = 1; x < width; x++ ) {
      // we know leftC (previousRight), upC (previousDown[x])
      //  compute upC, rightC(->previousRight), downC(->previousDown[x])
      // up
      upC = previousDown[x];
      // left
      leftC = previousRight;
      // right
      computeDirectionRIGHT( x, y, ray.direction ); ray.direction.normalize();
      previousRight = world.traceRay( ray, 1.0f ); ray.clear();
      // down
      computeDirectionDOWN( x, y, ray.direction ); ray.direction.normalize();
      previousDown[x] = world.traceRay( ray, 1.0f ); ray.clear();
      // write dest
      *(dest++) = clampRGB32( ( upC + leftC + previousRight + previousDown[x] ) * 0.25f );
    }
  }
  delete [] previousDown;

  return true;
}
