#ifndef __RENDERER_GRID_H__
#define __RENDERER_GRID_H__

#include "renderer.h"

struct RendererGrid: public TRenderer {
  //---- Datas
  u32 n;

  //---- Constructor
  RendererGrid();
  //---- Destructor
  ~RendererGrid();

  //---- Methods
  void setup( u32 *data, const u32 width, const u32 height, const u32 n );
  bool render( const TWorld &world );

  //**** Private
  private:
  TVector3 *yScanline;
  float weight;
};

#endif
