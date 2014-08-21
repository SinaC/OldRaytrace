#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "intersection.h"
#include "object.h"
#include "error.h"

//---------------------
//-- Intersection stack

TIntersectionStack::TIntersectionStack() {
  size = 0;
  index = 0;
  stack = NULL;
}

TIntersectionStack::~TIntersectionStack() {
  if ( stack )
    delete [] stack;
  stack = NULL;
  index = 0;
  size = 0;
}

void TIntersectionStack::create( const int _size ) {
  size = _size;
  stack = new TIntersection [size];
  index = 0;
}

void TIntersectionStack::clear() {
  for ( int i = 0; i < index; i++ )
    stack[i].clear();
  index = 0;
}

void TIntersectionStack::push( const TIntersection &intersection ) {
  if ( index >= size )
    g_error("Intersection stack is full\n");
  stack[index++].copyIntersectionEverything( intersection );
}

TIntersection TIntersectionStack::pop() {
  return stack[--index];
}

TIntersection TIntersectionStack::top() {
  return stack[index-1];
}

void TIntersectionStack::push( const float distance ) {
  if ( index >= size )
    g_error("Intersection stack is full\n");
  stack[index++].distance = distance;
}

void TIntersectionStack::push( const float distance, const u32 additional ) {
  if ( index >= size )
    g_error("Intersection stack is full\n");
  stack[index].distance = distance;
  stack[index].additionalInformation.side = additional; // FIXME
  index++;
}

void TIntersectionStack::push( const float distance, const TPrimitive *CSG, const bool inverse ) {
  if ( index >= size )
    g_error("Intersection stack is full\n");
  stack[index].distance = distance;
  stack[index].CSGPrimitive = CSG;
  stack[index].inverse = inverse;
  index++;
}

void TIntersectionStack::print( const char *msg ) const {
  printf("%s\n", msg );
  printf("size: %d\n", size );
  if ( index == 0 )
    printf("empty\n");
  else
    for ( int i = 0; i < index; i++ ) {
      char s[128];
      sprintf( s, "stack[%d]", i );
      stack[i].print( s );
    }
}

//---------------
//-- Intersection

TIntersection::TIntersection() {
  distance = INFINITE;
  inverse = false;
  object = NULL;
  CSGPrimitive = NULL;
  texture = NULL;
  ray = NULL;
  additionalInformation.side = 0;
}

TIntersection::~TIntersection() {
}

void TIntersection::copyIntersection( const TIntersection &inter ) {
  distance = inter.distance;
  additionalInformation.side = inter.additionalInformation.side;
  CSGPrimitive = inter.CSGPrimitive;
  inverse = inter.inverse;
  ray = inter.ray;
}

void TIntersection::copyIntersectionEverything( const TIntersection &intersection ) {
  pointWorldSpace = intersection.pointWorldSpace;
  pointObjectSpace = intersection.pointObjectSpace;
  distance = intersection.distance;
  object = intersection.object;
  normal = intersection.normal;
  rawNormal = intersection.rawNormal;
  texture = intersection.texture;
  CSGPrimitive = intersection.CSGPrimitive;
  inverse = intersection.inverse;
  additionalInformation = intersection.additionalInformation;
  ray = intersection.ray;
}

void TIntersection::clear() {
  object = NULL;
  texture = NULL;
  CSGPrimitive = NULL;
  inverse = false;
  distance = INFINITE;
  ray = NULL;
  additionalInformation.side = 0;
}

void TIntersection::print( const char *msg ) const {
  printf("%s  object: %x\n", msg, (int)object ); fflush(stdout);
  if ( object ) {
    pointWorldSpace.print("intersection point (world space)"); fflush(stdout);
    pointObjectSpace.print("intersection point (object space)"); fflush(stdout);
    printf("distance to intersection: %5.5f\n", distance ); fflush(stdout);
    object->print("intersected object"); fflush(stdout);
    normal.print("normal: "); fflush(stdout);
    printf("inverse: %s\n", inverse?"true":"false"); fflush(stdout);
    printf("CSGPrimitive: %x\n", (int)CSGPrimitive);
    printf("texture: %x\n", (int)texture);
  }
  else
    printf("NO INTERSECTION\n"); fflush(stdout);
}
