#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
using namespace std;

#include "color.h"
#include "math/vector3.h"
#include "types/basics.h"
#include "handle.h"

struct TCamera;
struct TObject;
struct KDTree;
struct UniformGrid;
struct TIntersection;
struct TRay;
struct TLight;
struct TPigment;
struct TAmbient;
struct TAtmospheric;
struct TAnimator;

struct TWorld: public THandle {
  //----- Datas
  //-- partitions
  KDTree *kdtree; // kd-tree
  UniformGrid *ugrid; // uniform grid

  vector<TObject *> objects; // Objects
  vector<TCamera *> cameras; // Cameras
  vector<TLight *> lights; // Lights
  TCamera *currentCamera; // Current camera
  TAmbient *ambient; // Ambient light
  TPigment *background; // Background
  TAtmospheric *atmospheric; // Atmospheric
  float atmosphereIOR; // Index of Refraction of atmosphere
  TAnimator *animator; // Animator

  //----- Constructor
  TWorld();
  //----- Destructor
  ~TWorld();

  //----- Setup
  void setup();
  void precalcFrame();

  //----- Methods
  //-- intersection methods
  // find the object the closest to ray.origin
  int findClosestIntersection( const TRay &ray,
			       TIntersection &intersection ) const;

  // find first object intersecting ray
  bool findFirstIntersection( const TRay &ray,
  			      const TIntersection &intersection,
  			      const float maxDist2,
  			      const TObject *&shadowCache ) const;
  // render a ray, called from a TRenderer
  TColor renderRay( const TPoint3 &a_Origin, const TVector3 &a_Direction ) const;

  //-- ray tracing
  // trace a ray and return color
  TColor traceRay( TRay &rayShading, const float attenuation ) const;
  // trace a ray but don't compute shading
  int traceRayNoShading( TRay &rayNoShading, const float attenuation ) const;
  // shade a ray after it has been traced by traceRayNoShading
  TColor shadeRay( const TRay &ray ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  //-- dump information to the standard output
  virtual void print( const char *msg ) const;

  //***** Private *****
  private:
  // method called in findClosestIntersection if ray.kind == RAY_CAMERA
  int findClosestIntersectionCamera( const TRay &ray,
				     TIntersection &intersection ) const;
  
  // methods called in traceRay
  TColor computeDiffuse( const TRay &ray ) const;
  TColor computeReflected( TRay &ray, const float attenuation ) const;
  TColor computeTransmitted( TRay &ray, const float attenuation ) const;
  // methods called in traceRayNoShading
  int computeDiffuseNoShading( TRay &ray ) const;
  int computeReflectedNoShading( TRay &ray, const float attenuation ) const;
  int computeTransmittedNoShading( TRay &ray, const float attenuation ) const;

  //--tags
  static const enum EWorldTags {
    TAG_world_objects,
    TAG_world_cameras,
    TAG_world_lights,
    TAG_world_currentCamera,
    TAG_world_ambient,
    TAG_world_background,
    TAG_world_atmospheric,
    TAG_world_atmosphereIOR,
    TAG_world_animator
  };
  static const TTagList worldTags [];
};

#endif
