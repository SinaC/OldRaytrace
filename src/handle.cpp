#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "handle.h"
#include "memory.h"

const TTagList THandle::handleTags [] = {
  { "name", TAG_handle_name },
  { NULL, -1 }
};

const TTagList THandle::handleSubClassTags [] = {
  {  "object", HANDLE_OBJECT },
  {  "texture", HANDLE_TEXTURE },
  {  "interior", HANDLE_INTERIOR },
  {  "finish", HANDLE_FINISH },
  {  "camera", HANDLE_CAMERA },
  {  "primitive", HANDLE_PRIMITIVE },
  {  "light", HANDLE_LIGHT },
  {  "bounding", HANDLE_BOUNDING },
  {  "ambient", HANDLE_AMBIENT },
  {  "pigment", HANDLE_PIGMENT },
  {  "atmospheric", HANDLE_ATMOSPHERIC },
  {  "colormap", HANDLE_COLORMAP },
  //  {  "pattern", HANDLE_PATTERN },
  {  "animator", HANDLE_ANIMATOR },
  {  "mover", HANDLE_MOVER },
  {  "bitmap", HANDLE_BITMAP },
  {  "normal", HANDLE_NORMAL },
  {  "turbulence", HANDLE_TURBULENCE },

  {  "meta", HANDLE_META }, // PrimitiveMetaObjects sub-class MetaSubObject
  {  "blob", HANDLE_BLOB }, // PrimitiveBlob sub-class BlobSubObject
  // World is not a parsable handle sub-class
  {  NULL, -1 }
};

THandle::THandle() {
  handleKind = HANDLE_NOTHING;
  name = "NO_NAME";
}

void THandle::setup() {
}

bool THandle::check() const {
  return true;
}

void THandle::error( const char *msg, ... ) const {
  va_list argptr;

  static char buf[4096]; 

  va_start(argptr, msg);
  vsprintf(buf, msg, argptr);
  printf("ERROR: %s", buf); 
  va_end(argptr);

  print("ERRONEOUS HANDLE:");
  shutdownRaytracing();
  exit(-1);
}

void THandle::print( const char *msg ) const {
  printf("%s\n", msg ); fflush(stdout);
  printf("NAME: [%s]  KIND: [%s]\n", name, handleName() ); fflush(stdout);
}

char *THandle::handleName() const {
  return handleName( handleKind );
}

char *THandle::handleName( const u32 kind ) const {
  if ( kind == HANDLE_WORLD )
    return "world";
  for ( int i = 0; handleSubClassTags[i].tagName != NULL; i++ )
    if ( handleSubClassTags[i].tagId == kind )
      return handleSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", kind );
  return buf;
}
