#ifndef __PIGMENT_TORUSMAP_H__
#define __PIGMENT_TORUSMAP_H__

#include "pigment.h"

struct TBitmapColor;

struct PigmentTorusmap: public TPigment {
  //----- Datas
  TBitmapColor *map;
  TVector3 gradient; // determine which coordinates are used for mapping

  //----- Constructor
  PigmentTorusmap();
  //----- Destructor
  ~PigmentTorusmap();

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
  static const enum EPigmentTorusmapTags {
    TAG_pigmentTorusmap_map,
    TAG_pigmentTorusmap_gradient
  };
  static const TTagList pigmentTorusmapTags [];
};

#endif
