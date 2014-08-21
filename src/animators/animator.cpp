#include <stdio.h>

#include "animator.h"
#include "mover.h"

const TTagList TAnimator::animatorTags [] = {
  { "movers", TAG_animator_movers },
  { "speed", TAG_animator_speed },
  { NULL, -1 }
};

TAnimator::TAnimator() {
  THandle::THandle(); handleKind = HANDLE_ANIMATOR;
  reset();
};

TAnimator::~TAnimator() {
  movers.clear();
}

void TAnimator::reset() {
  time=0;
}

void TAnimator::setup() {
//--  for ( int i = 0; i < numMovers; i++ )
//--    if ( movers[i] != NULL )
//--      movers[i]->setup();
  for ( vector<TMover*>::const_iterator iter = movers.begin();
	iter != movers.end();
	iter++ )
    (*iter)->setup();
}

void TAnimator::animate() {
  time += speed;
//--  for ( int i = 0; i < numMovers; i++ )
//--    if ( movers[i] != NULL )
//--      movers[i]->move( time );
  for ( vector<TMover*>::const_iterator iter = movers.begin();
	iter != movers.end();
	iter++ )
    (*iter)->move( time );
}

void TAnimator::print( const char *msg ) const {
  THandle::print( msg );
  printf("speed: %5.2f\n", speed );
//--  printf("numMovers: %d\n", numMovers );
//--  for (int i = 0; i < numMovers; i++ )
//--    if ( movers[i] == NULL )
//--      printf("NULL MOVER [%d]!!\n", i );
//--    else
//--      movers[i]->print("movers: ");
  for ( vector<TMover*>::const_iterator iter = movers.begin();
	iter != movers.end();
	iter++ )
    (*iter)->print("mover: ");
}
