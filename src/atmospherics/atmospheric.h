#ifndef __ATMOSPHERIC_H__
#define __ATMOSPHERIC_H__

#include "../intersection.h"
#include "../color.h"
#include "../handle.h"

struct TAtmospheric: public THandle {
  //----- Datas
  u32 atmosphericKind;
  static const enum EAtmosphericKind {
    ATMOSPHERIC_NOTHING,
    ATMOSPHERIC_FOG,
    ATMOSPHERIC_GROUND,
    ATMOSPHERIC_VOLUMETRIC
  };

  //----- Constructor
  TAtmospheric();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual TColor addColor( const TColor &currentColor,
			   const TIntersection &intersection ) const;
  virtual float getAttenuation( const TIntersection &intersection ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find subclass type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  char *atmosphericName() const;
  //-- tag
  static const enum EAtmosphericTags {
  };
  static const TTagList atmosphericTags [];
  static const TTagList atmosphericSubClassTags [];
};

#endif
