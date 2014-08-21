#include "bitvector.h"

TBitvector::TBitvector() {
  bitvector = 0;
}

u32 TBitvector::setBit( const u32 bitPosition ) {
  bitvector |= ( 1 << bitPosition );
  return bitvector;
}

u32 TBitvector::removeBit( const u32 bitPosition ) {
  bitvector &= ~( 1 << bitPosition );
  return bitvector;
}

u32 TBitvector::toggleBit( const u32 bitPosition ) {
  bitvector ^= ( 1 << bitPosition );
  return bitvector;
}

u32 TBitvector::getValue() const {
  return bitvector;
}
