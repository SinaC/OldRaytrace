#ifndef __FINISH_H__
#define __FINISH_H__

#include "handle.h"

struct TFinish: public THandle {
  //----- Datas
  float ambientFactor;

  float diffuseFactor;
  float brilliance;
  float specularFactor;
  float specularRoughness;
  float phongFactor;
  float phongSize;

  float reflectionFactor;
  float transmissionFactor;

  //----- Constructor
  TFinish();

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
  static const enum EFinishTags {
    TAG_finish_reflection,
    TAG_finish_transmission,
    TAG_finish_ambient,
    TAG_finish_diffuse,
    TAG_finish_specular,
    TAG_finish_roughness,
    TAG_finish_phong,
    TAG_finish_phongSize,
    TAG_finish_brilliance
  };
  static const TTagList finishTags [];
};

#endif
