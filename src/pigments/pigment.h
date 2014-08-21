#ifndef __PIGMENT_H__
#define __PIGMENT_H__

#include "../color.h"
#include "../math/vector3.h"
#include "../handle.h"

// Object's color
struct TPigment: public THandle {
  //----- Datas
  u32 pigmentKind;
  static const enum EPigmentKind {
    PIGMENT_NOTHING,
    PIGMENT_CHECKER,
    PIGMENT_CUBEMAP,
    PIGMENT_CYLINDRICAL,
    PIGMENT_GRANITE,
    PIGMENT_MANDEL,
    PIGMENT_MARBLE,
    PIGMENT_NOISY,
    PIGMENT_PLANAR,
    PIGMENT_SKYMAP,
    PIGMENT_SOLID,
    PIGMENT_SPHERICAL,
    PIGMENT_TORUSMAP,
    PIGMENT_WOOD,
  };
  TPoint3 anchor;
  TColor simple; // color used if shading is off

  //----- Constructor
  TPigment();
  //----- Destructor
  ~TPigment();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual TColor getColor( const TPoint3 &p ) const;

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
  char *pigmentName() const;
  //--tag
  static const enum EPigmentTags {
    TAG_pigment_anchor,
    TAG_pigment_simple
  };
  static const TTagList pigmentTags [];
  static const TTagList pigmentSubClassTags [];
};

#endif
