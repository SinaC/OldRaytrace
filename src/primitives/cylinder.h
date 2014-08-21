#ifndef __PRIMITIVE_CYLINDER_H__
#define __PRIMITIVE_CYLINDER_H__

#include "primitive.h"

struct PrimitiveCylinder: public TPrimitive {
  //----- Datas
  TVector3 center;
  TVector3 axis;
  float radius;
  bool capped; // center +/- (height/2) * axis
  float height;

  //----- Constructor
  PrimitiveCylinder();

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

  //****** Private ******
  private:
  //-- datas
  float invRadius; // OPTIMIZING DATAS
  float radius2; // OPTIMIZING DATAS
  TVector3 U, V, W; // OPTIMIZING DATAS
  TVector3 precalcRC, precalcO; // OPTIMIZING DATAS/FRAME  NON-CAPPED
  TVector3 precalcP; // OPTIMIZING DATAS/FRAME  CAPPED
  float precalcC; // OPTIMIZING DATAS/FRAME  CAPPED

  //-- methods capped
  bool intersectionRayCappedCamera( const TRay &ray,
				    TIntersection &inter ) const;
  bool intersectionRayCapped( const TRay &ray,
			      TIntersection &inter ) const;
  bool intersectsRayCapped( const TRay &ray, const float maxDist2 ) const;
  bool allIntersectionRayCappedCamera( const TRay &ray,
				       TIntersectionStack &IStack ) const;
  bool allIntersectionRayCapped( const TRay &ray,
				 TIntersectionStack &IStack ) const;
  bool insideCapped( const TPoint3 &p ) const;
  //-- methods not capped
  bool intersectionRayNonCappedCamera( const TRay &ray,
				       TIntersection &inter ) const;
  bool intersectionRayNonCapped( const TRay &ray,
				 TIntersection &inter ) const;
  bool intersectsRayNonCapped( const TRay &ray, const float maxDist2 ) const;
  bool allIntersectionRayNonCappedCamera( const TRay &ray,
					  TIntersectionStack &IStack ) const;
  bool allIntersectionRayNonCapped( const TRay &ray,
				 TIntersectionStack &IStack ) const;
  bool insideNonCapped( const TPoint3 &p ) const;

  //--tag
  static const enum EPrimitiveCylinderTags {
    TAG_primitiveCylinder_center,
    TAG_primitiveCylinder_radius,
    TAG_primitiveCylinder_axis,
    TAG_primitiveCylinder_height
  };
  static const TTagList primitiveCylinderTags [];
};

#endif

