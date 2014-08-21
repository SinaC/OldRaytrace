#ifndef __TURBULENCE_PERLIN_H__
#define __TURBULENCE_PERLIN_H__

#include "turbulence.h"

struct TurbulencePerlin: public TTurbulence {
  //---- Constructor
  TurbulencePerlin();

  //---- Methods
  virtual float getTurbulence1( const TPoint3 &p ) const;
  virtual TVector3 getTurbulence3( const TPoint3 &p ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  void print( const char *msg ) const;

  //***** Private
  private:
  //--tag
  static const enum ETurbulencePerlinTags {
  };
  static const TTagList turbulencePerlinTags [];
};

void perlinInit();

#endif
