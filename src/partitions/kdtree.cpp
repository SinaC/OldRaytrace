#include <stdio.h>
#include <string.h>

#include "kdtree.h"
#include "../ray.h"
#include "../object.h"
#include "../text/basics.h"
#include "../poly3d/rendersimple.h"
#include "../intersection.h"
#include "../boundings/aabb.h"
#include "../primitives/primitive.h"

//--#define __DEBUG__


// Stack operations.
void KDTree::TStack::init() {
  stack[0].node = NULL;
  stackPtr = 1;
}

void KDTree::TStack::push( BinNode *node, float min, float max ) {
  stack[stackPtr].node = node;
  stack[stackPtr].min = min;
  stack[stackPtr].max = max;
  (stackPtr)++;
}

void KDTree::TStack::pop( BinNode *&node, float &min, float &max ) {
  (stackPtr)--;
  node = stack[stackPtr].node;
  min = stack[stackPtr].min;
  max = stack[stackPtr].max;
}


// Returns the distance between origin and plane, measured along the input 
// direction. direction is a unit vector.
// Entry:
//   plane     - subdivision plane of current node
//   origin    - origin of the ray
//   direction - direction of the ray, must be a unit vector
// Exit:
//   returns the distance between the origin and plane measured along
//   the direction
//
// Note:
//   there is a function for each of the three subdivision planes
static float distanceToXPlane( const TPoint3 &plane, const TRay &ray ) {
#ifdef __DEBUG__
  printf("XPlane\n");
#endif
  return ( plane[__X] - ray.origin[__X] ) * ray.reverseDirection[__X];
}

static float distanceToYPlane( const TPoint3 &plane, const TRay &ray ) {
#ifdef __DEBUG__
  printf("YPlane\n");
#endif
  return ( plane[__Y] - ray.origin[__Y] ) * ray.reverseDirection[__Y];
}

static float distanceToZPlane( const TPoint3 &plane, const TRay &ray ) {
#ifdef __DEBUG__
  printf("ZPlane\n");
#endif
  return ( plane[__Z] - ray.origin[__Z] ) * ray.reverseDirection[__Z];
}

// Determines which of the half space of the two children contains origin, return 
// that child as near, the other as far.
// Entry:
//   currentNode - node currently working on
//   origin      - origin of the ray
// Exit:
//   nearNode - node whose half plane contains the origin
//   farNode  - node whose half plane does not contain the origin
// Note:
//   there is a function for each of the three subdivision planes
static void getXChildren( BinNode *currentNode, const TPoint3 &origin,
			  BinNode *&nearNode, BinNode *&farNode ) {

  // remember that child[0]->max or child[1]->min is the subdivision plane
  if ( currentNode->child[0]->max[__X] >= origin[__X] ) {
    nearNode = currentNode->child[0];
    farNode = currentNode->child[1];
  }
  else {
    farNode = currentNode->child[0];
    nearNode = currentNode->child[1];
  }
}

static void getYChildren( BinNode *currentNode, const TPoint3 &origin,
			  BinNode *&nearNode, BinNode *&farNode ) {

  // remember that child[0]->max or child[1]->min is the subdivision plane
  if ( currentNode->child[0]->max[__Y] >= origin[__Y] ) {
    nearNode = currentNode->child[0];
    farNode = currentNode->child[1];
  }
  else {
    farNode = currentNode->child[0];
    nearNode = currentNode->child[1];
  }
}

static void getZChildren( BinNode *currentNode, const TPoint3 &origin,
			  BinNode *&nearNode, BinNode *&farNode ) {

  // remember that child[0]->max or child[1]->min is the subdivision plane
  if ( currentNode->child[0]->max[__Z] >= origin[__Z] ) {
    nearNode = currentNode->child[0];
    farNode = currentNode->child[1];
  }
  else {
    farNode = currentNode->child[0];
    nearNode = currentNode->child[1];
  }
}

// Some miscellaneous supporting functions.
bool BinNode::leaf() const {
  return (child[0] == NULL);
}

bool BinNode::pointInNode( const TPoint3 &pt ) const {
  return ((pt[__X] >= min[__X] ) && (pt[__Y] >= min[__Y] ) && 
	  (pt[__Z] >= min[__Z] ) && (pt[__X] <= max[__X] ) && 
	  (pt[__Y] <= max[__Y] ) && (pt[__Z] <= max[__Z] ));
}

bool BinNode::geomInNode( const TObject *obj ) const {
  BoundingAABB box;
  box.m_Min = min;
  box.m_Max = max;
  if ( obj->bounding )
    return obj->bounding->intersectsBox( box );
  else
    return obj->primitive->intersectsBox( box );
}

static void pointAtDistance( const TRay &ray, const float distance,
			     TPoint3 &pt ) {
  pt[__X] = ray.origin[__X] + distance * ray.direction[__X];
  pt[__Y] = ray.origin[__Y] + distance * ray.direction[__Y];
  pt[__Z] = ray.origin[__Z] + distance * ray.direction[__Z];
}

// Minimal intersection depth.
#define DEPTH_TOLERANCE 1.0e-6
// Two values are equal if their difference is small than CLOSE_TOLERANCE.
#define CLOSE_TOLERANCE 1.0e-6
// Side ID
static const enum BoxSide {
  SIDE_X_MIN,     // x < 0
  SIDE_X_MAX,     // x > 0
  SIDE_Y_MIN,     // y < 0
  SIDE_Y_MAX,     // y > 0
  SIDE_Z_MIN,     // z < 0
  SIDE_Z_MAX      // z > 0
};

static bool rayBoxIntersect( const TRay &ray, const TPoint3 &min, const TPoint3 &max,
			     float &returnMin, float &returnMax ) {
  //    This routine intersects the ray with the box 
  //    defined by min and max, returns the intersection 
  //    status. If ray successfully intersects the box, 
  //    then this routine also returns the distances 
  //    (from the ray origin) to the two points that the
  //    ray intersects the box on.
  float tymin, tymax, tzmin, tzmax;
  if ( ray.direction[__X] >= 0 ) {
    returnMin = ( min[__X] - ray.origin[__X] ) * ray.reverseDirection[__X];
    returnMax = ( max[__X] - ray.origin[__X] ) * ray.reverseDirection[__X];
  }
  else {
    returnMin = ( max[__X] - ray.origin[__X] ) * ray.reverseDirection[__X];
    returnMax = ( min[__X] - ray.origin[__X] ) * ray.reverseDirection[__X];
  }
  if ( ray.direction[__Y] >= 0 ) {
    tymin = ( min[__Y] - ray.origin[__Y] ) * ray.reverseDirection[__Y];
    tymax = ( max[__Y] - ray.origin[__Y] ) * ray.reverseDirection[__Y];
  }
  else {
    tymin = ( max[__Y] - ray.origin[__Y] ) * ray.reverseDirection[__Y];
    tymax = ( min[__Y] - ray.origin[__Y] ) * ray.reverseDirection[__Y];
  }
  if ( ( returnMin > tymax ) || ( tymin > returnMax ) )
    return false;
  if ( tymin > returnMin )
    returnMin = tymin;
  if ( tymax < returnMax )
    returnMax = tymax;
  if ( ray.direction[__Z] >= 0 ) {
    tzmin = ( min[__Z] - ray.origin[__Z] ) * ray.reverseDirection[__Z];
    tzmax = ( max[__Z] - ray.origin[__Z] ) * ray.reverseDirection[__Z];
  }
  else {
    tzmin = ( max[__Z] - ray.origin[__Z] ) * ray.reverseDirection[__Z];
    tzmax = ( min[__Z] - ray.origin[__Z] ) * ray.reverseDirection[__Z];
  }
  if ( ( returnMin > tzmax ) || ( tzmin > returnMax ) )
    return false;
  if ( tzmin > returnMin )
    returnMin = tzmin;
  if ( tzmax < returnMax )
    returnMax = tzmax;
  return true;

//--OLD WAY--  int smin = -1, smax = -1; // Side hit for min/max intersection.
//--OLD WAY--  float t;
//--OLD WAY--  returnMin = 0.0f;
//--OLD WAY--  returnMax = INFINITE;
//--OLD WAY--
//--OLD WAY--  TVector3 P = ray.origin;
//--OLD WAY--  TVector3 D = ray.direction;
//--OLD WAY--  TVector3 rD = ray.reverseDirection;
//--OLD WAY--
//--OLD WAY--  // Sides first.
//--OLD WAY--  if ( D[__X] < -EPS) {
//--OLD WAY--    t = ( min[__X] - P[__X] ) * rD[__X];
//--OLD WAY--    if ( t < returnMin )
//--OLD WAY--      return false;
//--OLD WAY--    if ( t <= returnMax ) {
//--OLD WAY--      smax = SIDE_X_MIN;
//--OLD WAY--      returnMax = t;
//--OLD WAY--    }
//--OLD WAY--
//--OLD WAY--    t = ( max[__X] - P[__X] ) * rD[__X];
//--OLD WAY--    if ( t >= returnMin ) {
//--OLD WAY--      if ( t > returnMax )
//--OLD WAY--	return false;
//--OLD WAY--      smin = SIDE_X_MAX;
//--OLD WAY--      returnMin = t;
//--OLD WAY--    }
//--OLD WAY--  }
//--OLD WAY--  else if ( D[__X] > EPS ) {
//--OLD WAY--    t = ( max[__X] - P[__X] ) * rD[__X];
//--OLD WAY--    if ( t < returnMin )
//--OLD WAY--      return false;
//--OLD WAY--    if ( t <= returnMax ) {
//--OLD WAY--      smax = SIDE_X_MAX;
//--OLD WAY--      returnMax = t;
//--OLD WAY--    }
//--OLD WAY--    
//--OLD WAY--    t = (min[__X] - P[__X]) * rD[__X];
//--OLD WAY--    if ( t >= returnMin ) {
//--OLD WAY--      if ( t > returnMax )
//--OLD WAY--	return false;
//--OLD WAY--      smin = SIDE_X_MIN;
//--OLD WAY--      returnMin = t;
//--OLD WAY--    }
//--OLD WAY--  }
//--OLD WAY--  else if ( ( P[__X] < min[__X] ) || ( P[__X] > max[__X] ) )
//--OLD WAY--    return false;
//--OLD WAY--
//--OLD WAY--  // Check Top/Bottom.
//--OLD WAY--  if ( D[__Y] < -EPS ) {
//--OLD WAY--    t = ( min[__Y] - P[__Y] ) * rD[__Y];
//--OLD WAY--    if ( t < returnMin )
//--OLD WAY--      return false;
//--OLD WAY--
//--OLD WAY--    if ( t <= returnMax - CLOSE_TOLERANCE ) {
//--OLD WAY--      smax = SIDE_Y_MIN;
//--OLD WAY--      returnMax = t;
//--OLD WAY--    }
//--OLD WAY--    else
//--OLD WAY--      // If intersection points are close to each other find out
//--OLD WAY--      // which side to use, i.e. is most probably hit. [DB 9/94]
//--OLD WAY--      if ( t <= returnMax + CLOSE_TOLERANCE )
//--OLD WAY--        if ( -D[__Y] > fabsf(D[__X] ) )
//--OLD WAY--	  smax = SIDE_Y_MIN;
//--OLD WAY--    
//--OLD WAY--    t = (max[__Y] - P[__Y]) * rD[__Y];
//--OLD WAY--    if ( t >= returnMin + CLOSE_TOLERANCE ) {
//--OLD WAY--      if (t > returnMax)
//--OLD WAY--	return false;
//--OLD WAY--      smin = SIDE_Y_MAX;
//--OLD WAY--      returnMin = t;
//--OLD WAY--    }
//--OLD WAY--    else
//--OLD WAY--      // If intersection points are close to each other find out
//--OLD WAY--      // which side to use, i.e. is most probably hit. [DB 9/94]
//--OLD WAY--      if ( t >= returnMin - CLOSE_TOLERANCE )
//--OLD WAY--        if ( -D[__Y] > fabsf(D[__X]) )
//--OLD WAY--	  smin = SIDE_Y_MAX;
//--OLD WAY--  }
//--OLD WAY--  else if (D[__Y] > EPS ) {
//--OLD WAY--      t = (max[__Y] - P[__Y]) * rD[__Y];
//--OLD WAY--      if ( t < returnMin )
//--OLD WAY--	return false;
//--OLD WAY--      if ( t <= returnMax - CLOSE_TOLERANCE ) {
//--OLD WAY--        smax = SIDE_Y_MAX;
//--OLD WAY--        returnMax = t;
//--OLD WAY--      }
//--OLD WAY--      else
//--OLD WAY--	// If intersection points are close to each other find out
//--OLD WAY--        // which side to use, i.e. is most probably hit. [DB 9/94]
//--OLD WAY--        if ( t <= returnMax + CLOSE_TOLERANCE )
//--OLD WAY--          if ( D[__Y] > fabsf(D[__X]) )
//--OLD WAY--	    smax = SIDE_Y_MAX;
//--OLD WAY--      
//--OLD WAY--      t = ( min[__Y] - P[__Y] ) * rD[__Y];
//--OLD WAY--      if ( t >= returnMin + CLOSE_TOLERANCE ) {
//--OLD WAY--        if ( t > returnMax )
//--OLD WAY--	  return false;
//--OLD WAY--        smin = SIDE_Y_MIN;
//--OLD WAY--        returnMin = t;
//--OLD WAY--      }
//--OLD WAY--      else
//--OLD WAY--	// If intersection points are close to each other find out
//--OLD WAY--        // which side to use, i.e. is most probably hit. [DB 9/94]
//--OLD WAY--        if ( t >= returnMin - CLOSE_TOLERANCE )
//--OLD WAY--          if ( D[__Y] > fabsf(D[__X]) )
//--OLD WAY--	    smin = SIDE_Y_MIN;
//--OLD WAY--  }
//--OLD WAY--  else if ( ( P[__Y] < min[__Y] ) || ( P[__Y] > max[__Y] ) )
//--OLD WAY--    return false;
//--OLD WAY--
//--OLD WAY--  // Now front/back
//--OLD WAY--  if ( D[__Z] < -EPS ) {
//--OLD WAY--    t = ( min[__Z] - P[__Z] ) * rD[__Z];
//--OLD WAY--    if ( t < returnMin )
//--OLD WAY--      return false;
//--OLD WAY--    if ( t <= returnMax - CLOSE_TOLERANCE ) {
//--OLD WAY--      smax = SIDE_Z_MIN;
//--OLD WAY--      returnMax = t;
//--OLD WAY--    }
//--OLD WAY--    else
//--OLD WAY--      // If intersection points are close to each other find out
//--OLD WAY--      // which side to use, i.e. is most probably hit. [DB 9/94]
//--OLD WAY--      if ( t <= returnMax + CLOSE_TOLERANCE )
//--OLD WAY--        switch (smax) {
//--OLD WAY--	case SIDE_X_MIN :
//--OLD WAY--	case SIDE_X_MAX :
//--OLD WAY--	  if ( -D[__Z] > fabsf(D[__X]) )
//--OLD WAY--	    smax = SIDE_Z_MIN;
//--OLD WAY--	  break;
//--OLD WAY--	case SIDE_Y_MIN :
//--OLD WAY--	case SIDE_Y_MAX :
//--OLD WAY--	  if ( -D[__Z] > fabsf(D[__Y]) )
//--OLD WAY--	    smax = SIDE_Z_MIN;
//--OLD WAY--	  break;
//--OLD WAY--	}
//--OLD WAY--    
//--OLD WAY--    t = ( max[__Z] - P[__Z] ) * rD[__Z];
//--OLD WAY--    if ( t >= returnMin + CLOSE_TOLERANCE )  {
//--OLD WAY--      if ( t > returnMax )
//--OLD WAY--	return false;
//--OLD WAY--      smin = SIDE_Z_MAX;
//--OLD WAY--      returnMin = t;
//--OLD WAY--    }
//--OLD WAY--    else
//--OLD WAY--      // If intersection points are close to each other find out
//--OLD WAY--      // which side to use, i.e. is most probably hit. [DB 9/94]
//--OLD WAY--      if ( t >= returnMin - CLOSE_TOLERANCE )
//--OLD WAY--        switch (smin)  {
//--OLD WAY--          case SIDE_X_MIN :
//--OLD WAY--          case SIDE_X_MAX :
//--OLD WAY--	    if ( -D[__Z] > fabsf(D[__X]) )
//--OLD WAY--	      smin = SIDE_Z_MAX;
//--OLD WAY--	    break;
//--OLD WAY--          case SIDE_Y_MIN :
//--OLD WAY--          case SIDE_Y_MAX :
//--OLD WAY--	    if ( -D[__Z] > fabsf(D[__Y]) )
//--OLD WAY--	      smin = SIDE_Z_MAX;
//--OLD WAY--	    break;
//--OLD WAY--        }
//--OLD WAY--  }
//--OLD WAY--  else if ( D[__Z] > EPS ) {
//--OLD WAY--      t = ( max[__Z] - P[__Z] ) * rD[__Z];
//--OLD WAY--      if ( t < returnMin )
//--OLD WAY--	return false;
//--OLD WAY--      if ( t <= returnMax - CLOSE_TOLERANCE ) {
//--OLD WAY--        smax = SIDE_Z_MAX;
//--OLD WAY--        returnMax = t;
//--OLD WAY--      }
//--OLD WAY--      else
//--OLD WAY--	// If intersection points are close to each other find out
//--OLD WAY--	// which side to use, i.e. is most probably hit. [DB 9/94]
//--OLD WAY--        if ( t <= returnMax + CLOSE_TOLERANCE )
//--OLD WAY--          switch ( smax ) {
//--OLD WAY--            case SIDE_X_MIN :
//--OLD WAY--            case SIDE_X_MAX :
//--OLD WAY--	      if ( D[__Z] > fabsf(D[__X]) )
//--OLD WAY--		smax = SIDE_Z_MAX;
//--OLD WAY--	      break;
//--OLD WAY--            case SIDE_Y_MIN :
//--OLD WAY--            case SIDE_Y_MAX :
//--OLD WAY--	      if ( D[__Z] > fabsf(D[__Y]) )
//--OLD WAY--		smax = SIDE_Z_MAX;
//--OLD WAY--	      break;
//--OLD WAY--          }
//--OLD WAY--
//--OLD WAY--      t = ( min[__Z] - P[__Z]) * rD[__Z];
//--OLD WAY--      if ( t >= returnMin + CLOSE_TOLERANCE ) {
//--OLD WAY--        if ( t > returnMax )
//--OLD WAY--	  return false;
//--OLD WAY--        smin = SIDE_Z_MIN;
//--OLD WAY--        returnMin = t;
//--OLD WAY--      }
//--OLD WAY--      else
//--OLD WAY--	// If intersection points are close to each other find out
//--OLD WAY--	// which side to use, i.e. is most probably hit. [DB 9/94]
//--OLD WAY--        if ( t >= returnMin - CLOSE_TOLERANCE )
//--OLD WAY--          switch (smin) {
//--OLD WAY--            case SIDE_X_MIN :
//--OLD WAY--            case SIDE_X_MAX :
//--OLD WAY--	      if ( D[__Z] > fabsf(D[__X]) )
//--OLD WAY--		smin = SIDE_Z_MIN;
//--OLD WAY--	      break;
//--OLD WAY--            case SIDE_Y_MIN :
//--OLD WAY--            case SIDE_Y_MAX :
//--OLD WAY--	      if ( D[__Z] > fabsf(D[__Y]) )
//--OLD WAY--		smin = SIDE_Z_MIN;
//--OLD WAY--	      break;
//--OLD WAY--          }
//--OLD WAY--  }
//--OLD WAY--  else if ( ( P[__Z] < min[__Z]) || (P[__Z] > max[__Z]) )
//--OLD WAY--    return false;
//--OLD WAY--
//--OLD WAY--  if ( returnMin < DEPTH_TOLERANCE ) { // ray.origin was inside box
//--OLD WAY--    if ( returnMax < DEPTH_TOLERANCE )
//--OLD WAY--      return false;
//--OLD WAY--    returnMin = -INFINITE;
//--OLD WAY--    smin = smax; // TO DO
//--OLD WAY--  }
//--OLD WAY--
//--OLD WAY--  return true;
}

static bool rayObjClosestIntersectCamera( const TRay &ray, const vector<TObject *> &objList,
					  TObject *&obj, float &distance ) {
  //     This routine intersects ray with all of the objects 
  //     in the objList and returns the closest intersection 
  //     distance and the interesting object, if there is one.
  distance = INFINITE;
  for ( vector<TObject *>::const_iterator iter = objList.begin();
	iter != objList.end();
	iter++ ) {
    TObject *object = *iter;
#ifdef __DEBUG__
    object->print("object in node");
#endif
    TIntersection inter;
    if ( object->primitive->intersectionRayCamera( ray, inter ) ) { // if ray intersects object
#ifdef __DEBUG__
      printf("distance: %5.5f\n", inter.distance );
#endif
      if ( inter.distance < distance ) { // if this object is closest than previous
#ifdef __DEBUG__
	printf("closer distance: %5.5f   distance: %5.5f\n", inter.distance, distance );
#endif
	distance = inter.distance; // store new distance and this object
	obj = object;
      }
    }
#ifdef __DEBUG__
    else
      printf("no intersection with this object\n");
#endif
  }

  if ( distance < INFINITE ) { // if ray intersects at least one object
#ifdef __DEBUG__
    printf("found an intersection\n" );
#endif
    return true;
  }

#ifdef __DEBUG__
  printf("no intersection found in this node\n");
#endif

  return false;
}

static bool rayObjClosestIntersect( const TRay &ray, const vector<TObject *> &objList,
				    TObject *&obj, float &distance ) {
  //     This routine intersects ray with all of the objects 
  //     in the objList and returns the closest intersection 
  //     distance and the interesting object, if there is one.
  distance = INFINITE;
  for ( vector<TObject *>::const_iterator iter = objList.begin();
	iter != objList.end();
	iter++ ) {
    TObject *object = *iter;
#ifdef __DEBUG__
    object->print("object in node");
#endif
    TIntersection inter;
    if ( object->primitive->intersectionRay( ray, inter ) ) { // if ray intersects object
#ifdef __DEBUG__
      printf("distance: %5.5f\n", inter.distance );
#endif
      if ( inter.distance < distance ) { // if this object is closest than previous
#ifdef __DEBUG__
	printf("closer distance: %5.5f   distance: %5.5f\n", inter.distance, distance );
#endif
	distance = inter.distance; // store new distance and this object
	obj = object;
      }
    }
#ifdef __DEBUG__
    else
      printf("no intersection with this object\n");
#endif
  }

  if ( distance < INFINITE ) { // if ray intersects at least one object
#ifdef __DEBUG__
    printf("found an intersection\n" );
#endif
    return true;
  }

#ifdef __DEBUG__
  printf("no intersection found in this node\n");
#endif

  return false;
}

static bool rayObjIntersect( const TRay &ray, const vector<TObject *> &objList,
			     const float maxDist2, TObject *&obj ) {
  //     This routine intersects ray with all of the objects 
  //     in the objList and returns the first intersected object
  for ( vector<TObject *>::const_iterator iter = objList.begin();
	iter != objList.end();
	iter++ ) {
    TObject *object = *iter;
#ifdef __DEBUG__
    object->print("object in node");
#endif
    if ( object->primitive->intersectsRay( ray, maxDist2 ) ) { // if ray intersects object
      obj = object; // return this object
      return true;
    }
  }

  return false;
}

// Traverses ray through KDTree and intersects ray with all of the objects along
// the way. Returns the closest intersection distance and the intersecting object
// if there is one. Using intersectionRayCamera
// Entry:
//   ray     - the ray being traced
//   KDTree - the KD tree enclosing the entire environment
// Exit:
//   obj      - the closest object that intersects the ray
//   distance - distance to the intersecting object
bool KDTree::findClosestIntersectionCamera( const TRay &ray,
					    TObject *&obj, float &distance ) const {
  TStack *stack;
  BinNode *currentNode, 
    *nearChild, *farChild;
  float dist, _min, _max;
  TPoint3 p;

#ifdef __DEBUG__
  ray.origin.print("ray origin");
  ray.direction.print("ray direction");
  ray.reverseDirection.print("ray reverse direction");
#endif

  // test if the whole KD tree is missed by the input ray
  if ( !rayBoxIntersect( ray, min, max, _min, _max ) ) {
#ifdef __DEBUG__
      printf("whole KD tree is missed by ray\n");
#endif
      return false;
  }


#ifdef __DEBUG__
  printf("rayBoxIntersect: %5.5f   %5.5f\n", _min, _max );
#endif

  stack = new TStack;
  stack->init();

  currentNode = root;

  while ( currentNode != NULL ) {
    while ( !currentNode->leaf() ) {
#ifdef __DEBUG__
      currentNode->min.print("current node min"); currentNode->max.print("current node max");
      printf("is leaf: %d\n", currentNode->leaf() );
      currentNode->child[0]->max.print("cut plane");
#endif

      dist = currentNode->distanceToDivisionPlane( currentNode->child[0]->max, ray );
      currentNode->getChildren( currentNode, ray.origin, nearChild, farChild );

#ifdef __DEBUG__
      printf("distance to plane: %5.5f\n", dist );
      
      nearChild->min.print("near min"); nearChild->max.print("near max");
      printf("is near leaf: %d\n", nearChild->leaf() );
      farChild->min.print("far min"); farChild->max.print("far max");
      printf("is far leaf: %d\n", farChild->leaf() );
#endif

      if ( ( dist > _max ) || ( dist < 0 ) ) {
#ifdef __DEBUG__
	printf("using near child\n");
#endif
	currentNode = nearChild;
      }
      else if ( dist < _min ) {
#ifdef __DEBUG__
	printf("using far child\n");
#endif
	currentNode = farChild;
      }
      else {
	stack->push( farChild, dist, _max );
#ifdef __DEBUG__
	printf("-->PUSH far  keep near\n");
#endif
	currentNode = nearChild;
	_max = dist;
      }
    }

#ifdef __DEBUG__
    printf("rayObjClosestIntersect:\n");
    currentNode->min.print("currentNode min"); currentNode->max.print("currentNode max");
    printf("is leaf: %d\n", currentNode->leaf() );
#endif
    if ( rayObjClosestIntersectCamera( ray, currentNode->members, obj, distance ) ) {
      pointAtDistance( ray, distance, p );
#ifdef __DEBUG__
      p.print("pointAtDistance: ");
      printf("point in node ?\n");
      currentNode->min.print("currentNode min"); currentNode->max.print("currentNode max");
      printf("is leaf: %d\n", currentNode->leaf() );
#endif
      if ( currentNode->pointInNode( p ) )
	return true;
    }
    stack->pop( currentNode, _min,_max );
#ifdef __DEBUG__
    printf("-->POP\n");
    if ( currentNode ) {
      currentNode->min.print("currentNode min"); currentNode->max.print("currentNode max");
      printf("is leaf: %d\n", currentNode->leaf() );
    }
#endif
  }
  return false;
}


// Traverses ray through KDTree and intersects ray with all of the objects along
// the way. Returns the closest intersection distance and the intersecting object
// if there is one.
// Entry:
//   ray     - the ray being traced
//   KDTree - the KD tree enclosing the entire environment
// Exit:
//   obj      - the closest object that intersects the ray
//   distance - distance to the intersecting object
bool KDTree::findClosestIntersection( const TRay &ray,
				       TObject *&obj, float &distance ) const {
  TStack *stack;
  BinNode *currentNode, 
    *nearChild, *farChild;
  float dist, _min, _max;
  TPoint3 p;

#ifdef __DEBUG__
  ray.origin.print("ray origin");
  ray.direction.print("ray direction");
  ray.reverseDirection.print("ray reverse direction");
#endif

  // test if the whole KD tree is missed by the input ray
  if ( !rayBoxIntersect( ray, min, max, _min, _max ) ) {
#ifdef __DEBUG__
      printf("whole KD tree is missed by ray\n");
#endif
      return false;
  }


#ifdef __DEBUG__
  printf("rayBoxIntersect: %5.5f   %5.5f\n", _min, _max );
#endif

  stack = new TStack;
  stack->init();

  currentNode = root;

  while ( currentNode != NULL ) {
    while ( !currentNode->leaf() ) {
#ifdef __DEBUG__
      currentNode->min.print("current node min"); currentNode->max.print("current node max");
      printf("is leaf: %d\n", currentNode->leaf() );
      currentNode->child[0]->max.print("cut plane");
#endif

      dist = currentNode->distanceToDivisionPlane( currentNode->child[0]->max, ray );
      currentNode->getChildren( currentNode, ray.origin, nearChild, farChild );

#ifdef __DEBUG__
      printf("distance to plane: %5.5f\n", dist );
      
      nearChild->min.print("near min"); nearChild->max.print("near max");
      printf("is near leaf: %d\n", nearChild->leaf() );
      farChild->min.print("far min"); farChild->max.print("far max");
      printf("is far leaf: %d\n", farChild->leaf() );
#endif

      if ( ( dist > _max ) || ( dist < 0 ) ) {
#ifdef __DEBUG__
	printf("using near child\n");
#endif
	currentNode = nearChild;
      }
      else if ( dist < _min ) {
#ifdef __DEBUG__
	printf("using far child\n");
#endif
	currentNode = farChild;
      }
      else {
	stack->push( farChild, dist, _max );
#ifdef __DEBUG__
	printf("-->PUSH far  keep near\n");
#endif
	currentNode = nearChild;
	_max = dist;
      }
    }

#ifdef __DEBUG__
    printf("rayObjClosestIntersect:\n");
    currentNode->min.print("currentNode min"); currentNode->max.print("currentNode max");
    printf("is leaf: %d\n", currentNode->leaf() );
#endif
    if ( rayObjClosestIntersect( ray, currentNode->members, obj, distance ) ) {
      pointAtDistance( ray, distance, p );
#ifdef __DEBUG__
      p.print("pointAtDistance: ");
      printf("point in node ?\n");
      currentNode->min.print("currentNode min"); currentNode->max.print("currentNode max");
      printf("is leaf: %d\n", currentNode->leaf() );
#endif
      if ( currentNode->pointInNode( p ) )
	return true;
    }
    stack->pop( currentNode, _min,_max );
#ifdef __DEBUG__
    printf("-->POP\n");
    if ( currentNode ) {
      currentNode->min.print("currentNode min"); currentNode->max.print("currentNode max");
      printf("is leaf: %d\n", currentNode->leaf() );
    }
#endif
  }
  return false;
}


// Traverses ray through KDTree and intersects ray with all of the objects along
// the way. Returns the first intersecting object
// if there is one.
// Entry:
//   ray     - the ray being traced
//   KDTree - the KD tree enclosing the entire environment
// Exit:
//   obj      - the first object that intersects the ray
bool KDTree::findFirstIntersection( const TRay &ray, const float maxDist2,
				    TObject *&obj ) const {
  TStack *stack;
  BinNode *currentNode, 
    *nearChild, *farChild;
  float dist, _min, _max;
  TPoint3 p;

#ifdef __DEBUG__
  ray.origin.print("ray origin");
  ray.direction.print("ray direction");
  ray.reverseDirection.print("ray reverse direction");
#endif

  // test if the whole KD tree is missed by the input ray
  if ( !rayBoxIntersect(ray, min, max, _min, _max ) )
#ifdef __DEBUG__
    {
      printf("whole KD tree is missed by ray\n");
#endif
      return false;
#ifdef __DEBUG__
    }
#endif


#ifdef __DEBUG__
  printf("rayBoxIntersect: %5.5f   %5.5f\n", _min, _max );
#endif

  stack = new TStack;
  stack->init();

  currentNode = root;

  while ( currentNode != NULL ) {

    while ( !currentNode->leaf() ) {
#ifdef __DEBUG__
      currentNode->min.print("current node min"); currentNode->max.print("current node max");
      printf("is leaf: %d\n", currentNode->leaf() );
      currentNode->child[0]->max.print("cut plane");
#endif

      dist = currentNode->distanceToDivisionPlane( currentNode->child[0]->max, ray );
      currentNode->getChildren( currentNode, ray.origin, nearChild, farChild );

#ifdef __DEBUG__
      printf("distance to plane: %5.5f\n", dist );
      
      nearChild->min.print("near min"); nearChild->max.print("near max");
      printf("is near leaf: %d\n", nearChild->leaf() );
      farChild->min.print("far min"); farChild->max.print("far max");
      printf("is far leaf: %d\n", farChild->leaf() );
#endif

      if ( ( dist > _max ) || ( dist < 0 ) ) {
#ifdef __DEBUG__
	printf("using near child\n");
#endif
	currentNode = nearChild;
      }
      else if ( dist < _min ) {
#ifdef __DEBUG__
	printf("using far child\n");
#endif
	currentNode = farChild;
      }
      else {
	stack->push( farChild, dist, _max );
#ifdef __DEBUG__
	printf("-->PUSH far  keep near\n");
#endif
	currentNode = nearChild;
	_max = dist;
      }
    }

#ifdef __DEBUG__
    printf("rayObjIntersect:\n");
    currentNode->min.print("currentNode min");
    currentNode->max.print("currentNode max");
#endif
    if ( rayObjIntersect( ray, currentNode->members, maxDist2, obj ) )
      return true;
    stack->pop( currentNode, _min,_max );
#ifdef __DEBUG__
    printf("-->POP\n");
    if ( currentNode ) {
      currentNode->min.print("currentNode min"); currentNode->max.print("currentNode max");
      printf("is leaf: %d\n", currentNode->leaf() );
    }
#endif
  }
  return false;
}


// Builds the KD tree by subdividing along the center of x, y, or z bounds, one
// each time this function is called. This function calls itself recursively until
// either the tree is deeper than MaxDepth or all of the tree leaves contains less
// than MaxListLength of objects.
// Entry:
//   node          - node currently working on
//   depth         - current tree depth
//   MaxDepth      - Max allowed tree depth
//   MaxListLength - Max allowed object list length of a leave node
//   axis          - subdivision axis for the children of node
//                   (0-x, 1-y, 2-z)
void BinNode::subdivide( int depth, int MaxDepth, int MaxListLength, int axis ) {
  int i, nextAxis;

  child[0] = child[1] = NULL;

  if ( members.size() > MaxListLength
       && depth < MaxDepth ) {

    for ( i = 0; i < 2; i++ ) {

      child[i] = new BinNode;
      child[i]->min[__X] = min[__X];
      child[i]->min[__Y] = min[__Y];
      child[i]->min[__Z] = min[__Z];
      child[i]->max[__X] = max[__X];
      child[i]->max[__Y] = max[__Y];
      child[i]->max[__Z] = max[__Z];

      if ( axis == 1 ) {
	// current subdivision plane is x
	child[i]->min[__X] = 
	  min[__X] + 0.5 * i * (max[__X] - min[__X]);
	child[i]->max[__X] = 
	  min[__X] + 0.5 * (i+1) * (max[__X] - min[__X]);

	// child subdivision plane will be y
	nextAxis = 2;
	child[i]->distanceToDivisionPlane = distanceToYPlane;
	child[i]->getChildren = getYChildren;

      }
      else if ( axis == 2 ) {
	// current subdivision plane is y
	child[i]->min[__Y] = 
	  min[__Y] + 0.5 * i * ( max[__Y] - min[__Y] );
	child[i]->max[__Y] = 
	  min[__Y] + 0.5 * (i+1) * ( max[__Y] - min[__Y] );

	// child subdivision plane will be z
	nextAxis = 3;
	child[i]->distanceToDivisionPlane = distanceToZPlane;
	child[i]->getChildren = getZChildren;

      }
      else {
	// current subdivision plane is z
	child[i]->min[__Z] = 
	  min[__Z] + 0.5 * i * (max[__Z] - min[__Z]);
	child[i]->max[__Z] = 
	  min[__Z] + 0.5 * (i+1) * (max[__Z] - min[__Z]);

	// child subdivision plane will be x
	nextAxis = 1;
	child[i]->distanceToDivisionPlane = distanceToXPlane;
	child[i]->getChildren = getXChildren;
      }

      // add objects in children
      for ( vector<TObject *>::const_iterator iter = members.begin();
	    iter != members.end();
	    iter++ ) {
	TObject *obj = *iter;
	if ( child[i]->geomInNode( obj ) )
	  child[i]->members.push_back( obj );
      }

      child[i]->subdivide( depth+1, MaxDepth, MaxListLength, nextAxis );
    }
  }
}


// Initialize and start the building of KDTree.
// Entry:
//   KDTree       - The KDTree enclosing the entire scene
void KDTree::build( const vector<TObject *> objectPool ) {
  //--TO DO   CalculateTheExtentOfTheBinTree(&(KDTree->min), &(KDTree->max));
  min[__X] = -1000; min[__Y] = -1000; min[__Z] = -1000;
  max[__X] =  1000; max[__Y] =  1000; max[__Z] =  1000;

  //-- TO DO  KDTree->members = ObjectsWithinTheExtent(KDTree->min, KDTree->max);
  members = objectPool;

  //-- TO DO  KDTree->MaxDepth = GetMaxAllowedDepth();
  MaxDepth = 20;
  //-- TO DO  KDTree->MaxListLength = GetMaxAllowedListLength();
  MaxListLength = 3; // maximum number of objects in a node

  // Start building the KDTree by subdividing along the x axis first
  root = new BinNode;
  root->min = min;
  root->max = max;
  root->distanceToDivisionPlane = distanceToXPlane;
  root->getChildren = getXChildren;
  root->members = members;

  root->subdivide( 0, MaxDepth, MaxListLength, 1 );
}

KDTree::KDTree() {
  root = NULL;
  MaxDepth = 0;
  MaxListLength = 0;
}

KDTree::~KDTree() {
  // TO DO
}

void BinNode::print( const char *msg ) const {
  static int indent = 0;
  indent++;

  printf("%s%s [%d]\n", shiftFun(indent), msg, indent  );
  char buffer[64];
  sprintf( buffer, "%smin", shiftFun(indent) );
  min.print( buffer );
  sprintf( buffer, "%smax", shiftFun(indent) );
  max.print( buffer );
  for ( vector<TObject *>::const_iterator iter = members.begin();
	iter != members.end();
	iter++ ) {
    sprintf( buffer, "%sobject in node", shiftFun(indent) );
    (*iter)->print( buffer );
  }
  if ( child[0] != NULL ) {
    printf("%schild [0]\n", shiftFun(indent) );
    child[0]->print("child [0]");;
  }
  else
    printf("%schild[0] DOESN'T EXIST\n", shiftFun(indent) );
  if ( child[1] != NULL ) {
    printf("%schild [1]\n", shiftFun(indent) );
    child[1]->print("child [1]");;
  }
  else
    printf("%schild[1] DOESN'T EXIST\n", shiftFun(indent) );

  indent--;
}

void KDTree::print( const char *msg ) const {
  printf("%s\n", msg );
  min.print("min"); max.print("max");
  for ( vector<TObject *>::const_iterator iter = members.begin();
	iter != members.end();
	iter++ )
    (*iter)->print("object in KD");
  printf("MaxDepth: %d\n", MaxDepth );
  printf("MaxListLength: %d\n", MaxListLength );

  root->print( "root" );
}


void KDTree::render( u32 *screen, const u32 width, const u32 height, const TCameraSimple &camera ) {
  for ( vector<TObject *>::const_iterator iter = members.begin();
	iter != members.end();
	iter++ )
    (*iter)->renderSimple( screen, width, height, 0x00ffffff, camera );

  root->render( screen, width, height, camera );
}

void BinNode::render( u32 *screen, const u32 width, const u32 height, const TCameraSimple &camera ) {
  if ( leaf() ) {
    if ( members.size() > 0 )
      renderBox( screen, width, height, min, max, 0x00ff00ff, camera );
    else
      renderBox( screen, width, height, min, max, 0x0000ffff, camera );
  }

  if ( child[0] != NULL )
    child[0]->render( screen, width, height, camera );
  if ( child[1] != NULL )
    child[1]->render( screen, width, height, camera );
}
