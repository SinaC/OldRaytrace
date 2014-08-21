#ifndef __PLANE_H__
#define __PLANE_H__

#include "../math/vector4.h"

//!  Plane
class TPlane : public TVector4 {
 public:
  TPlane(){}
  TPlane( const float a, const float b, const float c, const float d ):TVector4(a,b,c,d){}
  
  void    setConstant( const float d );
  void    neg        ();
  double  distance   ( const TVector4 &p ) const;
  TPlane  &operator=  ( const TVector4 &v );
  TVector4  getNormal  () const;
  double  getConstant() const;
};

#endif  //  __PLANE_H__
