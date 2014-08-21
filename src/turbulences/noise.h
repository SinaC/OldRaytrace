#ifndef __TURBULENCE_NOISE_H__
#define __TURBULENCE_NOISE_H__

#include "turbulence.h"

struct TurbulenceNoise: public TTurbulence {
  //---- Constructor
  TurbulenceNoise();

  //---- Methods
  virtual float getTurbulence1( const TPoint3 &p ) const;
  virtual TVector3 getTurbulence3( const TPoint3 &p ) const;
  static TVector3 getNoise3( const TPoint3 &point );
  static float getNoise1( const TPoint3 &point );

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
  static const enum ETurbulenceNoiseTags {
  };
  static const TTagList turbulenceNoiseTags [];
};

void noiseInit();

#endif
