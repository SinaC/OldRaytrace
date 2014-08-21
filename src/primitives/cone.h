#ifndef __PRIMITIVE_CONE_H__
#define __PRIMITIVE_CONE_H__

#include "primitive.h"

struct PrimitiveCone: public TPrimitive {
  //----- Datas
  TPoint3 center;
  TVector3 axis;
  float angle; // 0 < angle < pi/2

  //----- Constructor
  PrimitiveCone();

  //----- Setup
  virtual void setup();
  
  //----- Methods
  virtual bool intersectionRay( const TRay &ray,
				TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float distMax2 ) const;
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

  //****** Private *****
 private:
  float cosAngle; // OPTIMIZING DATAS
  float sinAngle; // OPTIMIZING DATAS

  //--tag
  static const enum EPrimitiveConeTags {
    TAG_primitiveCone_center,
    TAG_primitiveCone_angle,
    TAG_primitiveCone_axis
  };
  static const TTagList primitiveConeTags [];
};

#endif
