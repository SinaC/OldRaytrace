#ifndef __RAY_VALUE_H__
#define __RAY_VALUE_H__

#pragma warning (disable:4200) // list of zero element in a struct or union (see ValueList)

struct THandle;
struct TColor;
struct TVector2;
struct TVector3;
struct TVector4;
struct FCT_DATA;

#define SVT_INT		(0)
#define SVT_STR		(1)
#define SVT_FLOAT       (2)
#define SVT_VECTOR3     (3)
#define SVT_COLOR       (4)
#define SVT_VAR         (5)
#define SVT_LIST        (6)
#define SVT_VECTOR4     (7)
#define SVT_HANDLE      (8)
#define SVT_FCT         (9)
#define SVT_VECTOR2	(10)
#define SVT_FLOAT_PTR   (11)
#define SVT_VOID	(99)

struct Value;
struct ValueList;

union val_types {
  long	          i;
  const char 	 *s;
  float           f;
  float          *fp;
  TVector3       *v3;
  TVector4       *v4;
  TVector2       *v2;
  TColor         *c;
  Value          *value;
  ValueList      *l;
  THandle        *h;
  FCT_DATA       *fct;
};


///////////////////////// VALUES //////////////////

struct Value {
  static const Value nullValue;

  int			typ;  
  union val_types	val;

  const char* asStr() const;
  long asInt() const;
  ValueList* asList() const;
  float asFloat() const;
  float* asFloatPointer() const;
  TVector2 asVector2() const;
  TVector2* asVector2Pointer() const;
  TVector3 asVector3() const;
  TVector3* asVector3Pointer() const;
  TVector4 asVector4() const;
  TVector4* asVector4Pointer() const;
  TColor asColor() const;
  THandle *asHandle() const;
  bool asBool() const; // added to avoid a warning when converting long to bool
  FCT_DATA* asFct() const;

  Value() {
    typ = SVT_INT;
    val.i = 0;
  }

  Value(ValueList* l) {
    typ = SVT_LIST;
    val.l = l;
  }

  Value(const long i) {
    typ = SVT_INT;
    val.i = i;
  }

  Value(const int i) {
    typ = SVT_INT;
    val.i = i;
  }

  Value(const unsigned int i) {
    typ = SVT_INT;
    val.i = i;
  }

  Value(FCT_DATA* f) {
    typ = SVT_FCT;
    val.fct = f;
  }

  Value(const float f);
  Value(const float *fPtr);
  Value(const TVector2 *vectPtr);
  Value(const TVector2 &vect);
  Value(const TVector3 *vectPtr);
  Value(const TVector3 &vect);
  Value(const TVector4 *vectPtr);
  Value(const TVector4 &vect);
  Value(const TColor *color);
  Value(const TColor &color);
  Value(THandle *handle);

  Value(Value* v) {
    typ = SVT_VAR;
    val.value = v;
  }

  Value(const char* s);


  void setValue(const Value& v);

  Value get() const {
    if (typ == SVT_VAR) {
      return val.value->get();
    } else {
      return *this;
    }
  };

  //--Value& operator=(const Value & v);
  //--void operator = ( const Value &v  );

  ~Value();
};

struct ValueList {
  int size;
  Value elems[0]; // warning with Visual Studio, check pragma on top

  Value get( const int n) const;

  static ValueList* newList(int size);
  static ValueList* emptyList();
};

Value operator+(const Value& a, const Value& b);
Value operator-(const Value& a, const Value& b);
Value operator*(const Value& a, const Value& b);
Value operator/(const Value& a, const Value& b);
Value operator-(const Value& r);
int operator==(const Value& a,  const Value& b);
int operator<(const Value& a,  const Value& b);
int operator>(const Value& a,  const Value& b);
int operator!=(const Value& a,  const Value& b);
int operator<=(const Value& a,  const Value& b);
int operator>=(const Value& a,  const Value& b);
Value operator!(const Value& r);

#endif
