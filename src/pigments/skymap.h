#ifndef __PIGMENT_SKYMAP_H__
#define __PIGMENT_SKYMAP_H__

#include "pigment.h"

struct TBitmapColor;

struct PigmentSkymap : public TPigment {
  //----- Datas
  TBitmapColor* map;

  //----- Constructor
  PigmentSkymap();
  //----- Destructor
  ~PigmentSkymap();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual TColor getColor( const TVector3 &hitVector ) const;  

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
  static const enum EPigmentSkymapTags {
    TAG_pigmentSkymap_map,
  };
  static const TTagList pigmentSkymapTags [];
};

#endif
