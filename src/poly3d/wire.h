#ifndef __POLY3D_WIRE_H__
#define __POLY3D_WIRE_H__

#include "../types/basics.h"
#include "../math/vector4.h"
#include "../color.h"

void bresen( u32 *bitmap, const u32 width, const u32 height,
	     const TVector4 &p1, const TVector4 &p2, const TColor &c );
void bresen( u32 *bitmap, const u32 width, const u32 height,
	     const u32 x1, const u32 y1, const u32 x2, const u32 y2, const u32 color );

#endif
