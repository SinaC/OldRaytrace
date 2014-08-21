#include "clipping.h"

#define __DEBUG__

#ifdef __DEBUG__
#include <stdio.h>
#endif

void clipToPlane( const TPolygon &poly, const TPlane &plane, TPolygon &out ) {
#ifdef __DEBUG__
  printf("CLIP TO PLANE:  %5.2f  %5.2f  %5.2f  %5.2f\n",
	 plane[0], plane[1], plane[2], plane[3] );
#endif

  std::list<TVertex>::const_iterator head = poly.verticesList.begin();
  std::list<TVertex>::const_iterator tail = poly.verticesList.end();
  std::list<TVertex>::const_iterator iterCur = poly.verticesList.begin();
  std::list<TVertex>::const_iterator iterNext = poly.verticesList.begin(); iterNext++;
  float distNext = INFINITE;
  bool inNext = false;
  // compute distance from current to plane
  float distCur = plane.distance( iterCur->position );
  // test if current is in inside
  bool inCur = ( distCur < 0 );

  int nVert= poly.verticesList.size();
  int i = 0;
  
  out.verticesList.clear();
  // Copy polygon information
  out.normal = poly.normal;
  out.color = poly.color;
  out.mapping = poly.mapping;
  out.state = POLYGON_STATE_NOTHING;

  while ( iterCur != tail ) {
#ifdef __DEBUG__
    printf("CURRENT:\n"
	   "Distance to frustum %5.2f   pt: %5.2f %5.2f %5.2f%s\n", distCur,
	   iterCur->position[0], iterCur->position[1], iterCur->position[2],
	   inCur?"  INSIDE":"  OUTSIDE");
#endif

    if ( inCur )
      out.verticesList.push_back( *iterCur ); // insert vertices in new polygon

    // compute distance from next to plane
    distNext = plane.distance( iterNext->position );
    // test if next is in inside
    inNext = ( distNext < 0 );

#ifdef __DEBUG__
    printf("NEXT:\n"
	   "Distance to frustum %5.2f   pt: %5.2f %5.2f %5.2f%s\n", distNext,
	   iterNext->position[0], iterNext->position[1], iterNext->position[2],
	   inNext?"  INSIDE":"  OUTSIDE");
#endif
    
    // add a new vertex if one end of current edge is inside
    //  and other is outside
    if ( inCur != inNext ) {
      float scale = -distCur / ( distNext - distCur );
      // create new vertex
      TVertex newV;
      newV.position = iterCur->position + ( iterNext->position - iterCur->position ) * scale;

      newV.color = iterCur->color + ( iterNext->color - iterCur->color ) * scale;
      newV.normal = iterCur->normal;
      newV.state = VERTEX_STATE_NOTHING;
      // insert in vertices list
      out.verticesList.push_back(newV);
#ifdef __DEBUG__
      printf("NEW VERTEX: %5.2f  %5.2f  %5.2f  %5.2f\n",
	     newV.position[0], newV.position[1], newV.position[2], newV.position[3] );
#endif
    }

    iterCur++;
    distCur = distNext;
    inCur = inNext;
    iterNext++;
    if ( iterNext == tail )
      iterNext = head;
  }
}


void clipToPlane2( TPolygon &poly, const TPlane &plane ) {
#ifdef __DEBUG__
  printf("CLIP TO PLANE:  %5.2f  %5.2f  %5.2f  %5.2f\n",
	 plane[0], plane[1], plane[2], plane[3] );
#endif

  std::list<TVertex>::iterator head = poly.verticesList.begin();
  std::list<TVertex>::iterator tail = poly.verticesList.end();
  std::list<TVertex>::iterator iterCur = poly.verticesList.begin();
  std::list<TVertex>::iterator iterNext = poly.verticesList.begin(); iterNext++;
  float distNext = INFINITE;
  bool inNext = false;
  // compute distance from current to plane
  float distCur = plane.distance( iterCur->position );
  // test if current is in inside
  bool inCur = ( distCur < 0 );

  while ( iterCur != tail ) {
#ifdef __DEBUG__
    printf("CURRENT:\n"
	   "Distance to frustum %5.2f   pt: %5.2f %5.2f %5.2f%s\n", distCur,
	   iterCur->position[0], iterCur->position[1], iterCur->position[2],
	   inCur?"  INSIDE":"  OUTSIDE");
#endif

    // point is outside plane mark it as hidden
    if ( !inCur )
      SET_BIT( iterCur->state, VERTEX_STATE_HIDDEN ); // hidden
    else {
      poly.verticesList.push_front( *iterCur ); // insert vertices in new polygon
      SET_BIT( iterCur->state, VERTEX_STATE_HIDDEN ); // set it as hidden so it will be removed from list
    }

    // compute distance from next to plane
    distNext = plane.distance( iterNext->position );
    // test if next is in inside
    inNext = ( distNext < 0 );

#ifdef __DEBUG__
    printf("NEXT:\n"
	   "Distance to frustum %5.2f   pt: %5.2f %5.2f %5.2f%s\n", distNext,
	   iterNext->position[0], iterNext->position[1], iterNext->position[2],
	   inNext?"  INSIDE":"  OUTSIDE");
#endif
    
    // add a new vertex if one end of current edge is inside
    //  and other is outside
    if ( inCur != inNext ) {
      float scale = -distCur / ( distNext - distCur );
      // create new vertex
      TVertex newV;
      newV.position = iterCur->position + ( iterNext->position - iterCur->position ) * scale;

      newV.color = iterCur->color + ( iterNext->color - iterCur->color ) * scale;
      newV.normal = iterCur->normal;
      newV.state = VERTEX_STATE_NOTHING;
      // insert in vertices list
      poly.verticesList.push_front(newV);
#ifdef __DEBUG__
      printf("NEW VERTEX: %5.2f  %5.2f  %5.2f  %5.2f\n",
	     newV.position[0], newV.position[1], newV.position[2], newV.position[3] );
#endif
    }

    iterCur++;
    distCur = distNext;
    inCur = inNext;
    iterNext++;
    if ( iterNext == tail )
      iterNext = head;
  }
  
  std::list<TVertex>::iterator iterV = poly.verticesList.begin();
  while ( iterV != poly.verticesList.end() ) {
    if ( IS_SET( iterV->state, VERTEX_STATE_HIDDEN ) ) // vertex hidden: remove
      poly.verticesList.erase(iterV++);
    else
      iterV++;
  }
}
