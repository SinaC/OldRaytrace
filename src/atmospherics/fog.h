#ifndef __ATMOSPHERIC_FOG_H__
#define __ATMOSPHERIC_FOG_H__

#include "atmospheric.h"

struct TTurbulence;

struct AtmosphericFog: public TAtmospheric {
  //----- Datas
  float density; // 1 / distance
  TColor color;
  float filter; // FIXME: in povray, color has 5 components RGB + filter + transmittance
  TTurbulence *turbulence;
  float turbulenceDepth;
  
  //----- Constructor
  AtmosphericFog();
  //----- Destructor
  ~AtmosphericFog();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual TColor addColor( const TColor &currentColor,
			   const TIntersection &intersection ) const;
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
  static const enum EAtmosphericFogTags {
    TAG_atmosphericFog_density,
    TAG_atmosphericFog_color,
    TAG_atmosphericFog_filter,
    TAG_atmosphericFog_turbulence,
    TAG_atmosphericFog_turbulenceDepth
  };
  static const TTagList atmosphericFogTags [];
};

#endif
