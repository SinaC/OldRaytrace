#include <stdio.h>

#include "bezier.h"

const TTagList MoverBezier::moverBezierTags [] = {
  { "segments", TAG_moverBezier_segments },
  { NULL, -1 }
};

MoverBezier::MoverBezier() {
  MoverVector::MoverVector(); moverKind = MOVER_BEZIER;
  numSegments = 0;
  segments = NULL;
}

void MoverBezier::setup() {
  totalTime = 0;
  for ( int i = 0; i < numSegments; i++ ) {
    segments[i].point1 = segments[i].point + segments[i].vector;
    totalTime += segments[i].time;
  }
}

TVector3 MoverBezier::vmove( const float time ) {
  // find segment
  float t = fmodf( time, totalTime ); // t in interval 0 -> totalTime
  int current;
  float timeLoop = 0;
  current = numSegments-1;
  for ( int i = 0; i < numSegments; i++ ) {
    if ( t >= timeLoop && t <= timeLoop + segments[i].time ) {
      current = i;
      break;
    }
    timeLoop += segments[i].time;
  }

  //--  printf("time: %5.2f  segment: %d\n", time, current );
  int next = (current+1)%numSegments;

  TPoint3 &start = segments[current].point;
  TPoint3 &start1 = segments[current].point1;
  TPoint3 &end = segments[next].point;
  TPoint3 &end1 = segments[next].point1;

  t = ( t - timeLoop ) / segments[current].time; // t in  0 -> 1.0
  //--  printf("t: %5.2f\n", t );
  float a = t;
  float b = 1.0f - t;

  // B^3 + 3B^2A + 3BA^2 + A^3
  return start *( b*b*b ) + start1 *( 3.0f*b*b*a ) + end1 *( 3.0f*b*a*a ) + end *(a*a*a);
}

void MoverBezier::print( const char *msg ) const {
  TMover::print(msg);

  printf("numSegments: %d\n", numSegments );
  for ( int i = 0; i < numSegments; i++ ) {
    printf("segment: %d\n", i );
    segments[i].point.print("point: ");
    segments[i].vector.print("vector: ");
    printf("time: %5.2f\n", segments[i].time );
  }
}
