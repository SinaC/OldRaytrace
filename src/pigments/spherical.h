#ifndef __PIGMENT_SPHERICAL_H__
#define __PIGMENT_SPHERICAL_H__

#include "pigment.h"

struct TBitmapColor;

struct PigmentSpherical : public TPigment {
  //----- Datas
  TBitmapColor *map;

  //----- Constructor
  PigmentSpherical();
  //----- Destructor
  ~PigmentSpherical();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual TColor getColor( const TPoint3 &p ) const;

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
  static const enum EPigmentSphericalTags {
    TAG_pigmentSpherical_map,
  };
  static const TTagList pigmentSphericalTags [];
};

#endif
