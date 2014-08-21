#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

#include "types/basics.h"
#include "math/vector3.h"

struct TObject;
struct TTexture;
struct TPrimitive;
struct TRay;

struct TIntersection;

struct TIntersectionStack {
  //----- Datas
  int size;
  TIntersection *stack;
  int index;

  //----- Constructor
  TIntersectionStack();
  //----- Destructor
  ~TIntersectionStack();

  //----- Methods
  void create( const int size );
  void push( const TIntersection &intersection );
  void push( const float distance );
  void push( const float distance, const u32 additional );
  void push( const float distance, const TPrimitive *CSG, const bool inverse );
  TIntersection pop();
  TIntersection top();
  void clear();

  //----- Output
  void print( const char *msg ) const;
};

struct TIntersection {
  //----- Datas
  TPoint3 pointWorldSpace; // intersection point between ray and primitive in world space
  TPoint3 pointObjectSpace; // intersection point between ray and primitive in object space
  float distance; // distance from ray.position to intersection point
  const TObject *object; // object intersected
  TVector3 normal; // normal to object at intersection point (perturbed)
  TVector3 rawNormal; // raw normal without any perturbation
  const TTexture *texture; // texture at intersection point
  const TRay *ray; // ray giving this intersection

  // CSG specific
  const TPrimitive *CSGPrimitive; // primitive hitted by a ray in a CSG
  bool inverse; // normal and inside must be inversed (used by CSG-difference)

  union {
    u8 side; // which side of box, cylinder or capsule
  } additionalInformation;

  //----- Constructor
  TIntersection();
  //----- Destructor
  ~TIntersection();

  //----- Methods
  void copyIntersection( const TIntersection &inter );
  void copyIntersectionEverything( const TIntersection &inter );
  void clear();

  //----- Output
  void print( const char *msg ) const;
};

#endif
