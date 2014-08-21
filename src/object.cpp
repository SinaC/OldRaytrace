#include <stdio.h>

#include "object.h"
#include "primitives/primitive.h"
#include "boundings/bounding.h"
#include "texture.h"

const TTagList TObject::objectTags [] = {
  { "primitive", TAG_object_primitive },
  { "bounding", TAG_object_bounding },
  { "texture", TAG_object_texture },
  { NULL, -1 }
};

TObject::TObject() {
  THandle::THandle(); handleKind = HANDLE_OBJECT;
  bounding = NULL;
  primitive = NULL;
  texture = NULL;
}

TObject::~TObject() {
  bounding = NULL;
  texture = NULL;
  primitive = NULL;
}

void TObject::setup() {
  if ( primitive )
    primitive->setup();
  else
    error("NO PRIMITIVE!!\n");
  if ( texture )
    texture->setup();
  else
    error("NO TEXTURE!!\n");
  if ( bounding )
    bounding->setup();
}

void TObject::print( const char *msg ) const {
  THandle::print( msg );
  if ( primitive )
    primitive->print("PRIMITIVE");
  else
    printf("NO PRIMITIVE!\n");
  if ( texture != NULL )
    texture->print("TEXTURE: ");
  else
    printf("NO TEXTURE!\n");
  if ( bounding != NULL )
    bounding->print("BOUNDING: ");
  else
    printf("NO BOUNDING VOLUME\n");
}

void TObject::renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const {
  bounding->renderSimple( screen, width, height, color, camera );
  primitive->renderSimple( screen, width, height, color, camera );
}
