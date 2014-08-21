#ifndef __LIGHT_SQUARE_H__
#define __LIGHT_SQUARE_H__

#include "dot.h"

// Square light
//  location is the center of a rectangle defined by 2 perpendicular vectors: axis1 and axis2
//  -----------
// | . | . | . |     rectangle with numDots = 5, 3
// |-----------|     .in fact we consider this square as 15 dot light (lighten only in dir half-space)
// | . | . | . |
// |-----------|
// | . | . | . |
//  -----------
// | . | . | . |
//  -----------
// | . | . | . |
//  -----------
struct LightSquare: public LightDot {
  // Datas
  TVector3 dir; // direction of each dot lights
  TVector3 axis1, axis2; // 2 perpendicular vector
  int numDots1, numDots2; // number of dots in both axis
  bool jitter; // jitter dot light in grid


  //----- Constructor
  LightSquare();
  //----- Destructor
  ~LightSquare();

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
  // compute amount of shadow  0: full shadow  1: no shadow   ]0,1[: soft shadow
  float getAmountShadow( const TWorld &world, const TRay &ray );

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  void print( const char *msg ) const;

  //***** Private
  private:
  //-- Optimizing datas
  int numDots; // numDots1 * numDots2
  float oneOverNumDots; // 1/numDots
  float dotSize1, dotSize2; // size of dot in both axis
  TPoint3 *dotGrid; // grid with numDots² dot, compute with dir, axis1, axis2 and numDots
  //--tag
  static const enum ELightSquareTags {
    //--    TAG_lightSquare_location,
    TAG_lightSquare_dir,
    TAG_lightSquare_axis,
    TAG_lightSquare_numdots,
    TAG_lightSquare_jitter
  };
  static const TTagList lightSquareTags [];
};

#endif
