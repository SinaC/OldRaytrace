#ifndef __COLORMAP_H__
#define __COLORMAP_H__

#include "color.h"
#include "handle.h"

struct TColorMap: public THandle {
  //----- Datas
  struct TColormapItem {
    float startValue;
    TColor color;

    float oneOverDiffPrevious; // computed in setup
  };

  TColormapItem *map;
  int nbItems;

  //----- Constructor
  TColorMap();

  //----- Setup
  virtual void setup();

  //----- Methods
  TColor getColor( const float value ) const;

  //---- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  //--tags
  static const enum EColormapTags {
    TAG_colormap_map//,
    //    TAG_colormap_function
  };
  static const TTagList colormapTags [];
};

#endif
