#ifndef __TRANSFORMED_H__
#define __TRANSFORMED_H__

#include "primitive.h"
#include "../math/transform.h"

struct PrimitiveTransformed: public TPrimitive {
  //----- Datas
  TPrimitive *primitive;
  TTransform transform;

  //----- Constructor
  PrimitiveTransformed();
  ~PrimitiveTransformed();

  //----- Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &world );
  
  //----- Methods
  virtual bool intersectionRay( const TRay &ray, TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float maxDist2 ) const;
  virtual bool intersectsBox( const BoundingAABB &a_Box ) const;
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

  //***** Private
  private:
  //--tag
  static const enum EPrimitiveTransformedTags {
    TAG_primitiveTransformed_primitive,
    TAG_primitiveTransformed_transformation,
    TAG_primitiveTransformed_rotate,
    TAG_primitiveTransformed_translate,
    TAG_primitiveTransformed_scale
  };
  static const TTagList primitiveTransformedTags [];
};

#endif
