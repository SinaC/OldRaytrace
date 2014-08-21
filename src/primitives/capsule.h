#ifndef __PRIMITIVE_CAPSULE_H__
#define __PRIMITIVE_CAPSULE_H__

#include "primitive.h"

struct PrimitiveCapsule: public TPrimitive {
  //------ Datas
  TVector3 p1, p2; // in WML: a segment is a point and a direction(not normalized) p2 = point + direction
  float radius;

  //------ Constructor
  PrimitiveCapsule();

  //------ Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &world );

  //------ Methods
  virtual bool intersectionRayCamera( const TRay &ray, TIntersection &inter ) const;
  virtual bool intersectionRay( const TRay &ray, TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float maxDist2 ) const;
  virtual bool allIntersectionRayCamera( const TRay &ray,
					 TIntersectionStack &IStack ) const;
  virtual bool allIntersectionRay( const TRay &ray, TIntersectionStack &IStack ) const;
  virtual TVector3 normalAt( const TIntersection &inter ) const;
  virtual bool inside( const TPoint3 &p ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private ****
  private:
  float radius2; // OPTIMIZING DATAS
  float invRadius; // OPTIMIZING DATAS
  TVector3 direction; //  // OPTIMIZING DATAS: p2 - p1
  TVector3 U, V, W; // OPTIMIZING DATAS
  float height; // OPTIMIZING DATAS: |p2-p1|
  TVector3 precalcP; // OPTIMIZING DATAS/FRAME
  float precalcC; // OPTIMIZING DATAS/FRAME

  //--tag
  static const enum EPrimitiveCapsuleTags {
    TAG_primitiveCapsule_p1,
    TAG_primitiveCapsule_p2,
    TAG_primitiveCapsule_radius,
  };
  static const TTagList primitiveCapsuleTags [];
};

#endif
