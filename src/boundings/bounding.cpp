#include <stdio.h>

#include "bounding.h"

const TTagList TBounding::boundingTags [] = {
  { NULL, -1 }
};

const TTagList TBounding::boundingSubClassTags [] = {
  {  "AABB", BOUNDING_AABB },
  {  "sphere", BOUNDING_SPHERE },
  {  "slabs", BOUNDING_SLAB },
  {  NULL, -1 }
};

TBounding::TBounding() {
  THandle::THandle(); handleKind = HANDLE_BOUNDING;
  boundingKind = BOUNDING_NOTHING;
}

void TBounding::create( const int numP, const TPoint3 *pts ) {
}

void TBounding::setup() {
}

bool TBounding::intersectsBox( BoundingAABB &a_Box ) const {
  error("TBounding::intersectsBox: YOU SHOULD NEVER SEE THIS");
  return false;
}

bool TBounding::intersectsRay( const TRay &ray ) const {
  error("TBounding::intersectsRay: YOU SHOULD NEVER SEE THIS");
  return false;
}

bool TBounding::inside( const TPoint3 &a_P ) const {
  error("TBounding::inside: YOU SHOULD NEVER SEE THIS");
  return false;
}

void TBounding::print( const char *msg ) const {
  THandle::print(msg);
  printf("KIND: [%s]\n", boundingName() );
}

void TBounding::renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const {
  // NOP
}

char *TBounding::boundingName() const {
  for ( int i = 0; boundingSubClassTags[i].tagName != NULL; i++ )
    if ( boundingSubClassTags[i].tagId == boundingKind )
      return boundingSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", boundingKind );
  return buf;
}
