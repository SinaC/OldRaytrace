#include <stdlib.h>
#include <string.h>

#include "tag.h"
#include "../text/basics.h"

//--#define __DEBUG__

// find tag in table
int TTagList::findId( const char *tagName, const TTagList *list ) {
  for ( int i = 0; list[i].tagName != NULL; i++ )
    if ( !list[i].constant
	 && !str_cmp( tagName, list[i].tagName ) )
      return list[i].tagId; // entry found
  return TAG_INVALID; // entry not found
}

// return string corresponding to id
const char *TTagList::findName( const u32 id, const TTagList *list ) {
  for ( int i = 0; list[i].tagName != NULL; i++ )
    if ( list[i].tagId == id )
      return list[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", id );
  return buf;
}

// return a string with each bit into []
const char *TTagList::findName( const TBitvector &bitvector, const TTagList *list ) {
  static char buf[1024];
  buf[0] = '\0';
  bool found = false;
  for ( int i = 0; list[i].tagName != NULL; i++ )
    if ( bitvector.isSet( list[i].tagId ) ) {
      char temp[128];
      sprintf( temp, "[%s]", list[i].tagName );
      strcat( buf, temp );
#ifdef __DEBUG__
      sprintf( temp, "i[%d] tagId[%d]", i, list[i].tagId );
      strcat( buf, temp );
#endif
      found = true;
    }
  if ( !found )
    sprintf( buf, "<no value>" );

#ifdef __DEBUG__
  char temp[128];
  sprintf( temp, "<<%d>>", bitvector.getValue() );
  strcat( buf, temp );
#endif

  return buf;
}
