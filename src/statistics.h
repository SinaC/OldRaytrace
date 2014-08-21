#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "types/basics.h"
#include "parser/tag.h"

struct TStatistic {
  //---- Datas
  static const enum EStatistic {
    STATISTIC_RAY,
    STATISTIC_CAMERA_RAY,
    STATISTIC_REFLECTED_RAY,
    STATISTIC_TRANSMITTED_RAY,
    STATISTIC_INTERNAL_REFLECTED_RAY,
    STATISTIC_LIGHT_RAY,
    STATISTIC_FIND_CLOSEST_INTERSECTION,
    STATISTIC_FIND_FIRST_INTERSECTION,
    STATISTIC_INTERSECTION,
    STATISTIC_INTERSECTS,
    STATISTIC_AMBIENT,
    STATISTIC_DIFFUSE,
    STATISTIC_ATMOSPHERIC,
    STATISTIC_MAX
  };

  //---- Constructor
  TStatistic();
  //---- Destructor
  ~TStatistic();

  //---- Methods
  inline void increase( const u32 whichOne ) {
    stats[whichOne]++;
  }
  void clear();

  //---- Output
  void print( const char *msg ) const;

  //**** Private
  protected:
  u32 *stats;
  static const TTagList statisticsTagList [];
  //--  char *getName( const u32 id ) const;
};

#endif
