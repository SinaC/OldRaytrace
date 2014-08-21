#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "../color.h"
#include "../math/vector3.h"
#include "../handle.h"

struct TWorld;
struct TRay;
struct TLighting;
struct TIntersection;
struct TObject;
struct TPrimitive;

struct TLight: public THandle {
  //----- Datas
  u32 lightKind;
  static const enum ELightKind {
    LIGHT_NOTHING,
    LIGHT_DOT,
    LIGHT_SQUARE//--,
    //--    LIGHT_SPHERICAL
  };
  TColor color;
  float brightness;
  TPrimitive *volume; // light ray bounding volume

  //----- Constructor
  TLight();
  //----- Destructor
  ~TLight();

  //----- Setup
  virtual void setup();
  virtual void precalcFrame( const TWorld &world );

  //----- Methods
  // compute total lighting coming from this to a point
  virtual bool computeLighting( const TWorld &world, const TRay &ray,
				TLighting &lighting );
  // check if a point is in light volume
  virtual bool isLighten( const TPoint3 &point ) const;

  // compute light ray and amount of light intersection receive from this light
  //  but don't compute any shading
  virtual bool getLight( const TWorld &world, const TRay &ray, TRay &lightRay );
  // compute lighting using ray.shadowRay
  virtual TColor getColor( const TWorld &world, const TRay &ray, const TRay &lightRay );

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find subclass type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  void print( const char *msg ) const;

  //****** Private
  protected:
  const TObject *shadowCache;

  private:
  char *lightName() const;
  //--tag
  static const enum ELightTags {
    TAG_light_color,
    TAG_light_brightness,
    //    TAG_light_powerOn,
    //    TAG_light_looksLike,
    TAG_light_volume
  };
  static const TTagList lightTags [];
  static const TTagList lightSubClassTags [];
};

#endif
