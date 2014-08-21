#ifndef __ATMOSPHERIC_VOLUMETRIC_H__
#define __ATMOSPHERIC_VOLUMETRIC_H__

#include "fog.h"

struct TPrimitive;

struct AtmosphericVolumetric: public AtmosphericFog {
  //----- Datas
  TPrimitive *volume;
  
  //----- Constructor
  AtmosphericVolumetric();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual float getAttenuation( const TIntersection &intersection ) const;

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
  static const enum EAtmosphericVolumetricTags {
    TAG_atmosphericVolumetric_volume,
  };
  static const TTagList atmosphericVolumetricTags [];
};

#endif
