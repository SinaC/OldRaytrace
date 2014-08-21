#ifndef __CLIPPING_H__
#define __CLIPPING_H__

#include "model.h"
#include "plane.h"
#include "polygon.h"

//void clipToPlane( TPolygon &poly, const Plane &plane );
void clipToPlane( const TPolygon &poly, const TPlane &plane, TPolygon &out );
void clipToPlane2( TPolygon &poly, const TPlane &plane );

#endif
