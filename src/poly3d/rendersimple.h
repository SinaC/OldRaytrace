#ifndef __POLY3D_RENDERSIMPLE_H__
#define __POLY3D_RENDERSIMPLE_H__

#include "camerasimple.h"
#include "../types/basics.h"
#include "../math/vector3.h"

void renderLine( u32 *screen, const u32 width, const u32 height,
		 const TPoint3 &from, const TPoint3 &to, const u32 color,
		 const TCameraSimple &camera );

void renderBox( u32 *screen, const u32 width, const u32 height,
		const TPoint3 &min, const TPoint3 &max, const u32 color,
		const TCameraSimple &camera );

#endif
