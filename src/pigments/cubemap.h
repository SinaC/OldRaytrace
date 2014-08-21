#ifndef __PIGMENT_CUBEMAP_H__
#define __PIGMENT_CUBEMAP_H__

#include "pigment.h"

struct TBitmapColor;

struct PigmentCubemap: public TPigment {
  //----- Datas
  // 0   0(right)   1(left)
  // 1   2(up)      3(down)
  // 2   4(front)   5(back)
  static const u8 left;
  static const u8 right;
  static const u8 up;
  static const u8 down;
  static const u8 front;
  static const u8 back;
  TBitmapColor *maps[6];

  //----- Constructor
  PigmentCubemap();
  //----- Destructor
  ~PigmentCubemap();

  //----- Setup
  virtual void setup();

  //----- Methods
  virtual TColor getColor( const TPoint3 &p ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  //--tag
  static const enum EPigmentCubemapTags {
    TAG_pigmentCubemap_maps,
  };
  static const TTagList pigmentCubemapTags [];
};

#endif
