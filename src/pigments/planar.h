#ifndef __PIGMENT_PLANAR_H__
#define __PIGMENT_PLANAR_H__

#include "pigment.h"

struct TBitmapColor;

struct PigmentPlanar: public TPigment {
  //----- Datas
  TBitmapColor *map;
  TVector3 gradient; // determine which coordinates are used for mapping
  float size; // virtually expand texture to fit a bigger (>1x1) "square" in world

  //----- Constructor
  PigmentPlanar();
  //----- Destructor
  ~PigmentPlanar();

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
  float invSize;
  //--tag
  static const enum EPigmentPlanarTags {
    TAG_pigmentPlanar_map,
    TAG_pigmentPlanar_gradient,
    TAG_pigmentPlanar_size
  };
  static const TTagList pigmentPlanarTags [];
};

#endif
