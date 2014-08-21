#include <stdio.h>

#include "../world.h"
#include "csg.h"
#include "../ray.h"

const TTagList PrimitiveCSG::primitiveCsgTags [] = {
  { "operation", TAG_primitiveCsg_operation },
  { "primitives", TAG_primitiveCsg_primitiveList },
  { NULL, -1 }
};

const TTagList PrimitiveCSG::primitiveCsgOperations [] = {
  { "union", CSG_UNION },
  { "intersection", CSG_INTERSECTION },
  { "difference", CSG_DIFFERENCE },
  { "merge", CSG_MERGE },
  { "single", CSG_SINGLE },
  { NULL, -1 }
};

PrimitiveCSG::PrimitiveCSG() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_CSG;
}

PrimitiveCSG::~PrimitiveCSG() {
  primitiveList.clear();
}

void PrimitiveCSG::addPrimitive( TPrimitive *primitive, const bool inverse ) {
  if ( inverse )
    primitive->additionalInformation.setBit( PRIMITIVE_INVERSE );
  else
    primitive->additionalInformation.setBit( PRIMITIVE_INVERSE );
  primitiveList.push_back( primitive );
}

void PrimitiveCSG::setup() {
  if ( primitiveList.size() < 2 )
    printf("At least 2 primitives in a CSG!!\n");
  bool inverse = false;
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    (*iterP)->setup();
    if ( (*iterP)->additionalInformation.isSet( PRIMITIVE_INVERSE ) )
      inverse = true;
  }
  if ( inverse
       && operation != CSG_DIFFERENCE )
    printf("Primitive(s) with inverse bit set but operation is not DIFFERENCE!\n");
}

void PrimitiveCSG::precalcFrame( const TWorld &world ) {
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ )
    (*iterP)->precalcFrame( world );
}

bool PrimitiveCSG::intersectionRay( const TRay &ray, TIntersection &inter ) const {
   switch ( operation ) {
   case CSG_UNION: return intersectionRayUnion( ray, inter ); break;
   case CSG_INTERSECTION: return intersectionRayIntersection( ray, inter ); break;
   case CSG_DIFFERENCE: return intersectionRayDifference( ray, inter ); break;
   case CSG_MERGE: return intersectionRayMerge( ray, inter ); break;
   default:
     printf("NOT YET IMPLEMENTED\n");
     return false;
   }
  return false;
}

bool PrimitiveCSG::intersectsRay( const TRay &ray, const float maxDist2 ) const {
  switch ( operation ) {
  case CSG_UNION: return intersectsRayUnion( ray, maxDist2 ); break;
  case CSG_INTERSECTION: return intersectsRayIntersection( ray, maxDist2 ); break;
  case CSG_DIFFERENCE: return intersectsRayDifference( ray, maxDist2 ); break;
  case CSG_MERGE: return intersectsRayMerge( ray, maxDist2 ); break;
  default:
    printf("NOT YET IMPLEMENTED\n");
    return false;
  }
  return false;
}

bool PrimitiveCSG::allIntersectionRay( const TRay &ray,
				       TIntersectionStack &IStack ) const {
  switch ( operation ) {
  case CSG_UNION: return allIntersectionRayUnion( ray, IStack ); break;
  case CSG_INTERSECTION: return allIntersectionRayIntersection( ray, IStack ); break;
  case CSG_DIFFERENCE: return allIntersectionRayDifference( ray, IStack ); break;
  case CSG_MERGE: return allIntersectionRayMerge( ray, IStack ); break;
  default:
    printf("NOT YET IMPLEMENTED\n");
    return false;
  }
  return false;
}

TVector3 PrimitiveCSG::normalAt( const TIntersection &inter ) const {
  if ( !inter.CSGPrimitive ) {
    printf("Problem in PrimitiveCSG::normalAt: no csg primitive computed in intersection!!\n"); fflush(stdout);
    return TVector3::zeroVector;
  }
  return inter.CSGPrimitive->normalAt( inter );
}

void PrimitiveCSG::print( const char *msg ) const {
  TPrimitive::print( msg );
  printf("operation: %d\n", operation ); fflush(stdout);
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ )
    (*iterP)->print("primitive in csg: "); fflush(stdout);
}

//************************** Union
//--- smallest intersection
bool PrimitiveCSG::intersectionRayUnion( const TRay &ray, TIntersection &inter ) const {
  bool found = false;

  inter.distance = INFINITE;
  // find closest intersection for each primitive and store smallest
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    TPrimitive *primitive = (*iterP);
    TIntersection i;
    if ( primitive->intersectionRay( ray, i ) )
      if ( i.distance < inter.distance
	   && i.distance > 0.0f ) {
	inter.copyIntersection( i );
	if ( i.CSGPrimitive )
	  inter.CSGPrimitive = i.CSGPrimitive;
	else
	  inter.CSGPrimitive = primitive;
	found = true;
      }
  }
  return found;
}
bool PrimitiveCSG::intersectsRayUnion( const TRay &ray, const float maxDist2 ) const {
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ )
    // exit if an intersection has been found
    if ( (*iterP)->intersectsRay( ray, maxDist2 ) )
      return true;

  return false;
}
bool PrimitiveCSG::allIntersectionRayUnion( const TRay &ray,
					    TIntersectionStack &IStack ) const {
  bool found = false;
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStack ) )
      found = true;
  }
  return found;
}

//************************** Intersection
//--- smallest intersection which IS in another primitive of intersection
bool PrimitiveCSG::intersectionRayIntersection( const TRay &ray,
						TIntersection &inter ) const {
//--OLD WAY--  bool found = false;
//--OLD WAY--  TIntersectionStack IStack;
//--OLD WAY--  IStack.create(64);
//--OLD WAY--
//--OLD WAY--  inter.distance = INFINITE;
//--OLD WAY--  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
//--OLD WAY--	iterP != primitiveList.end();
//--OLD WAY--	iterP++ ) {
//--OLD WAY--    IStack.clear();
//--OLD WAY--    TPrimitive *primitive = (*iterP);
//--OLD WAY--    // find all intersections
//--OLD WAY--    if ( primitive->allIntersectionRay( ray, IStack ) )
//--OLD WAY--      // found all intersections
//--OLD WAY--      for ( int i = 0; i < IStack.index; i++ ) {
//--OLD WAY--	// loop among intersections and find the closest which is in another primitives of intersection
//--OLD WAY--	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
//--OLD WAY--	bool inside = true;
//--OLD WAY--	// check if point is outside another primitives of intersection
//--OLD WAY--	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
//--OLD WAY--	      iterP2 != primitiveList.end();
//--OLD WAY--	      iterP2++ ) {
//--OLD WAY--	  TPrimitive *primitive2 = (*iterP2);
//--OLD WAY--	  if ( primitive2 != primitive
//--OLD WAY--	       && !primitive2->inside( p ) ) {
//--OLD WAY--	    inside = false;
//--OLD WAY--	    break;
//--OLD WAY--	  }
//--OLD WAY--	}
//--OLD WAY--	if ( inside
//--OLD WAY--	     && IStack.stack[i].distance < inter.distance
//--OLD WAY--	     && IStack.stack[i].distance > 0.0f ) {
//--OLD WAY--	  // found an intersection, closer in another primitives of intersection
//--OLD WAY--	  inter.copyIntersection( IStack.stack[i] );
//--OLD WAY--	  if ( IStack.stack[i].CSGPrimitive != NULL )
//--OLD WAY--	    inter.CSGPrimitive = IStack.stack[i].CSGPrimitive;
//--OLD WAY--	  else
//--OLD WAY--	    inter.CSGPrimitive = primitive;
//--OLD WAY--	  found = true;
//--OLD WAY--	}
//--OLD WAY--      }
//--OLD WAY--  }
//--OLD WAY--  return found;
  bool found = false;
  TIntersectionStack IStack;
  IStack.create(64);

  inter.distance = INFINITE;
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    IStack.clear();
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStack ) )
      // found all intersections
      for ( int i = 0; i < IStack.index; i++ ) {
	// loop among intersections and find the closest which is in another primitives of intersection
	// if this intersection is invalid or not closer: check next intersection
	if ( IStack.stack[i].distance >= inter.distance
	     || IStack.stack[i].distance <= 0.0f )
	  continue;
	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
	bool inside = true;
	// check if point is outside another primitives of intersection
	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
	      iterP2 != primitiveList.end();
	      iterP2++ ) {
	  TPrimitive *primitive2 = (*iterP2);
	  if ( primitive2 != primitive
	       && !primitive2->inside( p ) ) {
	    inside = false;
	    break;
	  }
	}
	if ( inside ) {
	  // found an intersection, closer in another primitives of intersection
	  inter.copyIntersection( IStack.stack[i] );
	  if ( IStack.stack[i].CSGPrimitive != NULL )
	    inter.CSGPrimitive = IStack.stack[i].CSGPrimitive;
	  else
	    inter.CSGPrimitive = primitive;
	  found = true;
	}
      }
  }
  return found;
}
bool PrimitiveCSG::intersectsRayIntersection( const TRay &ray, const float maxDist2 ) const {
//--OLD WAY--  TIntersectionStack IStack;
//--OLD WAY--  IStack.create(64);
//--OLD WAY--
//--OLD WAY--  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
//--OLD WAY--	iterP != primitiveList.end();
//--OLD WAY--	iterP++ ) {
//--OLD WAY--    IStack.clear();
//--OLD WAY--    TPrimitive *primitive = (*iterP);
//--OLD WAY--    // find all intersections
//--OLD WAY--    if ( primitive->allIntersectionRay( ray, IStack ) )
//--OLD WAY--      // found all intersections
//--OLD WAY--      for ( int i = 0; i < IStack.index; i++ ) {
//--OLD WAY--	// loop among intersections and find the closest which is in another primitives of intersection
//--OLD WAY--	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
//--OLD WAY--	bool inside = true;
//--OLD WAY--	// check if point is outside another primitives of intersection
//--OLD WAY--	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
//--OLD WAY--	      iterP2 != primitiveList.end();
//--OLD WAY--	      iterP2++ ) {
//--OLD WAY--	  TPrimitive *primitive2 = (*iterP2);
//--OLD WAY--	  if ( primitive2 != primitive
//--OLD WAY--	       && !primitive2->inside( p ) ) {
//--OLD WAY--	    inside = false;
//--OLD WAY--	    break;
//--OLD WAY--	  }
//--OLD WAY--	}
//--OLD WAY--	// if point is inside and is between 0 and maxDist2
//--OLD WAY--	if ( inside
//--OLD WAY--	     && IStack.stack[i].distance < maxDist2
//--OLD WAY--	     && IStack.stack[i].distance > 0.0f )
//--OLD WAY--	  return true;
//--OLD WAY--      }
//--OLD WAY--  }
//--OLD WAY--  return false;
  TIntersectionStack IStack;
  IStack.create(64);

  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    IStack.clear();
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStack ) )
      // found all intersections
      for ( int i = 0; i < IStack.index; i++ ) {
	// loop among intersections and find the closest which is in another primitives of intersection
	// if this intersection is invalid or not close enough: check next intersection
	if ( IStack.stack[i].distance >= maxDist2
	     || IStack.stack[i].distance <= 0.0f )
	  continue;
	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
	bool inside = true;
	// check if point is outside another primitives of intersection
	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
	      iterP2 != primitiveList.end();
	      iterP2++ ) {
	  TPrimitive *primitive2 = (*iterP2);
	  if ( primitive2 != primitive
	       && !primitive2->inside( p ) ) {
	    inside = false;
	    break;
	  }
	}
	// if point is inside and is between 0 and maxDist2
	if ( inside )
	  return true;
      }
  }
  return false;
}
bool PrimitiveCSG::allIntersectionRayIntersection( const TRay &ray,
						   TIntersectionStack &IStack ) const {
  bool found = false;
  TIntersectionStack IStackLocal;
  IStackLocal.create(64);

  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    IStackLocal.clear();
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStackLocal ) )
      // found all intersections
      for ( int i = 0; i < IStackLocal.index; i++ ) {
	// loop among intersections
	TPoint3 p = ray.origin + ( IStackLocal.stack[i].distance * ray.direction );
	bool inside = true;
	// check if point is outside another primitives of intersection
	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
	      iterP2 != primitiveList.end();
	      iterP2++ ) {
	  TPrimitive *primitive2 = (*iterP2);
	  if ( primitive2 != primitive
	       && !primitive2->inside( p ) ) {
	    inside = false;
	    break;
	  }
	}
	if ( inside ) {
	  if ( IStackLocal.stack[i].CSGPrimitive == NULL )
	    IStackLocal.stack[i].CSGPrimitive = primitive;
	  IStack.push( IStackLocal.stack[i] );
	  found = true;
	}
      }
  }
  return found;
}

//************************** Difference
//--- smallest intersection which IS in another primitive (inversed) of difference
bool PrimitiveCSG::intersectionRayDifference( const TRay &ray,
					      TIntersection &inter ) const {
//--OLD WAY--  bool found = false;
//--OLD WAY--  TIntersectionStack IStack;
//--OLD WAY--  IStack.create(64);
//--OLD WAY--
//--OLD WAY--  inter.distance = INFINITE;
//--OLD WAY--  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
//--OLD WAY--	iterP != primitiveList.end();
//--OLD WAY--	iterP++ ) {
//--OLD WAY--    IStack.clear();
//--OLD WAY--    TPrimitive *primitive = (*iterP);
//--OLD WAY--    // find all intersections
//--OLD WAY--    if ( primitive->allIntersectionRay( ray, IStack ) )
//--OLD WAY--      // found all intersections
//--OLD WAY--      for ( int i = 0; i < IStack.index; i++ ) {
//--OLD WAY--	// loop among intersections and find the closest which is in another primitives of intersection
//--OLD WAY--	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
//--OLD WAY--	bool keep = true;
//--OLD WAY--	// check if point is outside another primitives of intersection
//--OLD WAY--	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
//--OLD WAY--	      iterP2 != primitiveList.end();
//--OLD WAY--	      iterP2++ ) {
//--OLD WAY--	  TPrimitive *primitive2 = (*iterP2);
//--OLD WAY--	  if ( primitive2 != primitive
//--OLD WAY--	       && ( (!primitive2->inside( p )) ^ primitive2->additionalInformation.isSet( PRIMITIVE_INVERSE ) ) ) {
//--OLD WAY--	    keep = false;
//--OLD WAY--	    break;
//--OLD WAY--	  }
//--OLD WAY--	}
//--OLD WAY--	if ( keep
//--OLD WAY--	     && IStack.stack[i].distance < inter.distance
//--OLD WAY--	     && IStack.stack[i].distance > 0.0f ) {
//--OLD WAY--	  // found an intersection, closer, in another primitives of intersection
//--OLD WAY--	  inter.copyIntersection( IStack.stack[i] );
//--OLD WAY--	  if ( IStack.stack[i].CSGPrimitive != NULL )
//--OLD WAY--	    inter.CSGPrimitive = IStack.stack[i].CSGPrimitive;
//--OLD WAY--	  else
//--OLD WAY--	    inter.CSGPrimitive = primitive;
//--OLD WAY--	  //	  inter.inverse = primitive->additionalInformation.isSet( PRIMITIVE_INVERSE );
//--OLD WAY--	  inter.inverse = inter.CSGPrimitive->additionalInformation.isSet( PRIMITIVE_INVERSE );
//--OLD WAY--	  found = true;
//--OLD WAY--	}
//--OLD WAY--      }
//--OLD WAY--  }
//--OLD WAY--  return found;
  bool found = false;
  TIntersectionStack IStack;
  IStack.create(64);

  inter.distance = INFINITE;
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    IStack.clear();
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStack ) )
      // found all intersections
      for ( int i = 0; i < IStack.index; i++ ) {
	// loop among intersections and find the closest which is in another primitives of difference
	// if this intersection is invalid or not closer: check next intersection
	if ( IStack.stack[i].distance >= inter.distance
	     || IStack.stack[i].distance <= 0.0f )
	  continue;
	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
	bool keep = true;
	// check if point is outside another primitives of difference
	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
	      iterP2 != primitiveList.end();
	      iterP2++ ) {
	  TPrimitive *primitive2 = (*iterP2);
	  if ( primitive2 != primitive
	       && ( (!primitive2->inside( p )) ^ primitive2->additionalInformation.isSet( PRIMITIVE_INVERSE ) ) ) {
	    keep = false;
	    break;
	  }
	}
	if ( keep ) {
	  // found an intersection, closer, in another primitives of difference
	  inter.copyIntersection( IStack.stack[i] );
	  if ( IStack.stack[i].CSGPrimitive != NULL )
	    inter.CSGPrimitive = IStack.stack[i].CSGPrimitive;
	  else
	    inter.CSGPrimitive = primitive;
	  //	  inter.inverse = primitive->additionalInformation.isSet( PRIMITIVE_INVERSE );
	  inter.inverse = inter.CSGPrimitive->additionalInformation.isSet( PRIMITIVE_INVERSE );
	  found = true;
	}
      }
  }
  return found;
}
bool PrimitiveCSG::intersectsRayDifference( const TRay &ray, const float maxDist2 ) const {
//--OLD WAY--  TIntersectionStack IStack;
//--OLD WAY--  IStack.create(64);
//--OLD WAY--  
//--OLD WAY--  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
//--OLD WAY--	iterP != primitiveList.end();
//--OLD WAY--	iterP++ ) {
//--OLD WAY--    IStack.clear();
//--OLD WAY--    TPrimitive *primitive = (*iterP);
//--OLD WAY--    // find all intersections
//--OLD WAY--    if ( primitive->allIntersectionRay( ray, IStack ) )
//--OLD WAY--      // found all intersections
//--OLD WAY--      for ( int i = 0; i < IStack.index; i++ ) {
//--OLD WAY--	// loop among intersections and find the closest which is in another primitives of intersection
//--OLD WAY--	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
//--OLD WAY--	bool keep = true;
//--OLD WAY--	// check if point is outside another primitives of intersection
//--OLD WAY--	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
//--OLD WAY--	      iterP2 != primitiveList.end();
//--OLD WAY--	      iterP2++ ) {
//--OLD WAY--	  TPrimitive *primitive2 = (*iterP2);
//--OLD WAY--	  if ( primitive2 != primitive
//--OLD WAY--	       && ( (!primitive2->inside( p )) ^ primitive2->additionalInformation.isSet( PRIMITIVE_INVERSE ) ) ) {
//--OLD WAY--	    keep = false;
//--OLD WAY--	    break;
//--OLD WAY--	  }
//--OLD WAY--	}
//--OLD WAY--	// if point is inside and is between 0 and maxDist2
//--OLD WAY--	if ( keep
//--OLD WAY--	     && IStack.stack[i].distance < maxDist2
//--OLD WAY--	     && IStack.stack[i].distance > 0 )
//--OLD WAY--	  return true;
//--OLD WAY--      }
//--OLD WAY--  }
//--OLD WAY--  return false;
  TIntersectionStack IStack;
  IStack.create(64);
  
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    IStack.clear();
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStack ) )
      // found all intersections
      for ( int i = 0; i < IStack.index; i++ ) {
	// loop among intersections and find the closest which is in another primitives of difference
	// if this intersection is invalid or not close enough: check next intersection
	if ( IStack.stack[i].distance >= maxDist2
	     || IStack.stack[i].distance <= 0 )
	  continue;
	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
	bool keep = true;
	// check if point is outside another primitives of difference
	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
	      iterP2 != primitiveList.end();
	      iterP2++ ) {
	  TPrimitive *primitive2 = (*iterP2);
	  if ( primitive2 != primitive
	       && ( (!primitive2->inside( p )) ^ primitive2->additionalInformation.isSet( PRIMITIVE_INVERSE ) ) ) {
	    keep = false;
	    break;
	  }
	}
	// if point is inside and is between 0 and maxDist2
	if ( keep )
	  return true;
      }
  }
  return false;
}
bool PrimitiveCSG::allIntersectionRayDifference( const TRay &ray,
						 TIntersectionStack &IStack ) const {
  bool found = false;
  TIntersectionStack IStackLocal;
  IStackLocal.create(64);

  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    IStackLocal.clear();
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStackLocal ) )
      // found all intersections
      for ( int i = 0; i < IStackLocal.index; i++ ) {
	// loop among intersections and find the closest which is in another primitives of intersection
	TPoint3 p = ray.origin + ( IStackLocal.stack[i].distance * ray.direction );
	bool keep = true;
	// check if point is outside another primitives of intersection
	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
	      iterP2 != primitiveList.end();
	      iterP2++ ) {
	  TPrimitive *primitive2 = (*iterP2);
	  if ( primitive2 != primitive
	       && ( (!primitive2->inside( p )) ^ primitive2->additionalInformation.isSet( PRIMITIVE_INVERSE ) ) ) {
	    keep = false;
	    break;
	  }
	}
	if ( keep ) {
	  // found an intersection, closer, in another primitives of intersection
	  if ( IStackLocal.stack[i].CSGPrimitive == NULL )
	    IStackLocal.stack[i].CSGPrimitive = primitive;
	  IStackLocal.stack[i].inverse = IStackLocal.stack[i].CSGPrimitive->additionalInformation.isSet( PRIMITIVE_INVERSE );
	  IStack.push( IStackLocal.stack[i] );
	  found = true;
	}
      }
  }
  return found;
}

//************************** Merge
//--- smallest intersection which IS NOT in another primitive of merge
bool PrimitiveCSG::intersectionRayMerge( const TRay &ray, TIntersection &inter ) const {
//--OLD WAY--  bool found = false;
//--OLD WAY--  TIntersectionStack IStack;
//--OLD WAY--  IStack.create(64);
//--OLD WAY--  
//--OLD WAY--  inter.distance = INFINITE;
//--OLD WAY--  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
//--OLD WAY--	iterP != primitiveList.end();
//--OLD WAY--	iterP++ ) {
//--OLD WAY--    IStack.clear();
//--OLD WAY--    TPrimitive *primitive = (*iterP);
//--OLD WAY--    // find all intersections
//--OLD WAY--    if ( primitive->allIntersectionRay( ray, IStack ) )
//--OLD WAY--      // found all intersections
//--OLD WAY--      for ( int i = 0; i < IStack.index; i++ ) {
//--OLD WAY--	// loop among intersections and find the closest which is not in another primitives of merge
//--OLD WAY--	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
//--OLD WAY--	bool inside = false;
//--OLD WAY--	// check if point is inside another primitives of merge
//--OLD WAY--	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
//--OLD WAY--	      iterP2 != primitiveList.end();
//--OLD WAY--	      iterP2++ ) {
//--OLD WAY--	  TPrimitive *primitive2 = (*iterP2);
//--OLD WAY--	  if ( primitive2 != primitive
//--OLD WAY--	       && primitive2->inside( p ) ) {
//--OLD WAY--	    inside = true;
//--OLD WAY--	    break;
//--OLD WAY--	  }
//--OLD WAY--	}
//--OLD WAY--	if ( !inside
//--OLD WAY--	     && IStack.stack[i].distance < inter.distance
//--OLD WAY--	     && IStack.stack[i].distance > 0.0f ) {
//--OLD WAY--	  // found an intersection, closer not in another primitives of merge
//--OLD WAY--	  inter.copyIntersection( IStack.stack[i] );
//--OLD WAY--	  if ( IStack.stack[i].CSGPrimitive != NULL )
//--OLD WAY--	    inter.CSGPrimitive = IStack.stack[i].CSGPrimitive;
//--OLD WAY--	  else
//--OLD WAY--	    inter.CSGPrimitive = primitive;
//--OLD WAY--	  found = true;
//--OLD WAY--	}
//--OLD WAY--      }
//--OLD WAY--  }
//--OLD WAY--  return found;
  bool found = false;
  TIntersectionStack IStack;
  IStack.create(64);
  
  inter.distance = INFINITE;
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    IStack.clear();
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStack ) )
      // found all intersections
      for ( int i = 0; i < IStack.index; i++ ) {
	// loop among intersections and find the closest which is not in another primitives of merge
	// if this intersection is invalid or not closer: check next intersection
	if ( IStack.stack[i].distance >= inter.distance
	     || IStack.stack[i].distance <= 0.0f )
	  continue;
	TPoint3 p = ray.origin + ( IStack.stack[i].distance * ray.direction );
	bool inside = false;
	// check if point is inside another primitives of merge
	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
	      iterP2 != primitiveList.end();
	      iterP2++ ) {
	  TPrimitive *primitive2 = (*iterP2);
	  if ( primitive2 != primitive
	       && primitive2->inside( p ) ) {
	    inside = true;
	    break;
	  }
	}
	if ( !inside ) {
	  // found an intersection, closer not in another primitives of merge
	  inter.copyIntersection( IStack.stack[i] );
	  if ( IStack.stack[i].CSGPrimitive != NULL )
	    inter.CSGPrimitive = IStack.stack[i].CSGPrimitive;
	  else
	    inter.CSGPrimitive = primitive;
	  found = true;
	}
      }
  }
  return found;
}
bool PrimitiveCSG::intersectsRayMerge( const TRay &ray, const float maxDist2 ) const {
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ )
    // exit if an intersection has been found
    if ( (*iterP)->intersectsRay( ray, maxDist2 ) )
      return true;

  return false;
}
bool PrimitiveCSG::allIntersectionRayMerge( const TRay &ray,
					    TIntersectionStack &IStack ) const {
  bool found = false;
  TIntersectionStack IStackLocal;
  IStackLocal.create(64);
  
  for ( vector<TPrimitive *>::const_iterator iterP = primitiveList.begin();
	iterP != primitiveList.end();
	iterP++ ) {
    IStackLocal.clear();
    TPrimitive *primitive = (*iterP);
    // find all intersections
    if ( primitive->allIntersectionRay( ray, IStackLocal ) )
      // found all intersections
      for ( int i = 0; i < IStackLocal.index; i++ ) {
	// loop among intersections and find the closest which is not in another primitives of merge
	TPoint3 p = ray.origin + ( IStackLocal.stack[i].distance * ray.direction );
	bool inside = false;
	// check if point is inside another primitives of merge
	for ( vector<TPrimitive *>::const_iterator iterP2 = primitiveList.begin();
	      iterP2 != primitiveList.end();
	      iterP2++ ) {
	  TPrimitive *primitive2 = (*iterP2);
	  if ( primitive2 != primitive
	       && primitive2->inside( p ) ) {
	    inside = true;
	    break;
	  }
	}
	if ( !inside ) {
	  // found an intersection, closer not in another primitives of merge
	  if ( IStackLocal.stack[i].CSGPrimitive == NULL )
	    IStackLocal.stack[i].CSGPrimitive = primitive;
	  IStack.push( IStackLocal.stack[i] );
	  found = true;
	}
      }
  }
  return found;
}
