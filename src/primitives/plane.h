#ifndef __PRIMITIVE_PLANE_H__
#define __PRIMITIVE_PLANE_H__

#include "primitive.h"

struct PrimitivePlane: public TPrimitive {
  //----- Datas
  TVector3 m_Normal;
  float m_Distance;

  //----- Constructor
  PrimitivePlane();

  //----- Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &a_World );

  //----- Methods
  virtual bool intersectionRayCamera( const TRay &a_Ray,
				      TIntersection &a_Inter ) const;
  virtual bool intersectionRay( const TRay &a_Ray,
				TIntersection &a_Inter ) const;
  virtual bool intersectsRay( const TRay &a_Ray, const float a_DistMax2 ) const;
  virtual bool allIntersectionRayCamera( const TRay &a_Ray,
					 TIntersectionStack &a_IStack ) const;
  virtual bool allIntersectionRay( const TRay &a_Ray,
				   TIntersectionStack &a_IStack ) const;
  virtual TVector3 normalAt( const TIntersection &a_Inter ) const;
  virtual bool inside( const TPoint3 &a_P ) const;
  virtual bool intersectsBox( const BoundingAABB &a_Box ) const;
  virtual bool computeBoundingAABB( BoundingAABB &a_Box ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;
  
  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  //-- Datas
  float m_PrecalcV0; // OPTIMIZING DATAS/FRAME

  //--tag
  static const enum EPrimitivePlaneTags {
    TAG_primitivePlane_normal,
    TAG_primitivePlane_distance
  };
  static const TTagList primitivePlaneTags [];
};

#endif
