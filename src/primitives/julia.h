#ifndef __PRIMITIVE_JULIA_H__
#define __PRIMITIVE_JULIA_H__

#include "primitive.h"
#include "../math/vector2.h"
#include "../math/vector4.h"

struct PrimitiveJulia: public TPrimitive {
  //----- Datas
  static const enum EType {
    TYPE_QUATERNION,
    TYPE_HYPERCOMPLEX
  };
  static const enum ESubType {
    SUB_TYPE_EXP = 0,
    SUB_TYPE_LOG,
    SUB_TYPE_SIN,
    SUB_TYPE_ASIN,
    SUB_TYPE_COS,
    SUB_TYPE_ACOS,
    SUB_TYPE_TAN,
    SUB_TYPE_ATAN,
    SUB_TYPE_SINH,
    SUB_TYPE_ASINH,
    SUB_TYPE_COSH,
    SUB_TYPE_ACOSH,
    SUB_TYPE_TANH,
    SUB_TYPE_ATANH,
    SUB_TYPE_PWR,
    SUB_TYPE_SQR,
    SUB_TYPE_CUBE,
    SUB_TYPE_RECIPROCAL
  };
  TVector4 juliaParm;
  TVector4 slice;                 // hyperplane normal vector
  float sliceDist;                // distance from slice hyperplane to origin
  float exitValue;
  int n;                          // number of iterations
  float precision;                // precision value

  int algebra;                    // Quaternion or Hypercomplex
  int subType;                    // fractal subtype
  TComplex exponent;              // exponent of power function

  TVector3 center;                // used by fBoundMethod
  float radius2;                  // used by fBoundMethod

  //----- Constructor
  PrimitiveJulia();

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

  //**** Private
  //--  protected:
  typedef void (*NORMAL_CALC_METHOD) ( TVector3 &, const int, const PrimitiveJulia & );
  typedef bool (*ITERATION_METHOD) ( const TVector3 &, const PrimitiveJulia & );
  typedef bool (*D_ITERATION_METHOD) ( const TVector3 &, float &, const PrimitiveJulia & );
  typedef bool (*F_BOUND_METHOD) ( const TRay &, float &, float &, const PrimitiveJulia & );
  typedef void (*COMPLEX_FUNCTION_METHOD) ( TComplex &, const TComplex & );

  NORMAL_CALC_METHOD normalCalcMethod;
  ITERATION_METHOD iterationMethod;
  D_ITERATION_METHOD dIterationMethod;
  F_BOUND_METHOD fBoundMethod;
  COMPLEX_FUNCTION_METHOD complexFunctionMethod;

  static COMPLEX_FUNCTION_METHOD complexFunctionList [];

  private:
  //--tag
  static const enum EPrimitiveJuliaTags {
    TAG_primitiveJulia_param,
    TAG_primitiveJulia_slice,
    TAG_primitiveJulia_exitValue,
    TAG_primitiveJulia_n,
    TAG_primitiveJulia_precision,
    TAG_primitiveJulia_type,
    TAG_primitiveJulia_subType,
    TAG_primitiveJulia_center,
    TAG_primitiveJulia_radius2,
    TAG_primitiveJulia_exponent,
  };
  static const TTagList primitiveJuliaTags [];

  static const TTagList primitiveJuliaTypes [];
  static const TTagList primitiveJuliaSubTypes [];
};

// QUATERN.H
bool F_Bound_Julia ( const TRay &, float &, float &, const PrimitiveJulia & );
void Normal_Calc_Julia ( TVector3 &, const int, const PrimitiveJulia & );
void Normal_Calc_z3 ( TVector3 &, const int, const PrimitiveJulia & );
bool Iteration_Julia ( const TVector3 &, const PrimitiveJulia & );
bool D_Iteration_Julia ( const TVector3 &, float &, const PrimitiveJulia & );
bool Iteration_z3 ( const TVector3 &, const PrimitiveJulia & );
bool D_Iteration_z3 ( const TVector3 &, float &, const PrimitiveJulia & );

// HCMPLX.H
int HReciprocal ( float &xr, float &yr, float &zr, float &wr,
		  const float x, const float y, const float z, const float w );
void HFunc ( float &xr, float &yr, float &zr, float &wr,
	     const float x, const float y, const float z, const float w,
	     const PrimitiveJulia & );

bool F_Bound_HCompl ( const TRay &, float &, float &, const PrimitiveJulia & );
void Normal_Calc_HCompl ( TVector3 &, const int, const PrimitiveJulia & );
bool Iteration_HCompl ( const TVector3 &, const PrimitiveJulia & );
bool D_Iteration_HCompl ( const TVector3 &, float &, const PrimitiveJulia & );

bool F_Bound_HCompl_z3 ( const TRay &, float &, float &, const PrimitiveJulia & );
void Normal_Calc_HCompl_z3 ( TVector3 &, const int, const PrimitiveJulia & );
bool Iteration_HCompl_z3 ( const TVector3 &, const PrimitiveJulia & );
bool D_Iteration_HCompl_z3 ( const TVector3 &, float &, const PrimitiveJulia & );

bool F_Bound_HCompl_Reciprocal ( const TRay &, float &, float &, const PrimitiveJulia & );
void Normal_Calc_HCompl_Reciprocal ( TVector3 &, const int, const PrimitiveJulia & );
bool Iteration_HCompl_Reciprocal ( const TVector3 &, const PrimitiveJulia & );
bool D_Iteration_HCompl_Reciprocal ( const TVector3 &, float &, const PrimitiveJulia & );

bool F_Bound_HCompl_Func ( const TRay &, float &, float &, const PrimitiveJulia & );
void Normal_Calc_HCompl_Func ( TVector3 &, const int, const PrimitiveJulia & );
bool Iteration_HCompl_Func ( const TVector3 &, const PrimitiveJulia & );
bool D_Iteration_HCompl_Func ( const TVector3 &, float &, const PrimitiveJulia & );

#endif
