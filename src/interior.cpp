#include <stdio.h>

#include "interior.h"
#include "ior.h"

const TTagList TInterior::interiorTags [] = {
  { "ior", TAG_interior_ior },
  { "absorption", TAG_interior_absorption },
  { NULL, -1 }
};

TInterior::TInterior() {
  THandle::THandle(); handleKind = HANDLE_INTERIOR;
  ior = IOR_GLASS;
  absorption = TColor::white;  // attenuation = attenuation * powf( absorption, t )  where t = distance from origin to intersection
}

void TInterior::setup() {
}

void TInterior::print( const char *msg ) const {
  THandle::print( msg );
  printf("Index of Refraction: %5.5f\n", ior );
}
