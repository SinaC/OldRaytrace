#include "rendersimple.h"
#include "wire.h"

void renderLine( u32 *screen, const u32 width, const u32 height,
		 const TPoint3 &from, const TPoint3 &to, const u32 color,
		 const TCameraSimple &camera ) {
  float x1, y1, x2, y2;
  camera.convert3Dto2D( from, x1, y1 );
  camera.convert3Dto2D( to, x2, y2 );
  
  bresen( screen, width, height,
	  width/2+(int)x1, height/2+(int)y1,
	  width/2+(int)x2, height/2+(int)y2, color );
}

void renderBox( u32 *screen, const u32 width, const u32 height,
		const TPoint3 &min, const TPoint3 &max, const u32 color,
		const TCameraSimple &camera ) {
  renderLine( screen, width, height,
	      TPoint3( min[__X], min[__Y], min[__Z] ), TPoint3( max[__X], min[__Y], min[__Z] ), color,
	      camera );
  renderLine( screen, width, height,
	      TPoint3( min[__X], max[__Y], min[__Z] ), TPoint3( max[__X], max[__Y], min[__Z] ), color,
	      camera );
  renderLine( screen, width, height,
   	      TPoint3( min[__X], min[__Y], max[__Z] ), TPoint3( max[__X], min[__Y], max[__Z] ), color,
   	      camera );
  renderLine( screen, width, height,
   	      TPoint3( min[__X], max[__Y], max[__Z] ), TPoint3( max[__X], max[__Y], max[__Z] ), color,
   	      camera );
  
  renderLine( screen, width, height,
 	      TPoint3( min[__X], min[__Y], min[__Z] ), TPoint3( min[__X], max[__Y], min[__Z] ), color,
	      camera );
  renderLine( screen, width, height,
 	      TPoint3( max[__X], min[__Y], min[__Z] ), TPoint3( max[__X], max[__Y], min[__Z] ), color,
	      camera );
  renderLine( screen, width, height,
  	      TPoint3( min[__X], min[__Y], max[__Z] ), TPoint3( min[__X], max[__Y], max[__Z] ), color,
 	      camera );
  renderLine( screen, width, height,
  	      TPoint3( max[__X], min[__Y], max[__Z] ), TPoint3( max[__X], max[__Y], max[__Z] ), color,
 	      camera );
  
  renderLine( screen, width, height,
  	      TPoint3( min[__X], min[__Y], min[__Z] ), TPoint3( min[__X], min[__Y], max[__Z] ), color,
 	      camera );
  renderLine( screen, width, height,
  	      TPoint3( max[__X], min[__Y], min[__Z] ), TPoint3( max[__X], min[__Y], max[__Z] ), color,
 	      camera );
  renderLine( screen, width, height,
  	      TPoint3( min[__X], max[__Y], min[__Z] ), TPoint3( min[__X], max[__Y], max[__Z] ), color,
 	      camera );
  renderLine( screen, width, height,
 	      TPoint3( max[__X], max[__Y], min[__Z] ), TPoint3( max[__X], max[__Y], max[__Z] ), color,
 	      camera );
}
