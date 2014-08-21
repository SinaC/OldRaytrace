#ifndef __METAOBJECTS_H__
#define __METAOBJECTS_H__

#include "primitive.h"

#define MAX_META_POINTS (4)

struct MetaSubObject: public THandle {
  //---- Datas
  u32 metaKind;
  static const enum EMetaKind {
    META_NOTHING,
    META_SPHERE,
    META_CYLINDER,
    META_PLANE
  };
  TVector3 loc;
  float strength;

  //---- Constructor
  MetaSubObject();

  //---- Methods
  virtual float density( float x, float x2 ) const = 0;
  // get the max and do some precalc once a ray
  virtual void getMax( float &max_p, float &max_D, const TRay &ray ) = 0;
  virtual void addGrad( TVector3 &res, const TVector3 &hit ) = 0;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find subclass type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  virtual void print( const char *msg ) const;

  //**** Private
  private:
  char *metaName() const;
  //--tag
  static const enum EMetaTags {
    TAG_meta_loc,
    TAG_meta_strength
  };
  static const TTagList metaTags [];
  static const TTagList metaSubClassTags [];
};

struct MetaSphere : public MetaSubObject {
  //---- Constructor
  MetaSphere();

  //---- Methods
  virtual float density( float x, float x2 ) const;
  // get the max and do some precalc once a ray
  virtual void getMax( float &max_p, float &max_D, const TRay &ray );
  virtual void addGrad( TVector3 &res, const TVector3 &hit );

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  virtual void print( const char *msg ) const;

  //***** Private
 private:
  float B, C;
  static const enum EMetaSphereTags {
  };
  static const TTagList metaSphereTags [];
};

struct MetaCylinder : public MetaSubObject {
  //---- Constructor
  MetaCylinder();

  //---- Methods
  virtual float density( float x, float x2 ) const;
  // get the max and do some precalc once a ray
  virtual void getMax( float &max_p, float &max_D, const TRay &ray );
  virtual void addGrad( TVector3 &res, const TVector3 &hit );

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  virtual void print( const char *msg ) const;

  //***** Private
 private:
  float A, B, C;
  static const enum EMetaCylinderTags {
  };
  static const TTagList metaCylinderTags [];
};


struct MetaPlane : public MetaSubObject {
  //---- Datas
  float slant;

  //---- Constructor
  MetaPlane();

  //---- Methods
  virtual float density( float x, float x2 ) const;
  // get the max and do some precalc once a ray
  virtual void getMax( float &max_p, float &max_D, const TRay &ray );
  virtual void addGrad( TVector3 &res, const TVector3 &hit );

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  virtual void print( const char *msg ) const;

  //***** Private
 private:
  float A, B, C;
  static const enum EMetaPlaneTags {
    TAG_metaplane_slant,
  };
  static const TTagList metaPlaneTags [];
};

struct PrimitiveMetaObjects : public TPrimitive {
  //----- Datas
  float threshold;
  int num_subs;
  MetaSubObject* subs[MAX_META_POINTS];

  //----- Constructor
  PrimitiveMetaObjects();
  //----- Destructor
  ~PrimitiveMetaObjects();

  //----- Methods
  virtual bool intersectionRay( const TRay &ray,
				TIntersection &inter ) const;
  virtual bool intersectsRay( const TRay &ray, const float distMax2 ) const;
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
  float density(float x) const;
  //--tag
  static const enum EPrimitiveMetaTags {
    TAG_primitiveMeta_threshold,
    TAG_primitiveMeta_subs,
  };
  static const TTagList primitiveMetaTags [];
};

#endif
