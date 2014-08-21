#ifndef __LIGHT_DOT_H__
#define __LIGHT_DOT_H__

#include "light.h"

struct LightDot: public TLight {
  //----- Datas
  TPoint3 location;
  bool attenuation;

  //----- Constructor
  LightDot();
  //----- Destructor
  ~LightDot();

  //----- Setup

  //----- Methods
  // compute total lighting coming from this to a point
  virtual bool computeLighting( const TWorld &world, const TRay &ray,
				TLighting &lighting );

  // compute light ray and amount of light intersection receive from this light
  //  but don't compute any shading
  virtual bool getLight( const TWorld &world, const TRay &ray, TRay &lightRay );
  // compute lighting using ray.shadowRay
  virtual TColor getColor( const TWorld &world, const TRay &ray, const TRay &lightRay );

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  void print( const char *msg ) const;

  //***** Private
  private:
  //--tag
  static const enum ELightDotTags {
    TAG_lightDot_location,
    TAG_lightDot_attenuation
  };
  static const TTagList lightDotTags [];
};

#endif
