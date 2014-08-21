#include <stdio.h>

#include "statistics.h"

TStatistic::TStatistic() {
  stats = new u32 [STATISTIC_MAX-1];
}

TStatistic::~TStatistic() {
  if ( stats != NULL )
    delete [] stats;
  stats = NULL;
}

void TStatistic::clear() {
  for ( int i = 0; i < STATISTIC_MAX; i++ )
    stats[i] = 0;
}

void TStatistic::print( const char *msg ) const {
  printf("%s\n", msg );
  for ( int i = 0; i < STATISTIC_MAX; i++ )
    //--printf("%30s: %16d\n", getName( i ), stats[i] );
    printf("%30s: %16d\n", TTagList::findName( i, statisticsTagList ), stats[i] );
}

//--char *TStatistic::getName( const u32 id ) const {
//--  for ( u32 i = 0; statisticsTagList[i].tagName != NULL; i++ )
//--    if ( statisticsTagList[i].tagId == id )
//--      return statisticsTagList[i].tagName;
//--  static char buf[1024];
//--  sprintf( buf, "UNKNOWN[%d]", id );
//--  return buf;
//--}

const TTagList TStatistic::statisticsTagList [] = {
  { "shooted ray",  STATISTIC_RAY },
  { "camera ray", STATISTIC_CAMERA_RAY },
  { "reflected ray", STATISTIC_REFLECTED_RAY },
  { "transmitted ray",  STATISTIC_TRANSMITTED_RAY },
  { "internal reflected ray",  STATISTIC_INTERNAL_REFLECTED_RAY },
  { "light/shadow ray", STATISTIC_LIGHT_RAY },
  { "find closest intersection", STATISTIC_FIND_CLOSEST_INTERSECTION },
  { "find first intersection", STATISTIC_FIND_FIRST_INTERSECTION },
  { "primitive intersection", STATISTIC_INTERSECTION },
  { "primitive intersects", STATISTIC_INTERSECTS },
  { "compute ambient", STATISTIC_AMBIENT },
  { "compute diffuse", STATISTIC_DIFFUSE },
  { "compute atmospheric", STATISTIC_ATMOSPHERIC },
  {  NULL, -1 }
};
