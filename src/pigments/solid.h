#ifndef __PIGMENT_SOLID_H__
#define __PIGMENT_SOLID_H__

#include "pigment.h"

struct PigmentSolid: public TPigment {
  //----- Datas
  TColor color;

  //----- Constructor
  PigmentSolid();

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
  static const enum EPigmentSolidTags {
    TAG_pigmentSolid_color,
  };
  static const TTagList pigmentSolidTags [];
};

#endif
