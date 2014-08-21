#ifndef __MOVER_CIRCLE_H__
#define __MOVER_CIRCLE_H__

#include "mover.h"

struct MoverCircle : public MoverVector {
  //---- Datas
  TVector3 source;
  TVector3 axis;
  float speed;

  //---- Constructor
  MoverCircle();

  //---- Methods
  virtual TVector3 vmove(const float time);

  //---- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  virtual void print( const char *msg ) const;

  //**** Private
  private:
  //--tag
  static const enum EMoverCircleTags {
    TAG_moverCircle_source,
    TAG_moverCircle_axis,
    TAG_moverCircle_speed
  };
  static const TTagList moverCircleTags [];
};

#endif
