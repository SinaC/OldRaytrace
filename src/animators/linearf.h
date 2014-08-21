#ifndef __MOVER_LINEARF_H__
#define __MOVER_LINEARF_H__

#include "mover.h"

struct MoverLinearF : public MoverFloat {
  //---- Datas
  float zero;
  float fact;
  
  //---- Constructor
  MoverLinearF();

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
  static const enum EMoverLinearFTags {
    TAG_moverLinearF_zero,
    TAG_moverLinearF_factor
  };
  static const TTagList moverLinearFTags [];
};

#endif
