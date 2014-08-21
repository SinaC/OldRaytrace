#ifndef __PRIMITIVE_SUPERELLIPSOID_H__
#define __PRIMITIVE_SUPERELLIPSOID_H__

#include "primitive.h"

struct PrimitiveSuperellipsoid: public TPrimitive {
  //----- Datas
  float e, n;

  //----- Constructor
  PrimitiveSuperellipsoid();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual bool intersectionRay( const TRay &ray,
				TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float distMax2 ) const;
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

  //***** Private *****
  private:
  TVector3 power;
  //--tag
  static const enum EPrimitiveSuperellipsoidTags {
    TAG_primitiveSuperellipsoid_e,
    TAG_primitiveSuperellipsoid_n
  };
  static const TTagList primitiveSuperellipsoidTags [];

  bool intersectBox( const TRay &ray, float *dmin, float *dmax ) const;
  float evaluateSuperellipsoid( const TPoint3 &P ) const;
  float powerf( const float x, const float e ) const;
  int findRayPlanePoints( const TRay &ray, int cnt,
			  float *dists,
			  float mindist, float maxdist ) const;
  void solveHit1( const float v0, const TVector3 &tP0,
		  const float v1, const TVector3 &tP1,
		  TVector3 &P ) const;
  bool checkHit2( const TRay &ray, const float t0,
		  TVector3 &P0, float v0,
		  const float t1,
		  float *t, TVector3 &Q ) const;
};

#endif
