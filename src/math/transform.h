#include "vector3.h"
#include "matrix4.h"

struct TTransform {
  //----- Datas
  TMatrix4 matrix;
  TMatrix4 inverse;

  //----- Constructor
  TTransform();

  //----- Methods
  TPoint3 transformPoint( const TPoint3 &p ) const;
  TPoint3 inverseTransformPoint( const TPoint3 &p ) const;
  TVector3 transformDirection( const TVector3 &v ) const;
  TVector3 inverseTransformDirection( const TVector3 &v ) const;
  TVector3 transformNormal( const TVector3 &v ) const;
  TVector3 inverseTransformNormal( const TVector3 &v ) const;

  void computeMatrixTransform( const TMatrix4 &mat );
  void computeScalingTransform( const TVector3 &v );
  void computeTranslationTransform( const TVector3 &v );
  void computeRotationTransform( const TVector3 &v );
  void composeTransforms( const TTransform &newTransform );
  void computeAxisRotationTransform( const TVector3 &V1, const float angle );
  void computeCoordinateTransform( const TPoint3 &origin, const TVector3 &up,
				   const float radius, const float length );

  //----- Output
  void print( const char *msg ) const;
};  
