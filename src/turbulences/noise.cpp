#include <stdio.h>
#include <stdlib.h>

#include "noise.h"
#include "../random.h"

#define s_curve(t) ( (t) * (t) * (3.0f - 2.0f * (t)) )

#define INCRSUMP(mp,s,x,y,z) \
  ((s)*((mp[0])*0.5f + (mp[1])*(x) + (mp[2])*(y) + (mp[3])*(z)))

#define MINX (-10000.0f)
#define MINY MINX
#define MINZ MINX

short hashTable[4096];

float RTable[267] = {
       -1.0f,    0.604974f,   -0.937102f,    0.414115f,    0.576226f,  -0.0161593f,
   0.432334f,    0.103685f,    0.590539f,   0.0286412f,     0.46981f,    -0.84622f,
 -0.0734112f,   -0.304097f,    -0.40206f,   -0.210132f,   -0.919127f,    0.652033f,
   -0.83151f,   -0.183948f,   -0.671107f,    0.852476f,    0.043595f,   -0.404532f,
    0.75494f,   -0.335653f,    0.618433f,    0.605707f,    0.708583f,   -0.477195f,
   0.899474f,    0.490623f,    0.221729f,   -0.400381f,   -0.853727f,   -0.932586f,
   0.659113f,    0.961303f,    0.325948f,   -0.750851f,    0.842466f,    0.734401f,
  -0.649866f,    0.394491f,   -0.466056f,   -0.434073f,    0.109026f,   0.0847028f,
  -0.738857f,    0.241505f,     0.16228f,    -0.71426f,   -0.883665f,   -0.150408f,
   -0.90396f,   -0.686549f,   -0.785214f,    0.488548f,   0.0246433f,    0.142473f,
  -0.602136f,    0.375845f, -0.00779736f,    0.498955f,   -0.268147f,    0.856382f,
  -0.386007f,   -0.596094f,   -0.867735f,   -0.570977f,   -0.914366f,     0.28896f,
   0.672206f,   -0.233783f,     0.94815f,    0.895262f,    0.343252f,   -0.173388f,
  -0.767971f,   -0.314748f,    0.824308f,   -0.342092f,    0.721431f,    -0.24004f,
   -0.63653f,    0.553277f,    0.376272f,    0.158984f,   -0.452659f,    0.396323f,
  -0.420676f,   -0.454154f,    0.122179f,    0.295857f,   0.0664225f,   -0.202075f,
  -0.724788f,    0.453513f,    0.224567f,   -0.908812f,    0.176349f,   -0.320516f,
  -0.697139f,    0.742702f,   -0.900786f,    0.471489f,   -0.133532f,    0.119127f,
  -0.889769f,    -0.23183f,   -0.669673f,   -0.046891f,   -0.803433f,   -0.966735f,
   0.475578f,   -0.652644f,   0.0112459f,   -0.730007f,    0.128283f,    0.145647f,
  -0.619318f,    0.272023f,    0.392966f,    0.646418f,  -0.0207675f,   -0.315908f,
   0.480797f,    0.535668f,   -0.250172f,    -0.83093f,   -0.653773f,   -0.443809f,
   0.119982f,   -0.897642f,     0.89453f,    0.165789f,    0.633875f,   -0.886839f,
   0.930877f,   -0.537194f,    0.587732f,    0.722011f,   -0.209461f,  -0.0424659f,
  -0.814267f,   -0.919432f,    0.280262f,    -0.66302f,   -0.558099f,   -0.537469f,
  -0.598779f,    0.929656f,   -0.170794f,   -0.537163f,    0.312581f,    0.959442f,
   0.722652f,    0.499931f,    0.175616f,   -0.534874f,   -0.685115f,    0.444999f,
    0.17171f,    0.108202f,   -0.768704f,   -0.463828f,    0.254231f,    0.546014f,
   0.869474f,    0.875212f,   -0.944427f,    0.130724f,   -0.110185f,    0.312184f,
   -0.33138f,   -0.629206f,   0.0606546f,    0.722866f,  -0.0979477f,    0.821561f,
  0.0931258f,   -0.972808f,   0.0318151f,   -0.867033f,   -0.387228f,    0.280995f,
  -0.218189f,   -0.539178f,   -0.427359f,   -0.602075f,    0.311971f,    0.277974f,
   0.773159f,    0.592493f,  -0.0331884f,   -0.630854f,   -0.269947f,    0.339132f,
   0.581079f,    0.209461f,   -0.317433f,   -0.284993f,    0.181323f,    0.341634f,
   0.804959f,   -0.229572f,   -0.758907f,   -0.336721f,    0.605463f,   -0.991272f,
 -0.0188754f,   -0.300191f,    0.368307f,   -0.176135f,     -0.3832f,   -0.749569f,
    0.62356f,   -0.573938f,    0.278309f,   -0.971313f,    0.839994f,   -0.830686f,
   0.439078f,     0.66128f,    0.694514f,   0.0565042f,     0.54342f,   -0.438804f,
 -0.0228428f,   -0.687068f,    0.857267f,    0.301991f,   -0.494255f,   -0.941039f,
   0.775509f,    0.410575f,   -0.362081f,   -0.671534f,   -0.348379f,    0.932433f,
   0.886442f,    0.868681f,   -0.225666f,   -0.062211f,  -0.0976425f,   -0.641444f,
  -0.848112f,    0.724697f,    0.473503f,    0.998749f,    0.174701f,    0.559625f,
  -0.029099f,   -0.337392f,   -0.958129f,   -0.659785f,    0.236042f,   -0.246937f,
   0.659449f,   -0.027512f,    0.821897f,   -0.226215f,   0.0181735f,    0.500481f,
  -0.420127f,   -0.427878f,    0.566186f
};

#define Hash2d(a,b)   \
  hashTable[(int)(hashTable[(int)((a) & 0xfffL)] ^ ((b) & 0xfffL))]

#define Hash1d(a,b)   \
  hashTable[(int)(a) ^ ((b) & 0xfffL)]


void noiseInit() {
  for ( int i = 0; i < 4096; i++ )
    hashTable[i] = i;

  for ( int i = 4095; i >= 0; i--) {
    int j = MyRand() % 4096;
    int temp = hashTable[i];
    hashTable[i] = hashTable[j];
    hashTable[j] = temp;
  }
}

TVector3 TurbulenceNoise::getNoise3( const TPoint3 &point ) {
  float x, y, z;
  float *mp;
  long ix, iy, iz, jx, jy, jz;
  int ixiy_hash, ixjy_hash, jxiy_hash, jxjy_hash;
  float px, py, pz, s;
  float sx, sy, sz, tx, ty, tz;
  float txty, sxty, txsy, sxsy;
  
  x = point[__X]-MINX;
  y = point[__Y]-MINY;
  z = point[__Z]-MINZ;
  
  // its equivalent integer lattice point.
  ix = (long)x; iy = (long)y; iz = (long)z;
  jx = ix + 1; jy = iy + 1; jz = iz + 1;

  sx = s_curve(x - ix); sy = s_curve(y - iy); sz = s_curve(z - iz);
  
  // the complement values of sx,sy,sz
  tx = 1.0f - sx; ty = 1.0f - sy; tz = 1.0f - sz;
  
  //  interpolate!
  txty = tx * ty;
  sxty = sx * ty;
  txsy = tx * sy;
  sxsy = sx * sy;
  ixiy_hash = Hash2d(ix, iy);
  jxiy_hash = Hash2d(jx, iy);
  ixjy_hash = Hash2d(ix, jy);
  jxjy_hash = Hash2d(jx, jy);

  float resX, resY, resZ;

  mp = &RTable[(int) Hash1d(ixiy_hash, iz) & 0xFF];
  px = x - ix;  py = y - iy;  pz = z - iz;
  s = txty*tz;
  resX = INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resY = INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resZ = INCRSUMP(mp, s, px, py, pz);
  
  mp = &RTable[(int) Hash1d(jxiy_hash, iz) & 0xFF];
  px = x - jx;
  s = sxty*tz;
  resX += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resY += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resZ += INCRSUMP(mp, s, px, py, pz);
  
  mp = &RTable[(int) Hash1d(jxjy_hash, iz) & 0xFF];
  py = y - jy;
  s = sxsy*tz;
  resX += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resY += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resZ += INCRSUMP(mp, s, px, py, pz);
  
  mp = &RTable[(int) Hash1d(ixjy_hash, iz) & 0xFF];
  px = x - ix;
  s = txsy*tz;
  resX += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resY += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resZ += INCRSUMP(mp, s, px, py, pz);
  
  mp = &RTable[(int) Hash1d(ixjy_hash, jz) & 0xFF];
  pz = z - jz;
  s = txsy*sz;
  resX += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resY += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resZ += INCRSUMP(mp, s, px, py, pz);
  
  mp = &RTable[(int) Hash1d(jxjy_hash, jz) & 0xFF];
  px = x - jx;
  s = sxsy*sz;
  resX += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resY += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resZ += INCRSUMP(mp, s, px, py, pz);
  
  mp = &RTable[(int) Hash1d(jxiy_hash, jz) & 0xFF];
  py = y - iy;
  s = sxty*sz;
  resX += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resY += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resZ += INCRSUMP(mp, s, px, py, pz);
  
  mp = &RTable[(int) Hash1d(ixiy_hash, jz) & 0xFF];
  px = x - ix;
  s = txty*sz;
  resX += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resY += INCRSUMP(mp, s, px, py, pz);
  mp += 4;
  resZ += INCRSUMP(mp, s, px, py, pz);

  return TVector3( resX, resY, resZ );
}

float TurbulenceNoise::getNoise1( const TPoint3 &p ) {
  float x, y, z;
  float *mp;
  long ix, iy, iz, jx, jy, jz;
  int ixiy_hash, ixjy_hash, jxiy_hash, jxjy_hash;
  
  float sx, sy, sz, tx, ty, tz;
  float sum;
  
  float x_ix, x_jx, y_iy, y_jy, z_iz, z_jz, txty, sxty, txsy, sxsy;
  
  x = p[__X]-MINX;
  y = p[__Y]-MINY;
  z = p[__Z]-MINZ;
  
  // its equivalent integer lattice point.
  ix = (long)x; iy = (long)y; iz = (long)z;
  jx = ix + 1; jy = iy + 1; jz = iz + 1;
  
  sx = s_curve(x - ix); sy = s_curve(y - iy); sz = s_curve(z - iz);
  
  // the complement values of sx,sy,sz
  tx = 1.0f - sx; ty = 1.0f - sy; tz = 1.0f - sz;
  
  //  interpolate!
  x_ix = x - ix;
  x_jx = x - jx;
  y_iy = y - iy;
  y_jy = y - jy;
  z_iz = z - iz;
  z_jz = z - jz;
  txty = tx * ty;
  sxty = sx * ty;
  txsy = tx * sy;
  sxsy = sx * sy;
  ixiy_hash = Hash2d(ix, iy);
  jxiy_hash = Hash2d(jx, iy);
  ixjy_hash = Hash2d(ix, jy);
  jxjy_hash = Hash2d(jx, jy);
  
  mp = &RTable[(int) Hash1d(ixiy_hash, iz) & 0xFF];
  sum = INCRSUMP(mp, (txty*tz), x_ix, y_iy, z_iz);
  
  mp = &RTable[(int) Hash1d(jxiy_hash, iz) & 0xFF];
  sum += INCRSUMP(mp, (sxty*tz), x_jx, y_iy, z_iz);
  
  mp = &RTable[(int) Hash1d(ixjy_hash, iz) & 0xFF];
  sum += INCRSUMP(mp, (txsy*tz), x_ix, y_jy, z_iz);
  
  mp = &RTable[(int) Hash1d(jxjy_hash, iz) & 0xFF];
  sum += INCRSUMP(mp, (sxsy*tz), x_jx, y_jy, z_iz);
  
  mp = &RTable[(int) Hash1d(ixiy_hash, jz) & 0xFF];
  sum += INCRSUMP(mp, (txty*sz), x_ix, y_iy, z_jz);
  
  mp = &RTable[(int) Hash1d(jxiy_hash, jz) & 0xFF];
  sum += INCRSUMP(mp, (sxty*sz), x_jx, y_iy, z_jz);

  mp = &RTable[(int) Hash1d(ixjy_hash, jz) & 0xFF];
  sum += INCRSUMP(mp, (txsy*sz), x_ix, y_jy, z_jz);
  
  mp = &RTable[(int) Hash1d(jxjy_hash, jz) & 0xFF];
  sum += INCRSUMP(mp, (sxsy*sz), x_jx, y_jy, z_jz);
  
  sum = sum + 0.5f;                     // range at this point -0.5 - 0.5...
  
  if (sum < 0.0f)
    sum = 0.0f;
  if (sum > 1.0f)
    sum = 1.0f;
  
  return (sum);
}

TurbulenceNoise::TurbulenceNoise() {
  TTurbulence::TTurbulence(); turbulenceKind = TURBULENCE_NOISE;
}

float TurbulenceNoise::getTurbulence1( const TPoint3 &p ) const {
  float value = TurbulenceNoise::getNoise1( p );
  
  float l = lambda;
  float o = omega;

  for ( int i = 2; i <= octaves; i++ ) {
    TVector3 temp = p * l;
    value += ( o * getNoise1( temp ) );
    if ( i < octaves ) {
      l *= lambda;
      o *= omega;
    }
  }
  return value;
}

TVector3 TurbulenceNoise::getTurbulence3( const TPoint3 &p ) const {
  TVector3 result = getNoise3( p );
  
  float l = lambda;
  float o = omega;

  for ( int i = 2; i <= octaves; i++ ) {
    TVector3 temp = p * l;
    TVector3 value = TurbulenceNoise::getNoise3( temp );
    
    result += ( value * o );
    if ( i < octaves ) {
      l *= lambda;
      o *= omega;
    }
  }
  return result;
}

void TurbulenceNoise::print( const char *msg ) const {
  TTurbulence::print( msg );
}

const TTagList TurbulenceNoise::turbulenceNoiseTags [] = {
  { NULL, -1 }
};
