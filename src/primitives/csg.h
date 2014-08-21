#ifndef __PRIMITIVE_CSG_H__
#define __PRIMITIVE_CSG_H__

#include <vector>
using namespace std;

#include "primitive.h"

struct PrimitiveCSG: public TPrimitive {
  //----- Datas
  static const enum CSGTypes {
    CSG_UNION,
    CSG_INTERSECTION,
    CSG_DIFFERENCE,
    CSG_MERGE,
    CSG_SINGLE
  };
  int operation; // a value from enum CSGTypes

  //----- Constructor
  PrimitiveCSG();
  ~PrimitiveCSG();

  //----- Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &world );
  
  //----- Methods
  void addPrimitive( TPrimitive *primitive, const bool inverse );
  virtual bool intersectionRay( const TRay &ray, TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float maxDist2 ) const;
  virtual bool allIntersectionRay( const TRay &ray,
				   TIntersectionStack &IStack ) const;
  virtual TVector3 normalAt( const TIntersection &inter ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  vector<TPrimitive *> primitiveList;

  //-- Union
  bool intersectionRayUnion( const TRay &ray, TIntersection &inter ) const;
  bool intersectsRayUnion( const TRay &ray, const float maxDist2 ) const;
  bool allIntersectionRayUnion( const TRay &ray,
				TIntersectionStack &IStack ) const;

  //-- Intersection
  bool intersectionRayIntersection( const TRay &ray, TIntersection &inter ) const;
  bool intersectsRayIntersection( const TRay &ray, const float maxDist2 ) const;
  bool allIntersectionRayIntersection( const TRay &ray,
				       TIntersectionStack &IStack ) const;

  //-- Difference
  bool intersectionRayDifference( const TRay &ray, TIntersection &inter ) const;
  bool intersectsRayDifference( const TRay &ray, const float maxDist2 ) const;
  bool allIntersectionRayDifference( const TRay &ray,
				     TIntersectionStack &IStack ) const;

  //-- Merge
  bool intersectionRayMerge( const TRay &ray, TIntersection &inter ) const;
  bool intersectsRayMerge( const TRay &ray, const float maxDist2 ) const;
  bool allIntersectionRayMerge( const TRay &ray,
				TIntersectionStack &IStack ) const;

  //--tag
  static const enum EPrimitiveCsgTags {
    TAG_primitiveCsg_operation,
    TAG_primitiveCsg_primitiveList,
  };
  static const TTagList primitiveCsgTags [];

  static const TTagList primitiveCsgOperations [];
};

#endif
