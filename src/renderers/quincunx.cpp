#include <stdio.h>
#include <string.h>

#include "../world.h"
#include "../color.h"
#include "../ray.h"
#include "../cameras/camera.h"
#include "../math/math.h"
#include "quincunx.h"

// Quincunx
// X---X   corner weight: 0.125
// |   |   center weight: 0.5
// | X |
// |   |
// X---X
bool RendererQuincunx::render( const TWorld &world ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  u32 *dest = surface;

  u32 x, y;
  TColor topLeftC, centerC;
  TColor *previousBottomLeft, *previousBottomRight, previousTopRight;
  previousBottomLeft = new TColor[width];
  previousBottomRight = new TColor[width];
  // FIRST DOT, FIRST LINE
  // we know nothing
  //  compute topLeft, topRight(->previousTopRight), center,
  //          bottomLeft(->previousBottomLeft[0]), bottomRight(->previousBottomRight[0])
  // top left
  ray.clear(); ray.direction = topLeft; ray.direction.normalize();
  topLeftC = world.traceRay( ray, 1.0f );
  // top right
  ray.clear(); ray.direction = topLeft + ( rightStep * 1.0f ); ray.direction.normalize();
  previousTopRight = world.traceRay( ray, 1.0f );
  // center
  ray.clear(); ray.direction = topLeft - ( upStep * 0.5f ) + ( rightStep * 0.5f ); ray.direction.normalize();
  centerC = world.traceRay( ray, 1.0f );
  // bottom left
  ray.clear(); ray.direction = topLeft - ( upStep * 1.0f ); ray.direction.normalize();
  previousBottomLeft[0] = world.traceRay( ray, 1.0f );
  // bottom right
  ray.clear(); ray.direction = topLeft - ( upStep * 1.0f ) + ( rightStep * 1.0f ); ray.direction.normalize();
  previousBottomRight[0] = world.traceRay( ray, 1.0f );
  // write dest
  *(dest++) = clampRGB32( ( topLeftC + previousTopRight + previousBottomLeft[0] + previousBottomRight[0] ) * 0.125f
			  + centerC * 0.5f );
  // FIRST LINE, MINUS FIRST DOT
  for ( x = 1; x < width; x++ ) {
    // we know topLeft (previousTopRight) and bottomLeft (previousBottomRight[x-1])
    //  compute topRight(->previousTopRight), center and bottomRight(->previousBottomRight[x])
    //  copy previousBottomRight[x-1](->previousBottomLeft[x])
    // top left
    topLeftC = previousTopRight;
    // top right
    ray.clear(); ray.direction = topLeft + ( rightStep * (float)x ); ray.direction.normalize();
    previousTopRight = world.traceRay( ray, 1.0f );
    // center
    ray.clear(); ray.direction = topLeft - ( upStep * 0.5f ) + ( rightStep * ( (float)x+0.5f) ); ray.direction.normalize();
    centerC = world.traceRay( ray, 1.0f );
    // bottom left
    previousBottomLeft[x] = previousBottomRight[x-1];
    // bottom right
    ray.clear(); ray.direction = topLeft - ( upStep * 1.0f ) + ( rightStep * ((float)x+1.0f) ); ray.direction.normalize();
    previousBottomRight[x] = world.traceRay( ray, 1.0f );
    // write dest
    *(dest++) = clampRGB32( ( topLeftC + previousTopRight + previousBottomLeft[x] + previousBottomRight[x] ) * 0.125f
			    + centerC * 0.5f );
  }
  // REST OF SCREEN, MINUS FIRST LINE
  for ( y = 1; y < height; y++ ) {
    // FIRST DOT OF LINE
    // we know topLeft (previousBottomLeft[0]) and topRight (previousBottomRight[0])
    //  compute center, bottomLeft(->previousBottomLeft[0]), bottomRight(->previousBottomRight[0])
    // topLeft
    topLeftC = previousBottomLeft[0];
    // topRight
    previousTopRight = previousBottomLeft[0];
    // center
    ray.clear(); ray.direction = topLeft - ( upStep * ( (float)y + 0.5f ) ) + ( rightStep * ( (float)x+0.5f) ); ray.direction.normalize();
    centerC = world.traceRay( ray, 1.0f );
    // bottomLeft
    ray.clear(); ray.direction = topLeft - ( upStep * ( (float)y + 1.0f ) ); ray.direction.normalize();
    previousBottomLeft[0] = world.traceRay( ray, 1.0f );
    // bottomRight
    ray.clear(); ray.direction = topLeft - ( upStep * ( (float)y + 1.0f ) ) + ( rightStep * 1.0f ); ray.direction.normalize();
    previousBottomRight[0] = world.traceRay( ray, 1.0f );
    // write dest
    *(dest++) = clampRGB32( ( topLeftC + previousTopRight + previousBottomLeft[0] + previousBottomRight[0] ) * 0.125f
			    + centerC * 0.5f );
    // LINE, MINUS FIRST DOT
    for ( x = 1; x < width; x++ ) {
      // we know topLeft (previousBottomLeft[x]), topRight (previousBottomRight[x]),
      //         bottomLeft (previousBottomRight[x-1])
      //  compute center and bottomRight(->previousBottomRight[x])
      //  copy previousBottomRight[x-1](->previousBottomLeft[x])
      // topLeft
      topLeftC = previousBottomLeft[x];
      // topRight
      previousTopRight = previousBottomRight[x];
      // center
      ray.clear(); ray.direction = topLeft - ( upStep * ( (float)y+0.5f ) ) + ( rightStep * ( (float)x+0.5f) ); ray.direction.normalize();
      centerC = world.traceRay( ray, 1.0f );
      // bottomLeft
      previousBottomLeft[x] = previousBottomRight[x-1];
      // bottomRight
      ray.clear(); ray.direction = topLeft - ( upStep * ( (float)y+1.0f ) ) + ( rightStep * ((float)x+1.0f) ); ray.direction.normalize();
      previousBottomRight[x] = world.traceRay( ray, 1.0f );
      // write dest
      *(dest++) = clampRGB32( ( topLeftC + previousTopRight + previousBottomLeft[x] + previousBottomRight[x] ) * 0.125f
			      + centerC * 0.5f );
    }
  }

  delete [] previousBottomLeft;
  delete [] previousBottomRight;

  return true;
}

// Adaptative Quincunx
//  when a center and a corner color are not the "same", we subdivide resulting square
//  into 4 sub-squares and redo the same process
// 1-------2-------1
// |       |   B   |
// |   A   |   2---3
// |       |   | 3 |
// |       1---3---2
// |               |
// |   C       D   |
// |               |
// 1---------------1
const u32 minStep = 1;
//--------
//-- COLOR

// Up Left Square
// UL--
// |   |
//  ---DR
void RendererQuincunx::fillSquareColorUL( TRay &ray, const TWorld &world,
					  const u32 x, const u32 y, const u32 step,
					  const TColor &colorUL, const TColor &colorDR ) const {
  // if the two points has the same color, fill the square
  if ( sameColor( colorUL, colorDR ) ) {
    fillSquareUniform( surface, width, height, x, y, step, colorUL );
    return;
  }
  // else split square in 4 sub-squares
  // center
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); ray.direction.normalize();
  TColor colorC = world.traceRay( ray, 1.0f );
  // up right
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x+step) ); ray.direction.normalize();
  TColor colorUR = world.traceRay( ray, 1.0f );
  // down left
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x) ); ray.direction.normalize();
  TColor colorDL = world.traceRay( ray, 1.0f );
  // recursive call
  fillSquareColor( ray, world, x, y, step/2,
		   colorUL, colorUR, colorC, colorDL, colorDR );
}
// Up Right Square
//  ---UR
// |   |
// DL--
void RendererQuincunx::fillSquareColorUR( TRay &ray, const TWorld &world,
					  const u32 x, const u32 y, const u32 step,
					  const TColor &colorDL, const TColor &colorUR ) const {
  // if the two points has the same color, fill the square
  if ( sameColor( colorDL, colorUR ) ) {
    fillSquareUniform( surface, width, height, x, y, step, colorDL );
    return;
  }
  // else split square in 4 sub-squares
  // center
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); ray.direction.normalize();
  TColor colorC = world.traceRay( ray, 1.0f );
  // up left
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x) ); ray.direction.normalize();
  TColor colorUL = world.traceRay( ray, 1.0f );
  // down right
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x+step) ); ray.direction.normalize();
  TColor colorDR = world.traceRay( ray, 1.0f );
  // recursive call
  fillSquareColor( ray, world, x, y, step/2,
		   colorUL, colorUR, colorC, colorDL, colorDR );
}
// Down Left Square
//  ---UR
// |   |
// DL--
void RendererQuincunx::fillSquareColorDL( TRay &ray, const TWorld &world,
					  const u32 x, const u32 y, const u32 step,
					  const TColor &colorDL, const TColor &colorUR ) const {
  // if the two points has the same color, fill the square
  if ( sameColor( colorDL, colorUR ) ) {
    fillSquareUniform( surface, width, height, x, y, step, colorDL );
    return;
  }
  // else split square in 4 sub-squares
  // center
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); ray.direction.normalize();
  TColor colorC = world.traceRay( ray, 1.0f );
  // up left
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x) ); ray.direction.normalize();
  TColor colorUL = world.traceRay( ray, 1.0f );
  // down right
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x+step) ); ray.direction.normalize();
  TColor colorDR = world.traceRay( ray, 1.0f );
  // recursive call
  fillSquareColor( ray, world, x, y, step/2,
		   colorUL, colorUR, colorC, colorDL, colorDR );
}
// Down Right Square
// UL--
// |   |
//  ---DR
void RendererQuincunx::fillSquareColorDR( TRay &ray, const TWorld &world,
					  const u32 x, const u32 y, const u32 step,
					  const TColor &colorUL, const TColor &colorDR ) const {
  // if the two points has the same color, fill the square
  if ( sameColor( colorUL, colorDR ) ) {
    fillSquareUniform( surface, width, height, x, y, step, colorUL );
    return;
  }
  // else split square in 4 sub-squares
  // center
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); ray.direction.normalize();
  TColor colorC = world.traceRay( ray, 1.0f );
  // up right
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x+step) ); ray.direction.normalize();
  TColor colorUR = world.traceRay( ray, 1.0f );
  // down left
  ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x) ); ray.direction.normalize();
  TColor colorDL = world.traceRay( ray, 1.0f );
  // recursive call
  fillSquareColor( ray, world, x, y, step/2,
		   colorUL, colorUR, colorC, colorDL, colorDR );
}
void RendererQuincunx::fillSquareColor( TRay &ray, const TWorld &world,
					const u32 x, const u32 y, const u32 step,
					const TColor &colorUL, const TColor &colorUR,
					const TColor &colorC,
					const TColor &colorDL, const TColor &colorDR ) const {
  if ( step == minStep ) {
    u32 *dest = surface + x + y * width;
    *(dest) = clampRGB32( colorUL );
    *(dest+1) = clampRGB32( colorUR );
    *(dest+width) = clampRGB32( colorDL );
    *(dest+width+1) = clampRGB32( colorDR );
    return;
  }
  fillSquareColorUL( ray, world, x, y, step,
		     colorUL, colorC );
  fillSquareColorUR( ray, world, x+step, y, step,
		     colorC, colorUR );
  fillSquareColorDL( ray, world, x, y+step, step,
		     colorDL, colorC );
  fillSquareColorDR( ray, world, x+step, y+step, step,
		     colorC, colorDR );
}

bool RendererQuincunx::renderAdaptativeColor( const TWorld &world, const u32 step ) {
  TCamera *camera = world.currentCamera;
  TRay ray;
  ray.origin = camera->location;
  ray.kind = TRay::RAY_CAMERA;

  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  for ( u32 y = 0; y < height; y += step )
    for ( u32 x = 0; x < width; x += step ) {
      // Up Left
      ray.clear(); ray.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x) ); ray.direction.normalize();
      TColor colorUL = world.traceRay( ray, 1.0f );
      // Up Right
      ray.clear(); ray.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x+step) ); ray.direction.normalize();
      TColor colorUR = world.traceRay( ray, 1.0f );
      // Center
      ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); ray.direction.normalize();
      TColor colorC = world.traceRay( ray, 1.0f );
      // Down Left
      ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x) ); ray.direction.normalize();
      TColor colorDL = world.traceRay( ray, 1.0f );
      // Down Right
      ray.clear(); ray.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x+step) ); ray.direction.normalize();
      TColor colorDR = world.traceRay( ray, 1.0f );
      // fill squares
      fillSquareColor( ray, world, x, y, step/2,
		       colorUL, colorUR, colorC, colorDL, colorDR );
    }
  return true;
}

//------------
//-- PRIMITIVE
// UL--
// |   |
//  ---DR
bool RendererQuincunx::compareIntersectionsUL_DR( const TWorld &world,
						  const u32 x, const u32 y, const u32 step,
						  const TRay &r1, const TRay &r2 ) const {
  if ( r1.id != r2.id ) // different id -> different intersection or different lighting
    if ( ( r1.id & 15 ) == ( r2.id & 15 ) ) { // same intersection but different lighting
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::blue );
      return false;
    }
    else {
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::red ); 
      return false;
   }
  else
    if ( r1.id == 0 ) { // background (no intersection)
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::magenta * (2.0f/(float)step) );
      TColor c1 = world.shadeRay( r1 );
      TColor c2 = world.shadeRay( r2 );
      fillSquareUniform( surface, width, height, x, y, step, (c1+c2) * 0.5f );
    }
    else { // same intersection and same lighting
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::green * (2.0f/(float)step) );
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::blue );
      //--return true;

      u32 c1 = clampRGB32( world.shadeRay( r1 ) );
      u32 c4 = clampRGB32( world.shadeRay( r2 ) );

      TRay rayUR; rayUR.origin = r1.origin; rayUR.kind = TRay::RAY_CAMERA;
      rayUR.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x+step) ); rayUR.direction.normalize();
      world.traceRayNoShading( rayUR, 1.0f );
      u32 c2 = clampRGB32( world.shadeRay( rayUR ) );

      TRay rayDL; rayDL.origin = r1.origin; rayDL.kind = TRay::RAY_CAMERA;
      rayDL.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x) ); rayDL.direction.normalize();
      world.traceRayNoShading( rayDL, 1.0f );
      u32 c3 = clampRGB32( world.shadeRay( rayDL ) );

      //--drawBlock( surface, width, height, x, y, step, c1, c2, c3, c4 );
      fillSquareUniform( surface, width, height, x, y, step, packRGBColor( blend4RGB( c1, c2, c3, c4 ) ) );
    }
  return true;
}
//  ---UR
// |   |
// DL--
bool RendererQuincunx::compareIntersectionsDL_UR( const TWorld &world,
						  const u32 x, const u32 y, const u32 step,
						  const TRay &r1, const TRay &r2 ) const {
  if ( r1.id != r2.id ) // different id -> different intersection or different lighting
    if ( ( r1.id & 15 ) == ( r2.id & 15 ) ) { // same intersection but different lighting
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::blue );
      return false;
    }
    else {
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::red ); 
      return false;
   }
  else
    if ( r1.id == 0 ) { // background (no intersection)
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::magenta * (2.0f/(float)step) );
      TColor c1 = world.shadeRay( r1 );
      TColor c2 = world.shadeRay( r2 );
      fillSquareUniform( surface, width, height, x, y, step, (c1+c2) * 0.5f );
    }
    else { // same intersection and same lighting
      //fillSquareUniform( surface, width, height, x, y, step, TColor::green * (2.0f/(float)step) );
      //--fillSquareUniform( surface, width, height, x, y, step, TColor::green );
      //--return true;

      u32 c3 = clampRGB32( world.shadeRay( r1 ) );
      u32 c2 = clampRGB32( world.shadeRay( r2 ) );

      TRay rayUL; rayUL.origin = r1.origin; rayUL.kind = TRay::RAY_CAMERA;
      rayUL.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x) ); rayUL.direction.normalize();
      world.traceRayNoShading( rayUL, 1.0f );
      u32 c1 = clampRGB32( world.shadeRay( rayUL ) );
      
      TRay rayDR; rayDR.origin = r1.origin; rayDR.kind = TRay::RAY_CAMERA;
      rayDR.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x+step) ); rayDR.direction.normalize();
      world.traceRayNoShading( rayDR, 1.0f );
      u32 c4 = clampRGB32( world.shadeRay( rayDR ) );

      //--      drawBlock( surface, width, height, x, y, step, c1, c2, c3, c4 );
      fillSquareUniform( surface, width, height, x, y, step, packRGBColor( blend4RGB( c1, c2, c3, c4 ) ) );
    }
  return true;
}
// UL--
// |   |
//  ---DR
void RendererQuincunx::fillSquarePrimitiveUL( const TWorld &world,
					      const u32 x, const u32 y, const u32 step,
					      TRay &rayUL, TRay &rayDR ) const {
  if ( compareIntersectionsUL_DR( world, x, y, step, rayUL, rayDR ) )
    return;
  // if <> intersection, split and recursive call
  // Up Right
  TRay rayUR; rayUR.origin = rayUL.origin; rayUR.kind = TRay::RAY_CAMERA;
  rayUR.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x+step) ); rayUR.direction.normalize();
  world.traceRayNoShading( rayUR, 1.0f );
  // Center
  TRay rayC; rayC.origin = rayUL.origin; rayC.kind = TRay::RAY_CAMERA;
  rayC.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); rayC.direction.normalize();
  world.traceRayNoShading( rayC, 1.0f );
  // Down Left
  TRay rayDL; rayDL.origin = rayUL.origin; rayDL.kind = TRay::RAY_CAMERA;
  rayDL.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x) ); rayDL.direction.normalize();
  world.traceRayNoShading( rayDL, 1.0f );
  // fill squares
  fillSquarePrimitive( world, x, y, step/2,
  		       rayUL, rayUR, rayC, rayDL, rayDR );
}
//  ---UR
// |   |
// DL--
void RendererQuincunx::fillSquarePrimitiveUR( const TWorld &world,
					      const u32 x, const u32 y, const u32 step,
					      TRay &rayDL, TRay &rayUR ) const {
  if ( compareIntersectionsDL_UR( world, x, y, step, rayDL, rayUR ) )
    return;
  // if <> intersection, split and recursive call
  // Up Left
  TRay rayUL; rayUL.origin = rayUR.origin; rayUL.kind = TRay::RAY_CAMERA;
  rayUL.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x) ); rayUL.direction.normalize();
  world.traceRayNoShading( rayUL, 1.0f );
  // Center
  TRay rayC; rayC.origin = rayUR.origin; rayC.kind = TRay::RAY_CAMERA;
  rayC.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); rayC.direction.normalize();
  world.traceRayNoShading( rayC, 1.0f );
  // Down Right
  TRay rayDR; rayDR.origin = rayUR.origin; rayDR.kind = TRay::RAY_CAMERA;
  rayDR.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x+step) ); rayDR.direction.normalize();
  world.traceRayNoShading( rayDR, 1.0f );
  // fill squares
  fillSquarePrimitive( world, x, y, step/2,
  		       rayUL, rayUR, rayC, rayDL, rayDR );
}
//  ---UR
// |   |
// DL--
void RendererQuincunx::fillSquarePrimitiveDL( const TWorld &world,
					      const u32 x, const u32 y, const u32 step,
					      TRay &rayDL, TRay &rayUR ) const {
  if ( compareIntersectionsDL_UR( world, x, y, step, rayDL, rayUR ) )
    return;
  // if <> intersection, split and recursive call
  // Up Left
  TRay rayUL; rayUL.origin = rayUR.origin; rayUL.kind = TRay::RAY_CAMERA;
  rayUL.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x) ); rayUL.direction.normalize();
  world.traceRayNoShading( rayUL, 1.0f );
  // Center
  TRay rayC; rayC.origin = rayUR.origin; rayC.kind = TRay::RAY_CAMERA;
  rayC.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); rayC.direction.normalize();
  world.traceRayNoShading( rayC, 1.0f );
  // Down Right
  TRay rayDR; rayDR.origin = rayUR.origin; rayDR.kind = TRay::RAY_CAMERA;
  rayDR.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x+step) ); rayDR.direction.normalize();
  world.traceRayNoShading( rayDR, 1.0f );
  // fill squares
  fillSquarePrimitive( world, x, y, step/2,
  		       rayUL, rayUR, rayC, rayDL, rayDR );
}
// UL--
// |   |
//  ---DR
void RendererQuincunx::fillSquarePrimitiveDR( const TWorld &world,
					      const u32 x, const u32 y, const u32 step,
					      TRay &rayUL, TRay &rayDR ) const {
  if ( compareIntersectionsUL_DR( world, x, y, step, rayUL, rayDR ) )
    return;
  // if <> intersection, split and recursive call
  // Up Right
  TRay rayUR; rayUR.origin = rayUL.origin; rayUR.kind = TRay::RAY_CAMERA;
  rayUR.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x+step) ); rayUR.direction.normalize();
  world.traceRayNoShading( rayUR, 1.0f );
  // Center
  TRay rayC; rayC.origin = rayUL.origin; rayC.kind = TRay::RAY_CAMERA;
  rayC.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); rayC.direction.normalize();
  world.traceRayNoShading( rayC, 1.0f );
  // Down Left
  TRay rayDL; rayDL.origin = rayUL.origin; rayDL.kind = TRay::RAY_CAMERA;
  rayDL.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x) ); rayDL.direction.normalize();
  world.traceRayNoShading( rayDL, 1.0f );
  // fill squares
  fillSquarePrimitive( world, x, y, step/2,
  		       rayUL, rayUR, rayC, rayDL, rayDR );
}
// UL-- --UR
// |UL | UR|
//  ---C---
// |DL | DR|
// DL-- --DR
void RendererQuincunx::fillSquarePrimitive( const TWorld &world,
					    const u32 x, const u32 y, const u32 step,
					    TRay &rayUL, TRay &rayUR,
					    TRay &rayC,
					    TRay &rayDL, TRay &rayDR ) const {
  if ( step <= minStep ) {
    //--fillSquareUniform( surface, width, height, x, y, step*2, TColor::white );
    TColor c = world.shadeRay( rayC );
    fillSquareUniform( surface, width, height, x, y, step*2, c );
    return;
  }
  fillSquarePrimitiveUL( world, x, y, step,
			 rayUL, rayC );
  fillSquarePrimitiveUR( world, x+step, y, step,
  			 rayC, rayUR );
  fillSquarePrimitiveDL( world, x, y+step, step,
  			 rayDL, rayC );
  fillSquarePrimitiveDR( world, x+step, y+step, step,
  			 rayC, rayDR );
  //--  // clear rays
  //--  rayUL.clear(); rayUR.clear(); rayC.clear(); rayDL.clear(); rayDR.clear();
}
bool RendererQuincunx::renderAdaptativePrimitive( const TWorld &world, const u32 step ) {
  TCamera *camera = world.currentCamera;
  camera->computeDirectionVector( width, height, topLeft, rightStep, upStep );

  // clear buffer
  memset( surface, 0, width*height*4 );

  for ( u32 y = 0; y < height; y += step )
    for ( u32 x = 0; x < width; x += step ) {
      // Up Left
      TRay rayUL; rayUL.origin = camera->location; rayUL.kind = TRay::RAY_CAMERA;
      rayUL.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x) ); rayUL.direction.normalize();
      world.traceRayNoShading( rayUL, 1.0f );
      // Up Right
      TRay rayUR; rayUR.origin = camera->location; rayUR.kind = TRay::RAY_CAMERA;
      rayUR.direction = topLeft - ( upStep * (float)(y) ) + ( rightStep * (float)(x+step) ); rayUR.direction.normalize();
      world.traceRayNoShading( rayUR, 1.0f );
      // Center
      TRay rayC; rayC.origin = camera->location; rayC.kind = TRay::RAY_CAMERA;
      rayC.direction = topLeft - ( upStep * (float)(y+step/2) ) + ( rightStep * (float)(x+step/2) ); rayC.direction.normalize();
      world.traceRayNoShading( rayC, 1.0f );
      // Down Left
      TRay rayDL; rayDL.origin = camera->location; rayDL.kind = TRay::RAY_CAMERA;
      rayDL.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x) ); rayDL.direction.normalize();
      world.traceRayNoShading( rayDL, 1.0f );
      // Down Right
      TRay rayDR; rayDR.origin = camera->location; rayDR.kind = TRay::RAY_CAMERA;
      rayDR.direction = topLeft - ( upStep * (float)(y+step) ) + ( rightStep * (float)(x+step) ); rayDR.direction.normalize();
      world.traceRayNoShading( rayDR, 1.0f );
      // fill squares
      fillSquarePrimitive( world, x, y, step/2,
			   rayUL, rayUR, rayC, rayDL, rayDR );
    }
  return true;
}
