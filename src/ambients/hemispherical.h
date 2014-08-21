#ifndef __AMBIENT_HEMISPHERICAL_H__
#define __AMBIENT_HEMISPHERICAL_H__

#include "ambient.h"

struct AmbientHemispherical: public TAmbient {
  //---- Datas
  TColor colorTop; // color of top hemisphere
  TColor colorBottom; // color of bottom hemisphere
  TPoint3 center; // center of sphere
  TVector3 up; // up vector
  
  //---- Constructor
  AmbientHemispherical();

  //---- Setup
  virtual void setup();

  //---- Methods
  // v = point - center
  // angle = acosf( v . up )
  // if ( angle < pi/2 )   color = (1-sin(angle)/2) * top  +  (sin(angle)/2)*bottom
  // else                  color = (sin(angle)/2) * top  +  (1-sin(angle)/2)*bottom
  virtual TColor getColor( const TPoint3 &point ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  virtual void print( const char *msg ) const;

  //**** Private
  private:
  //--tag
  static const enum EAmbientHemisphericalTags {
    TAG_ambientHemispherical_center,
    TAG_ambientHemispherical_up,
    TAG_ambientHemispherical_colorTop,
    TAG_ambientHemispherical_colorBottom
  };
  static const TTagList ambientHemisphericalTags [];
};

#endif
