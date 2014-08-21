#ifndef __RENDERER_DIAMOND_H__
#define __RENDERER_DIAMOND_H__

#include "renderer.h"

struct RendererDiamond: public TRenderer {
  //---- Methods
  bool render( const TWorld &world );
};

#endif
