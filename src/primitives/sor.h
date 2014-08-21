#ifndef __PRIMITIVE_SOR_H__
#define __PRIMITIVE_SOR_H__

#include "primitive.h"
#include "../math/vector2.h"

struct BCylinder;

struct PrimitiveSor: public TPrimitive {
  //----- Datas
  struct SorSpline {
    struct SorSplineEntry {
      float A, B, C, D;
    };
    SorSplineEntry *entry;
    BCylinder *BCyl; // bounding cylinder
  };
  int number;
  SorSpline *spline; // list of spline segments
  float height1, height2; // min/max height
  float radius1, radius2; // min/max radius
  float baseRadius2; // radius^2 of base plane
  float capRadius2; // radius^2 of cap plane

  //----- Constructor
  PrimitiveSor();
  //----- Destructor
  ~PrimitiveSor();

  //----- Setup
  virtual void setup();
  
  //----- Methods
  void create( TVector2 *p ); // create the sor using a list of 2D points
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
  //--tag
  static const enum EPrimitiveSorTags {
    TAG_primitiveSor_points,
    TAG_primitiveSor_open
  };
  static const TTagList primitiveSorTags [];
};

#endif
