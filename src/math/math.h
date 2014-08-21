#ifndef __MATH_H__
#define __MATH_H__

#include <math.h>

#undef __X
#define __X (0)
#undef __Y
#define __Y (1)
#undef __Z
#define __Z (2)
#undef __W
#define __W (3)
#undef __T
#define __T (3)

#undef  EPS
#define EPS           (1e-4f)
#undef  INFINITE
#define INFINITE      (1e10f)

#undef  M_PI
#define M_PI            (3.14159265358979323846264338327950288419716939937510f)
#undef  M_ONE_OVER_PI
#define M_ONE_OVER_PI   (0.31830988618379067153776752674503f)
#undef  M_2_PI
#define M_2_PI          (2.0f*M_PI)
#undef  M_HALF_PI
#define M_HALF_PI       (M_PI*0.5f)
#undef  M_ONE_OVER_2_PI
#define M_ONE_OVER_2_PI (M_ONE_OVER_PI*0.5f)
#undef M_PI_180
#define M_PI_180        (0.01745329251994329576f)

#define DEGS_PER_RAD  (57.29577951308232286465f)
#define RADS_PER_DEG  (0.01745329251994329547f)
#define degs(x)       (x*DEGS_PER_RAD)
#define rads(x)       (x*RADS_PER_DEG)

template <typename T> inline void swap( T &x, T &y ) {
  T tmp = x;
  x = y;
  y = tmp;
}

#ifndef SQR
#define SQR(a) ((a)*(a))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MINi
#define MINi(a,b) ((a)<(b)?(0):(1))
#endif
#ifndef MAXi
#define MAXi(a,b) ((a)>(b)?(0):(1))
#endif

#ifndef MAX3
#define MAX3(x,y,z) (((x)>(y))?(((x)>(z))?(x):(z)):(((y)>(z))?(y):(z)))
#endif
#ifndef MIN3
#define MIN3(x,y,z) (((x)<(y))?(((x)<(z))?(x):(z)):(((y)<(z))?(y):(z)))
#endif

#ifndef MAX3i
#define MAX3i(x,y,z) (((x)>(y))?(((x)>(z))?(0):(2)):(((y)>(z))?(1):(2)))
#endif
#ifndef MIN3i
#define MIN3i(x,y,z) (((x)<(y))?(((x)<(z))?(0):(2)):(((y)<(z))?(1):(2)))
#endif

#ifndef RANGE
#define RANGE(m,x,M) ((x)<(m)?(m):((x)>(M))?(M):(x))
#endif

inline float fastInvSqrt( float x ) {
  //--  static int LOOP_PRECISION = 2;
  float xhalf = 0.5f * x;
  int i = *(int *)&x;
  i = 0x5f3759df - ( i >> 1 );
  x = *(float *)&i;
  x = x * ( 1.5f - xhalf*x*x ); // repeat for more precision
  x = x * ( 1.5f - xhalf*x*x ); // repeat for more precision
  return x;
}

//3*x^2-x^3
inline float cubicSpline( const float theta ) {
  return 3.0f*theta*theta - 2.0f*theta*theta*theta;
}

//same as cubicSpline but use x^2
//1/9*(22*x^2-17*x^4+4*t^6)
inline float cubicSpline2( const float t2 ) {
  float t4 = t2*t2;
  //**float s = 1.0f/9.0f*(22.0f*t2-17.0f*t4+4*t6);
  return (2.444444444f*t2-1.888888889f*t4+0.444444444f*(t4*t2));
}

inline void swap(               int & a,                int & b){                int t=a; a=b; b=t; }
inline void swap(         long  int & a,          long  int & b){          long  int t=a; a=b; b=t; }
inline void swap(unsigned long  int & a, unsigned long  int & b){ unsigned long  int t=a; a=b; b=t; }
inline void swap(         short int & a,          short int & b){          short int t=a; a=b; b=t; }
inline void swap(float              & a, float              & b){ float              t=a; a=b; b=t; }
inline void swap(unsigned short     & a, unsigned short     & b){ unsigned short     t=a; a=b; b=t; }
inline void swap(double             & a, double             & b){ double             t=a; a=b; b=t; }

#define lt(a, eps) ((a) < -eps)
#define le(a, eps) ((a) <= eps)
#define ge(a, eps) (!((a) < -eps))
#define eq(a, eps) (((a) <= eps) && !((a) < -eps))
#define gt(a, eps) (!((a) <= eps))

#define sqr(a) (a*a)

#endif
