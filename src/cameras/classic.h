#ifndef __CAMERA_CLASSIC_H__
#define __CAMERA_CLASSIC_H__

#include "camera.h"

struct CameraClassic: public TCamera {
  //----- Datas
  // use TCamera datas

  //----- Constructor
  CameraClassic();

  //----- Setup
  // use TCamera methods
  
  //----- Methods
  // use TCamera methods

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
  static const enum ECameraClassicTags {
  };
  static const TTagList cameraClassicTags [];
};

#endif
