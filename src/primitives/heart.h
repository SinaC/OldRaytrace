#ifndef __PRIMITIVE_HEART_H__
#define __PRIMITIVE_HEART_H__

#include "primitive.h"

struct PrimitiveHeart: public TPrimitive {
  //----- Datas
  TPoint3 center;
  float radius;
  float tStep;
  int refinementSteps;

  //----- Constructor
  PrimitiveHeart();

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

  //***** Private *****
  private:
  //----- Datas
  float radius2, invRadius; // optimizing datas
  //-- float cTerm; // optimizing datas
  //-- TVector3 origin; // optimizing datas
  //----- Methods
  float fieldEquation( const TVector3 &v ) const;

  //--tag
  static const enum EPrimitiveHeartTags {
    TAG_primitiveHeart_center,
    TAG_primitiveHeart_radius,
    TAG_primitiveHeart_step,
    TAG_primitiveHeart_refinement,
  };
  static const TTagList primitiveHeartTags [];
};

#endif
