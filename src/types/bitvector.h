#ifndef __TYPES_BITVECTOR_H__
#define __TYPES_BITVECTOR_H__

#include "basics.h"

struct TBitvector {
  private:
  u32 bitvector;

  public:
  TBitvector();
  u32 setBit( const u32 bitPosition );
  u32 removeBit( const u32 bitPosition );
  u32 toggleBit( const u32 bitPosition );
  u32 getValue() const;
  inline bool isSet( const u32 bitPosition ) const {
    return ( bitvector & ( 1 << bitPosition ) ) != 0;
  }
};

#endif
