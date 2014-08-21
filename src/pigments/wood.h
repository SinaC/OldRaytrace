#ifndef __PIGMENT_WOOD_H__
#define __PIGMENT_WOOD_H__

#include "pigment.h"
#include "../colormap.h"
#include "../turbulences/turbulence.h"

struct PigmentWood: public TPigment {
  //----- Datas
  TColorMap *map;
  TTurbulence *turbulence;

  //----- Constructor
  PigmentWood();
  //----- Destructor
  ~PigmentWood();

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
  static const enum EPigmentWoodTags {
    TAG_pigmentWood_map,
    TAG_pigmentWood_turbulence
  };
  static const TTagList pigmentWoodTags [];
};

#endif
