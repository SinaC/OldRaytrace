#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <list>

#include "vertex.h"
#include "../color.h"
#include "../types/bitvector.h"

#define POLYGON_MAPPING_NOTHING (0)
#define POLYGON_MAPPING_FLAT    (1)
#define POLYGON_MAPPING_GOURAUD (2)
#define POLYGON_MAPPING_TEXTURE (4)
#define POLYGON_MAPPING_CORRECT (8)
#define POLYGON_MAPPING_TRANSPARENT (16)
#define POLYGON_MAPPING_WIRE    (32)
#define POLYGON_MAPPING_DOT     (64)

#define POLYGON_STATE_NOTHING (0)
#define POLYGON_STATE_HIDDEN  (1)
#define POLYGON_STATE_CUT     (2)

struct TPolygon {
  std::list<TVertex> verticesList; // list of vertices

  TVector4  normal;       // polygon's normal

  TColor   color;        // polygon's color

  TBitvector mapping;    // kind of mapping
  TBitvector state;      // polygon state

  void computeNormal() {  // compute polygon's normal from vertices
    std::list<TVertex>::iterator i = verticesList.begin();
    TVertex *vert0 = &(*i); i++;
    TVertex *vert1 = &(*i); i++;
    TVertex *vert2 = &(*i);

    TVector4 edge1 = vert0->position - vert1->position;
    TVector4 edge2 = vert2->position - vert1->position;

    normal = -crossProduct( edge1, edge2 );
    normal.normalize();
  }

  TPolygon() {
    normal = TVector4( 0, 0, 0, 0 );
    color = TColor::black;
    mapping = POLYGON_MAPPING_NOTHING;
    state = POLYGON_STATE_NOTHING;
  }
};

#endif
