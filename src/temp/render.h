#ifndef __RENDER_H__
#define __RENDER_H__

#include "model.h"
#include "camera3d.h"

void render( TCamera3D &camera, TModel &model,
	     u32 *bitmap, const u32 width, const u32 height );

#endif
