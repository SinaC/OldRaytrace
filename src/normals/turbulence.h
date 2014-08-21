#ifndef __NORMAL_TURBULENCE_H__
#define __NORMAL_TURBULENCE_H__

#include "normal.h"

struct TTurbulence;

// Material definition
struct NormalTurbulence: public TNormal {
  //----- Datas
  //**  protected:
  TTurbulence *turbulence;
  float factor;

  public:

  //----- Constructors
  NormalTurbulence();

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
  static const enum ENormalTurbulenceTags {
    TAG_normalTurbulence_turbulence,
    TAG_normalTurbulence_factor
  };
  static const TTagList normalTurbulenceTags [];
};

#endif
