#ifndef __RENDERER_INTERPOLATED_H__
#define __RENDERER_INTERPOLATED_H__

#include "renderer.h"

struct RendererInterpolated: public TRenderer {
  //---- Datas
  u32 step;

  //---- Constructor
  RendererInterpolated();
  //---- Destructor
  ~RendererInterpolated();

  //---- Methods
  void setup( u32 *data, const u32 width, const u32 height, const u32 step );
  bool render( const TWorld &world );

  //***** Private
  protected:
  struct TVertex {
    u32 id;
    u32 color;
  };
  struct TBlock {
    TVertex *vertex[4];
  };
  TVertex *vertex;
  TBlock *block;
  u32 xBlocks, yBlocks;
};

#endif
