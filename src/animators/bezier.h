#ifndef __MOVER_BEZIER_H__
#define __MOVER_BEZIER_H__

#include "mover.h"

struct MoverBezier: public MoverVector {
  //---- Datas
  struct TSegment {
    TPoint3 point;
    TVector3 vector;
    float time;

    TPoint3 point1; // point + vector: compute in setup
  };
  TSegment *segments;
  int numSegments;
  float totalTime;

  //---- Constructor
  MoverBezier();

  //---- Setup
  // setup called once in world initialization
  virtual void setup();

  //---- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //---- Methods
  virtual TVector3 vmove( const float time );

  //---- Output
  virtual void print( const char *msg ) const;

  //**** Private
  private:
  //--tag
  static const enum EMoverBezierTags {
    TAG_moverBezier_segments
  };
  static const TTagList moverBezierTags [];
};

#endif
