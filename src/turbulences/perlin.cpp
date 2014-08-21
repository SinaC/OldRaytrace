// Original 'perlin noise' code by SinaC, following Mr Perlin's implementation
#include <stdlib.h>

#include "perlin.h"

#define M 0x100      // 256
#define BM 0xff      // 255

#define MINX (-10000.0f)
#define MINY MINX
#define MINZ MINX

static int p[ M + M + 2 ];
static float g3[ M + M + 2 ][ 3 ];

#define s_curve(t) ( t * t * (3.0f - 2.0f * t) )

#define lerp( t, a, b ) ( a + t * ( b - a ) )

#define setup(v,b0,b1,r0,r1)\
        b0 = ((int)v) & BM;\
        b1 = (b0+1) & BM;\
        r0 = v - (int)v;\
        r1 = r0 - 1.0f;

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

static float noise3( float x, float y, float z ) {
  int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
  float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, u, v, t;
  register int i, j;
  
  setup( x, bx0, bx1, rx0, rx1 );
  setup( y, by0, by1, ry0, ry1 );
  setup( z, bz0, bz1, rz0, rz1 );
  
  i = p[ bx0 ];
  j = p[ bx1 ];
  
  b00 = p[ i + by0 ];
  b10 = p[ j + by0 ];
  b01 = p[ i + by1 ];
  b11 = p[ j + by1 ];
  
  t  = s_curve(rx0);
  sy = s_curve(ry0);
  sz = s_curve(rz0);
  
  q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
  q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
  a = lerp(t, u, v);
  
  q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
  q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
  b = lerp(t, u, v);
  
  c = lerp(sy, a, b);
  
  q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
  q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
  a = lerp(t, u, v);
  
  q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
  q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
  b = lerp(t, u, v);
  
  d = lerp(sy, a, b);
  
  return lerp(sz, c, d);
}

static void normalize3( float v[3] ) {
  float s;
  
  //s = 1/(float)fsqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  s = fastInvSqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] = v[0] * s;
  v[1] = v[1] * s;
  v[2] = v[2] * s;
}

void perlinInit() {
  int i, j, k;

  for ( i = 0 ; i < M ; i++) {
    p[i] = i;
    for ( j = 0 ; j < 3 ; j++ )
      g3[ i ][ j ] = (float)( ( rand() % ( M + M ) ) - M ) / M;
    normalize3( g3[i] );
  }
  
  while ( --i ) {
    k = p[ i ];
    p[ i ] = p[ j = rand() % M ];
    p[ j ] = k;
  }
  
  for ( i = 0 ; i < M + 2 ; i++ ) {
    p[ M + i ] = p[ i ];
    for ( j = 0 ; j < 3 ; j++ )
      g3[M + i][j] = g3[i][j];
  }
}

TurbulencePerlin::TurbulencePerlin() {
  TTurbulence::TTurbulence(); turbulenceKind = TURBULENCE_NOISE;
}

float TurbulencePerlin::getTurbulence1( const TPoint3 &p ) const {
  float mulcol = 1.0f,
    noise = 0.0f,
    step = 1.0f/lambda;
  
  float x = p[__X]-MINX;
  float y = p[__Y]-MINY;
  float z = p[__Z]-MINZ;
  
  for ( int k = 0; k < octaves; k++ ) {
    noise += ( noise3( x*step, y*step, z*step ) * mulcol );
    step *= 2.0f;
    mulcol *= omega;
  }
  return fabsf(noise);
}

TVector3 TurbulencePerlin::getTurbulence3( const TPoint3 &p ) const {
  float x = getTurbulence1( p );
  float y = getTurbulence1( TVector3( p[__Y], p[__Z], p[__X] ) );
  float z = getTurbulence1( TVector3( p[__Z], p[__X], p[__Y] ) );
  return TVector3( x, y, z );
}

void TurbulencePerlin::print( const char *msg ) const {
  TTurbulence::print( msg );
}

const TTagList TurbulencePerlin::turbulencePerlinTags [] = {
  { NULL, -1 }
};
