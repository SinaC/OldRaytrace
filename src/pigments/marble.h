#ifndef __PIGMENT_MARBLE_H__
#define __PIGMENT_MARBLE_H__

#include "pigment.h"
#include "../colormap.h"
#include "../turbulences/turbulence.h"

struct PigmentMarble: public TPigment {
  //----- Datas
  TColorMap *map;
  TTurbulence *turbulence;

  //----- Constructor
  PigmentMarble();
  //----- Destructor
  ~PigmentMarble();

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
  static const enum EPigmentMarbleTags {
    TAG_pigmentMarble_map,
    TAG_pigmentMarble_turbulence
  };
  static const TTagList pigmentMarbleTags [];
};

#endif
