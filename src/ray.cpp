#include <stdlib.h>
#include <stdio.h>

#include "ray.h"
#include "object.h"
#include "error.h"

TRay::TRay() {
  reflectedRay = NULL;
  transmittedRay = NULL;
  for ( int i = 0; i < 4; i++ )
    lightRays[i] = NULL;
  insideListIndex = 0;
  id = 0;
  kind = RAY_NOTHING;
  assignId();
}

TRay::~TRay() {
  clear();
}

void TRay::assignId() {
  static u32 uniqueRayId = 0;
  rayId = uniqueRayId;
  uniqueRayId++;
}

void TRay::clear() {
  if ( reflectedRay != NULL )
    delete reflectedRay;
  reflectedRay = NULL;
  if ( transmittedRay != NULL )
    delete transmittedRay;
  transmittedRay = NULL;
  for ( int i = 0; i < 4; i++ ) {
    if ( lightRays[i] != NULL )
      delete lightRays[i];
    lightRays[i] = NULL;
  }
  intersection.clear();
  id = 0;
}

//-- STACK
// copy an inside list from a ray
void TRay::copyInsideList( const TRay &fromRay ) {
  for ( int i = 0; i < fromRay.insideListIndex; i++ )
    insideList[insideListIndex++] = fromRay.insideList[i];
}
// enter in "object"
void TRay::enter( const TObject *object ) {
  if ( insideListIndex >= MAX_NESTED_PRIMITIVES )
    g_error("TRay::enter: Too many nested refracting objects!\n");
  insideList[insideListIndex++] = object;
}
// leave out "object"
void TRay::leave( const TObject *object ) {
  int i;
  int n = -1;
  for ( i = 0; i < insideListIndex; i++ )
    if ( insideList[i] == object ) {
      n = i;
      break;
    }
  if ( n == -1 )
    g_error("TRay::leave: Object not found in insideList!\n");
  for ( i = n; i < insideListIndex; i++ )
    insideList[i] = insideList[i+1];
  insideListIndex--;
  if ( insideListIndex < 0 )
    g_error("TRay::leave: Too many exits from refraction!\n");
}
// check if "object" is in insideList
bool TRay::inside( const TObject *object ) const {
  for ( int i = 0; i < insideListIndex; i++ )
    if ( insideList[i] == object )
      return true;
  return false;
}
// return last element in list
const TObject* TRay::getLastInList() const {
  if ( insideListIndex == 0 )
    g_error("TRay::getLastInList: Empty list!\n");
  return insideList[insideListIndex-1];
}
// check if list is empty
bool TRay::isInsideListEmpty() const {
  if ( insideListIndex == 0 )
    return true;
  return false;
}

void TRay::update() {
  direction.normalize();
  reverseDirection = inverseVector( direction );
}

void TRay::copy( const TRay &ray ) {
  origin = ray.origin;
  direction = ray.direction;
  reverseDirection = ray.reverseDirection;
  target = ray.target;
}

void TRay::print( const char *msg ) const {
  printf("%s\n", msg );
  printf("ID: %8x\n", id );
  origin.print("ORIGIN: ");
  direction.print("DIRECTION: ");
  reverseDirection.print("REVERSEDIRECTION: ");
  target.print("TARGET: ");
  intersection.print("INTERSECTION: ");
  if ( reflectedRay != NULL )
    reflectedRay->print("REFLECTED: ");
  if ( transmittedRay != NULL )
    transmittedRay->print("TRANSMITTED: ");
  for ( int i = 0; i < 4; i++ )
    if ( lightRays[i] != NULL )
      lightRays[i]->print("LIGHT RAY: ");
  if ( insideListIndex > 0 ) {
    printf("INSIDE LIST:\n");  fflush(stdout);
    for ( int i = 0; i < insideListIndex; i++ )
      insideList[i]->print("INSIDE:");
  }
  else
    printf("IN ATMOSPHERE\n");
  fflush(stdout);
}
