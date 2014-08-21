#ifndef __PRIMITIVE_SPHERE_H__
#define __PRIMITIVE_SPHERE_H__

#include "primitive.h"

struct PrimitiveSphere: public TPrimitive {
  //----- Datas
  TPoint3 m_Center;
  float m_Radius;

  //----- Constructor
  PrimitiveSphere();

  //----- Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &world );
  
  //----- Methods
  virtual bool intersectionRayCamera( const TRay &ray, TIntersection &inter ) const;
  virtual bool intersectionRay( const TRay &ray,
				TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float distMax2 ) const;
  virtual bool allIntersectionRayCamera( const TRay &ray,
					 TIntersectionStack &IStack ) const;
  virtual bool allIntersectionRay( const TRay &ray,
				   TIntersectionStack &IStack ) const;
  virtual TVector3 normalAt( const TIntersection &inter ) const;
  virtual bool inside( const TPoint3 &p ) const;
  virtual bool computeBoundingAABB( BoundingAABB &a_Box ) const;
  virtual bool intersectsBox( const BoundingAABB &a_Box ) const;
  
  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //****** Private *****
 private:
  float m_Radius2; // OPTIMIZING DATAS
  float m_InvRadius; // OPTIMIZING DATAS
  TVector3 m_PrecalcRayToCenter; // OPTIMIZING DATAS/FRAME
  float m_PrecalcC; // OPTIMIZING DATAS/FRAME
  //--tag
  static const enum EPrimitiveSphereTags {
    TAG_primitiveSphere_center,
    TAG_primitiveSphere_radius
  };
  static const TTagList primitiveSphereTags [];
};

#endif
