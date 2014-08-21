#ifndef __RENDERER_CORNER_H__
#define __RENDERER_CORNER_H__

#include "renderer.h"

struct RendererCorner: public TRenderer {
  //---- Methods
  bool render( const TWorld &world );
};

#endif
