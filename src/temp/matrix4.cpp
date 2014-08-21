#include <stdio.h>

#include "matrix4.h"

static const float a_identity_matrix4[] = {
  1,0,0,0, 
  0,1,0,0,
  0,0,1,0,
  0,0,0,1
};
static const float a_swap_xy_matrix4[] = {
  0,1,0,0, 
  1,0,0,0,
  0,0,1,0,
  0,0,0,1
};
static const float a_swap_xz_matrix4[] = {
  0,0,1,0, 
  0,1,0,0,
  1,0,0,0,
  0,0,0,1
};
static const float a_swap_yz_matrix4[] = {
  1,0,0,0, 
  0,0,1,0,
  0,1,0,0,
  0,0,0,1
};
static const float a_mirror_x_matrix4[] = {
  -1, 0, 0, 0, 
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};
static const float a_mirror_y_matrix4[] = {
  1, 0, 0, 0, 
  0,-1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};
static const float a_mirror_z_matrix4[] = {
  1, 0, 0, 0, 
  0, 1, 0, 0,
  0, 0,-1, 0,
  0, 0, 0, 1
};
static const float *const mat_i   = &a_identity_matrix4[0];
static const float *const mat_sxy = &a_swap_xy_matrix4 [0];
static const float *const mat_sxz = &a_swap_xz_matrix4 [0];
static const float *const mat_syz = &a_swap_yz_matrix4 [0];
static const float *const mat_mx  = &a_mirror_x_matrix4[0];
static const float *const mat_my  = &a_mirror_y_matrix4[0];
static const float *const mat_mz  = &a_mirror_z_matrix4[0];
/*static*/ const TMatrix4 TMatrix4::IDENTITY( mat_i );
/*static*/ const TMatrix4 TMatrix4::SWAP_XY ( mat_sxy );
/*static*/ const TMatrix4 TMatrix4::SWAP_XZ ( mat_sxz );
/*static*/ const TMatrix4 TMatrix4::SWAP_YZ ( mat_syz );
/*static*/ const TMatrix4 TMatrix4::MIRROR_X( mat_mx );
/*static*/ const TMatrix4 TMatrix4::MIRROR_Y( mat_my );
/*static*/ const TMatrix4 TMatrix4::MIRROR_Z( mat_mz );

TMatrix4 TMatrix4::operator*( const TMatrix4 &mat ) const {
  // 36 muls, 27 adds
  // | m[0][0] m[1][0] m[2][0] m[3][0] |   | mat.m[0][0] mat.m[1][0] mat.m[2][0] mat.m[3][0] |   | m[0][0]*mat.m[0][0]+m[1][0]*mat.m[0][1]+m[2][0]*mat.m[0][2] m[0][0]*mat.m[1][0]+m[1][0]*mat.m[1][1]+m[2][0]*mat.m[1][2] m[0][0]*mat.m[2][0]+m[1][0]*mat.m[2][1]+m[2][0]*mat.m[2][2] m[0][0]*mat.m[3][0]+m[1][0]*mat.m[3][1]+m[2][0]*mat.m[3][2]+m[3][0] |
  // | m[0][1] m[1][1] m[2][1] m[3][1] |   | mat.m[0][1] mat.m[1][1] mat.m[2][1] mat.m[3][1] |   | m[0][1]*mat.m[0][0]+m[1][1]*mat.m[0][1]+m[2][1]*mat.m[0][2] m[0][1]*mat.m[1][0]+m[1][1]*mat.m[1][1]+m[2][1]*mat.m[1][2] m[0][1]*mat.m[2][0]+m[1][1]*mat.m[2][1]+m[2][1]*mat.m[2][2] m[0][1]*mat.m[3][0]+m[1][1]*mat.m[3][1]+m[2][1]*mat.m[3][2]+m[3][1] |
  // | m[0][2] m[1][2] m[2][2] m[3][2] | * | mat.m[0][2] mat.m[1][2] mat.m[2][2] mat.m[3][2] | = | m[0][2]*mat.m[0][0]+m[1][2]*mat.m[0][1]+m[2][2]*mat.m[0][2] m[0][2]*mat.m[1][0]+m[1][2]*mat.m[1][1]+m[2][2]*mat.m[1][2] m[0][2]*mat.m[2][0]+m[1][2]*mat.m[2][1]+m[2][2]*mat.m[2][2] m[0][2]*mat.m[3][0]+m[1][2]*mat.m[3][1]+m[2][2]*mat.m[3][2]+m[3][2] |
  // | 0   0   0   1   |   | 0     0     0     1     |   | 0                             0                             0                             1                                 |
  TMatrix4 mRet;

  mRet.m[0][0] = m[0][0]*mat.m[0][0] + m[1][0]*mat.m[0][1] + m[2][0]*mat.m[0][2] + m[3][0]*mat.m[0][3];
  mRet.m[1][0] = m[0][0]*mat.m[1][0] + m[1][0]*mat.m[1][1] + m[2][0]*mat.m[1][2] + m[3][0]*mat.m[1][3];
  mRet.m[2][0] = m[0][0]*mat.m[2][0] + m[1][0]*mat.m[2][1] + m[2][0]*mat.m[2][2] + m[3][0]*mat.m[2][3];
  mRet.m[3][0] = m[0][0]*mat.m[3][0] + m[1][0]*mat.m[3][1] + m[2][0]*mat.m[3][2] + m[3][0]*mat.m[3][3];

  mRet.m[0][1] = m[0][1]*mat.m[0][0] + m[1][1]*mat.m[0][1] + m[2][1]*mat.m[0][2] + m[3][1]*mat.m[0][3];
  mRet.m[1][1] = m[0][1]*mat.m[1][0] + m[1][1]*mat.m[1][1] + m[2][1]*mat.m[1][2] + m[3][1]*mat.m[1][3];
  mRet.m[2][1] = m[0][1]*mat.m[2][0] + m[1][1]*mat.m[2][1] + m[2][1]*mat.m[2][2] + m[3][1]*mat.m[2][3];
  mRet.m[3][1] = m[0][1]*mat.m[3][0] + m[1][1]*mat.m[3][1] + m[2][1]*mat.m[3][2] + m[3][1]*mat.m[3][3];

  mRet.m[0][2] = m[0][2]*mat.m[0][0] + m[1][2]*mat.m[0][1] + m[2][2]*mat.m[0][2] + m[3][2]*mat.m[0][3];
  mRet.m[1][2] = m[0][2]*mat.m[1][0] + m[1][2]*mat.m[1][1] + m[2][2]*mat.m[1][2] + m[3][2]*mat.m[1][3];
  mRet.m[2][2] = m[0][2]*mat.m[2][0] + m[1][2]*mat.m[2][1] + m[2][2]*mat.m[2][2] + m[3][2]*mat.m[2][3];
  mRet.m[3][2] = m[0][2]*mat.m[3][0] + m[1][2]*mat.m[3][1] + m[2][2]*mat.m[3][2] + m[3][2]*mat.m[3][3];

  mRet.m[0][3] = m[0][3]*mat.m[0][0] + m[1][3]*mat.m[0][1] + m[2][3]*mat.m[0][2] + m[3][3]*mat.m[0][3];
  mRet.m[1][3] = m[0][3]*mat.m[1][0] + m[1][3]*mat.m[1][1] + m[2][3]*mat.m[1][2] + m[3][3]*mat.m[1][3];
  mRet.m[2][3] = m[0][3]*mat.m[2][0] + m[1][3]*mat.m[2][1] + m[2][3]*mat.m[2][2] + m[3][3]*mat.m[2][3];
  mRet.m[3][3] = m[0][3]*mat.m[3][0] + m[1][3]*mat.m[3][1] + m[2][3]*mat.m[3][2] + m[3][3]*mat.m[3][3];

  return mRet;
}

//--void TMatrix4::operator=( const Quaternion &q ){
//--	// 9 muls, 15 adds
//--	double x2 = q.v[0] + q.v[0];
//--	double y2 = q.v[1] + q.v[1];
//--	double z2 = q.v[2] + q.v[2];
//--	double xx = q.v[0] * x2;  double xy = q.v[0] * y2;  double xz = q.v[0] * z2;
//--	double yy = q.v[1] * y2;  double yz = q.v[1] * z2;  double zz = q.v[2] * z2;
//--	double wx = q.v[3] * x2;  double wy = q.v[3] * y2;  double wz = q.v[3] * z2;
//--
//--	m[0][3] = 
//--	m[1][3] = 
//--	m[2][3] = 
//--	m[3][0] = m[3][1] = m[3][2] = 0; 
//--	m[3][3] = 1;
//--	m[0][0] = (float)(1-(yy+zz));  m[1][0] = (float)(xy+wz);      m[2][0] = (float)(xz-wy);
//--	m[0][1] = (float)(xy-wz);      m[1][1] = (float)(1-(xx+zz));  m[2][1] = (float)(yz+wx);
//--	m[0][2] = (float)(xz+wy);      m[1][2] = (float)(yz-wx);      m[2][2] = (float)(1-(xx+yy));
//--}

void TMatrix4::print( const char *msg ) const {
  printf("%s\n", msg );
  for ( int j = 0; j < 4; j++ ) {
    for ( int i = 0; i < 4; i++ )
      printf("%10.5f  ", m[i][j] );
    printf("\n");
  }
}
