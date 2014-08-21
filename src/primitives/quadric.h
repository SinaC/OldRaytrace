#ifndef __PRIMITIVE_QUADRIC_H__
#define __PRIMITIVE_QUADRIC_H__

#include "primitive.h"
#include "../math/matrix3.h"

// center-matrix form, (X-C)^T A (X-C) = R, where A is a positive
// definite matrix
struct PrimitiveQuadric: public TPrimitive {
  //----- Datas
  TVector3 center;
  float radius;
  TMatrix3 A;

  //----- Constructor
  PrimitiveQuadric();

  //----- Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &world );

  //----- Methods
  virtual bool intersectionRayCamera( const TRay &ray,
				      TIntersection &inter ) const;
  virtual bool intersectionRay( const TRay &ray,
				TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float distMax2 ) const;
  virtual bool allIntersectionRayCamera( const TRay &ray,
					 TIntersectionStack &IStack ) const;
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
  float radius2; // OPTIMIZING DATAS
  float invRadius; // OPTIMIZING DATAS
  TVector3 precalcKMatDiff; // OPTIMIZING DATAS/FRAME
  float precalcC; // OPTIMIZING DATAS/FRAME

  //--tag
  static const enum EPrimitiveQuadricTags {
    TAG_primitiveQuadric_center,
    TAG_primitiveQuadric_radius,
    TAG_primitiveQuadric_A,
  };
  static const TTagList primitiveQuadricTags [];
};

#endif
