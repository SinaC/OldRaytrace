#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "../color.h"
#include "../math/vector4.h"
#include "../types/bitvector.h"

#define VERTEX_STATE_NOTHING (0)
#define VERTEX_STATE_HIDDEN  (1)

struct TVertex {
  TPoint4 position;  // position in world, screen, ...
  TVector4 normal;   // normal to vertex
  TColor color;     // vertex color
  TBitvector state; // vertex's state

  TVertex() {
    position = TPoint4( 0, 0, 0, 0 );
    normal = TVector4( 0, 0, 0, 0 );
    color = TColor::black;
    state = VERTEX_STATE_NOTHING;
  }
  void operator =( const TVertex &v  ) {
    position = v.position;
    normal = v.normal;
    color = v.color;
    state = v.state;
  }
};

#endif
