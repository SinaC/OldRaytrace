#ifndef __PRIMITIVE_TORUS_H__
#define __PRIMITIVE_TORUS_H__

#include "primitive.h"

struct PrimitiveTorus: public TPrimitive {
  //----- Datas
  float   R1,R2;    // radii of torus

  //----- Constructor
  PrimitiveTorus();

  //----- Methods
  virtual bool intersectionRay( const TRay &ray, TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float maxDist2 ) const;
  virtual bool allIntersectionRay( const TRay &ray,
				   TIntersectionStack &IStack ) const;
  virtual TVector3 normalAt( const TIntersection &inter ) const;
  virtual bool inside( const TPoint3 &p ) const;

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
  static const enum EPrimitiveTorusTags {
    TAG_primitiveTorus_radius1,
    TAG_primitiveTorus_radius2,
  };
  static const TTagList primitiveTorusTags [];
};

#endif
