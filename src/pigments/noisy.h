#ifndef __PIGMENT_NOISY_H__
#define __PIGMENT_NOISY_H__

#include "pigment.h"

struct PigmentNoisy: public TPigment {
  //----- Datas

  //----- Constructor
  PigmentNoisy();

  //----- Methods
  virtual TColor getColor( const TPoint3 &p ) const;

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
  static const enum EPigmentNoisyTags {
  };
  static const TTagList pigmentNoisyTags [];
};

#endif
