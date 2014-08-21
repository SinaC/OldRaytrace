#ifndef __RAY_H__
#define __RAY_H__

#include "math/vector3.h"
#include "intersection.h"
#include "types/basics.h"

struct TObject;

#define MAX_NESTED_PRIMITIVES (10)

// id:  max 15 primitives, 4 lights, no transmission and max reflection depth 3
// ( 0-> 3)   primitive id + 1
// ( 4-> 7)   ( sum  1 << light id ) << 4
// ( 8->11)   ( reflected lvl 1 primitive id + 1 ) << 8
// (12->15)   ( sum  1 << reflected lvl 1 light id ) << 12
// (16->19)   ( reflected lvl 2 primitive id + 1 ) << 8
// (20->23)   ( sum  1 << reflected lvl 2 light id ) << 12
// (24->27)   ( reflected lvl 3 primitive id + 1 ) << 8
// (27->31)   ( sum  1 << reflected lvl 3 light id ) << 12
struct TRay {
  //----- Datas
  u32 kind;
  static const enum ERayKind {
    RAY_NOTHING,
    RAY_CAMERA,
    RAY_REFLECTED,
    RAY_TRANSMITTED,
    RAY_SHADOW,
    RAY_INTERNAL_REFLECTED
  };
  TPoint3 origin; // position of the ray
  TVector3 direction; // direction of the ray (normalized)
  TVector3 reverseDirection; // 1/direction (normalized) OPTIMIZATION
  TIntersection intersection; // intersection between ray and an object
  TPoint3 target; // used for shadow rays

  u32 id; // see comment above
  u32 rayId; // unique id

  TRay *reflectedRay; // reflected ray, NOT YET USED
  TRay *transmittedRay; // transmitted/refracted ray, NOT YET USED
  //--  TRay *lightRay; // used to compute shadow
  TRay *lightRays[4]; // max 4 light rays

  // list of nested object
  int insideListIndex;
  const TObject *insideList[MAX_NESTED_PRIMITIVES];

  //------ Constructor
  TRay();
  //------ Destructor
  ~TRay();

  //----- Methods
  void assignId();
  void copy( const TRay &ray );
  void clear(); // clear ray informations
  void update(); // normalized direction, compute reverseDirection
  //-- STACK
  // enter in "object"
  void enter( const TObject *object );
  // leave out "object"
  void leave( const TObject *object );
  // check if "object" is in insideList
  bool inside( const TObject *object ) const;
  // return last element in list
  const TObject* getLastInList() const;
  // check if list is empty
  bool isInsideListEmpty() const;
  // copy inside list
  void copyInsideList( const TRay &fromRay );

  //------ Output
  void print( const char *msg ) const;
};

#endif
