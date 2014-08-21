#include <stdio.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "corner.h"

// Corner  (inverted bilinear)
// X--X    sample weight: 0.25
// |  |
// X--X
bool RendererCorner::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  u32 *dest = surface;

  u32 x, y;
  TVector3 topRightDir, bottomRightDir;
  TColor topLeftC;
  TColor *previousBottomLeft, *previousBottomRight, previousTopRight;
  previousBottomLeft = new TColor[width];
  previousBottomRight = new TColor[width];
  // FIRST DOT, FIRST LINE
  // we know nothing
  //  compute topLeft, topRight(->previousTopRight)
  //          bottomLeft(->previousBottomLeft[0]), bottomRight(->previousBottomRight[0])
  // top left
  ray.clear(); ray.direction = topLeft; ray.direction.normalize();
  topLeftC = world.traceRay( ray, 1.0f );
  // top right
  topRightDir = topLeft + ( rightStep * 1.0f );
  ray.clear(); ray.direction = topRightDir; ray.direction.normalize();
  previousTopRight = world.traceRay( ray, 1.0f );
  // bottom left
  ray.clear(); ray.direction = topLeft - ( upStep * 1.0f ); ray.direction.normalize();
  previousBottomLeft[0] = world.traceRay( ray, 1.0f );
  // bottom right
  bottomRightDir = topLeft - ( upStep * 1.0f ) + ( rightStep * 1.0f );
  ray.clear(); ray.direction = bottomRightDir; ray.direction.normalize();
  previousBottomRight[0] = world.traceRay( ray, 1.0f );
  // write dest
  *(dest++) = clampRGB32( ( topLeftC + previousTopRight + previousBottomLeft[0] + previousBottomRight[0] ) * 0.25f );
  // FIRST LINE, MINUS FIRST DOT
  for ( x = 1; x < width; x++ ) {
    // we know topLeft (previousTopRight) and bottomLeft (previousBottomRight[x-1])
    //  compute topRight(->previousTopRight) and bottomRight(->previousBottomRight[x])
    //  copy previousBottomRight[x-1](->previousBottomLeft[x])
    // top left
    topLeftC = previousTopRight;
    // top right
    topRightDir = topRightDir + rightStep;
    ray.clear(); ray.direction = topRightDir; ray.direction.normalize();
    previousTopRight = world.traceRay( ray, 1.0f );
    // bottom left
    previousBottomLeft[x] = previousBottomRight[x-1];
    // bottom right
    bottomRightDir = bottomRightDir + rightStep;
    ray.clear(); ray.direction = bottomRightDir; ray.direction.normalize();
    previousBottomRight[x] = world.traceRay( ray, 1.0f );
    // write dest
    *(dest++) = clampRGB32( ( topLeftC + previousTopRight + previousBottomLeft[x] + previousBottomRight[x] ) * 0.25f );
  }
  // REST OF SCREEN, MINUS FIRST LINE
  for ( y = 1; y < height; y++ ) {
    // FIRST DOT OF LINE
    // we know topLeft (previousBottomLeft[0]) and topRight (previousBottomRight[0])
    //  compute bottomLeft(->previousBottomLeft[0]), bottomRight(->previousBottomRight[0])
    // topLeft
    topLeftC = previousBottomLeft[0];
    // topRight
    previousTopRight = previousBottomLeft[0];
    // bottomLeft
    ray.clear(); ray.direction = topLeft - ( upStep * ( (float)y + 1.0f ) ); ray.direction.normalize();
    previousBottomLeft[0] = world.traceRay( ray, 1.0f );
    // bottomRight
    bottomRightDir = topLeft - ( upStep * ( (float)y + 1.0f ) ) + ( rightStep * 1.0f );
    ray.clear(); ray.direction = bottomRightDir; ray.direction.normalize();
    previousBottomRight[0] = world.traceRay( ray, 1.0f );
    // write dest
    *(dest++) = clampRGB32( ( topLeftC + previousTopRight + previousBottomLeft[0] + previousBottomRight[0] ) * 0.25f );
    // LINE, MINUS FIRST DOT
    for ( x = 1; x < width; x++ ) {
      // we know topLeft (previousBottomLeft[x]), topRight (previousBottomRight[x]),
      //         bottomLeft (previousBottomRight[x-1])
      //  compute bottomRight(->previousBottomRight[x])
      //  copy previousBottomRight[x-1](->previousBottomLeft[x])
      // topLeft
      topLeftC = previousBottomLeft[x];
      // topRight
      previousTopRight = previousBottomRight[x];
      // bottomLeft
      previousBottomLeft[x] = previousBottomRight[x-1];
      // bottomRight
      bottomRightDir = bottomRightDir + rightStep;
      ray.clear(); ray.direction = bottomRightDir; ray.direction.normalize();
      previousBottomRight[x] = world.traceRay( ray, 1.0f );
      // write dest
      *(dest++) = clampRGB32( ( topLeftC + previousTopRight + previousBottomLeft[x] + previousBottomRight[x] ) * 0.25f );
    }
  }

  delete [] previousBottomLeft;
  delete [] previousBottomRight;

  return true;
}
