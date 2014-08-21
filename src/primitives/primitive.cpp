#include <stdio.h>

#include "primitive.h"
#include "../intersection.h"

const TTagList TPrimitive::primitiveTags [] = {
  { "inverse", PRIMITIVE_INVERSE },
  { "receive_no_shadow", PRIMITIVE_RECEIVE_NO_SHADOW },
  { "cast_no_shadow", PRIMITIVE_CAST_NO_SHADOW },
  { "cast_no_self_shadow", PRIMITIVE_CAST_NO_SELF_SHADOW },
  { "cast_no_reflection", PRIMITIVE_CAST_NO_REFLECTION },
  { "cast_no_transmission", PRIMITIVE_CAST_NO_TRANSMISSION },
  { "no_shading", PRIMITIVE_NO_SHADING },
  { "cast_no_self_reflection", PRIMITIVE_CAST_NO_SELF_REFLECTION },
  { "cast_no_self_transmission", PRIMITIVE_CAST_NO_SELF_TRANSMISSION },
  { "closed", PRIMITIVE_CLOSED },
  { "infinite", PRIMITIVE_INFINITE },
  { "2d", PRIMITIVE_2D },

  { NULL, -1 }
};

const TTagList TPrimitive::primitiveSubClassTags [] = {
  { "plane", PRIMITIVE_PLANE },
  { "sphere", PRIMITIVE_SPHERE },
  { "box", PRIMITIVE_BOX },
  { "torus", PRIMITIVE_TORUS },
  { "meta", PRIMITIVE_META },
  { "cylinder", PRIMITIVE_CYLINDER },
  { "capsule", PRIMITIVE_CAPSULE },
  { "blob", PRIMITIVE_BLOB },
  { "csg", PRIMITIVE_CSG },
  { "transformed", PRIMITIVE_TRANSFORMED },
  { "heart", PRIMITIVE_HEART },
  { "quadric", PRIMITIVE_QUADRIC },
  { "julia", PRIMITIVE_JULIA },
  { "superellipsoid", PRIMITIVE_SUPERELLIPSOID },
  { "sor", PRIMITIVE_SOR },
  { "cone", PRIMITIVE_CONE },
  {  NULL, -1 }
};

TPrimitive::TPrimitive() {
  THandle::THandle(); handleKind = HANDLE_PRIMITIVE;
  primitiveKind = PRIMITIVE_NOTHING;
}

TPrimitive::~TPrimitive() {
}

void TPrimitive::setup() {
}

void TPrimitive::precalcFrame( const TWorld &world ) {
}

bool TPrimitive::intersectionRayCamera( const TRay &ray,
					TIntersection &inter ) const {
  return intersectionRay( ray, inter );
}

bool TPrimitive::intersectionRay( const TRay &ray,
				  TIntersection &inter ) const {
  error("TPrimitive::intersectionRay: YOU SHOULD NEVER SEE THIS\n");
  return false;
}

bool TPrimitive::intersectsRay( const TRay &ray, const float distMax2 ) const {
  error("TPrimitive::intersectsRay: YOU SHOULD NEVER SEE THIS\n");
  return false;
}

bool TPrimitive::intersectsBox( const BoundingAABB &a_Box ) const {
  error("TPrimitive::intersectsBox: YOU SHOULD NEVER SEE THIS\n");
  return false;
}

bool TPrimitive::allIntersectionRayCamera( const TRay &ray,
					   TIntersectionStack &IStack ) const {
  return allIntersectionRay( ray, IStack );
}

bool TPrimitive::allIntersectionRay( const TRay &ray,
				     TIntersectionStack &IStack ) const {
  error("TPrimitive::allIntersectionRay: YOU SHOULD NEVER SEE THIS\n");
  return false;
}

TVector3 TPrimitive::normalAt( const TIntersection &inter ) const {
  error("TPrimitive::normalAt: YOU SHOULD NEVER SEE THIS\n");
  return TVector3::zeroVector;
}

bool TPrimitive::inside( const TPoint3 &p ) const {
  error("TPrimitive::inside: YOU SHOULD NEVER SEE THIS\n");
  return false;
}

bool TPrimitive::computeBoundingAABB( BoundingAABB &a_box ) const {
  error("TPrimitive::computeBoundingAABB: YOU SHOULD NEVER SEE THIS\n");
  return false;
}

void TPrimitive::print( const char *msg ) const {
  THandle::print(msg);
  printf("KIND: [%s]\n", primitiveName() );
//--  bool set = false;
//--  if ( additionalInformation.isSet( PRIMITIVE_INFINITE ) && (set = true ))
//--    printf("  [INFINITE]");
//--  if ( additionalInformation.isSet( PRIMITIVE_INVERSE ) && (set = true ))
//--    printf("  [INVERSE]");
//--  if ( additionalInformation.isSet( PRIMITIVE_NO_SHADING ) && (set = true ))
//--    printf("  [NO SHADING]");
//--  if ( additionalInformation.isSet( PRIMITIVE_CAST_NO_SHADOW ) && (set = true ))
//--    printf("  [CAST NO SHADOWS]");
//--  if ( additionalInformation.isSet( PRIMITIVE_RECEIVE_NO_SHADOW ) && (set = true ))
//--    printf("  [RECEIVE NO SHADOWS]");
//--  if ( additionalInformation.isSet( PRIMITIVE_CAST_NO_SELF_SHADOW ) && (set = true ))
//--    printf("  [CAST NO SHADOWS ON ITSELF]");
//--  if ( additionalInformation.isSet( PRIMITIVE_CAST_NO_REFLECTION ) && (set = true ))
//--    printf("  [CAST NO REFLECTIONS]");
//--  if ( additionalInformation.isSet( PRIMITIVE_CAST_NO_SELF_REFLECTION ) && (set = true ))
//--    printf("  [CAST NO REFLECTIONS ON ITSELF]");
//--  if ( additionalInformation.isSet( PRIMITIVE_CAST_NO_TRANSMISSION ) && (set = true ))
//--    printf("  [CAST NO TRANSMISSIONS]");
//--  if ( additionalInformation.isSet( PRIMITIVE_CAST_NO_SELF_TRANSMISSION ) && (set = true ))
//--    printf("  [CAST NO TRANSMISSIONS ON ITSELF]");
//--  if ( additionalInformation.isSet( PRIMITIVE_2D ) && (set = true ))
//--    printf("  [2D no interior]");
//--  if ( additionalInformation.isSet( PRIMITIVE_CLOSED ) && (set = true ))
//--    printf("  [closed]");
//--  if ( set )
//--    printf("\n");
  printf("additional: %s\n", TTagList::findName( additionalInformation, primitiveTags ) );
  fflush(stdout);
}

void TPrimitive::renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const {
}

char *TPrimitive::primitiveName() const {
  for ( int i = 0; primitiveSubClassTags[i].tagName != NULL; i++ )
    if ( primitiveSubClassTags[i].tagId == primitiveKind )
      return primitiveSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", primitiveKind );
  return buf;
}
