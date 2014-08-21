#ifndef __NORMAL_H__
#define __NORMAL_H__

#include "../handle.h"
#include "../intersection.h"

struct TNormal: public THandle {
  //----- Datas
  u32 normalKind;
  static const enum ENormalKind {
    NORMAL_NOTHING,
    NORMAL_BUMPS,
    NORMAL_TURBULENCE
  };

  //----- Constructor
  TNormal();
  //----- Destructor
  ~TNormal();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual void perturbNormal( TIntersection &inter ) const;

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
  char *normalName() const;
  //--tag
  static const enum ENormalTags {
  };
  static const TTagList normalTags [];
  static const TTagList normalSubClassTags [];
};

#endif
