#ifndef __AMBIENT_H__
#define __AMBIENT_H__

#include "../color.h"
#include "../math/vector3.h"
#include "../handle.h"

struct TAmbient: public THandle {
  //----- Datas
  u32 ambientKind;
  static const enum EAmbientKind {
    AMBIENT_NOTHING,
    AMBIENT_SOLID,
    AMBIENT_HEMISPHERICAL
  };

  //----- Constructor
  TAmbient();

  //---- Setup
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

  //---- Output
  virtual void print( const char *msg ) const;

  //---- Private
  private:
  char *ambientName() const;
  //--tag
  static const enum EAmbientTags {
  };
  static const TTagList ambientTags [];
  static const TTagList ambientSubClassTags [];
};

#endif
