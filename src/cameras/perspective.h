#ifndef __CAMERA_PERSPECTIVE_H__
#define __CAMERA_PERSPECTIVE_H__

#include "camera.h"

struct CameraPerspective: public TCamera {
  //----- Datas
  TVector3 direction;
  TVector3 sky;
  TVector3 right;
  TVector3 up;

  //----- Constructor
  CameraPerspective();

  //----- Setup
  // use TCamera methods
  
  //----- Methods
  virtual void update();  // compute U, V, N  with location and lookat
  virtual bool computeDirectionVector( const float width, const float height,
				       TVector3 &topLeft, TVector3 &rightStep, TVector3 &upStep ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  //--tag
  static const enum ECameraPerspectiveTags {
  };
  static const TTagList cameraPerspectiveTags [];
};


#endif
