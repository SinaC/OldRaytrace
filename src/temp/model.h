#ifndef __MODEL_H__
#define __MODEL_H__

#include <list>

#include "../math/vector4.h"
#include "../math/matrix4.h"
#include "polygon.h"

#define MODEL_STATE_NOTHING (0)
// Model is outside frustum
#define MODEL_STATE_OUTSIDE (1)
// Model is cut by frustum
#define MODEL_STATE_CUT     (2)

class TModel {
 public:
  TModel();

  float getClipRadius() const { return clipRadius; }

  TMatrix4 getWorldToLocalMatrix() const;
  TMatrix4 getLocalToWorldMatrix() const;
  TMatrix4 getViewMatrix() const;
  TMatrix4 getModelMatrix( TModel *camera ) const;
  TMatrix4 getScaledModelMatrix( TModel *camera ) const;
  
  void                                setPosition    ( const double x, const double y, const double z );
  void                                setPosition    ( const TVector4 &v );
  TVector4                            getPosition    () const;
  void                                transform      ( const TMatrix4 &m );
  void                                translate      ( const TVector4 &v );
  void                                roll           ( const double radians );
  void                                pitch          ( const double radians );
  void                                yaw            ( const double radians );
  void                                heading        ( const double radians );

  void                                computeClippingSphere();

  // protected:
 public:
  const char *name;       // model's name
  TPoint4  position;       // position in world
  TVector4 orientation;    // angle X, Y, Z

  TBitvector state;       // model's state
  float   clipRadius;     // for view volume clipping
  TPoint4  clipCenter;     // center of clipping sphere
  float   clippedDist[6]; // distance from clip center to view frustum
  bool    clippedState[6];// store if model cut view frustum

  std::list<TPolygon> polygonsList; // list of polygons
  std::list<TPolygon> currentPolygonsList; // current list of polygons
};

#endif
