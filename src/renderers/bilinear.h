#ifndef __RENDERER_BILINEAR_H__
#define __RENDERER_BILINEAR_H__

#include "renderer.h"

struct RendererBilinear: public TRenderer {
  //---- Constructor
  RendererBilinear();
  //---- Destructor
  ~RendererBilinear();

  //---- Methods
  void setup( u32 *dest, const u32 width, const u32 height );
  bool render( const TWorld &world );

  //**** Private
  protected:
  u32 resolutionWidth;
  u32 resolutionHeight;
  u32 *buffer;
};

#endif
