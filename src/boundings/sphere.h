#ifndef __BOUNDING_SPHERE_H__
#define __BOUNDING_SPHERE_H__

#include "../math/vector3.h"
#include "bounding.h"
#include "aabb.h"

// Axis-aligned bounding box
struct BoundingSphere: public TBounding {
  //----- Datas
  TPoint3 m_Center;
  float m_Radius2;

  //----- Constructor
  BoundingSphere();
  // create a sphere from a set of points
  virtual void create( const int a_NumP, const TPoint3 *a_Pts );

  //----- Methods
  virtual bool intersectsBox( const BoundingAABB &a_Box ) const;
  virtual bool intersectsRay( const TRay &a_Ray ) const;
  virtual bool inside( const TPoint3 &a_P ) const;
  void merge( const BoundingSphere &a_Sphere );

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
  static const enum EBoundingSphereTags {
    TAG_boundingSphere_center,
    TAG_boundingSphere_radius2
  };
  static const TTagList boundingSphereTags [];
};

#endif
