#ifndef __BOUNDING_H__
#define __BOUNDING_H__

#include "../math/vector3.h"
#include "../types/basics.h"
#include "../handle.h"

struct TIntersection;
struct TRay;
struct TCameraSimple;
struct BoundingAABB;

// Bounding Volume
struct TBounding: public THandle {
  //----- Datas
  u32 boundingKind;
  static const enum EBoundingKind {
    BOUNDING_NOTHING,
    BOUNDING_AABB,
    BOUNDING_SPHERE,
    BOUNDING_SLAB
  };

  //----- Constructor
  TBounding();
  // create a bounding volume from a set of points
  virtual void create( const int numP, const TPoint3 *pts );

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual bool intersectsBox( BoundingAABB &a_Box ) const;
  virtual bool intersectsRay( const TRay &ray ) const;
  virtual bool inside( const TPoint3 &a_P ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find subclass type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;
  virtual void renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const;

  ///***** Private
  private:
  char *boundingName() const;
  //-- tag
  static const enum EBoundingTags {
  };
  static const TTagList boundingTags [];
  static const TTagList boundingSubClassTags [];
};

#endif
