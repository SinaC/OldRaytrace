#include "math.h"
#include "solver.h"

// A coefficient smaller than SMALL_ENOUGH is considered to be zero (0.0).
#define SMALL_ENOUGH 1.0e-10

#define TWO_M_PI_3  2.0943951023931954923084
#define FOUR_M_PI_3 4.1887902047863909846168

int solveQuadric(double *x, double *results) {
  // x[0] * y^2 + x[1] * y + x[2]
  //  a            b          c
  // discr = b^2 - 4*a*c
  double discr = x[1]*x[1] - 4.0 * x[0] * x[2];
  if ( discr < 0.0 )
    return 0;
  int index = 0;
  results[index++] = (-x[1] - sqrt(discr)) / (2.0 * x[0]);
  results[index++] = (-x[1] + sqrt(discr)) / (2.0 * x[0]);
  return index;
}

int solveCubic(double *x, double *y) {
  double Q, R, Q3, R2, sQ, d, an, theta;
  double A2, a0, a1, a2, a3;

  a0 = x[0];
  if(a0==0.0){
	return(0);
  } 
  else {
    if(a0 != 1.0){
      a1 = x[1] / a0;
      a2 = x[2] / a0;
      a3 = x[3] / a0;
    } 
    else {
      a1 = x[1];
      a2 = x[2];
      a3 = x[3];
    }
  }

  A2 =  a1 *  a1;   Q = (A2  -  3.0 * a2) /  9.0;
  R  = (a1 * (A2 - 4.5 * a2) + 13.5 * a3) / 27.0;
  Q3 = Q  * Q * Q;
  R2 = R  * R;
  d  = Q3 - R2;
  an = a1 / 3.0;

  if(d>=0.0){
    // Three real roots.
    d = R / sqrt(Q3); theta = acos(d) / 3.0; sQ = -2.0 * sqrt(Q);
    y[0] = sQ * cos(theta              ) - an;
    y[1] = sQ * cos(theta +  TWO_M_PI_3) - an;
    y[2] = sQ * cos(theta + FOUR_M_PI_3) - an;
    return(3);
  }
  else {
    sQ = pow(sqrt(R2 - Q3) + fabs(R), 1.0 / 3.0);
    if(R < 0){
      y[0] =  (sQ + Q / sQ) - an;
    } else {
      y[0] = -(sQ + Q / sQ) - an;
    }
    return(1);
  }
}

int solveQuartic(double *x, double *results) {
  double cubic[4], roots[3];
  double c12, z, p, q, q1, q2, r, d1, d2;
  double c0, c1, c2, c3, c4;
  
  // Make sure the quartic has a leading coefficient of 1.0
  c0 = x[0];
  if(c0 != 1.0){
    c1 = x[1] / c0;
    c2 = x[2] / c0;
    c3 = x[3] / c0;
    c4 = x[4] / c0;
  }
  else {
    c1 = x[1];
    c2 = x[2];
    c3 = x[3];
    c4 = x[4];
  }
  
  // Compute the cubic resolvant
  c12 =               c1  * c1;
  p   = -0.37500000 * c12 + c2;
  q   =  0.12500000 * c12 * c1  - 0.5000 * c1  * c2 + c3;
  r   = -0.01171875 * c12 * c12 + 0.0625 * c12 * c2 - 0.25 * c1 * c3 + c4;
  
  cubic[0] =  1.0;
  cubic[1] = -0.5 * p;
  cubic[2] = -r;
  cubic[3] =  0.5 * r * p - 0.125 * q * q;
  
  int i = solveCubic(cubic, roots);
  
  if(i>0) z = roots[0]; else return 0;
  
  d1 = 2.0 * z - p; if(d1 < 0.0) if(d1 > -SMALL_ENOUGH) d1 = 0.0; else return 0;
  
  if(d1 < SMALL_ENOUGH){
    d2 = z * z - r;
    if(d2 < 0.0) return 0;
    d2 = sqrt(d2);
  }
  else {
    d1 = sqrt(d1);
    d2 = 0.5 * q / d1;
  }
  
  // Set up useful values for the quadratic factors
  q1 =    d1 * d1;
  q2 = -0.25 * c1; i = 0;
  
  // Solve the first quadratic
  p = q1 - 4.0 * (z - d2);
  if(p==0.0){
    results[i++] = -0.5 * d1 - q2;
  } 
  else {
    if(p > 0){
      p = sqrt(p);
      results[i++] = -0.5 * (d1 + p) + q2;
      results[i++] = -0.5 * (d1 - p) + q2;
    }
  }

  // Solve the second quadratic
  p = q1 - 4.0 * (z + d2);
  if(p==0.0){
    results[i++] = 0.5 * d1 - q2;
  } 
  else {
    if(p > 0){
      p = sqrt(p);
      results[i++] = 0.5 * (d1 + p) + q2;
      results[i++] = 0.5 * (d1 - p) + q2;
    }
  }

  return i;
}
