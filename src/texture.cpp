#include <stdio.h>

#include "texture.h"
#include "finish.h"
#include "pigments/pigment.h"
#include "interior.h"
#include "normals/normal.h"

const TTagList TTexture::textureTags [] = {
  { "finish", TAG_texture_finish },
  { "pigment", TAG_texture_pigment },
  { "interior", TAG_texture_interior },
  { "normal", TAG_texture_normal },
  { NULL, -1 }
};

TTexture::TTexture() {
  THandle::THandle(); handleKind = HANDLE_TEXTURE;
  finish = NULL;
  pigment = NULL;
  interior = NULL;
  normal = NULL;
}

TTexture::~TTexture() {
  finish = NULL;
  pigment = NULL;
  interior = NULL;
  normal = NULL;
}

void TTexture::setup() {
  if ( finish ) {
    finish->setup();
    if ( finish->transmissionFactor > 0.0f
	 && interior == NULL )
      error("TRANSMISSION FACTOR > 0 BUT NO INTERIOR!!\n");
  }
  else
    error("NO FINISH!!\n");
  if ( pigment )
    pigment->setup();
  else
    error("NO PIGMENT!!\n");
  if ( interior )
    interior->setup();
  if ( normal )
    normal->setup();
}

void TTexture::print( const char *msg ) const {
  THandle::print( msg );
  if ( finish )
    finish->print("FINISH:");
  if ( pigment )
    pigment->print("PIGMENT:");
  if ( interior )
    interior->print("INTERIOR:");
  if ( normal )
    normal->print("NORMAL:");
}
