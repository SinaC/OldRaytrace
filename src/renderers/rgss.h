#ifndef __RENDERER_RGSS_H__
#define __RENDERER_RGSS_H__

#include "renderer.h"

struct RendererRGSS: public TRenderer {
  //---- Methods
  bool render( const TWorld &world );
};

#endif
