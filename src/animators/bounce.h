#ifndef __MOVER_BOUNCE_H__
#define __MOVER_BOUNCE_H__

#include "mover.h"

struct MoverBounce : public MoverFloat {
  //---- Datas
  private:
  float time0;  
  public:
  float gravity, speed, height;
  
  //---- Constructor
  MoverBounce();

  //---- Methods
  virtual float fmove(const float time);

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
  static const enum EMoverBounceTags {
    TAG_moverBounce_gravity,
    TAG_moverBounce_speed,
    TAG_moverBounce_height
  };
  static const TTagList moverBounceTags [];
};

#endif
