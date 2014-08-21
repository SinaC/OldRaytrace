#ifndef __PIGMENT_CYLINDRICAL_H__
#define __PIGMENT_CYLINDRICAL_H__

#include "pigment.h"

struct TBitmapColor;

struct PigmentCylindrical: public TPigment {
  //----- Datas
  TBitmapColor *map;

  //----- Constructor
  PigmentCylindrical();
  //----- Destructor
  ~PigmentCylindrical();

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
  static const enum EPigmentCylindricalTags {
    TAG_pigmentCylindrical_map,
  };
  static const TTagList pigmentCylindricalTags [];
};

#endif
