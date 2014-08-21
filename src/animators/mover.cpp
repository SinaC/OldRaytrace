#include <stdio.h>

#include "mover.h"

const TTagList TMover::moverTags [] = {
  { "dest", TAG_mover_dest },
  { NULL, -1 }
};

const TTagList TMover::moverSubClassTags [] = {
  {  "addv", MOVER_ADDV },
  {  "circle", MOVER_CIRCLE },
  {  "linear", MOVER_LINEAR },
  {  "bounce", MOVER_BOUNCE },
  {  "lissajou", MOVER_LISSAJOU },
  {  "bezier", MOVER_BEZIER },
  {  NULL, -1 }
};

TMover::TMover() {
  THandle::THandle(); handleKind = HANDLE_MOVER;
  moverKind = MOVER_NOTHING;
  //--  numDest = 0;
  //--  dest = NULL;
}

TMover::~TMover() {
  dest.clear();
}

void TMover::print( const char *msg ) const {
  THandle::print(msg);
  printf("KIND: [%s]\n", moverName() );
//--  printf("numDest: %d\n", numDest );
//--  for ( int i = 0; i < numDest; i++ ) {
//--    if ( dest[i] == NULL )
//--      printf("NULL DESTINATION [%d]!!\n", i );
//--    else {
//--      if ( vMove )
//--	((TVector3*)dest[i])->print("dest: ");
//--      else
//--	printf("dest: %5.2f\n", *(float*)(dest[i]));
//--    }
//--  }
  for ( vector<void*>::const_iterator iter = dest.begin();
	iter != dest.end();
	iter++ ) {
    if ( vMove )
      ((TVector3*)(*iter))->print("dest: ");
    else
      printf("dest: %5.5f\n", *(float*)(*iter) );
  }
}
void TMover::move( const float time ) {
//--  if ( vMove ) {
//--    for ( int i = 0; i < numDest; i++ )
//--      if ( dest[i] != NULL )
//--	*(TVector3*)(dest[i]) = vmove( time );
//--  }
//--  else
//--    for ( int i = 0; i < numDest; i++ )
//--      if ( dest[i] != NULL )
//--	*(float*)(dest[i]) = fmove( time );
  if ( vMove )
    for ( vector<void*>::const_iterator iter = dest.begin();
	  iter != dest.end();
	  iter++ )
      *(TVector3*)(*iter) = vmove( time );
  else
    for ( vector<void*>::const_iterator iter = dest.begin();
	  iter != dest.end();
	  iter++ )
      *(float*)(*iter) = fmove( time );
}

char *TMover::moverName() const {
  for ( int i = 0; moverSubClassTags[i].tagName != NULL; i++ )
    if ( moverSubClassTags[i].tagId == moverKind )
      return moverSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", moverKind );
  return buf;
}
