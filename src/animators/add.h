#ifndef __MOVER_ADD_H__
#define __MOVER_ADD_H__

#include <vector>
using namespace std;

#include "mover.h"

struct MoverAddV : public MoverVector {
  //---- Datas
  //  TVector3* source1;
  //  TVector3* source2;
  vector<TVector3*> source;
  
  //---- Constructor
  MoverAddV();

  //---- Methods
  virtual TVector3 vmove( const float time);

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
  static const enum EMoverAddTags {
    //    TAG_moverAdd_source1,
    //    TAG_moverAdd_source2
    TAG_moverAdd_source
  };
  static const TTagList moverAddTags [];
};

#endif
