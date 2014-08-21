#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "../types/basics.h"
#include "../math/vector3.h"
#include "../types/bitvector.h"
#include "../handle.h"

struct TCameraSimple;
struct TRay;
struct TIntersection;
struct TIntersectionStack;
struct TCamera;
struct TWorld;
struct BoundingAABB;

struct TPrimitive: public THandle {
  //----- Datas
  u32 primitiveKind;
  static const enum EPrimitiveKind {
    PRIMITIVE_NOTHING,
    PRIMITIVE_PLANE,
    PRIMITIVE_SPHERE,
    PRIMITIVE_BOX,
    PRIMITIVE_TORUS,
    PRIMITIVE_META,
    PRIMITIVE_CYLINDER,
    PRIMITIVE_CAPSULE,
    PRIMITIVE_BLOB,
    PRIMITIVE_CSG,
    PRIMITIVE_TRANSFORMED,
    PRIMITIVE_HEART,
    PRIMITIVE_QUADRIC,
    PRIMITIVE_JULIA,
    PRIMITIVE_SUPERELLIPSOID,
    PRIMITIVE_SOR,
    PRIMITIVE_CONE
  };
  static const enum EAdditionalInformation { // also used as EPrimitiveTags
    PRIMITIVE_INFINITE = 1, // infinite? (non-capped cylinder, plane, quadric)
    PRIMITIVE_INVERSE, // used in CSG
    PRIMITIVE_NO_SHADING, // not shaded (no diffuse, reflection, ...)
    PRIMITIVE_CAST_NO_SHADOW, // doesn't cast shadows
    PRIMITIVE_RECEIVE_NO_SHADOW, // doesn't receive shadow
    PRIMITIVE_CAST_NO_SELF_SHADOW, // doesn't cast shadows on itself
    PRIMITIVE_CAST_NO_REFLECTION, // doesn't cast reflection
    PRIMITIVE_CAST_NO_SELF_REFLECTION, // doesn't cast reflection on itself
    PRIMITIVE_CAST_NO_TRANSMISSION, // doesn't cast transmission
    PRIMITIVE_CAST_NO_SELF_TRANSMISSION, // doesn't cast transmission on itself
    PRIMITIVE_CLOSED, // is primitive closed
    PRIMITIVE_2D // doesn't have an interior
  };
  TBitvector additionalInformation;
  u32 rayId; // id of last ray testing intersection with this

  //----- Constructor
  TPrimitive();
  //----- Destructor
  ~TPrimitive();

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
  virtual bool intersectsBox( const BoundingAABB &a_Box ) const;
  virtual bool computeBoundingAABB( BoundingAABB &a_Box ) const;

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

  //***** Private
  private:
  char *primitiveName() const;
  //--tag
  // EPrimitiveTags: to avoid to duplicate informations, we use EAdditionalInformation as EPrimitiveTags
  static const TTagList primitiveTags [];
  static const TTagList primitiveSubClassTags [];
};

#endif
