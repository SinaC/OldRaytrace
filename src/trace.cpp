#include <stdlib.h>
#include <stdio.h>

#include "world.h"
#include "ray.h"
#include "intersection.h"
#include "object.h"
#include "lights/light.h"
#include "lighting.h"
#include "primitives/primitive.h"
#include "finish.h"
#include "texture.h"
#include "pigments/pigment.h"
#include "interior.h"
#include "ambients/ambient.h"
#include "globals.h"
#include "atmospherics/atmospheric.h"
#include "normals/normal.h"

//--#define __DEBUG__

#define FORWARD (0.1f)

// Minimal attenuation, below this value: rays are ignored
#define MIN_ATTENUATION (0.03f)

// traceRay:
//   find closest intersection
//   if none: return background color
//   else
//     compute primitive color: intersectionColor
//     compute ambient color: ambient
//     compute diffuse color: diffuse
//     compute reflected color: reflected
//     compute transmitted color: transmitted
//     final color: intersectionColor * ( ambient + diffuse + reflected + transmitted )
//     add atmospheric color
//
//  ambient: world constant
//  diffuse: for each light
//             compute shadow, diffuse, specular, phong
//  reflected: if reflective object: recursive call to traceRay
//  transmitted: if non-opaque object: recursive call to traceRay
//  atmospheric: if fog/...: compute atmospheric

int reflectionDepth = 0;
int transmissionDepth = 0;
TColor TWorld::traceRay( TRay &ray, const float attenuation ) const {
#ifdef __DEBUG__
  printf("TWorld::traceRay [%2d, %2d]\n", reflectionDepth, transmissionDepth ); fflush(stdout);
#endif

  if ( attenuation < MIN_ATTENUATION ) {
#ifdef __DEBUG__
    printf("min attenuation reached [%5.5f] -> leaving\n", attenuation );
#endif
    return TColor::black;
  }

  stats.increase( TStatistic::STATISTIC_RAY );
  switch ( ray.kind ) {
  case TRay::RAY_CAMERA: stats.increase( TStatistic::STATISTIC_CAMERA_RAY ); break;
  case TRay::RAY_REFLECTED: stats.increase( TStatistic::STATISTIC_REFLECTED_RAY ); break;
  case TRay::RAY_TRANSMITTED: stats.increase( TStatistic::STATISTIC_TRANSMITTED_RAY ); break;
  case TRay::RAY_SHADOW: stats.increase( TStatistic::STATISTIC_INTERNAL_REFLECTED_RAY ); break;
  case TRay::RAY_INTERNAL_REFLECTED: stats.increase( TStatistic::STATISTIC_CAMERA_RAY ); break;
  }

  // compute 1/direction, used to compute faster ray/box intersection
  ray.update();

#ifdef __DEBUG__
  printf("find intersection\n"); fflush(stdout);
#endif
  
  // find closest intersection
  bool intersectionFound = ( findClosestIntersection( ray, ray.intersection ) != 0 );
  ray.intersection.ray = &ray;

#ifdef __DEBUG__
  printf("intersection computed [%d]\n", intersectionFound ); fflush(stdout);
#endif

  // no intersection found, return background color
  TColor resultColor = TColor::black;
  if ( !intersectionFound ) {
#ifdef __DEBUG__
    printf("not found -> get background\n"); fflush(stdout);
#endif
    
    if ( background )
      resultColor = background->getColor( ray.direction );
    else
      resultColor = TColor::black;
    
#ifdef __DEBUG__
    printf("background got\n"); fflush(stdout);
#endif
  }
  // intersection found, compute intersection color
  else {
#ifdef __DEBUG__
    printf("texture computed\n"); fflush(stdout);
#endif
    ray.intersection.texture = ray.intersection.object->texture;
    ray.target = ray.intersection.pointWorldSpace;

    if ( !globalSettings::shadingOn ) {
#ifdef __DEBUG__
      printf("no shading... base color\n"); fflush(stdout);
#endif
      if ( ray.intersection.texture == NULL
	   || ray.intersection.texture->pigment == NULL )
	resultColor = TColor::black;
      else
	resultColor = ray.intersection.texture->pigment->simple;
    }
    else {
#ifdef __DEBUG__
      printf("compute diffuse, ...\n"); fflush(stdout);
      ray.intersection.print("ray.intersection: "); fflush(stdout);
#endif

#ifdef __DEBUG__
      printf("compute normal...\n"); fflush(stdout);
#endif
      ray.intersection.rawNormal = ray.intersection.object->primitive->normalAt( ray.intersection );
#ifdef __DEBUG__
      printf("normal computed...\n"); fflush(stdout);
#endif
      if ( ray.intersection.inverse )
	ray.intersection.rawNormal = -ray.intersection.rawNormal;
#ifdef __DEBUG__
      printf("normal (inversed)\n"); fflush(stdout);
#endif

      // perturb normal
      if ( ray.intersection.texture != NULL
	   && ray.intersection.texture->normal != NULL )
	ray.intersection.texture->normal->perturbNormal( ray.intersection );
      // if no perturbation, copy raw normal to normal
      else
	ray.intersection.normal = ray.intersection.rawNormal;
      
#ifdef __DEBUG__
      ray.intersection.print("INTERSECTION: "); fflush(stdout);
#endif
      
#ifdef __DEBUG__
      printf("found -> computing lighting, reflection, transmission\n"); fflush(stdout);
#endif
      // if primitive doesn't care about lighting, we just use base color
      if ( ray.intersection.texture == NULL
	   || ray.intersection.texture->finish == NULL
	   || ray.intersection.texture->pigment == NULL ) {
#ifdef __DEBUG__
	printf("no lighting!\n"); fflush(stdout);
#endif
	resultColor = TColor::black;
      }
      // else, compute diffuse, ambient, reflective, transmittive
      else {
	// compute primitive color: already computed, stored in ray.intersection.color
	const TFinish *intersectionFinish = ray.intersection.texture->finish;
	// compute ambient color
	if ( globalSettings::ambientOn 
	     && ambient != NULL
	     && intersectionFinish->ambientFactor > 0.0f ) {
#ifdef __DEBUG__
	  printf("computing ambient\n"); fflush(stdout);
#endif
	  stats.increase( TStatistic::STATISTIC_AMBIENT );
	  resultColor += ( ambient->getColor( ray.intersection.pointWorldSpace )
			   * intersectionFinish->ambientFactor );
	}

	// compute diffuse color
	if ( globalSettings::diffuseOn
	     && intersectionFinish->diffuseFactor > 0.0f ) {
	  resultColor += computeDiffuse( ray );
	}

	// compute reflected color
	if ( globalSettings::reflectionOn
	     && reflectionDepth < globalSettings::maxReflectionDepth
	     && intersectionFinish->reflectionFactor > 0.0f ) {
	  //resultColor += computeReflected( ray, attenuation*intersectionFinish->reflectionFactor ) * intersectionFinish->reflectionFactor;
		resultColor += computeReflected( ray, attenuation*intersectionFinish->reflectionFactor );
	}

	// compute transmitted color
	if ( globalSettings::transmissionOn
	     && transmissionDepth < globalSettings::maxTransmissionDepth
	     && intersectionFinish->transmissionFactor > 0.0f
	     && ray.intersection.texture->interior != NULL ) {
	  //resultColor += computeTransmitted( ray, attenuation*intersectionFinish->transmissionFactor ) * intersectionFinish->transmissionFactor;
		resultColor += computeTransmitted( ray, attenuation*intersectionFinish->transmissionFactor );
	}

#ifdef __DEBUG__
	printf("mix with primitive color\n"); fflush(stdout);
#endif
	// multiply result with primitive color at intersection
	resultColor = resultColor * ray.intersection.texture->pigment->getColor( ray.intersection.pointWorldSpace );
      }
    }
  }    
  // compute atmospheric color
  if ( globalSettings::atmosphericOn
       && atmospheric != NULL ) {
#ifdef __DEBUG__
    printf("compute atmospheric\n");
#endif
    stats.increase( TStatistic::STATISTIC_ATMOSPHERIC );
    resultColor = atmospheric->addColor( resultColor, ray.intersection );
  }
    
  return resultColor * attenuation;
}

TColor TWorld::computeDiffuse( const TRay &ray ) const {
#ifdef __DEBUG__
  printf("compute diffuse\n"); fflush(stdout);
#endif

  TColor resultColor = TColor::black;
  for ( vector<TLight *>::const_iterator iter = lights.begin();
	iter != lights.end();
	iter++ ) {
    TLight *light = *iter;
    TLighting lighting;
    // if light is switched on and intersection get color from this light, add light
    if ( light->computeLighting( *this, ray, lighting ) )
      resultColor += lighting.color;
  }
  
#ifdef __DEBUG__
  printf("diffuse computed\n"); fflush(stdout);
#endif

  return resultColor;
}

TColor TWorld::computeReflected( TRay &ray, const float attenuation ) const {
#ifdef __DEBUG__
  printf("compute reflected\n"); fflush(stdout);
#endif

  if ( attenuation < MIN_ATTENUATION ) {
#ifdef __DEBUG__
    printf("min attenuation reached [%5.5f] -> leaving\n", attenuation ); fflush(stdout);
#endif
    return TColor::black;
  }

  // Compute reflection ray
  ray.reflectedRay = new TRay();

  ray.reflectedRay->copyInsideList( ray );
  ray.reflectedRay->direction = reflectVector( ray.direction, ray.intersection.normal );
  ray.reflectedRay->origin = ray.intersection.pointWorldSpace + ray.reflectedRay->direction * FORWARD;
  ray.reflectedRay->kind = TRay::RAY_REFLECTED;
  
  reflectionDepth++;
  TColor reflectedColor = traceRay( *ray.reflectedRay, attenuation );
  reflectionDepth--;

#ifdef __DEBUG__
  printf("reflected computed\n"); fflush(stdout);
#endif
  
  return reflectedColor;
}

TColor TWorld::computeTransmitted( TRay &ray, const float attenuation ) const {
#ifdef __DEBUG__
  printf("compute transmitted\n"); fflush(stdout);
#endif

  if ( attenuation < MIN_ATTENUATION ) {
#ifdef __DEBUG__
    printf("min attenuation reached [%5.5f] -> leaving\n", attenuation ); fflush(stdout);
#endif
    return TColor::black;
  }

  TColor newAttenuation = TColor( attenuation, attenuation, attenuation );

  ray.transmittedRay = new TRay();

  ray.transmittedRay->copyInsideList( ray );
  float ratio;
  float intersectionIOR = ray.intersection.texture->interior->ior;
  // if primitive is 2D, no entering/leaving
  bool prim2D = ray.intersection.object->primitive->additionalInformation.isSet(TPrimitive::PRIMITIVE_2D);
  // ray is entering from atmosphere
  if ( ray.transmittedRay->isInsideListEmpty() ) {
    if ( !prim2D ) // enters only if not 2D
      ray.transmittedRay->enter( ray.intersection.object );
    ratio = atmosphereIOR / intersectionIOR;
  }
  // ray is currently inside an object
  else {
    // ray is leaving the current object
    if ( ray.transmittedRay->inside( ray.intersection.object ) ) { // impossible if 2D
      ray.transmittedRay->leave( ray.intersection.object );
      float tempIOR;
      // ray is leaving to atmosphere
      if ( ray.transmittedRay->isInsideListEmpty() ) {
		tempIOR = atmosphereIOR;
      }
      // ray is leaving into another object
      else {
		tempIOR = ray.transmittedRay->getLastInList()->texture->interior->ior;
      }
      ratio = intersectionIOR / tempIOR;

	  // Compute new attenuation using interior's absorption  
	  if ( ray.intersection.texture->interior->absorption.RED < 1.0f
		  || ray.intersection.texture->interior->absorption.GREEN < 1.0f
		  || ray.intersection.texture->interior->absorption.BLUE < 1.0f ) {
	    newAttenuation.RED = newAttenuation.RED * powf( ray.intersection.texture->interior->absorption.RED, ray.intersection.distance );
		newAttenuation.GREEN = newAttenuation.GREEN * powf( ray.intersection.texture->interior->absorption.GREEN, ray.intersection.distance );
		newAttenuation.BLUE = newAttenuation.BLUE * powf( ray.intersection.texture->interior->absorption.BLUE, ray.intersection.distance );
	  }
    }
    // ray is entering a new object
    else {
      float tempIOR = ray.transmittedRay->getLastInList()->texture->interior->ior;
      if ( !prim2D ) // enters only if not 2D
		ray.transmittedRay->enter( ray.intersection.object );
		ratio = tempIOR / intersectionIOR;
    }
  }

  TVector3 normal = ray.intersection.normal;
  float ci = ray.direction | normal;
  if ( ci <= 0.0f )
    ci = -ci;
  else
    normal = -normal;
  float ratio2 = ratio * ratio;
  float ci2 = ci*ci;
  float criticalAngle = 1.0f + ratio2 * ( ci2 - 1.0f );
  TColor transmittedColor;
  // critical angle -> total internal reflection
  if ( criticalAngle < 0.0f ) {
    float twoCi = ci + ci;
    ray.transmittedRay->direction = ray.direction - ( twoCi * normal );
    ray.transmittedRay->origin = ray.intersection.pointWorldSpace + ( ray.transmittedRay->direction * FORWARD );
    ray.transmittedRay->kind = TRay::RAY_INTERNAL_REFLECTED;
  
    transmissionDepth++;
    transmittedColor = traceRay( *ray.transmittedRay, attenuation ) * ( 1.0f - ray.intersection.texture->finish->reflectionFactor );
    transmissionDepth--;
  }
  // not critical -> normal transmission
  else {
    ray.transmittedRay->direction = ( ray.direction * ratio )
      + ( normal * ( ratio * ci - sqrtf( criticalAngle ) ) );
    ray.transmittedRay->origin = ray.intersection.pointWorldSpace + ( ray.transmittedRay->direction * FORWARD );
    ray.transmittedRay->kind = TRay::RAY_TRANSMITTED;
    
    transmissionDepth++;
    transmittedColor = traceRay( *ray.transmittedRay, attenuation ) * newAttenuation;
    transmissionDepth--;
  }
#ifdef __DEBUG__
  printf("transmitted computed\n"); fflush(stdout);
#endif
  return transmittedColor;
}

// **********************************************************************
// ******************** Same functions as above but don't compute shading
// ******************** and compute ray.id
// ******************** Shading may be computed with shadeRay

int TWorld::traceRayNoShading( TRay &ray, const float attenuation ) const {
#ifdef __DEBUG__
  printf("TWorld::traceRayNoShading [%2d, %2d]\n", reflectionDepth, transmissionDepth ); fflush(stdout);
#endif

  if ( attenuation < MIN_ATTENUATION ) {
#ifdef __DEBUG__
    printf("min attenuation reached [%5.5f] -> leaving\n", attenuation );
#endif
    return 0;
  }

  stats.increase( TStatistic::STATISTIC_RAY );
  switch ( ray.kind ) {
  case TRay::RAY_CAMERA: stats.increase( TStatistic::STATISTIC_CAMERA_RAY ); break;
  case TRay::RAY_REFLECTED: stats.increase( TStatistic::STATISTIC_REFLECTED_RAY ); break;
  case TRay::RAY_TRANSMITTED: stats.increase( TStatistic::STATISTIC_TRANSMITTED_RAY ); break;
  case TRay::RAY_SHADOW: stats.increase( TStatistic::STATISTIC_INTERNAL_REFLECTED_RAY ); break;
  case TRay::RAY_INTERNAL_REFLECTED: stats.increase( TStatistic::STATISTIC_CAMERA_RAY ); break;
  }

  // compute 1/direction, used to compute faster ray/box intersection
  ray.update();

#ifdef __DEBUG__
  printf("find intersection\n"); fflush(stdout);
#endif
  
  // find closest intersection
  ray.id = findClosestIntersection( ray, ray.intersection );
  ray.intersection.ray = &ray;
  
#ifdef __DEBUG__
  printf("intersection computed [%d]\n", intersectionFound ); fflush(stdout);
#endif

  // no intersection found
  if ( ray.id == 0
       || ray.intersection.object == NULL )
    return 0;
  
  // intersection found, compute intersection color
#ifdef __DEBUG__
  printf("compute diffuse, ...\n"); fflush(stdout);
  ray.intersection.print("ray.intersection: "); fflush(stdout);
#endif
  
#ifdef __DEBUG__
  printf("compute normal...\n"); fflush(stdout);
#endif
  ray.intersection.rawNormal = ray.intersection.object->primitive->normalAt( ray.intersection );
#ifdef __DEBUG__
  printf("normal computed...\n"); fflush(stdout);
#endif
  if ( ray.intersection.inverse )
    ray.intersection.rawNormal = -ray.intersection.rawNormal;

  ray.intersection.texture = ray.intersection.object->texture;
  ray.target = ray.intersection.pointWorldSpace;
  
#ifdef __DEBUG__
  printf("normal (inversed?) and texture computed\n"); fflush(stdout);
#endif
 
  // perturb normal
  if ( ray.intersection.texture == NULL
       && ray.intersection.texture->normal != NULL )
    ray.intersection.texture->normal->perturbNormal( ray.intersection );
  // if no perturbation, copy raw normal to normal
  else
    ray.intersection.normal = ray.intersection.rawNormal;
 
#ifdef __DEBUG__
  ray.intersection.print("INTERSECTION: "); fflush(stdout);
#endif
  
#ifdef __DEBUG__
  printf("found -> computing lighting, reflection, transmission\n"); fflush(stdout);
#endif
  // compute diffuse, ambient, reflective, transmitive
  if ( ray.intersection.texture != NULL
       && ray.intersection.texture->finish != NULL
       && ray.intersection.texture->pigment != NULL ) {
    // compute primitive color: already computed, stored in ray.intersection.color
    const TFinish *intersectionFinish = ray.intersection.texture->finish;
    
    if ( globalSettings::diffuseOn
	 && intersectionFinish->diffuseFactor > 0.0f )
      ray.id = ray.id + ( computeDiffuseNoShading( ray ) << 4 );
    
    if ( globalSettings::reflectionOn
	 && reflectionDepth < globalSettings::maxReflectionDepth
	 && intersectionFinish->reflectionFactor > 0.0f )
      ray.id = ray.id + ( computeReflectedNoShading( ray, attenuation*intersectionFinish->reflectionFactor ) << 8 );
    
    // compute transmitted color
    if ( globalSettings::transmissionOn
	 && transmissionDepth < globalSettings::maxTransmissionDepth
	 && intersectionFinish->transmissionFactor > 0.0f
	 && ray.intersection.texture->interior != NULL )
      ray.id = ray.id + ( computeTransmittedNoShading( ray, attenuation*intersectionFinish->transmissionFactor ) << 8 );
  }
  return ray.id;
}

int TWorld::computeDiffuseNoShading( TRay &ray ) const {
#ifdef __DEBUG__
  printf("compute diffuse\n"); fflush(stdout);
#endif

  int lightId = 0;
  int i = 0;
  for ( vector<TLight *>::const_iterator iter = lights.begin();
	iter != lights.end();
	iter++, i++ ) {
    TLight *light = *iter;
    TRay lightRay;
    if ( light->getLight( *this, ray, lightRay ) ) {
      lightId = lightId + ( 1 << i );
      ray.lightRays[i] = new TRay();
      ray.lightRays[i]->copy( lightRay );
    }
  }
  
#ifdef __DEBUG__
  printf("diffuse computed\n"); fflush(stdout);
#endif

  return lightId;
}

int TWorld::computeReflectedNoShading( TRay &ray, const float attenuation ) const {
#ifdef __DEBUG__
  printf("compute reflected\n"); fflush(stdout);
#endif

  if ( attenuation < MIN_ATTENUATION ) {
#ifdef __DEBUG__
    printf("min attenuation reached [%5.5f] -> leaving\n", attenuation ); fflush(stdout);
#endif
    return 0;
  }

  // Compute reflection ray
  //  TRay reflectedRay;
  ray.reflectedRay = new TRay();

  ray.reflectedRay->copyInsideList( ray );
  ray.reflectedRay->direction = reflectVector( ray.direction, ray.intersection.normal );
  ray.reflectedRay->origin = ray.intersection.pointWorldSpace + ray.reflectedRay->direction * FORWARD;
  ray.reflectedRay->kind = TRay::RAY_REFLECTED;
  
  reflectionDepth++;
  ray.reflectedRay->id = traceRayNoShading( *ray.reflectedRay, attenuation );
  reflectionDepth--;

#ifdef __DEBUG__
  printf("reflected computed\n"); fflush(stdout);
#endif
  
  return ray.reflectedRay->id;
}

int TWorld::computeTransmittedNoShading( TRay &ray, const float attenuation ) const {
#ifdef __DEBUG__
  printf("compute transmitted\n"); fflush(stdout);
#endif

  if ( attenuation < MIN_ATTENUATION ) {
#ifdef __DEBUG__
    printf("min attenuation reached [%5.5f] -> leaving\n", attenuation ); fflush(stdout);
#endif
    return 0;
  }

  //TRay transmittedRay;
  ray.transmittedRay = new TRay();

  ray.transmittedRay->copyInsideList( ray );
  float ratio;
  float intersectionIOR = ray.intersection.texture->interior->ior;
  // if primitive is 2D, no entering/leaving
  bool prim2D = ray.intersection.object->primitive->additionalInformation.isSet(TPrimitive::PRIMITIVE_2D);
  // ray is entering from atmosphere
  if ( ray.transmittedRay->isInsideListEmpty() ) {
    if ( !prim2D ) // enters only if not 2D
      ray.transmittedRay->enter( ray.intersection.object );
    ratio = atmosphereIOR / intersectionIOR;
  }
  // ray is currently inside an object
  else {
    // ray is leaving the current object
    if ( ray.transmittedRay->inside( ray.intersection.object ) ) {
      ray.transmittedRay->leave( ray.intersection.object );
      float tempIOR;
      // ray is leaving to atmosphere
      if ( ray.transmittedRay->isInsideListEmpty() ) {
	tempIOR = atmosphereIOR;
      }
      // ray is leaving into another object
      else {
	tempIOR = ray.transmittedRay->getLastInList()->texture->interior->ior;
      }
      ratio = intersectionIOR / tempIOR;
    }
    // ray is entering a new object
    else {
      float tempIOR = ray.transmittedRay->getLastInList()->texture->interior->ior;
      if ( !prim2D ) // enters only if not 2D
	ray.transmittedRay->enter( ray.intersection.object );
      ratio = tempIOR / intersectionIOR;
    }
  }

  TVector3 normal = ray.intersection.normal;
  float ci = ray.direction | normal;
  if ( ci <= 0.0f )
    ci = -ci;
  else
    normal = -normal;
  float ratio2 = ratio * ratio;
  float ci2 = ci*ci;
  float criticalAngle = 1.0f + ratio2 * ( ci2 - 1.0f );
  // critical angle -> total internal reflection
  if ( criticalAngle < 0.0f ) {
    float twoCi = ci + ci;
    ray.transmittedRay->direction = ray.direction - ( twoCi * normal );
    ray.transmittedRay->origin = ray.intersection.pointWorldSpace + ( ray.transmittedRay->direction * FORWARD );
    ray.transmittedRay->kind = TRay::RAY_INTERNAL_REFLECTED;
  
    transmissionDepth++;
    ray.transmittedRay->id = traceRayNoShading( *ray.transmittedRay, attenuation );
    transmissionDepth--;
  }
  // not critical -> normal transmission
  else {
    ray.transmittedRay->direction = ( ray.direction * ratio )
      + ( normal * ( ratio * ci - sqrtf( criticalAngle ) ) );
    ray.transmittedRay->origin = ray.intersection.pointWorldSpace + ( ray.transmittedRay->direction * FORWARD );
    ray.transmittedRay->kind = TRay::RAY_TRANSMITTED;
    
    transmissionDepth++;
    ray.transmittedRay->id = traceRayNoShading( *ray.transmittedRay, attenuation );
    transmissionDepth--;
  }
#ifdef __DEBUG__
  printf("transmitted computed\n"); fflush(stdout);
#endif
  return ray.transmittedRay->id;
}

TColor TWorld::shadeRay( const TRay &ray ) const {
  TColor resultColor = TColor::black;
  if ( ray.id == 0 ) { // no intersection, background color
    if ( background )
      resultColor = background->getColor( ray.direction );
    else {
      resultColor = TColor::black;
    }
  }
  else { // intersection, compute texture, diffuse, reflective, transmittive
    if ( !globalSettings::shadingOn ) { // no shading, return base color
      if ( ray.intersection.texture == NULL
	   || ray.intersection.texture->pigment == NULL ) {
	resultColor = TColor::black;
      }
      else
	resultColor = ray.intersection.texture->pigment->simple;
    }
    else {
      if ( ray.intersection.texture == NULL
	   || ray.intersection.texture->finish == NULL
	   || ray.intersection.texture->pigment == NULL ) // no texture, black
	resultColor = TColor::black;
      // else, compute diffuse, ambient, reflective, transmittive
      else {
	// compute primitive color: already computed, stored in ray.intersection.color
	const TFinish *intersectionFinish = ray.intersection.texture->finish;
	// compute ambient color
	if ( globalSettings::ambientOn
	     && ambient != NULL
	     && intersectionFinish->ambientFactor > 0.0f ) {
	  stats.increase( TStatistic::STATISTIC_AMBIENT );
	  resultColor += ( ambient->getColor( ray.intersection.pointWorldSpace )
			   * intersectionFinish->ambientFactor );
	}

	// compute diffuse color
	if ( globalSettings::diffuseOn
	     && intersectionFinish->diffuseFactor > 0.0f ) {
	  u32 lightsId = (ray.id >> 4) & 15; // get light ids
	  u32 i = 0;
	  for ( vector<TLight *>::const_iterator iter = lights.begin();
		iter != lights.end();
		iter++, i++ )
	    if ( lightsId & ( 1 << i ) ) { // lighten by this light
	      TLight *light = *iter;
	      resultColor += light->getColor( *this, ray, *ray.lightRays[i] );
	    }
	}

	// compute reflected color
	if ( globalSettings::reflectionOn
	     && reflectionDepth+1 < globalSettings::maxReflectionDepth
	     && ray.reflectedRay != NULL
	     && intersectionFinish->reflectionFactor > 0.0f ) {
	  reflectionDepth++;
	  resultColor += shadeRay( *ray.reflectedRay ) * intersectionFinish->reflectionFactor;
	  reflectionDepth--;
	}

	// compute transmitted color
	if ( globalSettings::transmissionOn
	     && transmissionDepth+1 < globalSettings::maxTransmissionDepth
	     && ray.transmittedRay != NULL
	     && intersectionFinish->transmissionFactor > 0.0f
	     && ray.intersection.texture->interior != NULL ) {
	  transmissionDepth++;
	  resultColor += shadeRay( *ray.transmittedRay ) * intersectionFinish->transmissionFactor;
	  transmissionDepth--;
	}

	// multiply result with primitive color at intersection
	resultColor = resultColor * ray.intersection.texture->pigment->getColor( ray.intersection.pointWorldSpace );
      }
    }
  }    
  // compute atmospheric color
  if ( globalSettings::atmosphericOn
       && atmospheric != NULL ) {
    stats.increase( TStatistic::STATISTIC_ATMOSPHERIC );
    resultColor = atmospheric->addColor( resultColor, ray.intersection );
  }
    
  return resultColor;
}
