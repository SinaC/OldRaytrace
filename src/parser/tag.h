#ifndef __TAG_H__
#define __TAG_H__

#include "../types/basics.h"
#include "../types/bitvector.h"

// tag list
struct TTagList {
  //---- Datas
  char *tagName;
  u32 tagId;
  // if constant is set to true, findId will return -1
  bool constant;

  //---- Methods
  static int findId( const char *tagName, const TTagList *list );
  static const char *findName( const u32 id, const TTagList *list );
  static const char *findName( const TBitvector &bitvector, const TTagList *list );
};

#define TAG_INVALID (-1)

#endif
