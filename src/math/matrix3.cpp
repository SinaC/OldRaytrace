#include <stdio.h>

#include "matrix3.h"

static const float a_identity_matrix3[] = {
  1,0,0, 
  0,1,0,
  0,0,1
};
static const float a_swap_xy_matrix3[] = {
  0,1,0, 
  1,0,0,
  0,0,1
};
static const float a_swap_xz_matrix3[] = {
  0,0,1, 
  0,1,0,
  1,0,0
};
static const float a_swap_yz_matrix3[] = {
  1,0,0, 
  0,0,1,
  0,1,0
};
static const float a_mirror_x_matrix3[] = {
  -1, 0, 0, 
  0, 1, 0,
  0, 0, 1
};
static const float a_mirror_y_matrix3[] = {
  1, 0, 0,
  0,-1, 0,
  0, 0, 1
};
static const float a_mirror_z_matrix3[] = {
  1, 0, 0,
  0, 1, 0,
  0, 0,-1
};
static const float *const mat_i   = &a_identity_matrix3[0];
static const float *const mat_sxy = &a_swap_xy_matrix3 [0];
static const float *const mat_sxz = &a_swap_xz_matrix3 [0];
static const float *const mat_syz = &a_swap_yz_matrix3 [0];
static const float *const mat_mx  = &a_mirror_x_matrix3[0];
static const float *const mat_my  = &a_mirror_y_matrix3[0];
static const float *const mat_mz  = &a_mirror_z_matrix3[0];
/*static*/ const TMatrix3 TMatrix3::IDENTITY( mat_i );
/*static*/ const TMatrix3 TMatrix3::SWAP_XY ( mat_sxy );
/*static*/ const TMatrix3 TMatrix3::SWAP_XZ ( mat_sxz );
/*static*/ const TMatrix3 TMatrix3::SWAP_YZ ( mat_syz );
/*static*/ const TMatrix3 TMatrix3::MIRROR_X( mat_mx );
/*static*/ const TMatrix3 TMatrix3::MIRROR_Y( mat_my );
/*static*/ const TMatrix3 TMatrix3::MIRROR_Z( mat_mz );

TMatrix3 TMatrix3::operator*( const TMatrix3 &mat ) const {
  TMatrix3 mRet;

  mRet.m[0][0] = m[0][0]*mat.m[0][0] + m[1][0]*mat.m[0][1] + m[2][0]*mat.m[0][2];
  mRet.m[1][0] = m[0][0]*mat.m[1][0] + m[1][0]*mat.m[1][1] + m[2][0]*mat.m[1][2];
  mRet.m[2][0] = m[0][0]*mat.m[2][0] + m[1][0]*mat.m[2][1] + m[2][0]*mat.m[2][2];

  mRet.m[0][1] = m[0][1]*mat.m[0][0] + m[1][1]*mat.m[0][1] + m[2][1]*mat.m[0][2];
  mRet.m[1][1] = m[0][1]*mat.m[1][0] + m[1][1]*mat.m[1][1] + m[2][1]*mat.m[1][2];
  mRet.m[2][1] = m[0][1]*mat.m[2][0] + m[1][1]*mat.m[2][1] + m[2][1]*mat.m[2][2];

  mRet.m[0][2] = m[0][2]*mat.m[0][0] + m[1][2]*mat.m[0][1] + m[2][2]*mat.m[0][2];
  mRet.m[1][2] = m[0][2]*mat.m[1][0] + m[1][2]*mat.m[1][1] + m[2][2]*mat.m[1][2];
  mRet.m[2][2] = m[0][2]*mat.m[2][0] + m[1][2]*mat.m[2][1] + m[2][2]*mat.m[2][2];

  return mRet;
}

TMatrix3 rotateMatrix( const float th, const TVector3 & axis ) {
  TMatrix3 res;
  // rotates "th" radians around axis "axis"
  // !!! axis must be normalized !!!

  const float a=axis[__X];
  const float b=axis[__Y];
  const float c=axis[__Z];

  const float ct = cosf(th);
  const float st = sinf(th);
  const float oct = 1.0f-ct;

  //  res._11 = a*a*oct+ct;  res._12 = a*b*oct-c*st;res._13 = a*c*oct+b*st;
  //  res._21 = b*a*oct+c*st;res._22 = b*b*oct+ct;  res._23 = b*c*oct-a*st;
  //  res._31 = c*a*oct-b*st;res._32 = c*b*oct+a*st;res._33 = c*c*oct+ct;

  res.m[0][0] = a*a*oct+ct;  res.m[1][0] = a*b*oct-c*st;res.m[2][0] = a*c*oct+b*st;
  res.m[0][1] = b*a*oct+c*st;res.m[1][1] = b*b*oct+ct;  res.m[2][1] = b*c*oct-a*st;
  res.m[0][2] = c*a*oct-b*st;res.m[1][2] = c*b*oct+a*st;res.m[2][2] = c*c*oct+ct;

  return res;
}

void TMatrix3::print( const char *msg ) const {
  printf("%s\n", msg );
  for ( int j = 0; j < 3; j++ ) {
    for ( int i = 0; i < 3; i++ )
      printf("%10.5f  ", m[i][j] );
    printf("\n");
  }
}
