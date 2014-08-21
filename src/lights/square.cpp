#include <stdio.h>

#include "../world.h"
#include "../ray.h"
#include "../intersection.h"
#include "../lighting.h"
#include "../math/math.h"
#include "../finish.h"
#include "../texture.h"
#include "../globals.h"
#include "square.h"
#include "../jitter.h"

//#define __COMPLEX_LIGHTING__

const TTagList LightSquare::lightSquareTags [] = {
  //--  { "location", TAG_lightSquare_location },
  { "direction", TAG_lightSquare_dir },
  { "axis", TAG_lightSquare_axis },
  { "numdots", TAG_lightSquare_numdots },
  { "jitter", TAG_lightSquare_jitter },
  { NULL, -1 }
};

LightSquare::LightSquare() {
  LightDot::LightDot(); lightKind = LIGHT_SQUARE;
  dir = TVector3( 0, -1, 0 );
  location = TPoint3( 0, 500, 0 );
  axis1 = TVector3( 1, 0, 0 );
  axis2 = TVector3( 0, 0, 1 );
  dotGrid = NULL;
}

LightSquare::~LightSquare() {
  if ( dotGrid != NULL )
    delete [] dotGrid;
  dotGrid = NULL;
}

void LightSquare::setup() {
  LightDot::setup();
  dir.normalize();
  numDots = numDots1 * numDots2;
  oneOverNumDots = 1.0f / (float) numDots;
  dotSize1 = 1.0f/(float)numDots1;
  dotSize2 = 1.0f/(float)numDots2;
  dotGrid = new TPoint3[ numDots ];
}

void LightSquare::precalcFrame( const TWorld &world ) {
  LightDot::precalcFrame( world );
  TPoint3 *dotGridIdx = dotGrid;
  for ( int i = 0; i < numDots1; i++ ) {
    for ( int j = 0; j < numDots2; j++ ) {
      float subX = 0.0f;
      float subY = 0.0f;
      if ( jitter ) {
      	jitter1( i, j, 0, subX, subY );
	subX = ((subX * 2.0f)-1.0f)*0.5f;
      	subY = ((subY * 2.0f)-1.0f)*0.5f;
      }
      TPoint3 decal1 = axis1 * (((float)(i-numDots1/2)+subX)*dotSize1);
      TPoint3 decal2 = axis2 * (((float)(j-numDots2/2)+subY)*dotSize2);
      *dotGridIdx = location + decal1 + decal2;

      dotGridIdx++; // next dot in grid
    }
  }
}

bool LightSquare::isLighten( const TPoint3 &pt ) const {
  // check if point is in right half-space
  if ( !( ( ( pt - location ) | dir ) > 0 ) )
    return false;
  // check normal light volume
  if ( !TLight::isLighten( pt ) )
    return false;
  return true;
}

#ifdef __COMPLEX_LIGHTING__
bool LightSquare::computeLighting( const TWorld &world, const TRay &ray,
				   TLighting &lighting ) {
  lighting.light = this;
  lighting.visible = isLighten( ray.intersection.pointWorldSpace );
  // If point not in light volume, return false (no diffuse component)
  if ( !lighting.visible )
    return false;

  const TFinish *finish = ray.intersection.texture->finish;

  // reset lighting
  lighting.shadow = 0.0f;
  lighting.diffuse = 0.0f;
  lighting.specular = 0.0f;
  lighting.phong = 0.0f;
  float att = 0.0f;

  TRay lightRay;
  TPoint3 *dotGridIdx = dotGrid;
  for ( int i = 0; i < numDots; i++, dotGridIdx++ ) {
    // create a lightRay from dot in grid to intersection
    lightRay.direction = *dotGridIdx - ray.intersection.pointWorldSpace;
    float dist2 = lightRay.direction.magnitudeSquared();
    lightRay.direction *= fastInvSqrt( dist2 ); // normalize vector
    lightRay.origin = ray.intersection.pointWorldSpace + lightRay.direction * 0.1f; // origin is intersection
    lightRay.reverseDirection = inverseVector( lightRay.direction );
    lightRay.target = *dotGridIdx;

    stats.increase( TStatistic::STATISTIC_LIGHT_RAY );

    // TODO
    att += brightness;
    
    // if shadow: proceed with next lightRay
    if ( world.findFirstIntersection( lightRay, ray.intersection, dist2, shadowCache ) )
      continue;
    // if no shadow: add 1.0f (no shadow) to lighting.shadow
    lighting.shadow += 1.0f;

    stats.increase( TStatistic::STATISTIC_DIFFUSE );
    
    // Diffuse
    if ( finish->diffuseFactor > 0.0f ) {
      float angleLight = ray.intersection.normal | lightRay.direction;
      if ( angleLight > 0.0f )
	//--float intensity = powf( angleLight, finish->brilliance ); // FIXME
	lighting.diffuse += angleLight * finish->diffuseFactor;
    }
    
    // Specular
    if ( finish->specularFactor > 0.0f ) {
      TVector3 H = lightRay.direction - ray.direction; H.normalize();
      float angleSpecular = H | ray.intersection.normal;
      if ( angleSpecular > 0.0f ) {
	float intensity = powf( angleSpecular, finish->specularRoughness );
	lighting.specular += intensity * finish->specularFactor;
      }
    }
    
    // Phong
    if ( finish->phongFactor > 0.0f ) {
      TVector3 R = reflectVector( ray.direction, ray.intersection.normal );
      float anglePhong = R | lightRay.direction;
      if ( anglePhong > 0.0f ) {
	float intensity = powf( anglePhong, finish->phongSize );
	lighting.phong += intensity * finish->phongFactor;
      }
    }
  }

  // If full shadow, return false (no diffuse component)
  if ( lighting.shadow <= 0.0f )
    return false;

  // att, shadow, diffuse, specular and phong must be scaled by oneOverNumDots (=N)
  // att * N * shadow * N * ( diffuse * N + specular * N + phong * N )
  // = N*N * ( att * shadow * ( N * ( diffuse + specular + phong ) )
  // = N*N*N * ( att * shadow * ( diffuse + specular + phong ) )
  lighting.color = color * ( (oneOverNumDots*oneOverNumDots*oneOverNumDots) * att * lighting.shadow * ( lighting.diffuse + lighting.specular + lighting.phong ) );
  return true;
}
#else
bool LightSquare::computeLighting( const TWorld &world, const TRay &ray,
				   TLighting &lighting ) {
  lighting.light = this;
  lighting.visible = isLighten( ray.intersection.pointWorldSpace );
  // If point not in light volume, return false (no diffuse component)
  if ( !lighting.visible )
    return false;

  stats.increase( TStatistic::STATISTIC_LIGHT_RAY );

  // Compute vector from intersection to light
  TRay lightRay;
  lightRay.direction = location - ray.intersection.pointWorldSpace;
  float dist2 = lightRay.direction.magnitudeSquared(); // get vector length

  // test attenuation
  float att = brightness;
//--TODO  if ( attenuation )
//--TODO    att = ( brightness / ( 1.0f + dist2 ) );
//--TODO  if ( att < lightAttenuation )
//--TODO    return false;

  lightRay.direction *= fastInvSqrt( dist2 ); // normalize vector
  //--  lightRay.origin = ray.intersection.pointWorldSpace + lightRay.direction * 0.1f; // origin is intersection
  //--  lightRay.reverseDirection = inverseVector( lightRay.direction );
  //--  lightRay.target = location;
  lightRay.kind = TRay::RAY_SHADOW;

  // Shadow
//--  if ( globalSettings::shadowOn
//--       && world.findFirstIntersection( lightRay, ray.intersection, dist2, shadowCache ) ) {
//--    // If full shadow, return false (no diffuse component)
//--    lighting.shadow = 0.0f; // shadow
//--    return false;
//--  }
//--  else
//--    lighting.shadow = 1.0f; // no shadow
  if ( globalSettings::shadowOn ) {
    lighting.shadow = getAmountShadow( world, ray );
    // If full shadow, return false (no diffuse component)
    if ( lighting.shadow <= 0.0f )
      return false;
  }
  else lighting.shadow = 1.0f;

  const TFinish *finish = ray.intersection.texture->finish;

  stats.increase( TStatistic::STATISTIC_DIFFUSE );

  // Diffuse
  lighting.diffuse = 0.0f;
  if ( finish->diffuseFactor > 0.0f ) {
    // FIXME: create a light ray for each dot in square and compute average value
    // we compute angle between lightRay (from intersection to center of square)
    // this "trick" is a good approximation if dotSize is small
    float angleLight = ray.intersection.normal | lightRay.direction;
    if ( angleLight > 0.0f )
      //--float intensity = powf( angleLight, finish->brilliance ); // FIXME
      lighting.diffuse = angleLight * finish->diffuseFactor;
  }

  // Specular
  lighting.specular = 0.0f;
  if ( finish->specularFactor > 0.0f ) {
    TVector3 H = lightRay.direction - ray.direction; H.normalize();
    float angleSpecular = H | ray.intersection.normal;
    if ( angleSpecular > 0.0f ) {
      float intensity = powf( angleSpecular, finish->specularRoughness );
      lighting.specular = intensity * finish->specularFactor;
    }
  }

  // Phong
  lighting.phong = 0.0f;
  if ( finish->phongFactor > 0.0f ) {
    TVector3 R = reflectVector( ray.direction, ray.intersection.normal );
    float anglePhong = R | lightRay.direction;
    if ( anglePhong > 0.0f ) {
      float intensity = powf( anglePhong, finish->phongSize );
      lighting.phong = intensity * finish->phongFactor;
    }
  }

  lighting.color = color * ( att * lighting.shadow * ( lighting.diffuse + lighting.specular + lighting.phong ) );
  return true;
}
#endif

bool LightSquare::getLight( const TWorld &world, const TRay &ray, TRay &lightRay ) {
  // If point not in light volume, return false (no diffuse component)
  if ( !isLighten( ray.intersection.pointWorldSpace ) )
    return false;

  if ( !globalSettings::shadowOn )
    return true;

  stats.increase( TStatistic::STATISTIC_LIGHT_RAY );

  // Compute vector from intersection to light
  lightRay.direction = location - ray.intersection.pointWorldSpace;
  float dist2 = lightRay.direction.magnitudeSquared(); // get vector length

//--TODO  // test attenuation
//--TODO  float att = brightness;
//--TODO  if ( attenuation )
//--TODO    att = ( brightness / ( 1.0f + dist2 ) );
//--TODO  if ( att < lightAttenuation )
//--TODO    return false;

  lightRay.direction *= fastInvSqrt( dist2 ); // normalize vector
  lightRay.origin = ray.intersection.pointWorldSpace + lightRay.direction * 0.1f; // origin is intersection
  lightRay.reverseDirection = inverseVector( lightRay.direction );
  lightRay.target = location;
  lightRay.kind = TRay::RAY_SHADOW;

  // Compute amount of shadow receive  0: full shadow   1: no shadow
  if ( world.findFirstIntersection( lightRay, ray.intersection, dist2, shadowCache ) )
    return false;

  return true;
}

TColor LightSquare::getColor( const TWorld &world, const TRay &ray, const TRay &lightRay ) {
  const TFinish *finish = ray.intersection.texture->finish;

  stats.increase( TStatistic::STATISTIC_DIFFUSE );

  // Diffuse
  float diffuse = 0.0f;
  if ( finish->diffuseFactor > 0.0f ) {
    // FIXME: create a light ray for each dot in square and compute average value
    // we compute angle between lightRay (from intersection to center of square)
    // this "trick" is a good approximation if dotSize is small
    float angleLight = ray.intersection.normal | lightRay.direction;
    if ( angleLight > 0.0f )
      //--float intensity = powf( angleLight, finish->brilliance ); // FIXME
      diffuse = angleLight * finish->diffuseFactor;
  }

  // Specular
  float specular = 0.0f;
  if ( finish->specularFactor > 0.0f ) {
    TVector3 H = lightRay.direction - ray.direction; H.normalize();
    float angleSpecular = H | ray.intersection.normal;
    if ( angleSpecular > 0.0f ) {
      float intensity = powf( angleSpecular, finish->specularRoughness );
      specular = intensity * finish->specularFactor;
    }
  }

  // Phong
  float phong = 0.0f;
  if ( finish->phongFactor > 0.0f ) {
    TVector3 R = reflectVector( ray.direction, ray.intersection.normal );
    float anglePhong = R | lightRay.direction;
    if ( anglePhong > 0.0f ) {
      float intensity = powf( anglePhong, finish->phongSize );
      phong = intensity * finish->phongFactor;
    }
  }

  return color * ( brightness * ( diffuse + specular + phong ) );
}

float LightSquare::getAmountShadow( const TWorld &world, const TRay &ray ) {
  float shadow = 0.0f;

  // for each dot light in square: create a light ray and compute intersection
  TRay shadowRay;
  TPoint3 *dotGridIdx = dotGrid;
  for ( int i = 0; i < numDots; i++ ) {
    // create a lightRay from dot in grid to intersection
    shadowRay.direction = *dotGridIdx - ray.intersection.pointWorldSpace;
    float dist2 = shadowRay.direction.magnitudeSquared();

//--TODO    // test attenuation
//--TODO    float att = brightness;
//--TODO    if ( attenuation )
//--TODO      att = ( brightness / ( 1.0f + dist2 ) );
//--TODO    if ( att < lightAttenuation ) {
//--TODO      dotGridIdx++; // next dot in grid
//--TODO      continue;
//--TODO    }

    shadowRay.direction *= fastInvSqrt( dist2 ); // normalize vector
    shadowRay.origin = ray.intersection.pointWorldSpace + shadowRay.direction * 0.1f; // origin is intersection
    shadowRay.reverseDirection = inverseVector( shadowRay.direction );
    shadowRay.target = *dotGridIdx;
    shadowRay.kind = TRay::RAY_SHADOW;
    
//--    // if shadow: add 0.0f
//--    if ( world.findFirstIntersection( shadowRay, ray.intersection, dist2, shadowCache ) )
//--      shadowCache = shadowRay.intersectionObject; // store object in cache
//--    // if no shadow: add 1.0f
//--    else
//--      shadow += 1.0f;
    // if no shadow: add 1.0f, shadow cache is updated in findFirstIntersection
    if ( !world.findFirstIntersection( shadowRay, ray.intersection, dist2, shadowCache ) )
      shadow += 1.0f;
    // if shadow: add 0.0f

    dotGridIdx++; // next dot in grid
  }
  return shadow * oneOverNumDots;
}

void LightSquare::print( const char *msg ) const {
  TLight::print(msg);
  location.print("location: ");
  dir.print("direction: ");
  axis1.print("axis1: ");
  axis2.print("axis2: ");
  printf("numDots: %d | %d\n", numDots1, numDots2 );
  printf("jitter: %s\n", jitter?"true":"false");
  printf("dotGrid:\n");
  for ( int i = 0; i < numDots; i++ )
    dotGrid[i].print("dotGrid: ");
}
