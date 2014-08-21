#ifndef __PRIMITIVE_BLOB_H__
#define __PRIMITIVE_BLOB_H__

#include <vector>
using namespace std;

#include "primitive.h"

struct BlobSubObject: public THandle {
  //---- Datas
  u32 blobKind;
  static const enum EBlobKind {
    BLOB_NOTHING,
    BLOB_SPHERE,
    BLOB_CYLINDER,
    BLOB_BASE_HEMISPHERE, //   hemispheres are internally used to cap cylinder can't be parsed
    BLOB_APEX_HEMISPHERE
  };
  TVector3 origin; // origin
  float c[3];      // coeffs  c[2] is strength
  float f[5];      // final coeffs

  //---- Constructor
  BlobSubObject();

  //----- Setup
  virtual void setup( vector<BlobSubObject*> &blobs ) = 0;
  
  //----- Methods
  virtual void normalAt( TVector3 &normal, const TPoint3 &p ) const = 0;
  virtual float calculateFieldValue( const TPoint3 &p ) const = 0;
  virtual void computeCoeff( const TPoint3 &p, const TVector3 &v ) = 0;
  virtual bool intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const = 0;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find subclass type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //**** Private
  private:
  char *blobName() const;
  //--tag
  static const enum EBlobTags {
    TAG_blob_origin,
    TAG_blob_strength
  };
  static const TTagList blobTags [];
  static const TTagList blobSubClassTags [];
};

struct BlobSphere: public BlobSubObject {
  float radius2;      // radius squared

  //---- Constructor
  BlobSphere();

  //----- Setup
  virtual void setup( vector<BlobSubObject*> &blobs );

  //----- Methods
  virtual void normalAt( TVector3 &normal, const TPoint3 &p ) const;
  virtual float calculateFieldValue( const TPoint3 &p ) const;
  virtual void computeCoeff( const TPoint3 &p, const TVector3 &v );
  virtual bool intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  static const enum EBlobSphereTags {
    TAG_blobsphere_radius2
  };
  static const TTagList blobSphereTags [];
};

// z-axis cylinder
struct BlobCylinder: public BlobSubObject {
  float radius2;      // radius squared
  float len;         // cylinder length

  //---- Constructor
  BlobCylinder();

  //----- Setup
  virtual void setup( vector<BlobSubObject*> &blobs );

  //----- Methods
  virtual void normalAt( TVector3 &normal, const TPoint3 &p ) const;
  virtual float calculateFieldValue( const TPoint3 &p ) const;
  virtual void computeCoeff( const TPoint3 &p, const TVector3 &v );
  virtual bool intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  static const enum EBlobCylinderTags {
    TAG_blobcylinder_radius2,
    TAG_blobcylinder_len
  };
  static const TTagList blobCylinderTags [];
};

struct BlobBaseHemisphere: public BlobSubObject {
  float radius2;      // radius squared

  //---- Constructor
  BlobBaseHemisphere();

  //----- Setup
  virtual void setup( vector<BlobSubObject*> &blobs );

  //----- Methods
  virtual void normalAt( TVector3 &normal, const TPoint3 &p ) const;
  virtual float calculateFieldValue( const TPoint3 &p ) const;
  virtual void computeCoeff( const TPoint3 &p, const TVector3 &v );
  virtual bool intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const;

  //----- Output
  virtual void print( const char *msg ) const;
};

struct BlobApexHemisphere: public BlobSubObject {
  float radius2;      // radius squared
  float len;       // cylinder length

  //---- Constructor
  BlobApexHemisphere();

  //----- Setup
  virtual void setup( vector<BlobSubObject*> &blobs );

  //----- Methods
  virtual void normalAt( TVector3 &normal, const TPoint3 &p ) const;
  virtual float calculateFieldValue( const TPoint3 &p ) const;
  virtual void computeCoeff( const TPoint3 &p, const TVector3 &v );
  virtual bool intersectionRay( const TRay &ray, const float mindist, float &tmin, float &tmax ) const;

  //----- Output
  virtual void print( const char *msg ) const;
};

struct PrimitiveBlob: public TPrimitive {
  //----- Datas
  //  int numBlobs;
  vector<BlobSubObject *> blobs;
  float threshold;

  //----- Constructor
  PrimitiveBlob();
  //----- Destructor
  ~PrimitiveBlob();

  //----- Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &world );
  
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

  //***** Private
  protected:
  //----- Datas
  struct TInterval {
    u32 type;
    float bound;
    BlobSubObject *element;
  };
  u32 intervalCount;
  TInterval *intervals;
  static const enum EIntervalType {
    ENTERING,
    EXITING
  };

  //----- Methods
  float calculateFieldValue( const TPoint3 &p ) const;
  int determineInfluence( const TRay &ray, float minDist ) const;
  void insertHit( const float t0, const float t1, BlobSubObject *blob );

  private:
  //--tag
  static const enum EPrimitiveBlobTags {
    TAG_primitiveBlob_threshold,
    TAG_primitiveBlob_blobs
  };
  static const TTagList primitiveBlobTags [];
};

#endif
