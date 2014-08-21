#include <stdio.h>

#include "add.h"

const TTagList MoverAddV::moverAddTags [] = {
  //  { "source1", TAG_moverAdd_source1 },
  //  { "source2", TAG_moverAdd_source2 },
  { "source", TAG_moverAdd_source },
  { NULL, -1 }
};

MoverAddV::MoverAddV() {
  MoverVector(); moverKind = MOVER_ADDV;
  //  source1 = NULL;
  //  source2 = NULL;
}

TVector3 MoverAddV::vmove( const float time ) {
  TVector3 sum = TVector3::zeroVector;
  
  for ( vector<TVector3*>::const_iterator iter = source.begin();
	iter != source.end();
	iter++ )
    sum += *(*iter);
  return sum;
  //  return *source1 + *source2;
}

void MoverAddV::print( const char *msg ) const {
  TMover::print(msg);
  for ( vector<TVector3*>::const_iterator iter = source.begin();
	iter != source.end();
	iter++ )
    (*iter)->print("source: ");
  //  source1->print("source1: ");
  //  source2->print("source1: ");
}
