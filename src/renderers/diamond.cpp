#include <stdio.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "diamond.h"


// Diamond
//  _X_    sample weight: 0.25
// |   |
// X   X
// |   |
//  -X-
bool RendererDiamond::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  u32 *dest = surface;

  u32 x = 0, y = 0;
  TVector3 upDir, rightDir, downDir;
  TColor upC, leftC;
  TColor *previousDown, previousRight;
  previousDown = new TColor [width];
  // FIRST DOT, FIRST LINE
  // we know nothing
  //  compute upC, leftC, rightC(->previousRight), downC(->previousDown[0])
  // up
  upDir = topLeft + ( rightStep * 0.5f );
  ray.direction = upDir; ray.direction.normalize();
  upC = world.traceRay( ray, 1.0f ); ray.clear();
  // left
  ray.direction = topLeft - ( upStep * 0.5f ); ray.direction.normalize();
  leftC = world.traceRay( ray, 1.0f ); ray.clear();
  // right
  rightDir = topLeft - ( upStep * 0.5f ) + ( rightStep * 1.0f );
  ray.direction = rightDir; ray.direction.normalize();
  previousRight = world.traceRay( ray, 1.0f ); ray.clear();
  // down
  downDir = topLeft - ( upStep * 1.0f ) + ( rightStep * 0.5f );
  ray.direction = downDir; ray.direction.normalize();
  previousDown[0] = world.traceRay( ray, 1.0f ); ray.clear();
  // write dest
  *(dest++) = clampRGB32( ( upC + leftC + previousRight + previousDown[0] ) * 0.25f );
  // FIRST LINE, MINUS FIRST DOT
  for ( x = 1; x < width; x++ ) {
    // we know leftC (previousRight)
    //  compute upC, rightC(->previousRight), downC(->previousDown[x])
    // up
    upDir = upDir + rightStep;
    ray.direction = upDir; ray.direction.normalize();
    upC = world.traceRay( ray, 1.0f ); ray.clear();
    // left
    leftC = previousRight;
    // right
    rightDir = rightDir + rightStep;
    ray.direction = rightDir; ray.direction.normalize();
    previousRight = world.traceRay( ray, 1.0f ); ray.clear();
    // down
    downDir = downDir + rightStep;
    ray.direction = downDir; ray.direction.normalize();
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
    ray.direction = topLeft - ( upStep * ((float)y+0.5f) ); ray.direction.normalize();
    leftC = world.traceRay( ray, 1.0f ); ray.clear();
    // right
    rightDir = topLeft - ( upStep * ((float)y+0.5f) ) + ( rightStep * 1.0f );
    ray.direction = rightDir; ray.direction.normalize();
    previousRight = world.traceRay( ray, 1.0f ); ray.clear();
    // down
    downDir = topLeft - ( upStep * ((float)y+1.0f) ) + ( rightStep * 0.5f );
    ray.direction = downDir; ray.direction.normalize();
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
      rightDir = rightDir + rightStep;
      ray.direction = rightDir; ray.direction.normalize();
      previousRight = world.traceRay( ray, 1.0f ); ray.clear();
      // down
      downDir = downDir + rightStep;
      ray.direction = downDir; ray.direction.normalize();
      previousDown[x] = world.traceRay( ray, 1.0f ); ray.clear();
      // write dest
      *(dest++) = clampRGB32( ( upC + leftC + previousRight + previousDown[x] ) * 0.25f );
    }
  }
  delete [] previousDown;

  return true;
}
