#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "handle.h"

struct TFinish;
struct TInterior;
struct TPigment;
struct TNormal;

struct TTexture: public THandle {
  //----- Datas
  TFinish *finish;
  TPigment *pigment;
  TInterior *interior;
  TNormal *normal;

  //----- Constructor
  TTexture();
  //----- Destructor
  ~TTexture();

  //----- Setup
  virtual void setup();

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  //--tag
  static const enum ETextureTags {
    TAG_texture_finish,
    TAG_texture_pigment,
    TAG_texture_interior,
    TAG_texture_normal
  };
  static const TTagList textureTags [];
};

#endif
