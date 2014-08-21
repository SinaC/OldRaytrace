#ifndef __AMBIENT_SOLID_H__
#define __AMBIENT_SOLID_H__

#include "ambient.h"

struct AmbientSolid: public TAmbient {
  //---- Datas
  TColor color;
  
  //---- Constructor
  AmbientSolid();

  //---- Methods
  virtual TColor getColor( const TPoint3 &point ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  virtual void print( const char *msg ) const;

  //**** Private
  private:
  static const enum EAmbientSolidTags {
    TAG_ambientSolid_color
  };
  static const TTagList ambientSolidTags [];
};

#endif
