#ifndef __BCYLINDER_H__
#define __BCYLINDER_H__

#include "../math/vector3.h"

struct BCylinderIntersection {
  int n;      // number of cylinder hit
  float d[2]; // intersection distance(s)
  float w[2]; // intersection parameter(s)
};

struct BCylinderEntry {
  int r1, r2; // index of min/max segment radius
  int h1, h2; // index of min/max segment height
};

struct BCylinder {
  //----- Datas
  int number; // Number of bounding cylinders.
  int nRadius; // Number of different bound-radii.
  int nHeight; // Number of different bound-heights.
  float *radius; // List of different bound-radii.
  float *height; // List of different bound-heights.
  BCylinderIntersection *rInt; // BCyl intersections list.
  BCylinderIntersection *hInt; // BCyl intersections list.
  BCylinderIntersection *intervals; // BCyl intersection intervals.
  BCylinderEntry *entry; // BCyl elements.

  //----- Methods
  static BCylinder *create( const int number,
			    float *tmp_r1, float *tmp_r2,
			    float *tmp_h1, float *tmp_h2 );
  void destroy();
  int intersect( const TPoint3 &P, const TVector3 &D );

  //***** Private
  private:
  void insertHit( BCylinderIntersection &element, int &cnt );
  void intersectBoundElements( const TPoint3 &P, const TVector3 &D );
  int intersectThickCylinder( BCylinderEntry *entry, float *dist );
};

#endif
