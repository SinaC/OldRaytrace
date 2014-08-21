#include "model.h"
#include "camera3d.h"
#include "polygon.h"

#define __DEBUG__

#ifdef __DEBUG__
#include <stdio.h>
#endif

void render( TCamera3D &camera, TModel &model,
	     u32 *bitmap, const u32 width, const u32 height );
  int i, j, k;
  std::list<TPolygon>::iterator iterP;

#ifdef __DEBUG__
  printf("RENDER FRAME...\n"
	 "----------------\n");

  printf("CLEAR PREVIOUS FRAME INFORMATIONS...\n");
#endif

  model.currentPolygonsList.clear();
  for ( i = 0; i < 6; i++ ) {
    model.clippedDist[i] = -INFINITE;
    model.clippedState[i] = false;
    model.state = MODEL_STATE_NOTHING;
  }
  iterP = model.polygonsList.begin();
  while ( iterP != model.polygonsList.end() ) {
    iterP->state = POLYGON_STATE_NOTHING;
    std::list<TVertex>::iterator iterV = iterP->verticesList.begin();
    while ( iterV != iterP->verticesList.end() ) {
      iterV->state = VERTEX_STATE_NOTHING;
      iterV++;
    }
    iterP++;
  }

#ifdef __DEBUG__
  printf("BASE MODEL...\n");
  i = 0;
  iterP = model.polygonsList.begin();
  while ( iterP != model.polygonsList.end() ) {
    TPolygon *poly = &(*iterP);

    printf("Poly [%d]  %d\n", i, poly->state );
    std::list<TVertex>::iterator iterV = poly->verticesList.begin();
    j = 0;
    while ( iterV != poly->verticesList.end() ) {
      TVertex *vert = &(*iterV);
      printf(" --> Vertex [%d]  %5.2f  %5.2f  %5.2f  %5.2f\n", j,
	     vert->position[0], vert->position[1], vert->position[2], vert->position[3] );
      iterV++;
      j++;
    }
    iterP++;
    i++;
  }
#endif

#ifdef __DEBUG__
  printf("UPDATE CAMERA...\n");
#endif
  // Camera update -- compute camera matrix, frustum, ...
  camera.update();

#ifdef __DEBUG__
  printf("SPHERE CLIPPING...\n");
#endif
  for ( i = 0; i < 6; i++ ) {
    Plane *plane = &(camera.frustum[i]);
    model.clippedDist[i] = plane->distance( model.clipCenter );

    if ( model.clippedDist[i] >= model.clipRadius ) { // outside: stop clipping
      model.state = MODEL_STATE_OUTSIDE;
      break; // return ?
    }
    else if ( model.clippedDist[i] >= -model.clipRadius ) { // cut by the plane
      model.state = MODEL_STATE_CUT;
      model.clippedState[i] = true;
    }
#ifdef __DEBUG__
    // else inside frustum
    printf("Clip [%d]  %5.2f %5.2f %5.2f %5.2f  dist: %5.2f   cut: %d  state: %d\n", i,
	   (*plane)[0], (*plane)[1], (*plane)[2], (*plane)[3],
	   model.clippedDist[i], model.clippedState[i], model.state );
#endif
  }

  // if Sphere outside -- stop with this model
  if ( IS_SET( model.state, MODEL_STATE_OUTSIDE ) ) {
#ifdef __DEBUG__
    printf("MODEL OUTSIDE FRUSTUM...\n");
#endif
    return;
  }

#ifdef __DEBUG__
  printf("BACKFACE CULLING...\n");
#endif
  // Backface culling -- remove polygons not facing camera
  //iterP = model.currentPolygonsList.begin();
  iterP = model.polygonsList.begin();
  Vector4 cameraDirection = camera.lookAt - camera.position; cameraDirection.normalize();
  //while ( iterP != model.currentPolygonsList.end() ) {
  while ( iterP != model.polygonsList.end() ) {
    TPolygon *poly = &(*iterP);
    float dotP = poly->normal | cameraDirection;
    if ( dotP >= 0 )
      poly->state = POLYGON_STATE_HIDDEN;
#ifdef __DEBUG__
    printf("mapping: %d  dot product: %5.2f  dir: %5.2f  %5.2f  %5.2f%s\n",
	   poly->mapping, dotP,
	   cameraDirection[0], cameraDirection[1], cameraDirection[2],
	   IS_SET(poly->state,POLYGON_STATE_HIDDEN)?"  HIDDEN":"  VISIBLE");
#endif
    iterP++;
  }

#ifdef __DEBUG__
  printf("COPY POLYGONS...\n");
#endif
  // copy visible polygons 
  iterP = model.polygonsList.begin();
  while ( iterP != model.polygonsList.end() ) {
    if ( !IS_SET( iterP->state, POLYGON_STATE_HIDDEN ) )
      model.currentPolygonsList.push_back( *iterP );
    iterP++;
  }

#ifdef __DEBUG__
  printf("3D-CLIPPING...\n");
#endif
  // 3D-Clipping -- clip each polygon on nearest clipping plane
  if ( IS_SET( model.state, MODEL_STATE_CUT ) ) {
    for ( i = 0; i < 6; i++ ) // should call on nearest plane first
      // only consider cutting frustum planes
      if ( model.clippedState[i] == true ) {
	Plane *plane = &(camera.frustum[i]);
#ifdef __DEBUG__
	printf("Clipping plane [%d]  %5.2f  %5.2f  %5.2f  %5.2f\n", i,
	       plane->v[0], plane->v[1], plane->v[2], plane->v[3] );
#endif
	std::list<TPolygon>::iterator iterP = model.currentPolygonsList.begin();
	while ( iterP != model.currentPolygonsList.end() ) {
	  if ( !IS_SET( iterP->state, POLYGON_STATE_HIDDEN ) ) {
	    //clipToPlane2( *iterP, *plane );
	    TPolygon poly;
	    clipToPlane( *iterP, *plane, poly );
	    model.currentPolygonsList.push_front( poly ); // add new poly
	    model.currentPolygonsList.erase( iterP++ ); // remove old poly (could just mark it as HIDDEN)
	  }
	  else
	    iterP++;
	}
      }
  }

#ifdef __DEBUG__
  printf("WORLD TO SCREEN...\n");
#endif
  // Compute general matrix
  Matrix m = camera.projectionMatrix * camera.toScreenSpaceMatrix;

#ifdef __DEBUG__
  printf("General Matrix:");
  for ( i = 0; i < 4; i++ ) {
    for ( j = 0; j < 4; j++ )
      printf("  %5.2f", m(j,i) );
    printf("\n");
  }
#endif

  // World to screen
  iterP = model.currentPolygonsList.begin();
  i = 0;
  while ( iterP != model.currentPolygonsList.end() ) {
    TPolygon *poly = &(*iterP);

#ifdef __DEBUG__
    printf("Poly [%d]  state: %d  mapping: %d\n", i, poly->state, poly->mapping );
#endif
    if ( !IS_SET( poly->state, POLYGON_STATE_HIDDEN ) ) {
      std::list<TVertex>::iterator iterV = poly->verticesList.begin();
      j = 0;
      while ( iterV != poly->verticesList.end() ) {
	TVertex *vert = &(*iterV);
#ifdef __DEBUG__
	printf("  Vertex [%d]  %5.2f  %5.2f  %5.2f  %5.2f\n", j,
	       vert->position[0], vert->position[1], vert->position[2], vert->position[3] );
#endif
	vert->position = m * vert->position;
	vert->position.homogenize();
	
	vert->position[0] = (vert->position[0]+1.0)*(width-0.5)*0.5;
	vert->position[1] = (vert->position[1]+1.0)*(height-0.5)*0.5;
	
#ifdef __DEBUG__
	printf(" --> Vertex [%d]  %5.2f  %5.2f  %5.2f  %5.2f\n", j,
	       vert->position[0], vert->position[1], vert->position[2], vert->position[3] );
#endif
	iterV++;
	j++;
      }
    }
    iterP++;
    i++;
  }

#ifdef __DEBUG__
  printf("TESSELATING...\n");
#endif
  // Tesselate
  tesselate( model.currentPolygonsList );
  iterP = model.currentPolygonsList.begin();
  i = 0;
  while ( iterP != model.currentPolygonsList.end() ) {
    TPolygon *current = &(*iterP);

#ifdef __DEBUG__
    printf("Poly [%d]  %d\n", i, current->state );
#endif
    std::list<TVertex>::iterator iterV = current->verticesList.begin();
    j = 0;
    while ( iterV != current->verticesList.end() ) {
      TVertex *vert = &(*iterV);
#ifdef __DEBUG__
      printf(" --> Vertex [%d]  %5.2f  %5.2f  %5.2f  %5.2f\n", j,
	     vert->position[0], vert->position[1], vert->position[2], vert->position[3] );
#endif
      iterV++;
      j++;
    }
    iterP++;
    i++;
  }

#ifdef __DEBUG__
  printf("RENDER MODEL...\n");
#endif
  // Render
  iterP = model.currentPolygonsList.begin();
  while ( iterP != model.currentPolygonsList.end() ) {
    TPolygon *current = &(*iterP);
    if ( !IS_SET( current->state, POLYGON_STATE_HIDDEN ) ) {
      // WIRE
      if ( IS_SET( current->mapping, POLYGON_MAPPING_WIRE ) ) {
	std::list<TVertex>::iterator iterV = current->verticesList.begin();
	
	while ( iterV != current->verticesList.end() ) {
	  std::list<TVertex>::iterator iterNext = iterV; iterNext++;
	  if ( iterNext == current->verticesList.end() )
	    iterNext = current->verticesList.begin();
	  bresen( bitmap, width, height,
		  iterV->position, iterNext->position, TColor::white );
	  iterV++;
	}
      }
      // DOT
      if ( IS_SET( current->mapping, POLYGON_MAPPING_DOT ) ) {
	std::list<TVertex>::iterator iterV = current->verticesList.begin();
	
	u32 col = clampRGB32( TColor::white );
	while ( iterV != current->verticesList.end() ) {
	  bitmap[iterV->position[__X]+iterV->position[__Y]*width] = col;
	  iterV++;
	}
      }
    }
    iterP++;
  }
}
