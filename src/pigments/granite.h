#ifndef __PIGMENT_GRANITE_H__
#define __PIGMENT_GRANITE_H__

#include "pigment.h"
#include "../colormap.h"

struct PigmentGranite: public TPigment {
  //----- Datas
  TColorMap *map;

  //----- Constructor
  PigmentGranite();
  //----- Destructor
  ~PigmentGranite();

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
  static const enum EPigmentGraniteTags {
    TAG_pigmentGranite_map,
  };
  static const TTagList pigmentGraniteTags [];
};

#endif
