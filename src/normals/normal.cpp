#include <stdio.h>

#include "normal.h"

const TTagList TNormal::normalTags [] = {
  { NULL, -1 }
};

const TTagList TNormal::normalSubClassTags [] = {
  { "bumps", NORMAL_BUMPS },
  { "turbulence", NORMAL_TURBULENCE },
  {  NULL, -1 }
};

TNormal::TNormal() {
  THandle::THandle(); handleKind = HANDLE_NORMAL;
  normalKind = NORMAL_NOTHING;
}

TNormal::~TNormal() {
}

void TNormal::setup() {
}

void TNormal::perturbNormal( TIntersection &inter ) const {
  error("TNormal::perturbNormal: YOU SHOULD NEVER SEE THIS\n");
}

void TNormal::print( const char *msg ) const {
  THandle::print(msg);
  printf("KIND: [%s]\n", normalName() );
}

char *TNormal::normalName() const {
  for ( int i = 0; normalSubClassTags[i].tagName != NULL; i++ )
    if ( normalSubClassTags[i].tagId == normalKind )
      return normalSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", normalKind );
  return buf;
}
