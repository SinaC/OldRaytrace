#ifndef __CAMERA3D_H__
#define __CAMERA3D_H__

#include "model.h"
#include "plane.h"

#define FRUSTUM_LEFT (0)
#define FRUSTUM_RIGHT (1)
#define FRUSTUM_BOTTOM (2)
#define FRUSTUM_TOP (3)
#define FRUSTUM_FRONT (4)
#define FRUSTUM_BACK (5)

#define CLIP_FRUSTUM_INSIDE  (0)
#define CLIP_FRUSTUM_OUTSIDE (1)
#define CLIP_FRUSTUM_CUT     (2)

class TCamera3D: public TModel {
 public:
  TCamera3D();

  void     update              ();
  
  void     setMirrorX          ( const bool mirror_x );
  void     setNearFar          ( const float near_clip, const float far_clip );
  double   getNear             ();
  double   getFar              ();
  void     setZoomFactor       ( const float zoom_factor );
  double   getZoomFactor       () const;
  void     setFov              ( const float fov );
  double   getFov              () const;
  void     modFov              ( const float mod );
  void     setAspect           ( const float asp );
  double   getAspect           () const;
  void     setLookAt           ( const TVector4 &v );
  void     updateOrientation();
  
  TMatrix4   getFrustumMatrix    ( const float left, const float right, const float bottom, const float top, const float nearval, const float farval ) const;
  TMatrix4   getPerspectiveMatrix( const float fovy, const float aspect, const float zNear,  const float zFar ) const;
  TVector4  projectVector       ( const TVector4 &v ) const;

  void     computeFrustum      ( const float fovy, const float aspect, const float zNear, const float zFar );
  TPlane    getFrustum         ( const int planeId ) const;
  bool     cullPoint           ( const TVector4 &v ) const;
  bool     cullSphere          ( const TVector4 &v, const float radius ) const;
  bool     cullAABox           ( const TVector4 &pos, const TVector4 &size );
  
  
  // protected:
  TVector4                 lookAt;                   //  Camera lookAt: can replace orientation
  TMatrix4                 toScreenSpaceMatrix;      //  To screen space -matrix
  TMatrix4                 viewMatrix;               //  Current/latest view matrix
  TMatrix4                 projectionMatrix;         //  Current/latest projection matrix
  double                   nearClip;                 //  Near clip plane z
  double                   farClip;                  //  Far clip plane z
  double                   fov;                      //  Field of vision
  double                   aspect;                   //  Aspect X/Y
  float                    zoomFactor;               //  Zoom Factor
  TPlane                   frustum[6];               //  Holds the Current Frustum Plane Equations
  bool                     mirrorX;                  //  Mirror X
  bool                     projectionDirty;          //  Projection needs to be redone?
};

#endif
