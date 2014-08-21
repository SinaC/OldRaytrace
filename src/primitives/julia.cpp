#include <stdlib.h>
#include <stdio.h>

#include "julia.h"
#include "../ray.h"

const TTagList PrimitiveJulia::primitiveJuliaTags [] = {
  { "param", TAG_primitiveJulia_param },
  { "slice", TAG_primitiveJulia_slice },
  { "exitValue", TAG_primitiveJulia_exitValue },
  { "n", TAG_primitiveJulia_n },
  { "precision", TAG_primitiveJulia_precision },
  { "type", TAG_primitiveJulia_type },
  { "subtype", TAG_primitiveJulia_subType },
  { "center", TAG_primitiveJulia_center },
  { "radius", TAG_primitiveJulia_radius2 },
  { "exponent", TAG_primitiveJulia_exponent },
  { NULL, -1 }
};

const TTagList PrimitiveJulia::primitiveJuliaTypes [] = {
  { "quaternion", TYPE_QUATERNION },
  { "hypercomplex", TYPE_HYPERCOMPLEX },
  { NULL, -1 }
};
const TTagList PrimitiveJulia::primitiveJuliaSubTypes [] = {
  { "exp", SUB_TYPE_EXP },
  { "log", SUB_TYPE_LOG },
  { "sin", SUB_TYPE_SIN },
  { "asin", SUB_TYPE_ASIN },
  { "cos", SUB_TYPE_COS },
  { "acos", SUB_TYPE_ACOS },
  { "tan", SUB_TYPE_TAN },
  { "atan", SUB_TYPE_ATAN },
  { "sinh", SUB_TYPE_SINH },
  { "asinh", SUB_TYPE_ASINH },
  { "cosh", SUB_TYPE_COSH },
  { "acosh", SUB_TYPE_ACOSH },
  { "tanh", SUB_TYPE_TANH },
  { "atanh", SUB_TYPE_ATANH },
  { "power", SUB_TYPE_PWR },
  { "sqr", SUB_TYPE_SQR },
  { "cube", SUB_TYPE_CUBE },
  { "reciprocal", SUB_TYPE_RECIPROCAL },
  { NULL, -1 }
};

const float fractalTolerance = 1e-7f;

float *Sx, *Sy, *Sz, *Sw; // global variables
TVector3 direction;       // global variables
TVector3 computedNormal;  // global variable

// TODO: struct with  int (subType) and  fct ptr (corresponding complex function)
PrimitiveJulia::COMPLEX_FUNCTION_METHOD PrimitiveJulia::complexFunctionList [] = {
  // must match SUB_TYPE list in julia.h
  TComplex::exp,
  TComplex::log,
  TComplex::sin,
  TComplex::asin,
  TComplex::cos,
  TComplex::acos,
  TComplex::tan,
  TComplex::atan,
  TComplex::sinh,
  TComplex::asinh,
  TComplex::cosh,
  TComplex::acosh,
  TComplex::tanh,
  TComplex::atanh,
  TComplex::pwr 
};

PrimitiveJulia::PrimitiveJulia() {
  TPrimitive::TPrimitive(); primitiveKind = PRIMITIVE_JULIA;

  center = TVector3::zeroVector;

  juliaParm[__X] = 1.0f;
  juliaParm[__Y] = 0.0f;
  juliaParm[__Z] = 0.0f;
  juliaParm[__T] = 0.0f;

  slice[__X] = 0.0f;
  slice[__Y] = 0.0f;
  slice[__Z] = 0.0f;
  slice[__T] = 1.0f;
  sliceDist = 0.0f;

  exitValue = 4.0f;

  n = 20;

  precision = 1.0f / 20.0f;

  algebra = TYPE_QUATERNION;
  subType = SUB_TYPE_SQR;

  normalCalcMethod = NULL;
  iterationMethod   = NULL;
  dIterationMethod = NULL;
  fBoundMethod     = NULL;
  complexFunctionMethod = NULL;

  radius2 = 0.0f;
  exponent[__X] = 0.0f;
  exponent[__X] = 0.0f;

  Sx = NULL; Sy = NULL; Sz = NULL; Sw = NULL;
  direction = TVector3::zeroVector;
}

void PrimitiveJulia::setup() {
  float R;
  switch ( algebra ) {
  case TYPE_QUATERNION:
    switch( subType ) {
    case SUB_TYPE_CUBE:
      iterationMethod = Iteration_z3;
      normalCalcMethod = Normal_Calc_z3;
      dIterationMethod = D_Iteration_z3;
      break;
    case SUB_TYPE_SQR:
      iterationMethod = Iteration_Julia;
      dIterationMethod = D_Iteration_Julia;
      normalCalcMethod = Normal_Calc_Julia;
      break;
    default:
      error("PrimitiveJulia::setup: Illegal function: quaternion only supports sqr and cube\n");
    }
    fBoundMethod = F_Bound_Julia;
    R = 1.0f + sqrtf( SQR( juliaParm[__X] ) +
		      SQR( juliaParm[__Y] ) +
		      SQR( juliaParm[__Z] ) +
		      SQR( juliaParm[__T] ) );
    R += fractalTolerance; // fix bug when Julia_Parameter exactly 0
    if (R > 2.0f)
      R = 2.0f;
    exitValue = SQR( R );
    break;
  case TYPE_HYPERCOMPLEX:
    switch ( subType) {
    case SUB_TYPE_RECIPROCAL:
      iterationMethod = Iteration_HCompl_Reciprocal;
      normalCalcMethod = Normal_Calc_HCompl_Reciprocal;
      dIterationMethod = D_Iteration_HCompl_Reciprocal;
      fBoundMethod = F_Bound_HCompl_Reciprocal;
      break;
    case SUB_TYPE_EXP: case SUB_TYPE_LOG: case SUB_TYPE_SIN:
    case SUB_TYPE_ASIN: case SUB_TYPE_COS: case SUB_TYPE_ACOS:
    case SUB_TYPE_TAN: case SUB_TYPE_ATAN: case SUB_TYPE_SINH:
    case SUB_TYPE_ASINH: case SUB_TYPE_COSH: case SUB_TYPE_ACOSH:
    case SUB_TYPE_TANH: case SUB_TYPE_ATANH: case SUB_TYPE_PWR: 
      iterationMethod = Iteration_HCompl_Func;
      normalCalcMethod = Normal_Calc_HCompl_Func;
      dIterationMethod = D_Iteration_HCompl_Func;
      fBoundMethod = F_Bound_HCompl_Func;
      complexFunctionMethod = complexFunctionList[subType];
      break;
    case SUB_TYPE_CUBE:
      iterationMethod = Iteration_HCompl_z3;
      normalCalcMethod = Normal_Calc_HCompl_z3;
      dIterationMethod = D_Iteration_HCompl_z3;
      fBoundMethod = F_Bound_HCompl_z3;
      break;

    default:  // SQR_STYPE or else...
      iterationMethod = Iteration_HCompl;
      normalCalcMethod = Normal_Calc_HCompl;
      dIterationMethod = D_Iteration_HCompl;
      fBoundMethod = F_Bound_HCompl;
      break;
    }
    R = 4.0f;
    exitValue = 16.0f;
    break;
  default:
    error("PrimitiveJulia::setup: Algebra unknown in fractal.\n");
  }

  if ( Sx != NULL )
    delete [] Sx;
  Sx = new float [n+1];
  if ( Sy != NULL )
    delete [] Sy;
  Sy = new float [n+1];
  if ( Sz != NULL )
    delete [] Sz;
  Sz = new float [n+1];
  if ( Sw != NULL )
    delete [] Sw;
  Sw = new float [n+1];

  slice.normalize();

  radius2 = SQR(R);
}

bool PrimitiveJulia::intersectionRay( const TRay &ray,
				      TIntersection &inter ) const {
  TIntersectionStack IStack;
  IStack.create( 16 );
  if ( !PrimitiveJulia::allIntersectionRay( ray, IStack ) )
    return false;
  bool found = false;
  inter.distance = INFINITE;
  for ( int i = 0; i < IStack.index; i++ )
    if ( IStack.stack[i].distance < inter.distance ) {
      found = true;
      inter.distance = IStack.stack[i].distance;
    }
  return found;
}

bool PrimitiveJulia::intersectsRay( const TRay &ray, const float distMax2 ) const {
  TIntersection inter;
  if ( !PrimitiveJulia::intersectionRay( ray, inter ) )
    return false;
  if ( inter.distance*inter.distance < distMax2 )
    return true;
  return false;
}

bool PrimitiveJulia::allIntersectionRay( const TRay &ray,
					 TIntersectionStack &IStack ) const {
  bool Intersection_Found;
  int Last = 0;
  int CURRENT, NEXT;
  float Depth, Depth_Max;
  float Dist, Dist_Next;

  float minDepth = INFINITE;

  TVector3 IPoint, Mid_Point, Next_Point, Real_Pt;
  TVector3 Real_Normal, F_Normal;
  TRay New_Ray;

  Intersection_Found = false;

  if ( !fBoundMethod( ray, Depth, Depth_Max, *this ) )
    return false;

  //--  IStack.push( Depth );
  //--  IStack.push( Depth_Max );
  //--  return true;

  if ( Depth_Max < fractalTolerance )
    return false;

  if ( Depth < fractalTolerance )
    Depth = fractalTolerance;

  direction = ray.direction;

  // Jump to starting point
  Next_Point = ray.origin + ( Depth * ray.direction );
  CURRENT = dIterationMethod( Next_Point, Dist, *this );

  // Light ray starting inside ?
  if ( CURRENT ) {
    Next_Point = Next_Point + ( ( 2.0f * fractalTolerance ) * ray.direction );
    Depth += 2.0f * fractalTolerance;

    if ( Depth > Depth_Max )
      return false;

    CURRENT = dIterationMethod( Next_Point, Dist, *this );
  }

  // Ok. Trace it
  while ( Depth < Depth_Max ) {
    // Get close to the root: Advance with Next_Point, keeping track of last
    // position in IPoint...
    
    while (1) {
      if ( Dist < precision )
        Dist = precision;
      
      Depth += Dist;
      
      if (Depth > Depth_Max)
	return Intersection_Found;
      
      IPoint = Next_Point;
      Next_Point = Next_Point + ( Dist * ray.direction );
      
      NEXT = dIterationMethod( Next_Point, Dist_Next, *this );
      
      if ( NEXT != CURRENT ) {
        // Set surface was crossed...
        Depth -= Dist;
        break;
      }
      else
        Dist = Dist_Next; // not reached
    }

    // then, polish the root via bisection method...
    while ( Dist > fractalTolerance ) {
      Dist *= 0.5f;
      Mid_Point = IPoint + ( Dist * ray.direction );

      Last = iterationMethod( Mid_Point, *this );

      if ( Last == CURRENT ) {
	IPoint = Mid_Point;

        Depth += Dist;

        if ( Depth > Depth_Max )
          return Intersection_Found;
      }
    }
    
    if ( CURRENT == false ) { // Mid_Point isn't inside the set
      IPoint = IPoint + ( Dist * ray.direction );

      Depth += Dist;

      iterationMethod( IPoint, *this );
    }
    else {
      if ( Last != CURRENT )
        iterationMethod( IPoint, *this );
    }

    Real_Pt = IPoint;
    normalCalcMethod( Real_Normal, n, *this );

    Real_Normal.normalize();
    if ( Depth < minDepth
	 && Depth > 0.0f ) {
      minDepth = Depth;
      computedNormal = Real_Normal; // FIXME
    }
    IStack.push( Depth ); // TODO: could also save normal
    Intersection_Found = true;

    // Start over where work was left
    IPoint = Next_Point;
    Dist = Dist_Next;
    CURRENT = NEXT;
  }
  
  return Intersection_Found;
}

TVector3 PrimitiveJulia::normalAt( const TIntersection &inter ) const {
  //  return TVector3::zeroVector;
  return computedNormal; // FIXME
}

bool PrimitiveJulia::inside( const TPoint3 &p ) const {
  return iterationMethod( p, *this );
}

void PrimitiveJulia::print( const char *msg ) const {
  TPrimitive::print( msg );
  juliaParm.print("parameters: ");
  slice.print("slice normal: ");
  printf("slice dist: %5.5f\n", sliceDist );
  printf("exit value: %5.5f\n", exitValue );
  printf("n: %d\n", n );
  printf("precision: %5.5f\n", precision );
  printf("algebra: %d\n", algebra );
  printf("subType: %d\n", subType );
  exponent.print("exponent: ");
  center.print("center:");
  printf("radius2: %5.5f\n", radius2 );
}

//-------------
//-- QUATERNION
//-------------

#define Deriv_z2(n1,n2,n3,n4 )              \
{                                           \
  tmp = (n1)*x - (n2)*y - (n3)*z - (n4)*w;  \
  (n2) = (n1)*y + x*(n2);                   \
  (n3) = (n1)*z + x*(n3);                   \
  (n4) = (n1)*w + x*(n4);                   \
  (n1) = tmp;                               \
}

#define Deriv_z3(n1,n2,n3,n4)              \
{                                          \
  dtmp = 2.0f*((n2)*y + (n3)*z + (n4)*w);  \
  dtmp2 = 6.0f*x*(n1) - dtmp;              \
  (n1) = ( (n1)*x3 - x*dtmp )*3.0f;        \
  (n2) = (n2)*x4 + y*dtmp2;                \
  (n3) = (n3)*x4 + z*dtmp2;                \
  (n4) = (n4)*x4 + w*dtmp2;                \
}


bool Iteration_z3( const TPoint3 &point, const PrimitiveJulia &julia ) {
  int i;
  float x, y, z, w;
  float d, x2, tmp;
  
  Sx[0] = x = point[__X];
  Sy[0] = y = point[__Y];
  Sz[0] = z = point[__Z];
  Sw[0] = w = ( julia.sliceDist
		- julia.slice[__X]*x
		- julia.slice[__Y]*y
		- julia.slice[__Z]*z ) / julia.slice[__T];
  
  for ( i = 1; i <= julia.n; ++i ) {
    d = y * y + z * z + w * w;
    
    x2 = x * x;
    
    if ( (d + x2) > julia.exitValue )
      return false;

    tmp = 3.0f * x2 - d;

    Sx[i] = x = x * (x2 - 3.0f * d) + julia.juliaParm[__X];
    Sy[i] = y = y * tmp + julia.juliaParm[__Y];
    Sz[i] = z = z * tmp + julia.juliaParm[__Z];
    Sw[i] = w = w * tmp + julia.juliaParm[__T];
  }

  return true;
}

bool Iteration_Julia( const TPoint3 &point, const PrimitiveJulia &julia ) {
  int i;
  float x, y, z, w;
  float d, x2;

  Sx[0] = x = point[__X];
  Sy[0] = y = point[__Y];
  Sz[0] = z = point[__Z];
  Sw[0] = w = ( julia.sliceDist
		- julia.slice[__X]*x
		- julia.slice[__Y]*y
		- julia.slice[__Z]*z ) / julia.slice[__T];

  for ( i = 1; i <= julia.n; ++i) {
    d = y * y + z * z + w * w;

    x2 = x * x;

    if ( (d + x2) > julia.exitValue )
      return false;

    x *= 2.0f;

    Sy[i] = y = x * y + julia.juliaParm[__Y];
    Sz[i] = z = x * z + julia.juliaParm[__Z];
    Sw[i] = w = x * w + julia.juliaParm[__T];
    Sx[i] = x = x2 - d + julia.juliaParm[__X];

  }

  return true;
}

//----------- Distance estimator + iterations ------------
bool D_Iteration_z3( const TPoint3 &point, float &Dist, const PrimitiveJulia &julia ) {
  int i, j;
  float Norm, d;
  float xx, yy, zz;
  float x, y, z, w;
  float tmp, x2;
  float Pow;
  
  x = Sx[0] = point[__X];
  y = Sy[0] = point[__Y];
  z = Sz[0] = point[__Z];
  w = Sw[0] = ( julia.sliceDist
		- julia.slice[__X]*x
		- julia.slice[__Y]*y
		- julia.slice[__Z]*z ) / julia.slice[__T];

  for ( i = 1; i <= julia.n; i++ ) {
    d = y * y + z * z + w * w;

    x2 = x * x;

    if ( (Norm = d + x2) > julia.exitValue ) {
      // Distance estimator
      x = Sx[0];
      y = Sy[0];
      z = Sz[0];
      w = Sw[0];

      Pow = 1.0f / 3.0f;

      for ( j = 1; j < i; ++j ) {
        xx = x * Sx[j] - y * Sy[j] - z * Sz[j] - w * Sw[j];
        yy = x * Sy[j] + y * Sx[j] - z * Sw[j] + w * Sz[j];
        zz = x * Sz[j] + y * Sw[j] + z * Sx[j] - w * Sy[j];
        w  = x * Sw[j] - y * Sz[j] + z * Sy[j] + w * Sx[j];

        x = xx;
        y = yy;
        z = zz;

        Pow /= 3.0f;
      }

      Dist = Pow * sqrtf( Norm / (x * x + y * y + z * z + w * w) ) * logf( Norm );

      return false;
    }

    tmp = 3.0f * x2 - d;

    Sx[i] = x = x * (x2 - 3.0f * d) + julia.juliaParm[__X];
    Sy[i] = y = y * tmp + julia.juliaParm[__Y];
    Sz[i] = z = z * tmp + julia.juliaParm[__Z];
    Sw[i] = w = w * tmp + julia.juliaParm[__T];
  }

  Dist = julia.precision;

  return true;
}

bool D_Iteration_Julia( const TPoint3 &point, float &Dist, const PrimitiveJulia &julia ) {
  int i, j;
  float Norm, d;
  float x, y, z, w;
  float xx, yy, zz, x2;
  float Pow;

  x = Sx[0] = point[__X];
  y = Sy[0] = point[__Y];
  z = Sz[0] = point[__Z];
  w = Sw[0] = ( julia.sliceDist
		- julia.slice[__X]*x
		- julia.slice[__Y]*y
		- julia.slice[__Z]*z ) / julia.slice[__T];

  for ( i = 1; i <= julia.n; i++ ) {
    d = y * y + z * z + w * w;

    x2 = x * x;

    if ( (Norm = d + x2) > julia.exitValue) {
      // Distance estimator
      x = Sx[0];
      y = Sy[0];
      z = Sz[0];
      w = Sw[0];

      Pow = 1.0f / 2.0f;

      for ( j = 1; j < i; ++j ) {
        xx = x * Sx[j] - y * Sy[j] - z * Sz[j] - w * Sw[j];
        yy = x * Sy[j] + y * Sx[j] + w * Sz[j] - z * Sw[j];
        zz = x * Sz[j] + z * Sx[j] + y * Sw[j] - w * Sy[j];
        w  = x * Sw[j] + w * Sx[j] + z * Sy[j] - y * Sz[j];

        x = xx;
        y = yy;
        z = zz;

        Pow /= 2.0f;
      }

      Dist = Pow * sqrtf( Norm / (x * x + y * y + z * z + w * w) ) * logf( Norm );

      return false;
    }

    x *= 2.0f;

    Sy[i] = y = x * y + julia.juliaParm[__Y];
    Sz[i] = z = x * z + julia.juliaParm[__Z];
    Sw[i] = w = x * w + julia.juliaParm[__T];
    Sx[i] = x = x2 - d + julia.juliaParm[__X];

  }

  Dist = julia.precision;

  return true;
}

void Normal_Calc_z3( TVector3 &Result, const int N_Max, const PrimitiveJulia &julia ) {
  float
  n11 = 1.0f, n12 = 0.0f, n13 = 0.0f, n14 = 0.0f,
  n21 = 0.0f, n22 = 1.0f, n23 = 0.0f, n24 = 0.0f,
  n31 = 0.0f, n32 = 0.0f, n33 = 1.0f, n34 = 0.0f;

  float x, y, z, w;
  int i;
  float tmp, dtmp, dtmp2, x2, x3, x4;

  x = Sx[0];
  y = Sy[0];
  z = Sz[0];
  w = Sw[0];

  for ( i = 1; i <= N_Max; i++ ) {
    tmp = y * y + z * z + w * w;

    x2 = x * x;
    x3 = x2 - tmp;
    x4 = 3.0f * x2 - tmp;

    Deriv_z3(n11, n12, n13, n14);
    Deriv_z3(n21, n22, n23, n24);
    Deriv_z3(n31, n32, n33, n34);

    x = Sx[i];
    y = Sy[i];
    z = Sz[i];
    w = Sw[i];
  }

  Result[__X] = n11 * x + n12 * y + n13 * z + n14 * w;
  Result[__Y] = n21 * x + n22 * y + n23 * z + n24 * w;
  Result[__Z] = n31 * x + n32 * y + n33 * z + n34 * w;
}

void Normal_Calc_Julia( TVector3 &Result, const int N_Max, const PrimitiveJulia &julia ) {
  float
  n11 = 1.0f, n12 = 0.0f, n13 = 0.0f, n14 = 0.0f,
  n21 = 0.0f, n22 = 1.0f, n23 = 0.0f, n24 = 0.0f,
  n31 = 0.0f, n32 = 0.0f, n33 = 1.0f, n34 = 0.0f;
  float tmp;
  float x, y, z, w;
  int i;

  x = Sx[0];
  y = Sy[0];
  z = Sz[0];
  w = Sw[0];

  for ( i = 1; i <= N_Max; i++ ) {
    Deriv_z2(n11, n12, n13, n14);
    Deriv_z2(n21, n22, n23, n24);
    Deriv_z2(n31, n32, n33, n34);

    x = Sx[i];
    y = Sy[i];
    z = Sz[i];
    w = Sw[i];
  }

  Result[__X] = n11 * x + n12 * y + n13 * z + n14 * w;
  Result[__Y] = n21 * x + n22 * y + n23 * z + n24 * w;
  Result[__Z] = n31 * x + n32 * y + n33 * z + n34 * w;
}

bool F_Bound_Julia( const TRay &ray, float &Depth_Min, float &Depth_Max, const PrimitiveJulia &julia ) {
  TVector3 rayToCenter = julia.center - ray.origin;
  float B = ( ray.direction | rayToCenter );
  float C = rayToCenter.magnitudeSquared() - julia.radius2;
  float discriminant = B*B-C;

  if ( discriminant < EPS )
    return false;

  float sqrtDiscr = sqrtf(discriminant);
  Depth_Min = B-sqrtDiscr;
  Depth_Max = B+sqrtDiscr;
  return true;
}

//----------------
//-- HYPER COMPLEX
//----------------

#define HMult(xr,yr,zr,wr,x1,y1,z1,w1,x2,y2,z2,w2)        \
    (xr) = (x1) * (x2) - (y1) * (y2) - (z1) * (z2) + (w1) * (w2);   \
    (yr) = (y1) * (x2) + (x1) * (y2) - (w1) * (z2) - (z1) * (w2);   \
    (zr) = (z1) * (x2) - (w1) * (y2) + (x1) * (z2) - (y1) * (w2);   \
    (wr) = (w1) * (x2) + (z1) * (y2) + (y1) * (z2) + (x1) * (w2);

#define HSqr(xr,yr,zr,wr,x,y,z,w)         \
    (xr) = (x) * (x) - (y) * (y) - (z) * (z) + (w) * (w) ;  \
    (yr) = 2.0f * ( (x) * (y) - (z) * (w) );     \
    (zr) = 2.0f * ( (z) * (x) - (w) * (y) );       \
    (wr) = 2.0f * ( (w) * (x) + (z) * (y) );

int HReciprocal( float &xr, float &yr, float &zr, float &wr,
		 const float x, const float y, const float z, const float w ) {
  float det, mod, xt_minus_yz;
  
  det = ((x - w) * (x - w) + (y + z) * (y + z)) * ((x + w) * (x + w) + (y - z) * (y - z));
  
  if ( det == 0.0f )
    return -1;
  
  mod = (x * x + y * y + z * z + w * w);
  
  xt_minus_yz = x * w - y * z;
  
  float invDet = 1.0f / det;
  xr = (x * mod - 2.0f * w * xt_minus_yz) * invDet;
  yr = (-y * mod - 2.0f * z * xt_minus_yz) * invDet;
  zr = (-z * mod - 2.0f * y * xt_minus_yz) * invDet;
  wr = (w * mod - 2.0f * x * xt_minus_yz) * invDet;

  return 0;
}

void HFunc( float &xr, float &yr, float &zr, float &wr,
	    const float x, const float y, const float z, const float w,
	    const PrimitiveJulia &julia ) {
  TComplex a, b, ra, rb;
  
  // convert to duplex form
  a[__X] = x - w;
  a[__Y] = y + z;
  b[__X] = x + w;
  b[__Y] = y - z;
  
  if( julia.subType == PrimitiveJulia::SUB_TYPE_PWR )
     complexExponent = julia.exponent;
  
  // apply function to each part
  julia.complexFunctionMethod( ra, a );
  julia.complexFunctionMethod( rb, b );

  // convert back
  xr = 0.5f * (ra[__X] + rb[__X]);
  yr = 0.5f * (ra[__Y] + rb[__Y]);
  zr = 0.5f * (ra[__Y] - rb[__Y]);
  wr = 0.5f * (rb[__X] - ra[__X]);
}


// ***************************************************************
// *-------------------------- z2 -------------------------------*
// ***************************************************************
bool Iteration_HCompl( const TPoint3 &IPoint, const PrimitiveJulia &julia ) {
  int i;
  float yz, xw;
  float x, y, z, w;

  x = Sx[0] = IPoint[__X];
  y = Sy[0] = IPoint[__Y];
  z = Sz[0] = IPoint[__Z];
  w = Sw[0] = ( julia.sliceDist
                  - julia.slice[__X]*x
                  - julia.slice[__Y]*y
                  - julia.slice[__Z]*z ) / julia.slice[__T];
  
  for (i = 1; i <= julia.n; ++i) {
    yz = y * y + z * z;
    xw = x * x + w * w;

    if ((xw + yz) > julia.exitValue)
      return false;

    Sx[i] = xw - yz + julia.juliaParm[__X];
    Sy[i] = 2.0f * (x * y - z * w) + julia.juliaParm[__Y];
    Sz[i] = 2.0f * (x * z - w * y) + julia.juliaParm[__Z];
    Sw[i] = 2.0f * (x * w + y * z) + julia.juliaParm[__T];

    w = Sw[i];
    x = Sx[i];

    z = Sz[i];
    y = Sy[i];
  }

  return true;
}

bool D_Iteration_HCompl( const TPoint3 &IPoint, float &Dist, const PrimitiveJulia &julia ) {
  int i;
  float yz, xw;
  float F_Value, Step;
  float x, y, z, w;
  TVector3 H_Normal;

  x = Sx[0] = IPoint[__X];
  y = Sy[0] = IPoint[__Y];
  z = Sz[0] = IPoint[__Z];
  w = Sw[0] = ( julia.sliceDist
                  - julia.slice[__X]*x 
                  - julia.slice[__Y]*y 
                  - julia.slice[__Z]*z ) / julia.slice[__T]; 

  for (i = 1; i <= julia.n; ++i) {
    yz = y * y + z * z;
    xw = x * x + w * w;

    if ((F_Value = xw + yz) > julia.exitValue)
    {
      Normal_Calc_HCompl( H_Normal, i - 1, julia );

      Step = H_Normal | direction;

      if ( Step < -fractalTolerance ) {
        Step = -2.0f * Step;

        if ( (F_Value > julia.precision * Step)
	     && (F_Value < 30.0f * julia.precision * Step) ) {
          Dist = F_Value / Step;

          return false;
        }
      }

      Dist = julia.precision;

      return false;
    }

    Sx[i] = xw - yz + julia.juliaParm[__X];
    Sy[i] = 2.0f * (x * y - z * w) + julia.juliaParm[__Y];
    Sz[i] = 2.0f * (x * z - w * y) + julia.juliaParm[__Z];
    Sw[i] = 2.0f * (x * w + y * z) + julia.juliaParm[__T];

    w = Sw[i];
    x = Sx[i];

    z = Sz[i];
    y = Sy[i];
  }

  Dist = julia.precision;

  return true;
}

void Normal_Calc_HCompl( TVector3 &Result, const int N_Max, const PrimitiveJulia &julia ) {
  float n1, n2, n3, n4;
  int i;
  float x, y, z, w;
  float xx, yy, zz, ww;
  float Pow;

  // Algebraic properties of hypercomplexes allows simplifications in
  // computations...

  x = Sx[0];
  y = Sy[0];
  z = Sz[0];
  w = Sw[0];

  Pow = 2.0f;

  for (i = 1; i < N_Max; ++i) {
    // For a map z->f(z), f depending on c, one must perform here :
    // (x,y,z,w) * df/dz(Sx[i],Sy[i],Sz[i],Sw[i]) -> (x,y,z,w) ,
    // up to a constant.
    
    // ****************** Case z->z^2+c *****************
    // the df/dz part needs no work

    HMult( xx, yy, zz, ww, Sx[i], Sy[i], Sz[i], Sw[i], x, y, z, w );

    w = ww;
    z = zz;
    y = yy;
    x = xx;

    Pow *= 2.0f;
  }

  n1 = Sx[N_Max] * Pow;
  n2 = Sy[N_Max] * Pow;
  n3 = Sz[N_Max] * Pow;
  n4 = Sw[N_Max] * Pow;

  Result[__X] = x * n1 + y * n2 + z * n3 + w * n4;
  Result[__Y] = -y * n1 + x * n2 - w * n3 + z * n4;
  Result[__Z] = -z * n1 - w * n2 + x * n3 + y * n4;
}

bool F_Bound_HCompl( const TRay &ray, float &Depth_Min, float &Depth_Max, const PrimitiveJulia &julia ) {
  return F_Bound_Julia( ray, Depth_Min, Depth_Max, julia );
}

// ***************************************************************
// *-------------------------- z3 -------------------------------*
// ***************************************************************

bool Iteration_HCompl_z3( const TPoint3 &IPoint, const PrimitiveJulia &julia ) {
  int i;
  float Norm, xx, yy, zz, ww;
  float x, y, z, w;

  x = Sx[0] = IPoint[__X];
  y = Sy[0] = IPoint[__Y];
  z = Sz[0] = IPoint[__Z];
  w = Sw[0] = ( julia.sliceDist
                  - julia.slice[__X]*x 
                  - julia.slice[__Y]*y 
                  - julia.slice[__Z]*z ) / julia.slice[__T]; 

  for (i = 1; i <= julia.n; ++i) {
    Norm = x * x + y * y + z * z + w * w;

    // is this test correct ?
    if ( Norm > julia.exitValue )
      return false;

    // ************** Case: z->z^2+c *********************
    HSqr(xx, yy, zz, ww, x, y, z, w);

    x = Sx[i] = xx + julia.juliaParm[__X];
    y = Sy[i] = yy + julia.juliaParm[__Y];
    z = Sz[i] = zz + julia.juliaParm[__Z];
    w = Sw[i] = ww + julia.juliaParm[__T];

  }
  return true;
}

bool D_Iteration_HCompl_z3( const TPoint3 &IPoint, float &Dist, const PrimitiveJulia &julia ) {
  int i;
  float xx, yy, zz, ww;
  float F_Value, Step;
  float x, y, z, w;
  TVector3 H_Normal;

  x = Sx[0] = IPoint[__X];
  y = Sy[0] = IPoint[__Y];
  z = Sz[0] = IPoint[__Z];
  w = Sw[0] = ( julia.sliceDist
		- julia.slice[__X]*x
		- julia.slice[__Y]*y
		- julia.slice[__Z]*z ) / julia.slice[__T];

  for (i = 1; i <= julia.n; ++i) {
    F_Value = x * x + y * y + z * z + w * w;

    if ( F_Value > julia.exitValue ) {
      Normal_Calc_HCompl_z3( H_Normal, i - 1, julia );

      Step = H_Normal | direction;

      if ( Step < -fractalTolerance ) {
        Step = -2.0f * Step;

        if ( (F_Value > julia.precision * Step )
	     && (F_Value < 30.0f * julia.precision * Step) ) {
          Dist = F_Value / Step;

          return false;
        }
      }

      Dist = julia.precision;

      return false;
    }

    // ************** Case: z->z^2+c *********************

    HSqr(xx, yy, zz, ww, x, y, z, w);

    x = Sx[i] = xx + julia.juliaParm[__X];
    y = Sy[i] = yy + julia.juliaParm[__Y];
    z = Sz[i] = zz + julia.juliaParm[__Z];
    w = Sw[i] = ww + julia.juliaParm[__T];
  }

  Dist = julia.precision;

  return true;
}

void Normal_Calc_HCompl_z3( TVector3 &Result, const int N_Max, const PrimitiveJulia &julia ) {
  float n1, n2, n3, n4;
  int i;
  float x, y, z, w;
  float xx, yy, zz, ww;

  // Algebraic properties of hypercomplexes allows simplifications in
  // computations...

  x = Sx[0];
  y = Sy[0];
  z = Sz[0];
  w = Sw[0];

  for ( i = 1; i < N_Max; ++i ) {
    // For a map z->f(z), f depending on c, one must perform here :
    // (x,y,z,w) * df/dz(Sx[i],Sy[i],Sz[i],Sw[i]) -> (x,y,z,w) ,
    // up to a constant.

    // ****************** Case z->z^2+c *****************
    // the df/dz part needs no work

    HMult(xx, yy, zz, ww, Sx[i], Sy[i], Sz[i], Sw[i], x, y, z, w);

    x = xx;
    y = yy;
    z = zz;
    w = ww;
  }

  n1 = Sx[N_Max];
  n2 = Sy[N_Max];
  n3 = Sz[N_Max];
  n4 = Sw[N_Max];

  Result[__X] = x * n1 + y * n2 + z * n3 + w * n4;
  Result[__Y] = -y * n1 + x * n2 - w * n3 + z * n4;
  Result[__Z] = -z * n1 - w * n2 + x * n3 + y * n4;
}

bool F_Bound_HCompl_z3( const TRay &Ray, float &Depth_Min, float &Depth_Max, const PrimitiveJulia &julia ) {
  return F_Bound_HCompl( Ray, Depth_Min, Depth_Max, julia );
}

// ***************************************************************
// *-------------------------- Inv ------------------------------*
// ***************************************************************

bool Iteration_HCompl_Reciprocal( const TPoint3 &IPoint, const PrimitiveJulia &julia ) {
  int i;
  float Norm, xx, yy, zz, ww;
  float x, y, z, w;

  x = Sx[0] = IPoint[__X];
  y = Sy[0] = IPoint[__Y];
  z = Sz[0] = IPoint[__Z];
  w = Sw[0] = ( julia.sliceDist
                  - julia.slice[__X]*x 
                  - julia.slice[__Y]*y 
                  - julia.slice[__Z]*z ) / julia.slice[__T]; 

  for ( i = 1; i <= julia.n; ++i ) {
    Norm = x * x + y * y + z * z + w * w;

    if ( Norm > julia.exitValue )
      return false;

    HReciprocal( xx, yy, zz, ww, x, y, z, w );

    x = Sx[i] = xx + julia.juliaParm[__X];
    y = Sy[i] = yy + julia.juliaParm[__Y];
    z = Sz[i] = zz + julia.juliaParm[__Z];
    w = Sw[i] = ww + julia.juliaParm[__T];

  }
  return true;
}

bool D_Iteration_HCompl_Reciprocal( const TPoint3 &IPoint, float &Dist, const PrimitiveJulia &julia ) {
  int i;
  float xx, yy, zz, ww;
  float F_Value, Step;
  float x, y, z, w;
  TVector3 H_Normal;

  x = Sx[0] = IPoint[__X];
  y = Sy[0] = IPoint[__Y];
  z = Sz[0] = IPoint[__Z];
  w = Sw[0] = ( julia.sliceDist
                  - julia.slice[__X]*x 
                  - julia.slice[__Y]*y 
                  - julia.slice[__Z]*z ) / julia.slice[__T]; 

  for ( i = 1; i <= julia.n; ++i ) {
    F_Value = x * x + y * y + z * z + w * w;

    if ( F_Value > julia.exitValue ) {
      Normal_Calc_HCompl_Reciprocal( H_Normal, i - 1, julia );

      Step = H_Normal | direction;

      if ( Step < -fractalTolerance) {
        Step = -2.0f * Step;

        if (( F_Value > julia.precision * Step)
	    && F_Value < (30.0f * julia.precision * Step) ) {
          Dist = F_Value / Step;

	  return false;
        }
      }
      
      Dist = julia.precision;

      return false;
    }

    HReciprocal( xx, yy, zz, ww, x, y, z, w );

    x = Sx[i] = xx + julia.juliaParm[__X];
    y = Sy[i] = yy + julia.juliaParm[__Y];
    z = Sz[i] = zz + julia.juliaParm[__Z];
    w = Sw[i] = ww + julia.juliaParm[__T];

  }

  Dist = julia.precision;

  return true;
}

void Normal_Calc_HCompl_Reciprocal( TVector3 &Result, const int N_Max, const PrimitiveJulia &julia ) {
  float n1, n2, n3, n4;
  int i;
  float x, y, z, w;
  float xx, yy, zz, ww;
  float xxx, yyy, zzz, www;

  // Algebraic properties of hypercomplexes allows simplifications in
  // computations...

  x = Sx[0];
  y = Sy[0];
  z = Sz[0];
  w = Sw[0];

  for ( i = 1; i < N_Max; ++i ) {
    // ****************** Case: z->1/z+c *****************
    HReciprocal( xx, yy, zz, ww, Sx[i], Sy[i], Sz[i], Sw[i] );
    HSqr( xxx, yyy, zzz, www, xx, yy, zz, ww );
    HMult( xx, yy, zz, ww, x, y, z, w, -xxx, -yyy, -zzz, -www );

    x = xx;
    y = yy;
    z = zz;
    w = ww;
  }

  n1 = Sx[N_Max];
  n2 = Sy[N_Max];
  n3 = Sz[N_Max];
  n4 = Sw[N_Max];

  Result[__X] = x * n1 + y * n2 + z * n3 + w * n4;
  Result[__Y] = -y * n1 + x * n2 - w * n3 + z * n4;
  Result[__Z] = -z * n1 - w * n2 + x * n3 + y * n4;
}

bool F_Bound_HCompl_Reciprocal( const TRay &Ray, float &Depth_Min, float &Depth_Max, const PrimitiveJulia &julia ) {
  return F_Bound_HCompl( Ray, Depth_Min, Depth_Max, julia );
}

// ***************************************************************
// *------------------------- Func ------------------------------*
// ***************************************************************

bool Iteration_HCompl_Func( const TPoint3 &IPoint, const PrimitiveJulia &julia ) {
  int i;
  float Norm, xx, yy, zz, ww;
  float x, y, z, w;

  x = Sx[0] = IPoint[__X];
  y = Sy[0] = IPoint[__Y];
  z = Sz[0] = IPoint[__Y];
  w = Sw[0] = ( julia.sliceDist
                  - julia.slice[__X]*x 
                  - julia.slice[__Y]*y 
                  - julia.slice[__Z]*z ) / julia.slice[__T]; 

  for ( i = 1; i <= julia.n; ++i ) {
    Norm = x * x + y * y + z * z + w * w;

    if ( Norm > julia.exitValue )
      return false;

    HFunc( xx, yy, zz, ww, x, y, z, w, julia );

    x = Sx[i] = xx + julia.juliaParm[__X];
    y = Sy[i] = yy + julia.juliaParm[__Y];
    z = Sz[i] = zz + julia.juliaParm[__Z];
    w = Sw[i] = ww + julia.juliaParm[__T];
  }
  return true;
}

bool D_Iteration_HCompl_Func( const TPoint3 &IPoint, float &Dist, const PrimitiveJulia &julia ) {
  int i;
  float xx, yy, zz, ww;
  float F_Value, Step;
  float x, y, z, w;
  TVector3 H_Normal;

  x = Sx[0] = IPoint[__X];
  y = Sy[0] = IPoint[__Y];
  z = Sz[0] = IPoint[__Z];
  w = Sw[0] = ( julia.sliceDist
		- julia.slice[__X]*x 
		- julia.slice[__Y]*y 
		- julia.slice[__Z]*z ) / julia.slice[__T]; 

  for ( i = 1; i <= julia.n; ++i ) {
    F_Value = x * x + y * y + z * z + w * w;

    if ( F_Value > julia.exitValue ) {
      Normal_Calc_HCompl_Func( H_Normal, i - 1, julia );

      Step = H_Normal | direction;

      if ( Step < -fractalTolerance ) {
        Step = -2.0f * Step;

        if ( (F_Value > julia.precision * Step)
	     && F_Value < (30.0f * julia.precision * Step) ) {
          Dist = F_Value / Step;
	  
	  return false;
        }
      }

      Dist = julia.precision;

      return false;
    }

    HFunc( xx, yy, zz, ww, x, y, z, w, julia );

    x = Sx[i] = xx + julia.juliaParm[__X];
    y = Sy[i] = yy + julia.juliaParm[__Y];
    z = Sz[i] = zz + julia.juliaParm[__Z];
    w = Sw[i] = ww + julia.juliaParm[__T];

  }

  Dist = julia.precision;

  return true;
}

void Normal_Calc_HCompl_Func( TVector3 &Result, const int N_Max, const PrimitiveJulia &julia ) {
  float n1, n2, n3, n4;
  int i;
  float x, y, z, w;
  float xx, yy, zz, ww;
  float xxx, yyy, zzz, www;

  // Algebraic properties of hypercomplexes allows simplifications in
  // computations...

  x = Sx[0];
  y = Sy[0];
  z = Sz[0];
  w = Sw[0];

  for ( i = 1; i < N_Max; ++i ) {
    // *************** Case: z-> f(z)+c ************************

    HFunc( xx, yy, zz, ww, Sx[i], Sy[i], Sz[i], Sw[i], julia );
    HMult( xxx, yyy, zzz, www, xx, yy, zz, ww, x, y, z, w );

    x = xxx;
    y = yyy;
    z = zzz;
    w = www;
  }

  n1 = Sx[N_Max];
  n2 = Sy[N_Max];
  n3 = Sz[N_Max];
  n4 = Sw[N_Max];

  Result[__X] = x * n1 + y * n2 + z * n3 + w * n4;
  Result[__Y] = -y * n1 + x * n2 - w * n3 + z * n4;
  Result[__Z] = -z * n1 - w * n2 + x * n3 + y * n4;
}

bool F_Bound_HCompl_Func( const TRay &Ray, float &Depth_Min, float &Depth_Max, const PrimitiveJulia &julia ) {
  return F_Bound_HCompl( Ray, Depth_Min, Depth_Max, julia );
}
