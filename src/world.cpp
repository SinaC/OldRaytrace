#include <stdio.h>

#include "world.h"
#include "cameras/camera.h"
#include "ior.h"
#include "object.h"
#include "partitions/kdtree.h"
#include "intersection.h"
#include "ray.h"
#include "lights/light.h"
#include "primitives/primitive.h"
#include "ambients/ambient.h"
#include "pigments/pigment.h"
#include "globals.h"
#include "boundings/bounding.h"
#include "atmospherics/atmospheric.h"
#include "animators/animator.h"
#include "partitions/uniformgrid.h"

#define __DEBUG__

const TTagList TWorld::worldTags [] = {
  { "objects", TAG_world_objects },
  { "cameras", TAG_world_cameras },
  { "lights", TAG_world_lights },
  { "currentCamera", TAG_world_currentCamera },
  { "ambient", TAG_world_ambient },
  { "background", TAG_world_background },
  { "atmospheric", TAG_world_atmospheric },
  { "ior", TAG_world_atmosphereIOR },
  { "animator", TAG_world_animator },
  { NULL, -1 }
};

TWorld::TWorld() {
  THandle::THandle(); handleKind = HANDLE_WORLD;
  currentCamera = NULL;
  atmosphereIOR = IOR_AIR;
  ambient = NULL;
  background = NULL;
  atmospheric = NULL;
  animator = NULL;

  kdtree = NULL;
  ugrid = NULL;
}

TWorld::~TWorld() {
  objects.clear();
  cameras.clear();
  lights.clear();
  ambient = NULL;
  background = NULL;
  currentCamera = NULL;
  //  if ( kdtree != NULL )
  //    delete kdtree;
  //  if ( ugrid != NULL )
  //    delete ugrid;
}

void TWorld::setup() {
  if ( objects.size() == 0 )
    error("TWorld::setup: NO OBJECTS\n");
  for ( vector<TObject *>::const_iterator iterO = objects.begin();
	iterO != objects.end();
	iterO++ )
    (*iterO)->setup();

  if ( cameras.size() == 0 )
    error("TWorld::setup: NO CAMERAS\n");
  for ( vector<TCamera *>::const_iterator iterC = cameras.begin();
	iterC != cameras.end();
	iterC++ )
    (*iterC)->setup();

  for ( vector<TLight *>::const_iterator iterL = lights.begin();
	iterL != lights.end();
	iterL++ )
    (*iterL)->setup();
  if ( !currentCamera )
    currentCamera = *cameras.begin();
  if ( ambient )
    ambient->setup();
  if ( atmospheric )
    atmospheric->setup();
  if ( background )
    background->setup();
  if ( animator )
    animator->setup();
  fflush(stdout);

  // use partitions ?
  if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_KDTREE ) ) {
    kdtree = new KDTree();
    // compute kd-tree
    kdtree->build( objects );
  }
  else if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_UNIFORM_GRID ) ) {
    ugrid = new UniformGrid;
    // get world size
    TPoint3 wMin = TPoint3( INFINITE, INFINITE, INFINITE );
    TPoint3 wMax = TPoint3( -INFINITE, -INFINITE, -INFINITE );
    for ( vector<TObject *>::const_iterator iterO = objects.begin();
	  iterO != objects.end();
	  iterO++ ) {
      TObject *obj = (*iterO);
      TPrimitive *prim = obj->primitive;
      BoundingAABB pBox;
      if ( !prim->computeBoundingAABB( pBox ) )
	obj->error("TWorld::setup: cannot construct primitive aabb\n");
      for ( int i = 0; i < 3; i++ ) {
	if ( pBox.m_Min[i] < wMin[i] ) wMin[i] = pBox.m_Min[i];
	if ( pBox.m_Max[i] > wMax[i] ) wMax[i] = pBox.m_Max[i];
      }
    }
    BoundingAABB wBox;
    wBox.m_Min = wMin;
    wBox.m_Max = wMax;
    // compute uniform grid
    if ( !ugrid->build( objects, 2, wBox ) )
      error("TWorld::setup: problem while constructing uniform grid");
  }
}

void TWorld::precalcFrame() {
  for ( vector<TObject *>::const_iterator iterO = objects.begin();
	iterO != objects.end();
	iterO++ )
    (*iterO)->primitive->precalcFrame( *this );

  for ( vector<TCamera *>::const_iterator iterC = cameras.begin();
	iterC != cameras.end();
	iterC++ )
    (*iterC)->precalcFrame( *this );

  for ( vector<TLight *>::const_iterator iterL = lights.begin();
	iterL != lights.end();
	iterL++ )
    (*iterL)->precalcFrame( *this );
  fflush(stdout);
}

void TWorld::print( const char *msg ) const {
  printf("%s\n", msg );
  for ( vector<TObject *>::const_iterator iterO = objects.begin();
	iterO != objects.end();
	iterO++ )
    (*iterO)->print( "OBJECT" );
  fflush(stdout);

  for ( vector<TCamera *>::const_iterator iterC = cameras.begin();
	iterC != cameras.end();
	iterC++ )
    (*iterC)->print( "CAMERA" );
  fflush(stdout);

  for ( vector<TLight *>::const_iterator iterL = lights.begin();
	iterL != lights.end();
	iterL++ )
    (*iterL)->print( "LIGHT" );
  fflush(stdout);

  if ( currentCamera )
    currentCamera->print( "CURRENT CAMERA:" );
  fflush(stdout);

  if ( ambient )
    ambient->print("AMBIENT LIGHT:");
  fflush(stdout);
  if ( background )
    background->print("BACKGROUND: ");
  fflush(stdout);
  if ( atmospheric )
    atmospheric->print("ATMOSPHERIC: ");
  fflush(stdout);
  if ( animator )
    animator->print("ANIMATOR: ");
  fflush(stdout);

  if ( kdtree != NULL )
    kdtree->print("=======\n"
		  "KD-TREE\n"
		  "=======");
  fflush(stdout);

  if ( ugrid != NULL )
    ugrid->print("=====\n"
		 "UGRID\n"
		 "=====");
  fflush(stdout);
}

int TWorld::findClosestIntersectionCamera( const TRay &ray,
					   TIntersection &intersection ) const {
  stats.increase( TStatistic::STATISTIC_FIND_CLOSEST_INTERSECTION );
  if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_KDTREE )
       && kdtree != NULL ) { // if kd-tree exists: use it
    float dist;
    TObject *obj;
    bool found =  kdtree->findClosestIntersectionCamera( ray, obj, dist );
    if ( found ) {
      intersection.object = obj;
      intersection.distance = dist;
      intersection.pointWorldSpace = ray.origin + ( dist * ray.direction );
      return 1;
    }
    return 0;
  }
  else if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_UNIFORM_GRID )
       && ugrid != NULL ) { // if uniform grid exists: use it
    float dist;
    TObject *obj;
    bool found =  ugrid->findClosestIntersectionCamera( ray, obj, dist );
    if ( found ) {
      intersection.object = obj;
      intersection.distance = dist;
      intersection.pointWorldSpace = ray.origin + ( dist * ray.direction );
      return 1;
    }
    return 0;
  }
  else { // else: test intersections with each objects
    float minDist = INFINITE;
    TObject *currentObject = NULL;
    bool found = false;
    int objId = -1;
    int i = 0;
    for ( vector<TObject *>::const_iterator iter = objects.begin();
	  iter != objects.end();
	  iter++, i++ ) {
      TObject *obj = *iter;

      // intersects bounding box?
      if ( obj->bounding != NULL
	   && !obj->bounding->intersectsRay( ray ) )
	continue;

      stats.increase( TStatistic::STATISTIC_INTERSECTION );
      if ( obj->primitive->intersectionRayCamera( ray, intersection ) ) { // found an intersection with that object
	if ( intersection.distance < minDist
	     && intersection.distance < MAX_DISTANCE ) { // new intersection is closer
	  minDist = intersection.distance;
	  currentObject = obj;
	  objId = i;
	  found = true;
	}
      }
    }
    if ( !found )
      return 0;
    else {
      intersection.pointWorldSpace = ray.origin + ( minDist * ray.direction );
      intersection.distance = minDist;
      intersection.object = currentObject;
      return objId+1;
    }
  }
}
int TWorld::findClosestIntersection( const TRay &ray,
				      TIntersection &intersection ) const {
  // if ray starts from camera: use optimized intersection test
  if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_FIRST_RAY )
       && ray.kind == TRay::RAY_CAMERA )
    return findClosestIntersectionCamera( ray, intersection );

  stats.increase( TStatistic::STATISTIC_FIND_CLOSEST_INTERSECTION );
  if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_KDTREE )
       && kdtree != NULL ) { // if kd-tree exists: use it
    float dist;
    TObject *obj;
    bool found =  kdtree->findClosestIntersection( ray, obj, dist );
    if ( found ) {
      intersection.object = obj;
      intersection.distance = dist;
      intersection.pointWorldSpace = ray.origin + ( dist * ray.direction );
      return 1;
    }
    return 0;
  }
  else if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_UNIFORM_GRID )
       && ugrid != NULL ) { // if uniform grid exists: use it
    float dist;
    TObject *obj;
    bool found =  ugrid->findClosestIntersection( ray, obj, dist );
    if ( found ) {
      intersection.object = obj;
      intersection.distance = dist;
      intersection.pointWorldSpace = ray.origin + ( dist * ray.direction );
      
      intersection.object->print("INTERSECTED OBJECT:");
      return 1;
    }
    return 0;
  }
  else { // else: test intersections with each objects
    float minDist = INFINITE;
    TObject *currentObject = NULL;
    bool found = false;
    int objId = -1;
    int i = 0;
    TIntersection localIntersection;
    for ( vector<TObject *>::const_iterator iter = objects.begin();
	  iter != objects.end();
	  iter++, i++ ) {
      TObject *obj = *iter;

      // intersects bounding box?
      if ( obj->bounding != NULL
	   && !obj->bounding->intersectsRay( ray ) )
	continue;

      stats.increase( TStatistic::STATISTIC_INTERSECTION );
      if ( obj->primitive->intersectionRay( ray, localIntersection ) ) // found an intersection with that object
	if ( localIntersection.distance < minDist ) { // new intersection is closer
	  intersection.copyIntersection( localIntersection );
	  minDist = localIntersection.distance;
	  currentObject = obj;
	  objId = i;
	  found = true;
	}
    }
    if ( !found )
      return 0;
    else {
      intersection.pointWorldSpace = ray.origin + ( minDist * ray.direction );
      intersection.distance = minDist;
      intersection.object = currentObject;
      return objId+1;
    }
  }
}

bool TWorld::findFirstIntersection( const TRay &ray,
				    const TIntersection &intersection,
				    const float maxDist2,
				    const TObject *&shadowCache ) const {
  stats.increase( TStatistic::STATISTIC_FIND_FIRST_INTERSECTION );
  // first, we test shadow cache
  if ( globalSettings::shadowCacheOn
       && shadowCache != NULL ) {
    stats.increase( TStatistic::STATISTIC_INTERSECTS );
       //--       && !( shadowCache->additional.isSet( TPrimitive::PRIMITIVE_CAST_NO_SHADOW ) )
       //--       && !( shadowCache == intersection.object->primitive
       //--	     && intersection.primitive->additional.isSet( TPrimitive::PRIMITIVE_CAST_NO_SELF_SHADOW ) )
    if ( shadowCache->primitive->intersectsRay( ray, maxDist2 ) )
      return true;
  }

  if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_KDTREE )
       && kdtree != NULL ) { // if kd-tree exists: use it
    TObject *obj;
    if ( kdtree->findFirstIntersection( ray, maxDist2, obj ) ) {
      shadowCache = obj;
      return true;
    }
  }
  else if ( globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_UNIFORM_GRID )
	    && ugrid != NULL ) { // if uniform grid exists: use it
    TObject *obj;
    if ( ugrid->findFirstIntersection( ray, maxDist2, obj ) ) {
      shadowCache = obj;
      return true;
    }
  }
  else { // else: test intersections with each object
    for ( vector<TObject *>::const_iterator iter = objects.begin();
	  iter != objects.end();
	  iter++ ) {
      TObject *obj = *iter;
      
      // same object as shadow cache
      if ( globalSettings::shadowCacheOn
	   && obj == shadowCache )
	continue;
      
      // intersects bounding box?
      if ( obj->bounding != NULL
	   && !obj->bounding->intersectsRay( ray ) )
	continue;
      
      stats.increase( TStatistic::STATISTIC_INTERSECTS );
      if ( obj->primitive->intersectsRay( ray, maxDist2 ) ) { // found an intersection with that object
	shadowCache = obj;
	return true;
      }
    }
  }
  return false;
}

TColor TWorld::renderRay( const TPoint3 &a_Origin, const TVector3 &a_Direction ) const {
  static TRay ray;
  ray.clear();
  ray.origin = a_Origin;
  ray.direction = a_Direction;
  ray.update();
  ray.kind = TRay::RAY_CAMERA;

#ifdef __DEBUG__
  printf("TWorld::renderRay\n");
  ray.origin.print("ray origin: ");
  ray.direction.print("ray direction: ");
#endif

  // advance ray.origin to ugrid bounding box boundary
  if ( ugrid != NULL
       && globalSettings::optimizations.isSet( globalSettings::OPTIMIZE_UNIFORM_GRID ) ) {
#ifdef __DEBUG__
    ugrid->m_Extends.print("world extends");
#endif
    if ( !ugrid->m_Extends.inside( ray.origin ) ) {
#ifdef __DEBUG__
      printf("origin not inside extends -> recomputing origin\n");
#endif
      float dist = INFINITE;
      if ( ugrid->m_Extends.intersectionRay( ray.origin, ray.direction, ray.reverseDirection, dist ) ) {
	ray.origin = ray.origin + ( ( dist + EPS ) * ray.direction );
#ifdef __DEBUG__
	printf("distance to box: %5.5f\n", dist );
	ray.origin.print("new origin: ");
#endif
      }
      else {
#ifdef __DEBUG__
	printf("ray doesn't intersect world extends\n");
#endif
	return TColor::black;
      }
    }
  }
  TColor rayColor = traceRay( ray, 1.0f );

  rayColor.print("ray color: ");

  return rayColor;
}
