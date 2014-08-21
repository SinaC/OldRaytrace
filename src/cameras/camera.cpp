#include <stdio.h>

#include "../world.h"
#include "camera.h"
#include "../math/math.h"

//--#define __DEBUG

const TTagList TCamera::cameraTags [] = {
  { "location", TAG_camera_location },
  { "lookat", TAG_camera_lookat },
  { "vertical", TAG_camera_vertical },
  { "fov", TAG_camera_FOV },
  { NULL, -1 }
};

const TTagList TCamera::cameraSubClassTags [] = {
  { "classic", CAMERA_CLASSIC },
  { "perspective", CAMERA_PERSPECTIVE },
  {  NULL, -1 }
};

TCamera::TCamera() {
  THandle::THandle(); handleKind = HANDLE_CAMERA;
  cameraKind = CAMERA_NOTHING;
  location = TPoint3( 0, 50, -100 );
  lookat = TPoint3( 0, 0, 0 );
  vertical = TVector3( 0, 1, 0 );
  fov = 90.0f;
}

TCamera::~TCamera() {
}

void TCamera::setup() {
  update();
}

void TCamera::precalcFrame( const TWorld &world ) {
}

void TCamera::update() {
  vertical.normalize(); 
  N = lookat-location;
  N.normalize();

  U = crossProduct( vertical, N );
  // CHECK: these lines should fix the so called "look at" bug
  if ( U.magnitudeSquared() < EPS )
    U = crossProduct( V, N ); // use the previous vertical
  U.normalize();

  V = crossProduct( N, U );
  V.normalize();
}

bool TCamera::computeDirectionVector( const float width, const float height,
				      TVector3 &topLeft, TVector3 &rightStep, TVector3 &upStep ) const {
  float widthCamera = tanf(rads(fov*0.5f));
  float heightCamera = widthCamera * height / width;
  
  topLeft = N
    + (V * heightCamera)
    - (U * widthCamera);
  rightStep = U * ( 2.0f * widthCamera / width ); // U * ( 2.0f * widthCamera / width )
  upStep = V * ( 2.0f * heightCamera / height ); // V * ( 2.0f * widthCamera / width )

#ifdef __DEBUG
  topLeft.print("topLeft:");
  rightStep.print("rightStep:");
  upStep.print("upStep:");
#endif

  return true;
}

void TCamera::print( const char *msg ) const {
  THandle::print( msg );
  printf("KIND: [%s]\n", cameraName() );
  location.print("location: ");
  lookat.print("lookat: ");
  vertical.print("vertical: ");
  printf("fov: %5.5f\n", fov );
}

char *TCamera::cameraName() const {
  for ( int i = 0; cameraSubClassTags[i].tagName != NULL; i++ )
    if ( cameraSubClassTags[i].tagId == cameraKind )
      return cameraSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", cameraKind );
  return buf;
}
