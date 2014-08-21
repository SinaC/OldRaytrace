#include <stdio.h>

#include "../world.h"
#include "../ray.h"
#include "../intersection.h"
#include "../lighting.h"
#include "../math/math.h"
#include "../finish.h"
#include "../texture.h"
#include "../globals.h"
#include "dot.h"

const TTagList LightDot::lightDotTags [] = {
  { "location", TAG_lightDot_location },
  { "attenuation", TAG_lightDot_attenuation },
  { NULL, -1 }
};

LightDot::LightDot() {
  TLight::TLight(); lightKind = LIGHT_DOT;
  attenuation = false;
}

LightDot::~LightDot() {
}

bool LightDot::computeLighting( const TWorld &world, const TRay &ray,
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
  lightRay.origin = ray.intersection.pointWorldSpace + lightRay.direction * 0.1f; // origin is intersection
  lightRay.reverseDirection = inverseVector( lightRay.direction );
  lightRay.target = location;
  lightRay.kind = TRay::RAY_SHADOW;

  // Shadow
  if ( globalSettings::shadowOn
       && world.findFirstIntersection( lightRay, ray.intersection, dist2, shadowCache ) ) {
    // If full shadow, return false (no diffuse component)
    lighting.shadow = 0.0f; // shadow
    return false;
  }
  else
    lighting.shadow = 1.0f; // no shadow

  const TFinish *finish = ray.intersection.texture->finish;

  stats.increase( TStatistic::STATISTIC_DIFFUSE );

  // Diffuse
  lighting.diffuse = 0.0f;
  if ( finish->diffuseFactor > 0.0f ) {
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
  //  lighting.color = color * ( brightness * lighting.shadow * ( lighting.diffuse + lighting.specular + lighting.phong ) );
  return true;
}

// compute light ray and amount of light intersection received from this light
//  but don't compute any shading
bool LightDot::getLight( const TWorld &world, const TRay &ray, TRay &lightRay ) {
  // If point not in light volume, return false (no diffuse component)
  if ( !isLighten( ray.intersection.pointWorldSpace ) )
    return false;

  if ( !globalSettings::shadowOn )
    return true;

  stats.increase( TStatistic::STATISTIC_LIGHT_RAY );

  // Compute vector from intersection to light
  lightRay.direction = location - ray.intersection.pointWorldSpace;
  float dist2 = lightRay.direction.magnitudeSquared(); // get vector length
  lightRay.direction *= fastInvSqrt( dist2 ); // normalize vector
  lightRay.origin = ray.intersection.pointWorldSpace + lightRay.direction * 0.1f; // origin is intersection
  lightRay.reverseDirection = inverseVector( lightRay.direction );
  lightRay.target = location;

  // Compute amount of shadow receive  0: full shadow   1: no shadow
  if ( world.findFirstIntersection( lightRay, ray.intersection, dist2, shadowCache ) )
    return false;

  return true;
}

// compute lighting using ray.shadowRay
TColor LightDot::getColor( const TWorld &world, const TRay &ray, const TRay &lightRay ) {
  const TFinish *finish = ray.intersection.texture->finish;

  stats.increase( TStatistic::STATISTIC_DIFFUSE );

  // Diffuse
  float diffuse = 0.0f;
  if ( finish->diffuseFactor > 0.0f ) {
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

void LightDot::print( const char *msg ) const {
  TLight::print( msg );
  location.print("location: ");
}
