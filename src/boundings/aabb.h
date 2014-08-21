#ifndef __BOUNDING_AABB_H__
#define __BOUNDING_AABB_H__

#include "../math/vector3.h"
#include "bounding.h"

// Axis-aligned bounding box
struct BoundingAABB: public TBounding {
  //----- Datas
  TPoint3 m_Min, m_Max;

  //----- Constructor
  BoundingAABB();
  // create an AABB from a set of points
  virtual void create( const int a_NumP, const TPoint3 *a_Pts );

  //----- Methods
  virtual bool intersectsBox( const BoundingAABB &a_Box ) const;
  virtual bool intersectsRay( const TRay &a_Ray ) const;
  virtual bool inside( const TPoint3 &a_P ) const;
  bool intersectionRay( const TPoint3 &a_Origin, const TVector3 &a_Direction, const TVector3 &a_ReverseDirection,
			float &a_Distance ) const;

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
  static const enum EBoundingAABBTags {
    TAG_boundingAABB_min,
    TAG_boundingAABB_max
  };
  static const TTagList boundingAABBTags [];
};

#endif
