#ifndef __INTERIOR_H__
#define __INTERIOR_H__

#include "handle.h"
#include "color.h"

// Object's interior: index of refraction, media, ...
struct TInterior: public THandle {
  //----- Datas
  float ior;
  TColor absorption; // One absorption factor / color component

  //----- Constructor
  TInterior();

  //----- Setup
  virtual void setup();

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
  static const enum EInteriorTags {
    TAG_interior_ior,
	TAG_interior_absorption
  };
  static const TTagList interiorTags [];
};

#endif
