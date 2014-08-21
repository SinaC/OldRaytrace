#ifndef __PIGMENT_MANDEL_H__
#define __PIGMENT_MANDEL_H__

#include "pigment.h"
#include "../colormap.h"

struct PigmentMandel: public TPigment {
  //----- Datas
  TColorMap *map;
  int iterations;

  //----- Constructor
  PigmentMandel();
  //----- Destructor
  ~PigmentMandel();

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
  static const enum EPigmentMandelTags {
    TAG_pigmentMandel_map,
    TAG_pigmentMandel_iterations
  };
  static const TTagList pigmentMandelTags [];
};

#endif
