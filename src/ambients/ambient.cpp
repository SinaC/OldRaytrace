#include <stdio.h>

#include "ambient.h"

const TTagList TAmbient::ambientTags [] = {
  { NULL, -1 }
};

const TTagList TAmbient::ambientSubClassTags [] = {
  {  "solid", AMBIENT_SOLID },
  {  "hemispherical", AMBIENT_HEMISPHERICAL },
  {  NULL, -1 }
};

TAmbient::TAmbient() {
  THandle::THandle(); handleKind = HANDLE_AMBIENT;
  ambientKind = AMBIENT_NOTHING;
}

void TAmbient::setup() {
}

TColor TAmbient::getColor( const TPoint3 &p ) const {
  error("TAmbient::getColor!\n");
  return TColor::black;
}

void TAmbient::print( const char *msg ) const {
  THandle::print( msg );
  printf("KIND: [%s]\n", ambientName() );
}

char* TAmbient::ambientName() const {
  for ( int i = 0; ambientSubClassTags[i].tagName != NULL; i++ )
    if ( ambientSubClassTags[i].tagId == ambientKind )
      return ambientSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", ambientKind );
  return buf;
}
