#ifndef __ATMOSPHERIC_GROUND_H__
#define __ATMOSPHERIC_GROUND_H__

#include "fog.h"

struct AtmosphericGround: public AtmosphericFog {
  //----- Datas
  float offset;
  float altitude;
  TVector3 up;
  
  //----- Constructor
  AtmosphericGround();

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
  static const enum EAtmosphericGroundTags {
    TAG_atmosphericGround_offset,
    TAG_atmosphericGround_altitude,
    TAG_atmosphericGround_up,
  };
  static const TTagList atmosphericGroundTags [];
};

#endif
