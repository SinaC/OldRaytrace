#include <stdio.h>

#include "vector2.h"

const TVector2 TVector2::zeroVector( 0, 0 );
const TVector2 TVector2::oneVector( 1, 1 );
const TVector2 TVector2::twoVector( 2, 2 );

void TVector2::print( const char *msg ) const {
  printf("%s ( %5.5f, %5.5f )\n", msg, v[0], v[1] ); fflush(stdout);
}

TComplex complexExponent = TComplex( 0.0f, 0.0f );

void TComplex::mult( TComplex &target, const TComplex &source1, const TComplex &source2 ) {
  float tmpx;
  tmpx = source1[__X] * source2[__X] - source1[__Y] * source2[__Y];
  target[__Y] = source1[__X] * source2[__Y] + source1[__Y] * source2[__X];
  target[__X] = tmpx;
}

void TComplex::div( TComplex &target, const TComplex &source1, const TComplex &source2 ) {
  float mod,tmpx,yxmod,yymod;
  mod = SQR(source2[__X]) + SQR(source2[__Y]);
  if (mod==0)
     return;
  yxmod = source2[__X]/mod;
  yymod = - source2[__Y]/mod;
  tmpx = source1[__X] * yxmod - source1[__Y] * yymod;
  target[__Y] = source1[__X] * yymod + source1[__Y] * yxmod;
  target[__X] = tmpx;
}

void TComplex::exp( TComplex &target, const TComplex &source ) {
  float expx;
  expx = expf(source[__X]);
  target[__X] = expx * cosf(source[__Y]);
  target[__Y] = expx * sinf(source[__Y]);
}

void TComplex::sin( TComplex &target, const TComplex &source ) {
  target[__X] = sinf(source[__X]) * coshf(source[__Y]);
  target[__Y] = cosf(source[__X]) * sinhf(source[__Y]);
}

void TComplex::sinh( TComplex &target, const TComplex &source ) {
  target[__X] = sinhf(source[__X]) * cosf(source[__Y]);
  target[__Y] = coshf(source[__X]) * sinf(source[__Y]);
}

void TComplex::cos( TComplex &target, const TComplex &source ) {
  target[__X] = cosf(source[__X]) * coshf(source[__Y]);
  target[__Y] = -sinf(source[__X]) * sinhf(source[__Y]);
}

void TComplex::cosh( TComplex &target, const TComplex &source ) {
  target[__X] = coshf(source[__X]) * cosf(source[__Y]);
  target[__Y] = sinhf(source[__X]) * sinf(source[__Y]);
}

void TComplex::log( TComplex &target, const TComplex &source ) {
  float mod,zx,zy;
  mod = sqrtf(source[__X] * source[__X] + source[__Y] * source[__Y]);
  zx = logf(mod);
  zy = atan2f(source[__Y],source[__X]);

  target[__X] = zx;
  target[__Y] = zy;
}

void TComplex::sqrt( TComplex &target, const TComplex &source ) {
  float mag;
  float theta;

  if ( source[__X] == 0.0 && source[__Y] == 0.0 )
    target[__X] = target[__Y] = 0.0;
  else {   
    mag   = sqrtf(sqrtf(SQR(source[__X]) + SQR(source[__Y])));
    theta = atan2f(source[__Y], source[__X]) / 2;
    target[__Y] = mag * sinf(theta);
    target[__X] = mag * cosf(theta);
  }
}

// rz=Arcsin(z)=-i*Log{i*z+sqrt(1-z*z)}
void TComplex::asin( TComplex &target, const TComplex &source ) {
  TComplex tempz1,tempz2;

  TComplex::mult( tempz1, source, source );
  tempz1[__X] = 1 - tempz1[__X]; tempz1[__Y] = -tempz1[__Y]; 
  TComplex::sqrt( tempz1, tempz1 );

  tempz2[__X] = -source[__Y]; tempz2[__Y] = source[__X];
  tempz1[__X] += tempz2[__X];  tempz1[__Y] += tempz2[__Y];

  TComplex::log( tempz1, tempz1 );
  target[__X] = tempz1[__Y];  target[__Y] = -tempz1[__X];   
}

void TComplex::acos( TComplex &target, const TComplex &source ) {
  TComplex temp;

  TComplex::mult( temp, source, source );
  temp[__X] -= 1;
  TComplex::sqrt( temp, temp );

  temp[__X] += source[__X]; temp[__Y] += source[__Y];
  
  TComplex::log( temp, temp );
  target[__X] = temp[__Y];  target[__Y] = -temp[__X]; 
}

void TComplex::asinh( TComplex &target, const TComplex &source ) {
  TComplex temp;

  TComplex::mult( temp, source, source );
  temp[__X] += 1;   
  TComplex::sqrt( temp, temp );
  temp[__X] += source[__X]; temp[__Y] += source[__Y]; 
  TComplex::log( target, temp );
}

/* rz=Arccosh(z)=Log(z+sqrt(z*z-1)} */
void TComplex::acosh (TComplex &target, const TComplex &source ) {
  TComplex tempz;
  TComplex::mult( tempz, source, source );
  tempz[__X] -= 1; 
  TComplex::sqrt( tempz, tempz );
  tempz[__X] = source[__X] + tempz[__X]; tempz[__Y] = source[__Y] + tempz[__Y];
  TComplex::log( target, tempz );
}

/* rz=Arctanh(z)=1/2*Log{(1+z)/(1-z)} */
void TComplex::atanh( TComplex &target, const TComplex &source ) {
  TComplex temp0,temp1,temp2;
   
  if ( source[__X] == 0.0f ) {
    target[__X] = 0.0f;
    target[__Y] = atanf( source[__Y] );
    return;
  }
  else {
    if ( fabsf(source[__X]) == 1.0f && source[__Y] == 0.0f )
      return;
    else if ( fabsf( source[__X]) < 1.0f && source[__Y] == 0.0f ) {
      target[__X] = logf((1.0f+source[__X])/(1.0f-source[__X]))/2.0f;
      target[__Y] = 0.0f;
      return;
    } 
    else {
      temp0[__X] = 1 + source[__X]; temp0[__Y] = source[__Y];
      temp1[__X] = 1 - source[__X]; temp1[__Y] = -source[__Y]; 
      TComplex::div( temp2, temp0, temp1 );
      TComplex::log( temp2, temp2 );
      target[__X] = 0.5f * temp2[__X]; target[__Y] = 0.5f * temp2[__Y];
      return;
    }
  }
}

/* rz=Arctan(z)=i/2*Log{(1-i*z)/(1+i*z)} */
void TComplex::atan( TComplex &target, const TComplex &source ) {
  TComplex temp0,temp1,temp2,temp3;
  if ( source[__X] == 0.0f && source[__Y] == 0.0f )
    target[__X] = target[__Y] = 0.0f;
  else if ( source[__X] != 0.0f && source[__Y] == 0.0f ) {
    target[__X] = atanf( source[__X] );
    target[__Y] = 0.0f;
  }
  else if ( source[__X] == 0.0f && source[__Y] != 0.0f ) {
    temp0[__X] = source[__Y];  temp0[__Y] = 0.0f;
    TComplex::atanh( temp0, temp0 );
    target[__X] = -temp0[__Y]; target[__Y] = temp0[__X];
  } 
  else if ( source[__X] != 0.0 && source[__Y] != 0.0f ) {
    temp0[__X] = -source[__Y]; temp0[__Y] = source[__X]; 
    temp1[__X] = 1.0f - temp0[__X]; temp1[__Y] = -temp0[__Y];   
    temp2[__X] = 1.0f + temp0[__X]; temp2[__Y] = temp0[__Y]; 

    TComplex::div( temp3, temp1, temp2 );
    TComplex::log( temp3, temp3 );
    target[__X] = -temp3[__Y] * 0.5f; target[__Y] = 0.5f * temp3[__X]; 
  }
}

void TComplex::tan( TComplex &target, const TComplex &source ) {
  float x, y, sinx, cosx, sinhy, coshy, denom;
  x = 2.0f * source[__X];
  y = 2.0f * source[__Y];
  sinx = sinf(x); cosx = cosf(x);
  sinhy = sinhf(y); coshy = coshf(y);
  denom = cosx + coshy;
  if (denom == 0.0f)
    return;
  target[__X] = sinx/denom;
  target[__Y] = sinhy/denom;
}

void TComplex::tanh( TComplex &target, const TComplex &source ) {
  float x, y, siny, cosy, sinhx, coshx, denom;
  x = 2.0f * source[__X];
  y = 2.0f * source[__Y];
  siny = sinf(y); cosy = cosf(y);
  sinhx = sinhf(x); coshx = coshf(x);
  denom = coshx + cosy;
  if (denom == 0)
    return;
  target[__X] = sinhx/denom;
  target[__Y] = siny/denom;
}

void TComplex::power( TComplex &target, const TComplex &source1, const TComplex &source2 ) {
  TComplex cLog, t;
  float e2x;

  if (source1[__X] == 0 && source1[__Y] == 0) {
    target[__X] = target[__Y] = 0.0f;
    return;
  }

  TComplex::log( cLog, source1 );
  TComplex::mult( t, cLog, source2 );

  if ( t[__X] < -690.0f )
    e2x = 0.0f;
  else
    e2x = expf(t[__X]);
  target[__X] = e2x * cosf(t[__Y]);
  target[__Y] = e2x * sinf(t[__Y]);
}

void TComplex::pwr( TComplex &target, const TComplex &source ) {
  TComplex::power( target, source, complexExponent );
}
