#include <stdio.h>

#include "../world.h"
#include "transformed.h"
#include "../ray.h"
#include "../boundings/aabb.h"

const TTagList PrimitiveTransformed::primitiveTransformedTags [] = {
  { "primitive", TAG_primitiveTransformed_primitive },
  { "transformation", TAG_primitiveTransformed_transformation },
  { "rotate", TAG_primitiveTransformed_rotate },
  { "translate", TAG_primitiveTransformed_translate },
  { "scale", TAG_primitiveTransformed_scale },
  { NULL, -1 }
};

PrimitiveTransformed::PrimitiveTransformed() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_TRANSFORMED;
}

PrimitiveTransformed::~PrimitiveTransformed() {
  primitive = NULL;
}

void PrimitiveTransformed::setup() {
  if ( primitive != NULL )
    primitive->setup();
}

void PrimitiveTransformed::precalcFrame( const TWorld &world ) {
  if ( primitive != NULL )
    primitive->precalcFrame( world );
}

bool PrimitiveTransformed::intersectionRay( const TRay &ray, TIntersection &inter ) const {
  TRay newRay;

  // transform ray into object spaces
  newRay.origin = transform.inverseTransformPoint( ray.origin );
  newRay.direction = transform.inverseTransformDirection( ray.direction );
  newRay.reverseDirection = inverseVector( newRay.direction );
  //--  newRay.target = transform.inverseTransformPoint( ray.target );

  return primitive->intersectionRay( newRay, inter );
}

bool PrimitiveTransformed::intersectsRay( const TRay &ray, const float maxDist2 ) const {
  TRay newRay;

  // transform ray into object spaces
  newRay.origin = transform.inverseTransformPoint( ray.origin );
  newRay.direction = transform.inverseTransformDirection( ray.direction );
  newRay.reverseDirection = inverseVector( newRay.direction );
  //--  newRay.target = transform.inverseTransformPoint( ray.target );

  return primitive->intersectsRay( newRay, maxDist2 );
}

bool PrimitiveTransformed::intersectsBox( const BoundingAABB &a_Box ) const {
  BoundingAABB box;
  box.m_Min = transform.inverseTransformPoint( a_Box.m_Min );
  box.m_Max = transform.inverseTransformPoint( a_Box.m_Max );

  return primitive->intersectsBox( box );
}

bool PrimitiveTransformed::allIntersectionRay( const TRay &ray,
					       TIntersectionStack &IStack ) const {
  TRay newRay;

  // transform ray into object spaces
  newRay.origin = transform.inverseTransformPoint( ray.origin );
  newRay.direction = transform.inverseTransformDirection( ray.direction );
  newRay.reverseDirection = inverseVector( newRay.direction );
  //--  newRay.target = transform.inverseTransformPoint( ray.target );

  return primitive->allIntersectionRay( newRay, IStack );
}

TVector3 PrimitiveTransformed::normalAt( const TIntersection &inter ) const {
  TIntersection newInter;

  // transform intersection point into object spaces
  newInter.copyIntersection( inter );
  newInter.pointWorldSpace = transform.inverseTransformPoint( inter.pointWorldSpace );

  TVector3 result = primitive->normalAt( newInter );
  return transform.transformNormal( result );
}

bool PrimitiveTransformed::inside( const TPoint3 &p ) const {
  TPoint3 newP;

  // transform point into object spaces
  newP = transform.inverseTransformPoint( p );

  return primitive->inside( newP );
}

void PrimitiveTransformed::print( const char *msg ) const {
  TPrimitive::print( msg );
  primitive->print("primitive");
  transform.print("transformation: ");
}
