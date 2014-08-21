#ifndef __PRIMITIVE_BOX_H__
#define __PRIMITIVE_BOX_H__

#include "primitive.h"

struct PrimitiveBox: public TPrimitive {
  //----- Datas
  TPoint3 m_Min, m_Max;

  //----- Constructor
  PrimitiveBox();

  //----- Setup
  virtual void precalcFrame( const TWorld &a_World );

  //----- Methods
  virtual bool intersectionRayCamera( const TRay &a_Ray, TIntersection &a_Inter ) const;
  virtual bool intersectionRay( const TRay &a_Ray,
				TIntersection &a_Inter ) const;
  virtual bool intersectsRay( const TRay &a_Ray, const float a_DistMax2 ) const;
  virtual bool allIntersectionRayCamera( const TRay &a_Ray,
					 TIntersectionStack &a_IStack ) const;
  virtual bool allIntersectionRay( const TRay &a_Ray,
				   TIntersectionStack &a_IStack ) const;
  virtual TVector3 normalAt( const TIntersection &a_Inter ) const;
  virtual bool inside( const TPoint3 &a_P ) const;
  virtual bool intersectsBox( const BoundingAABB &a_box ) const;
  virtual bool computeBoundingAABB( BoundingAABB &a_Box ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;
  
  //----- Output
  virtual void print( const char *msg ) const;
  virtual void renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const;

  //***** Private
  private:
  TVector3 precalcRayToMin; // OPTIMIZING DATAS/FRAME
  TVector3 precalcRayToMax; // OPTIMIZING DATAS/FRAME
  //--tag
  static const enum EPrimitiveBoxTags {
    TAG_primitiveBox_min,
    TAG_primitiveBox_max
  };
  static const TTagList primitiveBoxTags [];
};

#endif
