
#include "plane.h"

/*!	
  Ax + Bx + Cx + D = 0
	
  -( Ax + Bx + Cx ) = D
*/

/*
  TPlane::TPlane( const TVector4 &p1, const TVector4 &p2, const TVector4 &p3 ){
  *this = (p1-p2) * (p1-p3);
  this->Norm();
  d = -( r*p1.r + s*p1.s + t*p1.t );
  } */


/*!
  Distance - returns positive or negative or (near) zero
  if point is on side or another or in this plane.
*/
double TPlane::distance( const TVector4 &p ) const {
  return( v[0]*p.v[0] + v[1]*p.v[1] + v[2]*p.v[2] + v[3] );
}

void TPlane::setConstant( const float d ) {
  v[3] = d;
}

//!  Negate plane, that is, flip
void TPlane::neg(){
  v[0] = -v[0];
  v[1] = -v[1];
  v[2] = -v[2];
  v[3] = -v[3];
}



TVector4 TPlane::getNormal() const {
  return TVector4( v[0], v[1], v[2], 0 );
}


double TPlane::getConstant() const {
  return v[3];
}

//!  Assign vector4
TPlane &TPlane::operator=( const TVector4 &vec ){
  v[0] = vec.v[0];
  v[1] = vec.v[1];
  v[2] = vec.v[2]; 
  v[3] = vec.v[3]; 
  return( *this );
}
