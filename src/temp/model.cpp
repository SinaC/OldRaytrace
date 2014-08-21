#include "model.h"

TModel::TModel() {
  //  const char *name;
  position = TPoint4( 0, 0, 0, 0 );
  orientation = TVector4( 0, 0, 0, 0 );
  clipRadius = 0.0f;
  clipCenter = 0.0f;
  state = MODEL_STATE_NOTHING;
}

TMatrix4 TModel::getWorldToLocalMatrix() const {
  //TMatrix4 m = orientation;
  TMatrix4 m; m.rotation( orientation );
  float  x = (float) -position.v[0];
  float  y = (float) -position.v[1];
  float  z = (float) -position.v[2];
  
  m.translate( x, y, z );
  return m;
}


/*!
  Simple transformation matrix from local coordinates
  to world coordinates. No tricks.
  
  localToWorld() is usually used for model matrices.
*/
TMatrix4 TModel::getLocalToWorldMatrix() const {
  //  TMatrix4 r = orientation;
  TMatrix4 r; r.rotation( orientation );
  TMatrix4 t;
  r.transpose();
  t.translateMatrix( position );
  return t * r;
}


/*!
  Camera matrix which only contains rotations;
  translations are expexted to be applied into
  objects' model matrices instead.
*/
TMatrix4 TModel::getViewMatrix() const {
  // Don't use the normal view matrix because it causes precision
  // problems if the camera is too far away from the origin.
  // Instead, pretend the camera is at the origin and offset all
  // model matrices by subtracting the camera's position.
  //  TMatrix4 m = orientation;
  TMatrix4 m; m.rotation( orientation );
  return m;
}


/*!
  Return model matrix, which works by applying camera positioning
  into objects' model matrices
*/
TMatrix4 TModel::getModelMatrix( TModel *camera ) const {
  TMatrix4 m;
  //  m.modelMatrix( orientation, position-camera->position );
  return m;
}


/*!
  \author Sean O'Neil
  
  Return scaled model matrix. Makes all objects fit to depth range, but objects
  might overlap in z buffer because of this
*/
TMatrix4 TModel::getScaledModelMatrix( TModel *camera ) const {
  // This code scales the planet's size and distance to the camera
  // down when it's too far away.
  // This solves a problem with many video card drivers where objects
  // too far away aren't rendering properly
  // It also alleviates the Z-buffer precision problem caused by having
  // your near and far clipping planes too far apart.
  const double MAX_DISTANCE    =   256.0;             // Distance to desired far clipping plane
  const double MAX_DISCERNABLE = (512.0f*512.0f);     // Beyond this distance, everything is rendered at MAX_DISTANCE
  const double HALF_MAX        = (MAX_DISTANCE*0.5);  // Everything between HALF_MAX and MAX_DISCERNABLE is scaled exponentially between HALF_MAX and MAX_DISTANCE
  TMatrix4 mat      = getModelMatrix( camera );
  TVector4 vec      = position - camera->position;
  double distance = vec.magnitude();
  
  if( distance > HALF_MAX ){
    vec           /= distance;
    double factor  = MAX_DISTANCE;
    
    if( distance < MAX_DISCERNABLE ){
      factor = (HALF_MAX + HALF_MAX * (1 - exp((HALF_MAX - distance) / MAX_DISCERNABLE)));
    }
    vec *= factor;
    mat.m[3][0]  = (float)vec.v[0];
    mat.m[3][1]  = (float)vec.v[1];
    mat.m[3][2]  = (float)vec.v[2];
    factor /= distance;
    mat.scale( (float)factor, (float)factor, (float)factor );
  }
  return mat;
}

void TModel::setPosition( const double x, const double y, const double z ){
  position.v[0] = x;
  position.v[1] = y;
  position.v[2] = z;
  position.v[3] = 1.0f;
}

//!  Get position relative to parent.
TVector4 TModel::getPosition() const {
  return position;
}

//!  Set position relative to parent
void TModel::setPosition( const TVector4 &v ){
  position = v;
}

void TModel::translate( const TVector4 &v ){
  position += v;
}

void TModel::transform( const TMatrix4 &m ){
  //  orientation *= m;
  orientation = m * orientation;
}

/*!
  TModel is rotated around roll axis (Z) by angle radians.
*/
void TModel::roll( const double radians ) {
  //  orientation.rotate( orientation.getViewAxis(), radians );
  orientation[2] += radians;
}

/*!
	Model is rotated around pitch axis (X) by angle radians.
*/
void TModel::pitch( const double radians ) {
  //  orientation.rotate( orientation.getRightAxis(), radians );
  orientation[0] += radians;
}

/*!
	Model is rotated around yaw axis (Y) by angle radians.
*/
void TModel::yaw( const double radians ) {
  //  orientation.rotate( orientation.getUpAxis(), radians );
  orientation[1] += radians;
}

//! Compute model's clipping sphere
void TModel::computeClippingSphere() {
  float meanX = 0,
    meanY = 0,
    meanZ = 0;
  int numVertices = 0;

  // Search gravity center
  std::list<TPolygon>::iterator iterP = polygonsList.begin();
  while ( iterP != polygonsList.end() ) {
    TPolygon *poly = &(*iterP);
    
    std::list<TVertex>::iterator iterV = poly->verticesList.begin();
    while ( iterV != poly->verticesList.end() ) {
      TVertex *vert = &(*iterV);

      meanX += vert->position[0];
      meanY += vert->position[1];
      meanZ += vert->position[2];
      numVertices++;

      iterV++;
    }

    iterP++;
  }

  meanX /= (float)numVertices;
  meanY /= (float)numVertices;
  meanZ /= (float)numVertices;

  // search radius
  clipRadius = -INFINITE;
  iterP = polygonsList.begin();
  while ( iterP != polygonsList.end() ) {
    TPolygon *poly = &(*iterP);
    
    std::list<TVertex>::iterator iterV = poly->verticesList.begin();
    while ( iterV != poly->verticesList.end() ) {
      TVertex *vert = &(*iterV);

      float x = vert->position[0];
      float y = vert->position[1];
      float z = vert->position[2];
      float dist = sqrt( x*x + y*y + z*z );
      if ( dist > clipRadius ) clipRadius = dist;

      iterV++;
    }

    iterP++;
  }

  clipCenter = TPoint4( meanX, meanY, meanZ, 0 );
}
