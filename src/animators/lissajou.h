#ifndef __MOVER_LISSAJOU_H__
#define __MOVER_LISSAJOU_H__

#include "mover.h"

struct MoverLissajou : public MoverVector {
  //---- Datas
  TVector3 pulse;
  TVector3 phase;
  TVector3 ampl;  
  TVector3 shift;

  //---- Constructor
  MoverLissajou();

  //---- Methods
  virtual TVector3 vmove(const float time);

  //---- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Output
  virtual void print( const char *msg ) const;

  //---- Private
  private:
  //--tag
  static const enum EMoverLissajouTags {
    TAG_moverLissajou_pulse,
    TAG_moverLissajou_phase,
    TAG_moverLissajou_amplitude,
    TAG_moverLissajou_shift
  };
  static const TTagList moverLissajouTags [];
};

#endif
