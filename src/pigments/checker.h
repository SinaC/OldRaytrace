#ifndef __PIGMENT_CHECKER_H__
#define __PIGMENT_CHECKER_H__

#include "pigment.h"

struct PigmentChecker: public TPigment {
  //----- Datas
  TColor colorBlack;
  TColor colorWhite;
  TVector3 size; // block size

  //----- Constructor
  PigmentChecker();

  // Setup
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
  TVector3 invSize;  // Optimizing datas
  //--tag
  static const enum EPigmentCheckerTags {
    TAG_pigmentChecker_black,
    TAG_pigmentChecker_white,
    TAG_pigmentChecker_size,
  };
  static const TTagList pigmentCheckerTags [];
};

#endif
