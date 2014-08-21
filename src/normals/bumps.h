#ifndef __NORMAL_BUMPS_H__
#define __NORMAL_BUMPS_H__

#include "normal.h"

// Material definition
struct NormalBumps: public TNormal {
  //----- Datas
  //**  protected:
  float bumps;

  public:

  //----- Constructors
  NormalBumps();

  //----- Setup
  // check enough informations have been parsed
  virtual bool check() const;

  //----- Methods
  virtual void perturbNormal( TIntersection &inter ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //----- Private
  private:
  static const enum ENormalBumpsTags {
    TAG_normalBumps_bumps
  };
  static const TTagList normalBumpsTags [];
};

#endif
