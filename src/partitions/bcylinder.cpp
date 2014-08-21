#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bcylinder.h"
#include "../error.h"

// Find all intersections of the current ray with the bounding
// cylinder of the given segment. The intersection tests are
// done in intersect_bound_elements() and the results stored
// in the lathe structure are evaluated here.
int BCylinder::intersectThickCylinder( BCylinderEntry *entry, float *dist ) {
  int n = 0;
  // Intersect ray with the cap-plane.
  if ( hInt[entry->h2].n ) {
    float r = hInt[entry->h2].w[0];
    if ( r >= radius[entry->r1] &&
	 r <= radius[entry->r2] )
      dist[n++] = hInt[entry->h2].d[0];
  }
  // Intersect ray with the base-plane.
  if ( hInt[entry->h1].n ) {
    float r = hInt[entry->h1].w[0];
    if ( r >= radius[entry->r1] &&
	 r <= radius[entry->r2] )
      dist[n++] = hInt[entry->h1].d[0];
  }
  // Intersect with inner cylinder.
  if ( rInt[entry->r1].n ) {
    float h = rInt[entry->r1].w[0];
    if ( h >= height[entry->h1] &&
	 h <= height[entry->h2] )
      dist[n++] = rInt[entry->r1].d[0];
    h = rInt[entry->r1].w[1];
    if ( h >= height[entry->h1] &&
	 h <= height[entry->h2] )
      dist[n++] = rInt[entry->r1].d[1];
  }
  // Intersect with outer cylinder.
  if ( rInt[entry->r2].n ) {
    float h = rInt[entry->r2].w[0];
    if ( h >= height[entry->h1] &&
	 h <= height[entry->h2] )
      dist[n++] = rInt[entry->r2].d[0];
    h = rInt[entry->r2].w[1];
    if ( h >= height[entry->h1] &&
	 h <= height[entry->h2] )
      dist[n++] = rInt[entry->r2].d[1];
  }

  // Sort intersections.
  for ( int i = 0; i < n; i++ )
    for ( int j = 0; j < n - i - 1; j++ )
      if ( dist[j] > dist[j+1] ) {
        float k = dist[j];
        dist[j] = dist[j+1];
        dist[j+1] = k;
      }
  return n;
}

// Intersect all bounding discs and cylinders and store
//  the intersections found in the lathe structure.
// By intersecting all different discs and cylinders once
//  we avoid testing the same cylinders and discs more than
//  once. This happened when we tested one bounding cylinder
//  after the other.
void BCylinder::intersectBoundElements( const TPoint3 &P, const TVector3 &D ) {
  int i;

  // Init constants.
  float a = D[__X] * D[__X] + D[__Z] * D[__Z];
  float b = P[__X] * D[__X] + P[__Z] * D[__Z];

  float bb = b * b;
  float b2 = 2.0f * b;
  float c = P[__X] * P[__X] + P[__Z] * P[__Z];

  // Intersect all rings.
  if ( D[__Y] < -EPS
       || D[__Y] > EPS )
    for ( i = 0; i < nHeight; i++ ) {
      hInt[i].n = 0;
      float k = ( height[i] - P[__Y] ) / D[__Y];
      hInt[i].n = 1;
      hInt[i].d[0] = k;
      hInt[i].w[0] = k * ( a * k + b2 ) + c;
    }
  else
    for ( i = 0; i < nHeight; i++ )
      hInt[i].n = 0;

  // Intersect all cylinders.
  for ( i = 0; i < nRadius; i++ ) {
    rInt[i].n = 0;
    if ( radius[i] > EPS ) {
      float d = bb - a * ( c - radius[i] );
      if ( d > 0.0f ) {
        d = sqrtf(d);
        float k = ( -b + d ) / a;
        rInt[i].n = 2;
        rInt[i].d[0] = k;
        rInt[i].w[0] = P[__Y] + k * D[__Y];
        k = ( -b - d ) / a;
        rInt[i].d[1] = k;
        rInt[i].w[1] = P[__Y] + k * D[__Y];
      }
    }
  }
}

// Insert an intersection into the depth sorted intersection list.
void BCylinder::insertHit( BCylinderIntersection &element, int &cnt ) {
  int k;
  intervals[cnt] = element;
  for ( k = 0; element.d[0] > intervals[k].d[0]; k++ );

  if ( k < cnt ) {
    memmove(&intervals[k+1], &intervals[k], (cnt-k)*sizeof(BCylinderIntersection));
    intervals[k] = element;
  }

  cnt++;
}

// Intersect given ray with all bounding cylinders of the given lathe
//  and return a sorted list of intersection depths and segments hit.
int BCylinder::intersect( const TPoint3 &P, const TVector3 &D ) {
  BCylinderIntersection Inter;

  int cnt = 0;
  // Intersect all cylinder and plane elements.
  intersectBoundElements( P, D );
  // Intersect all spline segments.
  for ( int i = 0; i < number; i++ ) {
    BCylinderEntry *Entry = &(entry[i]);
    float dist[4];
    switch ( intersectThickCylinder( Entry, dist ) ) {
    case 1:
    case 3:
      g_error("BCylinder::intersect: Illegal number of intersections.");
      break;
    case 2:
      if ( dist[0] > EPS ) {
	Inter.d[0] = dist[0];
	Inter.n    = i;
	insertHit( Inter, cnt );
      }
      else if ( dist[1] > EPS ) {
	Inter.d[0] = 0.0;
	Inter.n    = i;
	insertHit( Inter, cnt );
      }
      break;
    case 4:
      if ( dist[0] > EPS ) {
	Inter.d[0] = dist[0];
	Inter.n    = i;
	insertHit( Inter, cnt );
      }
      else if ( dist[1] > EPS ) {
	Inter.d[0] = 0.0f;
	Inter.n    = i;
	insertHit( Inter, cnt );
      }
      else if ( dist[2] > EPS ) {
	Inter.d[0] = dist[2];
	Inter.n    = i;
	insertHit( Inter, cnt );
      }
      else if ( dist[3] > EPS ) {
	Inter.d[0] = 0.0f;
	Inter.n    = i;
	insertHit( Inter, cnt );
      }
      break;
    }
  }

  return cnt;
}

// Create a bounding cylinder data structure from the given
//  radii and heights.
BCylinder *BCylinder::create( const int number,
			      float *tmp_r1, float *tmp_r2,
			      float *tmp_h1, float *tmp_h2 ) {
  int i, j;

  // Allocate bounding cylinder.
  BCylinder *bcyl = new BCylinder;
  // Allocate entries.
  bcyl->number = number;
  bcyl->entry = new BCylinderEntry [number];
  // Allocate intersection lists.
  bcyl->hInt = new BCylinderIntersection [2*number];
  bcyl->rInt = new BCylinderIntersection [2*number];
  bcyl->intervals = new BCylinderIntersection [4*number];
  // Allocate temporary lists.
  int *tmp_r1_index = new int [number];
  int *tmp_r2_index = new int [number];
  int *tmp_h1_index = new int [number];
  int *tmp_h2_index = new int [number];
  float *tmp_radius = new float [2*number];
  float *tmp_height = new float [2*number];
  // Get different bounding radii and heights.
  int nr = 0;
  int nh = 0;
  for ( i = 0; i < number; i++ ) {
    tmp_r1_index[i] = -1;
    tmp_r2_index[i] = -1;
    tmp_h1_index[i] = -1;
    tmp_h2_index[i] = -1;

    for ( j = 0; j < nr; j++ )
      if ( tmp_r1[i] == tmp_radius[j] )
        break;
    if ( j == nr )
      tmp_radius[nr++] = tmp_r1[i];
    tmp_r1_index[i] = j;

    for ( j = 0; j < nr; j++ )
      if ( tmp_r2[i] == tmp_radius[j] )
        break;
    if ( j == nr )
      tmp_radius[nr++] = tmp_r2[i];
    tmp_r2_index[i] = j;

    for ( j = 0; j < nh; j++ )
      if ( tmp_h1[i] == tmp_height[j] )
        break;
    if ( j == nh )
      tmp_height[nh++] = tmp_h1[i];
    tmp_h1_index[i] = j;

    for ( j = 0; j < nh; j++ )
      if ( tmp_h2[i] == tmp_height[j] )
        break;
    if ( j == nh )
      tmp_height[nh++] = tmp_h2[i];
    tmp_h2_index[i] = j;
  }

  // Copy lists into the lathe.
  bcyl->radius = new float [nr];
  bcyl->height = new float [nh];
  for ( i = 0; i < nr; i++ )
    bcyl->radius[i] = SQR( tmp_radius[i] );
  for (i = 0; i < nh; i++)
    bcyl->height[i] = tmp_height[i];

  // Assign height and radius indices.
  bcyl->nRadius = nr;
  bcyl->nHeight = nh;

  for ( i = 0; i < number; i++ ) {
    bcyl->entry[i].r1 = tmp_r1_index[i];
    bcyl->entry[i].r2 = tmp_r2_index[i];
    bcyl->entry[i].h1 = tmp_h1_index[i];
    bcyl->entry[i].h2 = tmp_h2_index[i];
  }

  // Get rid of temp. memory.
  delete [] tmp_height;
  delete [] tmp_radius;
  delete [] tmp_h2_index;
  delete [] tmp_h1_index;
  delete [] tmp_r2_index;
  delete tmp_r1_index;

  return bcyl;
}

// Destroy a bounding cylinder.
void BCylinder::destroy() {
  delete [] entry;
  delete [] radius;
  delete [] height;
  delete [] rInt;
  delete [] hInt;
  delete [] intervals;
}
