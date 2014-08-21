#include <stdlib.h>

#include "perspective.h"

const TTagList CameraPerspective::cameraPerspectiveTags [] = {
  { NULL, -1 }
};

CameraPerspective::CameraPerspective() {
  TCamera::TCamera(); cameraKind = CAMERA_PERSPECTIVE;

  direction = TVector3( 0, 0, 1 );
  sky = TVector3( 0, 1, 0 );
  right = TVector3( 1.33f, 0, 0 );
  up = TVector3( 0, 1, 0 );
}

void CameraPerspective::update() {
	{
	// Compute Direction using LookAt
	N = direction;
	U = up;
	V = right;
	float dL = N.magnitude();
	float uL = U.magnitude();
	float rL = V.magnitude();
	TVector3 tempV = crossProduct( U, N );
	float handedness = tempV | V;
	N = lookat - location;
	if ( N.magnitudeSquared() < EPS ) {
		return;
	//--throw System.Exception("Camera location and lookat must be different");
	} // TODO
	N.normalize();
	tempV = V;
	V = sky * N;
	if ( V.x < EPS
		&& V.y < EPS
		&& V.z < EPS )
		V = tempV;
	V.normalize();
	U = crossProduct( N, V );
	N = N * dL;
	if ( handedness > 0 )
		V = V * rL;
	else
		V = V * -rL;
	U = U * uL;
	}
	{
	// Apply FOV
	float rL = V.magnitude();
	float dL = rL / tanf( fov * M_PI / 360.0f ) / 2.0f;
	N = N * dL;
	}

	//--TCamera::update();
}

bool CameraPerspective::computeDirectionVector( const float width, const float height,
						TVector3 &topLeft, TVector3 &rightStep, TVector3 &upStep ) const {
	// N: direction
	// V: right
	// U: up
  rightStep = V / width;
  upStep = U / height;
  topLeft = N + U * ( height - 1.0f ) / height;
//--      float x0 = (float)x/(float)resolutionWidth - 0.5f;
//--      float y0 = (float)(resolutionHeight-1-y) /(float)resolutionHeight  - 0.5f;
//--      ray.direction = N + x0 * U + y0 * V;
//--      ray.direction.normalize();
/*
  float widthCamera = tanf(rads(fov*0.5f));
  float heightCamera = widthCamera * height / width;
  
  topLeft = N
    + (U * heightCamera)
    - (V * widthCamera);
  rightStep = V * ( 2.0f * widthCamera / width );
  upStep = U * ( 2.0f * heightCamera / height );
*/
  return true;
}


void CameraPerspective::print( const char *msg ) const {
  TCamera::print(msg);
}
