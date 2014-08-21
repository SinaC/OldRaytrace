#ifndef __TURBULENCE_H__
#define __TURBULENCE_H__

#include "../math/vector3.h"
#include "../handle.h"

struct TTurbulence: public THandle {
  //---- Datas
  u32 turbulenceKind;
  static const enum ETurbulenceKind {
    TURBULENCE_NOTHING,
    TURBULENCE_PERLIN,
    TURBULENCE_NOISE
  };
  TVector3 turbulence;
  int octaves;
  float lambda, omega;

  //---- Constructor
  TTurbulence();

  //---- Setup
  void setup();

  //---- Methods
  virtual float getTurbulence1( const TPoint3 &p ) const;
  virtual TVector3 getTurbulence3( const TPoint3 &p ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find subclass type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  void print( const char *msg ) const;

  //***** Private
  private:
  //--tag
  char *turbulenceName() const;
  static const enum ETurbulenceTags {
    TAG_turbulence_turbulence,
    TAG_turbulence_octaves,
    TAG_turbulence_lambda,
    TAG_turbulence_omega
  };
  static const TTagList turbulenceTags [];
  static const TTagList turbulenceSubClassTags [];
};

#endif
