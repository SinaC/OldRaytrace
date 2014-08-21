#include "types/basics.h"

// Fast (?) blending courtesy of Michael Herf (www.stereopsis.com)
// slightly modified
// equivalent to  a + alpha * (b-a)/a
inline u32 blend8( const u32 a, const u32 b, u32 alpha ) {
  u32 dstrb = a & 0xff00ff;
  u32 dstag = a >> 8 & 0xff00ff;
  
  u32 srcrb = b & 0xff00ff;
  u32 srcag = b >> 8 & 0xff00ff;
  
  u32 drb = srcrb - dstrb;
  u32 dag = srcag - dstag;
  
  drb *= alpha; dag *= alpha;
  drb >>= 3; dag >>= 3;
  
  const u32 rb = (drb + dstrb) & 0x00ff00ff;
  const u32 ag = (dag + dstag) << 8 & 0xff00ff00;
  
  return rb | ag;
}

// Fast (?) blending courtesy of Michael Herf (www.stereopsis.com)
// slightly modified
// equivalent to  a + alpha * (b-a)/a
inline u32 blend4( const u32 a, const u32 b, u32 alpha ) {
  u32 dstrb = a & 0xff00ff;
  u32 dstag = a >> 8 & 0xff00ff;
  
  u32 srcrb = b & 0xff00ff;
  u32 srcag = b >> 8 & 0xff00ff;
  
  u32 drb = srcrb - dstrb;
  u32 dag = srcag - dstag;
  
  drb *= alpha; dag *= alpha;
  drb >>= 2; dag >>= 2;
  
  const u32 rb = (drb + dstrb) & 0x00ff00ff;
  const u32 ag = (dag + dstag) << 8 & 0xff00ff00;
  
  return rb | ag;
}
