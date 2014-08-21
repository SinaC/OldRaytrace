#ifndef __FONT_H__
#define __FONT_H__

#include "../types/basics.h"

bool readFont( const char *filename );
void displayString( const u32 x, const u32 y,
		    u32 cc, const u32 cf, const bool transparent,
		    u32 *vs, const u32 width, const u32 height,
		    const char *s, ... );

#endif
