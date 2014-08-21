#include <windows.h>
#include <stdio.h>

#include "tessel.h"

#define __DEBUG__

#ifdef __DEBUG__
#include <stdio.h>
#endif

// Tesselate a model (split polygons into triangles)
void tesselate( std::list<TPolygon> &polygonsList ) {
#ifdef __DEBUG__
  printf("model: #polygons: %d\n", polygonsList.size() );
#endif

  std::list<TPolygon>::iterator iterP = polygonsList.begin();
  while ( iterP != polygonsList.end() ) {
    TPolygon *poly = &(*iterP);

    bool removed = false;

    if ( !IS_SET( poly->state, POLYGON_STATE_HIDDEN ) ) { // polygon visible
#ifdef __DEBUG__
      printf("polygon: #vertices: %d\n", poly->verticesList.size() );

      printf("deleting hidden vertices...");
#endif
      // First: remove every hidden vertices
      std::list<TVertex>::iterator iterV = poly->verticesList.begin();
      while ( iterV != poly->verticesList.end() ) {
	if ( IS_SET( iterV->state, VERTEX_STATE_HIDDEN ) ) // vertex hidden: remove
	  poly->verticesList.erase(iterV++);
	else
	  iterV++;
      }
#ifdef __DEBUG__
      printf(" %d vertices left\n", poly->verticesList.size() );
#endif
      // Second: tesselate into triangles
      if ( poly->verticesList.size() > 3 ) { // if more than 3 vertices
	// each triangles will start with first vertex
	std::list<TVertex>::iterator firstV = poly->verticesList.begin();
	std::list<TVertex>::iterator secondV = firstV; secondV++;
	std::list<TVertex>::iterator thirdV = secondV; thirdV++;
	while ( thirdV != poly->verticesList.end() ) {
	  // create a new polygon
	  TPolygon newP;
	  newP.verticesList.clear();
	  newP.verticesList.push_front( *firstV );
	  newP.verticesList.push_front( *secondV );
	  newP.verticesList.push_front( *thirdV );
	  // copy information from old polygon
	  newP.mapping = poly->mapping;
	  newP.state = poly->state;
	  newP.texture = poly->texture;
	  newP.color = poly->color;
	  newP.normal = poly->normal;
	  // put polygon in model, insert in head so we don't try to tesselate it
	  polygonsList.push_front(newP);
	  // next vertices
	  secondV++;
	  thirdV++;
	}
	// remove this polygon from list
	polygonsList.erase(iterP++);
	removed = true;
      }
    }
    if ( !removed )
      iterP++;
  }
#ifdef __DEBUG__
  printf("After tesselating: %d polygons\n", polygonsList.size() );
#endif
}
