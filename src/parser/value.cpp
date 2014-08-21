#include <stdio.h>
#include <stdlib.h>

#include "../text/basics.h"
#include "value.h"
#include "parser_skel.h"

#include "../math/vector2.h"
#include "../math/vector3.h"
#include "../math/vector4.h"
#include "../color.h"
#include "../handle.h"
#include "functions.h"

// -------- Value: constants
const Value Value::nullValue( (long)0 );

// -------- Value: name
static const char *value_type_name( const int type ) {
  switch ( type ) {
  case SVT_INT: return "int"; break;
  case SVT_STR: return "string"; break;
  case SVT_FLOAT: return "float"; break;
  case SVT_FLOAT_PTR: return "floatPtr"; break;
  case SVT_COLOR: return "color"; break;
  case SVT_VECTOR2: return "vector2"; break;
  case SVT_VECTOR3: return "vector3"; break;
  case SVT_VECTOR4: return "vector4"; break;
  case SVT_HANDLE: return "handle"; break;
  case SVT_FCT: return "function"; break;
  case SVT_VAR: return "var"; break;
  case SVT_LIST: return "list"; break;
  case SVT_VOID: return "void"; break;
  default: return "unknown"; break;
  }
}

// ------- Value: constructors and destructor

Value:: Value(const char * s) {
  typ = SVT_STR;
  val.s = str_dup(s);
}

Value:: Value(const float f) {
  typ = SVT_FLOAT;
  val.f = f;
}

Value:: Value(const float *fPtr) {
  typ = SVT_FLOAT_PTR;
  val.fp = (float*)fPtr;
}

Value:: Value(const TVector2 *vect) {
  typ = SVT_VECTOR2;
  val.v2 = (TVector2*)vect;
}

Value:: Value(const TVector2 &vect) {
  typ = SVT_VECTOR2;
  val.v2 = (TVector2 *)&vect;
}

Value:: Value(const TVector3 *vect) {
  typ = SVT_VECTOR3;
  val.v3 = (TVector3*)vect;
}

Value:: Value(const TVector3 &vect) {
  typ = SVT_VECTOR3;
  val.v3 = (TVector3 *)&vect;
}

Value:: Value(const TVector4 *vect) {
  typ = SVT_VECTOR4;
  val.v4 = (TVector4*)vect;
}

Value:: Value(const TVector4 &vect) {
  typ = SVT_VECTOR4;
  val.v4 = (TVector4 *)&vect;
}

Value:: Value(const TColor *color) {
  typ = SVT_COLOR;
  val.c = (TColor*)color;
}

Value:: Value(const TColor &color ) {
  typ = SVT_COLOR;
  val.c = (TColor *)&color;
}

Value:: Value(THandle *handle ) {
  typ = SVT_HANDLE;
  val.h = handle;
}

Value:: ~Value() {
}

// -------- Value: setValue method

void Value::setValue(const Value& v) {
  if (typ == SVT_VAR)
    *(val.value) = v.get();
  else
    p_error("Trying to set constant! :(");
}

// ------ Value: conversion

char *computeListString(ValueList *l) {
  char *buf = new char [4096];
  int start = 0;
  strcpy(buf,"[");
  if ( l->size > 25 ) {
    strcat(buf,"...");
    start = l->size-25;
  }
  for ( int i = start; i < l->size; i++ ) {
    strcat(buf, l->elems[i].asStr());
    if ( i < l->size-1 )
      strcat(buf,",");
  }
  strcat(buf,"]");
  return buf;
}

const char* Value::asStr() const {
  Value tmp = get();
  // Modified by SinaC 2003, size was 1024 before
  static char buf[4096];
  switch(tmp.typ) {
  case SVT_STR:
    return tmp.val.s;
  case SVT_INT:
    sprintf( buf, "INT[%ld]", tmp.val.i );
    return buf;
  case SVT_FLOAT:
    sprintf( buf, "FLOAT[%f]", tmp.val.f );
    return buf;
  case SVT_FLOAT_PTR:
    sprintf( buf, "FLOAT PTR[%f]", *tmp.val.fp );
    return buf;
  case SVT_VECTOR2:
    if ( tmp.val.v2 == NULL )
      sprintf(buf, "Vector2(NULL)");
    else
      sprintf( buf, "Vector2( %5.2f, %5.2f )", tmp.val.v2->v[0], tmp.val.v2->v[1] );
  case SVT_VECTOR3:
    if ( tmp.val.v3 == NULL )
      sprintf(buf, "Vector(NULL)");
    else
      sprintf( buf, "Vector( %5.2f, %5.2f, %5.2f )", tmp.val.v3->v[0], tmp.val.v3->v[1], tmp.val.v3->v[2] );
    return buf;
  case SVT_VECTOR4:
    if ( tmp.val.v4 == NULL )
      sprintf(buf, "Vector4(NULL)");
    else
      sprintf( buf, "Vector4( %5.2f, %5.2f, %5.2f, %5.2f )",
	       tmp.val.v4->v[0], tmp.val.v4->v[1], tmp.val.v4->v[2], tmp.val.v4->v[3] );
    return buf;
  case SVT_COLOR:
    if ( tmp.val.c == NULL )
      sprintf(buf, "Color(NULL)");
    else
      sprintf( buf, "Color( %5.2f, %5.2f, %5.2f )", tmp.val.c->r(), tmp.val.c->g(), tmp.val.c->b() );
    return buf;
  case SVT_HANDLE:
    if ( tmp.val.h == NULL )
      sprintf(buf, "HANDLE(NULL)");
    else
      sprintf( buf, "HANDLE %s", tmp.val.h->handleName() );
    return buf;
  case SVT_LIST:
    if ( tmp.val.l == NULL )
      sprintf(buf, "List(NULL)");
    else
      return computeListString(tmp.val.l);
  case SVT_FCT:
    if ( tmp.val.fct == NULL )
      sprintf(buf, "Function(NULL)");
    else
      sprintf( buf, "Function( %20s, [%3d] )", tmp.val.fct->name, tmp.val.fct->nbparm );
    return buf;
  default:
    sprintf( buf, "<typ=%d>", tmp.typ );
    return buf;
  }
};

long Value::asInt() const {
  Value tmp = get();
  if ( tmp.typ != SVT_INT )
    p_error("Trying to cast into int a non int (%s)", tmp.asStr());
  return tmp.val.i;
}

float Value::asFloat() const {
  Value tmp = get();
  if ( tmp.typ == SVT_INT )
    return (float)tmp.val.i;
  else if ( tmp.typ == SVT_FLOAT )
    return tmp.val.f;
  else if ( tmp.typ == SVT_FLOAT_PTR )
    return *tmp.val.fp;
  else
    p_error("Trying to cast into float a non float, non int (%s)", tmp.asStr() );
  return 0.0f;
}

float *Value::asFloatPointer() const {
  Value tmp = get();
  if ( tmp.typ != SVT_FLOAT_PTR )
    p_error("Trying to cast into float pointer a non float pointer (%s)", tmp.asStr() );
  return tmp.val.fp;
}

TVector2* Value::asVector2Pointer() const {
  Value tmp = get();
  if ( tmp.typ != SVT_VECTOR2 )
    p_error("Trying to cast into vector2 a non vector (%s)", tmp.asStr());
  return tmp.val.v2;
}

TVector2 Value::asVector2() const {
  Value tmp = get();
  if ( tmp.typ != SVT_VECTOR2 )
    p_error("Trying to cast into vector2(ptr) a non vector(ptr) (%s)", tmp.asStr());
  return *tmp.val.v2;
}

TVector3* Value::asVector3Pointer() const {
  Value tmp = get();
  if ( tmp.typ != SVT_VECTOR3 )
    p_error("Trying to cast into vector3 a non vector (%s)", tmp.asStr());
  return tmp.val.v3;
}

TVector3 Value::asVector3() const {
  Value tmp = get();
  if ( tmp.typ != SVT_VECTOR3 )
    p_error("Trying to cast into vector3(ptr) a non vector(ptr) (%s)", tmp.asStr());
  return *tmp.val.v3;
}

TVector4* Value::asVector4Pointer() const {
  Value tmp = get();
  if ( tmp.typ != SVT_VECTOR4 )
    p_error("Trying to cast into vector4 a non vector4 (%s)", tmp.asStr());
  return tmp.val.v4;
}

TVector4 Value::asVector4() const {
  Value tmp = get();
  if ( tmp.typ != SVT_VECTOR4 )
    p_error("Trying to cast into vector4(ptr) a non vector4(ptr) (%s)", tmp.asStr());
  return *tmp.val.v4;
}

TColor Value::asColor() const {
  Value tmp = get();
  if ( tmp.typ != SVT_COLOR )
    p_error("Trying to cast into color a non color (%s)", tmp.asStr());
  return *tmp.val.c;
}

THandle* Value::asHandle() const {
  Value tmp = get();
  if ( tmp.typ != SVT_HANDLE )
    p_error("Trying to cast into handle a non handle (%s)", tmp.asStr());
  return tmp.val.h;
}

ValueList* Value::asList() const {
  Value tmp = get();

  switch (tmp.typ) {
  case SVT_LIST:
    return tmp.val.l;

    /* a good idea ?
  // if it's a string we wordize it
  case SVT_STR:
    return wordize(tmp.val.s).val.l;
    */
  default:
    p_error("Failed to convert into a list (%s)", tmp.asStr() );
  }
  return ValueList::emptyList();
}

bool Value::asBool() const { // added to avoid a warning when converting long to bool
  Value tmp = get();
  if ( tmp.typ != SVT_INT )
    p_error("Trying to cast into bool a non bool (%s)", tmp.asStr() );
  int i = tmp.asInt();
  if ( i == 0 )
    return false;
  return true;
}

FCT_DATA* Value::asFct() const {
  Value tmp = get();
  if ( tmp.typ != SVT_FCT )
    p_error("Trying to cast into FCT a non fct (%s)", tmp.asStr() );
  return tmp.val.fct;
}

// ------- ValueList
ValueList* ValueList::newList(int size) {
  ValueList* vl = (ValueList*) malloc( size*sizeof(Value) + sizeof(int));
  if ( vl == NULL )
    p_error("ValueList::newList  Out of memory!!");
  vl->size = size;
  memset(vl->elems, 0, size*sizeof(Value));
  return vl;
}

ValueList* ValueList::emptyList() {
  return newList(0);
}

Value ValueList::get( const int n ) const {
  if ( n >= size || n < 0 )
    p_error("Invalid parameter [%d] for get (>= size[%d] or < 0)", n, size );
  return Value(elems[n]);
}

// -------- Value: operator +, -, *, /

Value operator+(const Value& a, const Value& b) {
  const Value& a0 = a.get();
  const Value& b0 = b.get();

  if ( a0.typ == SVT_STR ) {
    char* buf = new char[strlen(a0.val.s) + strlen(b0.asStr()) + 1];
    strcpy(buf, a0.val.s);
    strcat(buf, b0.asStr());
    Value res = Value(buf);
    delete[] buf;
    return res;
  }

  if (a0.typ != b0.typ) 
    p_error("Operator + must be applied to homogenous values %s (typ %s) and %s (typ %s)", 
	    a0.asStr(), value_type_name( a0.typ ), 
	    b0.asStr(), value_type_name( b0.typ ) );
  switch (a0.typ) {
  case SVT_INT:
    return a0.val.i + b0.val.i;
    break;
  case SVT_FLOAT:
    return a0.val.f + b0.val.f;
    break;
  case SVT_VECTOR2:
    return *a0.val.v2 + *b0.val.v2;
    break;
  case SVT_VECTOR3:
    return *a0.val.v3 + *b0.val.v3;
    break;
  case SVT_VECTOR4:
    return *a0.val.v4 + *b0.val.v4;
    break;
  case SVT_COLOR:
    return *a0.val.c + *b0.val.c;
    break;
  case SVT_LIST: {
    ValueList* vl = ValueList::newList(a0.val.l->size + b0.val.l->size);
    memcpy(vl->elems,                  a0.val.l->elems, sizeof(Value) * a0.val.l->size);
    memcpy(vl->elems + a0.val.l->size, b0.val.l->elems, sizeof(Value) * b0.val.l->size);
    return vl;
    break;
  }
  default:
    p_error("Operator + cannot be applied to this type of values (only int, float, vector, color, string and list) (typ %s)",
	    value_type_name( a0.typ ));
  }
  return Value((long)0);
}
Value operator-(const Value& a, const Value& b) {
  const Value& a0 = a.get();
  const Value& b0 = b.get();

  if (a0.typ != b0.typ) 
    p_error("Operator - must be applied to homogenous values %s (typ %s) and %s (typ %s)", 
	    a0.asStr(), value_type_name( a0.typ ), 
	    b0.asStr(), value_type_name( b0.typ ) );
  switch (a0.typ) {
  case SVT_INT:
    return a0.val.i - b0.val.i;
    break;
  case SVT_FLOAT:
    return a0.val.f - b0.val.f;
    break;
  case SVT_VECTOR2:
    return *a0.val.v2 - *b0.val.v2;
    break;
  case SVT_VECTOR3:
    return *a0.val.v3 - *b0.val.v3;
    break;
  case SVT_VECTOR4:
    return *a0.val.v4 - *b0.val.v4;
    break;
  case SVT_COLOR:
    return *a0.val.c - *b0.val.c;
    break;
  default:
    p_error("Operator - cannot be applied to this type of values (only int, float, vector and color) (typ %s)",
	    value_type_name( a0.typ ));
  }
  return Value((long)0);
}

// FIXME: operator *  and /   can be applied to homogenous type
//  vector * float
//  vector * vector
//  float * vector
//  color * float
//  color * color
//  float * color
//  float * float
//  float * int
//  int * float <-- lose precision
Value operator*(const Value& a, const Value& b) {
  const Value& a0 = a.get();
  const Value& b0 = b.get();

  if (a0.typ != b0.typ) 
    p_error("Operator * must be applied to homogenous values %s (typ %s) and %s (typ %s)", 
	    a0.asStr(), value_type_name( a0.typ ), 
	    b0.asStr(), value_type_name( b0.typ ) );
  switch (a0.typ) {
  case SVT_INT:
    return a0.val.i * b0.val.i;
    break;
  case SVT_FLOAT:
    return a0.val.f * b0.val.f;
    break;
  case SVT_VECTOR2:
    return *a0.val.v2 * *b0.val.v2; // FIXME: ONLY VECTOR * FLOAT  and FLOAT * VECTOR
    break;
  case SVT_VECTOR3:
    return *a0.val.v3 * *b0.val.v3; // FIXME: ONLY VECTOR * FLOAT  and FLOAT * VECTOR
    break;
  case SVT_VECTOR4:
    return *a0.val.v4 * *b0.val.v4; // FIXME: ONLY VECTOR * FLOAT  and FLOAT * VECTOR
    break;
  case SVT_COLOR:
    return *a0.val.c * *b0.val.c; // FIXME: ONLY COLOR * FLOAT  and FLOAT * COLOR
    break;
  default:
    p_error("Operator * cannot be applied to this type of values (only int, float, vector and color) (typ %s)",
	    value_type_name( a0.typ ));
  }
  return Value((long)0);
}

Value operator/(const Value& a, const Value& b) {
  const Value& a0 = a.get();
  const Value& b0 = b.get();

  if (a0.typ != b0.typ) 
    p_error("Operator / must be applied to homogenous values %s (typ %s) and %s (typ %s)", 
	    a0.asStr(), value_type_name( a0.typ ), 
	    b0.asStr(), value_type_name( b0.typ ) );
  switch (a0.typ) {
  case SVT_INT:
    return a0.val.i / b0.val.i;
    break;
  case SVT_FLOAT:
    return a0.val.f / b0.val.f;
    break;
  case SVT_VECTOR2:
    return (*a0.val.v2) / (*b0.val.v2); // FIXME: ONLY VECTOR / FLOAT  and FLOAT / VECTOR
    break;
  case SVT_VECTOR3:
    return (*a0.val.v3) / (*b0.val.v3); // FIXME: ONLY VECTOR / FLOAT  and FLOAT / VECTOR
    break;
  case SVT_VECTOR4:
    return (*a0.val.v4) / (*b0.val.v4); // FIXME: ONLY VECTOR / FLOAT  and FLOAT / VECTOR
    break;
  case SVT_COLOR:
    return (*a0.val.c) / (*b0.val.c); // FIXME: ONLY COLOR / FLOAT  and FLOAT / COLOR
    break;
  default:
    p_error("Operator / cannot be applied to this type of values (only int, float, vector and color) (typ %s)",
	    value_type_name( a0.typ ));
  }
  return Value((long)0);
}

Value operator-(const Value& r) {
  const Value &a0 =  r.get();
  switch (a0.typ) {
  case SVT_INT:
    return -a0.val.i;
    break;
  case SVT_FLOAT:
    return -a0.val.f;
    break;
  case SVT_VECTOR2:
    return -(*a0.val.v2);
    break;
  case SVT_VECTOR3:
    return -(*a0.val.v3);
    break;
  case SVT_VECTOR4:
    return -(*a0.val.v4);
    break;
  default:
    p_error("Unary Operator - cannot be applied to this type of values (only int, float and vector) (typ %s)",
	    value_type_name( a0.typ ));
  }
  return Value((long)0);
}

// -------- Value: operator ==, <, >, <=, >=, !=

const float COMPARAISON_EPS = 0.0001f;
int operator==(const Value& a,  const Value& b) {
  Value tmpa = a.get(), 
    tmpb = b.get();

  if ( tmpa.typ != tmpb.typ ) {
    char tmpaS[1024];
    strcpy( tmpaS, tmpa.asStr() );
    p_error("Trying to compare (==) differently typed values (typ %s[%s] | typ %s[%s])",
	    value_type_name( tmpa.typ ), tmpaS,
	    value_type_name( tmpb.typ ), tmpb.asStr() );
  }

  switch ( tmpa.typ ) {
  case SVT_INT:
    return tmpa.val.i == tmpb.val.i;
  case SVT_FLOAT:
    return fabsf( tmpa.val.f - tmpb.val.f ) < COMPARAISON_EPS;
  case SVT_STR:
    return !strcmp( tmpa.asStr(), tmpb.asStr() );
  default:
    p_error("Trying to compare (==) unsupported types (typ %s | typ %s)",
	    value_type_name( tmpa.typ ), value_type_name( tmpb.typ ) );
  }
  return 0;
}

int operator<(const Value& a,  const Value& b) {
  Value tmpa = a.get(), 
    tmpb = b.get();
  if ( tmpa.typ != tmpb.typ ) {
    char tmpaS[1024];
    strcpy( tmpaS, tmpa.asStr() );
    p_error("Trying to compare (<) differently typed values (typ %s[%s] | typ %s[%s])",
	    value_type_name( tmpa.typ ), tmpaS,
	    value_type_name( tmpb.typ ), tmpb.asStr() );
  }
  switch ( tmpa.typ ) {
  case SVT_INT:
    return tmpa.val.i < tmpb.val.i;
  case SVT_FLOAT:
    return tmpa.val.f < tmpb.val.f;
  default:
    p_error("Trying to compare (<) unsupported types (typ %s | typ %s)",
	    value_type_name( tmpa.typ ), value_type_name( tmpb.typ ) );
  }
  return 0;
}
int operator>(const Value& a,  const Value& b) {
  Value tmpa = a.get(), 
    tmpb = b.get();
  if ( tmpa.typ != tmpb.typ ) {
    char tmpaS[1024];
    strcpy( tmpaS, tmpa.asStr() );
    p_error("Trying to compare (>) differently typed values (typ %s[%s] | typ %s[%s])",
	    value_type_name( tmpa.typ ), tmpaS,
	    value_type_name( tmpb.typ ), tmpb.asStr() );
  }

  switch ( tmpa.typ ) {
  case SVT_INT:
    return tmpa.val.i > tmpb.val.i;
  case SVT_FLOAT:
    return tmpa.val.f > tmpb.val.f;
  default:
    p_error("Trying to compare (>) unsupported types (typ %s | typ %s)",
	    value_type_name( tmpa.typ ), value_type_name( tmpb.typ ) );
  }
  return 0;
}
int operator!=(const Value& a,  const Value& b) {
  return !( a == b );
}
int operator<=(const Value& a,  const Value& b) {
  Value tmpa = a.get(), 
    tmpb = b.get();
  if ( tmpa.typ != tmpb.typ ) {
    char tmpaS[1024];
    strcpy( tmpaS, tmpa.asStr() );
    p_error("Trying to compare (<=) differently typed values (typ %s[%s] | typ %s[%s])",
	    value_type_name( tmpa.typ ), tmpaS,
	    value_type_name( tmpb.typ ), tmpb.asStr() );
  }
  switch ( tmpa.typ ) {
  case SVT_INT:
    return tmpa.val.i <= tmpb.val.i;
  case SVT_FLOAT:
    return tmpa.val.f <= tmpb.val.f;
  default:
    p_error("Trying to compare (<=) unsupported types (typ %s | typ %s)",
	    value_type_name( tmpa.typ ), value_type_name( tmpb.typ ) );
  }
  return 0;
}
int operator>=(const Value& a,  const Value& b) {
  Value tmpa = a.get(), 
    tmpb = b.get();
  if ( tmpa.typ != tmpb.typ ) {
    char tmpaS[1024];
    strcpy( tmpaS, tmpa.asStr() );
    p_error("Trying to compare (>=) differently typed values (typ %s[%s] | typ %s[%s])",
	    value_type_name( tmpa.typ ), tmpaS,
	    value_type_name( tmpb.typ ), tmpb.asStr() );
  }

  switch ( tmpa.typ ) {
  case SVT_INT:
    return tmpa.val.i >= tmpb.val.i;
  case SVT_FLOAT:
    return tmpa.val.f >= tmpb.val.f;
  default:
    p_error("Trying to compare (>=) unsupported types (typ %s | typ %s)",
	    value_type_name( tmpa.typ ), value_type_name( tmpb.typ ) );
  }
  return 0;
}
Value operator!(const Value& r) {
  return r.asInt() == 0;
}
