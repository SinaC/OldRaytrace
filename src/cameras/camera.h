#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../math\vector3.h"
#include "../handle.h"

struct TWorld;

struct TCamera: public THandle {
  //----- Datas
  u32 cameraKind;
  static const enum ECameraKind {
    CAMERA_NOTHING,
    CAMERA_CLASSIC,
    CAMERA_PERSPECTIVE
  };
  TVector3 vertical;            // vertical vector used by lookAt
  TVector3 lookat;              // camera's lookat
  TPoint3 location;             // camera location
  float fov;                    // field of view for x axis (angle in rad)

  //----- Constructor
  TCamera();
  //----- Destructor
  ~TCamera();

  //----- Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &world );

  //----- Methods
  virtual void update();  // compute U, V, N  with location and lookat
  virtual bool computeDirectionVector( const float width, const float height,
				       TVector3 &topLeft, TVector3 &rightStep, TVector3 &upStep ) const;
  inline const TVector3& getU() const { return U; };
  inline const TVector3& getV() const { return V; };
  inline const TVector3& getN() const { return N; };

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find subclass type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Protected
  protected:
  //-- Datas
  TVector3 U, V, N;             // camera space axis

  //***** Private
  private:
  //-- Methods
  char *cameraName() const;
  //--tag
  static const enum ECameraTags {
    TAG_camera_location,
    TAG_camera_lookat,
    TAG_camera_vertical,
    TAG_camera_FOV
  };
  static const TTagList cameraTags [];
  static const TTagList cameraSubClassTags [];
};

#endif
