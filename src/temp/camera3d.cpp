#include "camera3d.h"
#include "../math/math.h"

TCamera3D::TCamera3D() :
  nearClip  (   10    ),
  farClip   ( 1000    ),
  fov       (   90    ),
  zoomFactor(    1.0f ) {
  viewMatrix          = TMatrix4::IDENTITY;
  projectionMatrix    = TMatrix4::IDENTITY;
  toScreenSpaceMatrix = TMatrix4::IDENTITY;
  mirrorX             = false;
  projectionDirty     = true;
}

void TCamera3D::setMirrorX( const bool mirror_x ) {
  this->mirrorX = mirror_x;
}

void TCamera3D::setNearFar( const float near_clip, const float far_clip ){
  this->nearClip = near_clip;
  this->farClip  = far_clip;
}

double TCamera3D::getNear(){
  return this->nearClip;
}

double TCamera3D::getFar(){
  return this->farClip;
}

//!  Set field of vision
void TCamera3D::setFov( const float fov ){
  this->fov         = fov;
  this->zoomFactor = 1 / tan( fov/2 );
}

void TCamera3D::modFov( const float mod ){
  fov += mod;
  this->zoomFactor = 1 / tan( rads(fov)/2 );
}

//!  Return field of vision
double TCamera3D::getFov() const {
  return fov;
}

void TCamera3D::setZoomFactor( const float zoom_factor ){
  this->zoomFactor = zoom_factor;
  this->fov = degs( 2 * atan( 1 / zoomFactor ) );
}

double TCamera3D::getZoomFactor() const {
  return this->zoomFactor;
}

// Create frustum with fov, aspect, near and far information
static TPlane createPlaneFrustum( const TVector4 &normal,
				  const TVector4 &position,
				  const TMatrix4 &viewMatrix ) {
  TPlane p;
  const float CLIP_PLANE_EPSILON = 0.0001;
  p = viewMatrix * normal;
  float distance = -( ( position | p.getNormal() ) + CLIP_PLANE_EPSILON );
  p.setConstant( distance );
  return p;
}
void TCamera3D::computeFrustum( const float fovy, const float aspect, const float zNear, const float zFar ) {
  TMatrix4 reverseViewMatrix = getViewMatrix();
  reverseViewMatrix.transpose();

  TVector4 normal;
  float s = sin( rads( fovy / 2.0 ) );
  float c = cos( rads( fovy / 2.0 ) );
  
  // left clip plane
  normal = TVector4( -c, 0, s, 0 );
  frustum[FRUSTUM_LEFT] = createPlaneFrustum( normal, position, reverseViewMatrix );
  // right clip plane
  normal = TVector4( c, 0, s, 0 );
  frustum[FRUSTUM_RIGHT] = createPlaneFrustum( normal, position, reverseViewMatrix );
  // bottom clip plane
  normal = TVector4( 0, -c, s, 0 );
  frustum[FRUSTUM_BOTTOM] = createPlaneFrustum( normal, position, reverseViewMatrix );
  // top clip plane
  normal = TVector4( 0, c, s, 0 );
  frustum[FRUSTUM_TOP] = createPlaneFrustum( normal, position, reverseViewMatrix );

  // front clip plane
  normal = TVector4( 0, 0, -1, 0 );
  TPoint4 pointNear = position + reverseViewMatrix * ( normal * zNear ); // point near from camera
  TPoint4 pointFar = position + reverseViewMatrix * ( normal * zFar ); // point far from camera
  frustum[FRUSTUM_FRONT] = createPlaneFrustum( normal, pointNear, reverseViewMatrix );
  frustum[FRUSTUM_FRONT].neg();
  // back clip plane
  normal = TVector4( 0, 0, 1, 0 );
  frustum[FRUSTUM_BACK] = createPlaneFrustum( normal, pointFar, reverseViewMatrix );
  frustum[FRUSTUM_BACK].neg();
}

/*!
  \brief Test If A Point Is In The Frustum.
  \todo  Optimization: remember which plane caused false last time and test it first
*/
bool TCamera3D::cullPoint( const TVector4 &v ) const {
  for ( int i = 0; i < 6; i++ )
    if ( frustum[i].distance( v ) <= 0 )
      return false;
  return true;
}

/*
  \brief Test If A Sphere Is In The Frustum
  \todo  Optimization: remember which plane caused false last time and test it first
*/
bool TCamera3D::cullSphere( const TVector4 &v, float radius ) const {
  for ( int i = 0; i < 6; i++ )
    if ( frustum[i].distance( v ) <= -radius )
      return false;
  return true;
}

/*!
  \brief Test If A box Is In The Frustum
  \todo  Optimization: for each corner remember which plane caused continue last time and test it first
*/
bool TCamera3D::cullAABox( const TVector4 &pos, const TVector4 &size ){
  TVector4 min = pos - size;
  TVector4 max = pos + size;
  for ( int i = 0; i < 6; i++ ) {
    if ( frustum[i].distance( min ) > 0 ) continue;
    if ( frustum[i].distance( TVector4( max[0], min[1], min[2], 0 ) ) > 0 ) continue;
    if ( frustum[i].distance( TVector4( min[0], max[1], min[2], 0 ) ) > 0 ) continue;
    if ( frustum[i].distance( TVector4( max[0], max[1], min[2], 0 ) ) > 0 ) continue;
    if ( frustum[i].distance( TVector4( min[0], min[1], max[2], 0 ) ) > 0 ) continue;
    if ( frustum[i].distance( TVector4( max[0], min[1], max[2], 0 ) ) > 0 ) continue;
    if ( frustum[i].distance( TVector4( min[0], max[1], max[2], 0 ) ) > 0 ) continue;
    if ( frustum[i].distance( max ) > 0 ) continue;
    return false;
  }
  return true;
}

//!  Calculate frustum matrix
TMatrix4 TCamera3D::getFrustumMatrix( const float left, const float right, const float bottom, const float top, const float nearval, const float farval ) const {
  float x, y, a, b, c, d;
  TMatrix4 frustum_matrix;
  
  x =  (nearval + nearval) / (right  - left   );
  y =  (nearval + nearval) / (top    - bottom );
  a =  (right   + left   ) / (right  - left   );
  b =  (top     + bottom ) / (top    - bottom );
  c = -(farval  + nearval) / (farval - nearval);
  d = -(2 * farval * nearval) / (farval - nearval);
  
#	define M(row,col) frustum_matrix.m[col][row]
  M(0,0) = x;  M(0,1) = 0;  M(0,2) =  a;  M(0,3) = 0;
  M(1,0) = 0;  M(1,1) = y;  M(1,2) =  b;  M(1,3) = 0;
  M(2,0) = 0;  M(2,1) = 0;  M(2,2) =  c;  M(2,3) = d;
  M(3,0) = 0;  M(3,1) = 0;  M(3,2) = -1;  M(3,3) = 0;
#	undef M
  
  return frustum_matrix;
}


//!  Calculate perspective projection matrix
TMatrix4 TCamera3D::getPerspectiveMatrix( const float fovy, const float aspect, const float zNear, const float zFar ) const {
  float xmin, xmax, ymin, ymax;
  
  ymax =  zNear * tan( ( fovy / 2.0 ) * M_PI / 180.0 );
  ymin = -ymax;
  xmin =  ymin * aspect;
  xmax =  ymax * aspect;
  
  return getFrustumMatrix( xmin, xmax, ymin, ymax, zNear, zFar );
}

//!  Project vector from world coordinates to screen coordinates
TVector4 TCamera3D::projectVector( const TVector4 &v ) const{
  return toScreenSpaceMatrix.transformVector( v );
}

void TCamera3D::setAspect( const float asp ) {
  aspect = asp;
}

double TCamera3D::getAspect() const {
  return aspect;
}

TPlane TCamera3D::getFrustum( const int planeId ) const {
  return frustum[planeId];
}

//! Update camera matrices and frustum
void TCamera3D::update() {
  updateOrientation();
  viewMatrix = getViewMatrix();
  toScreenSpaceMatrix = getWorldToLocalMatrix();
  projectionMatrix = getPerspectiveMatrix( fov, aspect, nearClip, farClip );
  computeFrustum( fov, aspect, nearClip, farClip );
}

//! Update orientation with lookAt
void TCamera3D::updateOrientation() {
  TVector4 dir = position - lookAt;
  dir.normalize();
  orientation[1] = atan2( dir[0], dir[2] ); // yaw
  orientation[0] = asin( dir[1] ); // pitch
}

//! Set lookAt and update orientation
void TCamera3D::setLookAt( const TVector4 &v ) {
  lookAt = v;
  updateOrientation();
}
