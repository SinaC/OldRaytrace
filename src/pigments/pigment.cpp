#include <stdio.h>

#include "pigment.h"

const TTagList TPigment::pigmentTags [] = {
  { "anchor", TAG_pigment_anchor },
  { "simple", TAG_pigment_simple },

  { NULL, -1 }
};

const TTagList TPigment::pigmentSubClassTags [] = {
  { "checker", PIGMENT_CHECKER },
  { "cubemap", PIGMENT_CUBEMAP },
  { "cylindrical", PIGMENT_CYLINDRICAL },
  { "granite", PIGMENT_GRANITE },
  { "mandel", PIGMENT_MANDEL },
  { "marble", PIGMENT_MARBLE },
  { "noisy", PIGMENT_NOISY },
  { "planar", PIGMENT_PLANAR },
  { "skymap", PIGMENT_SKYMAP },
  { "solid", PIGMENT_SOLID },
  { "spherical", PIGMENT_SPHERICAL },
  { "torusmap", PIGMENT_TORUSMAP },
  { "wood", PIGMENT_WOOD },

  {  NULL, -1 }
};

TPigment::TPigment() {
  THandle::THandle(); handleKind = HANDLE_PIGMENT;
  pigmentKind = PIGMENT_NOTHING;
  anchor = TVector3::zeroVector;
  simple = TColor::white;
}

TPigment::~TPigment() {
}

void TPigment::setup() {
}

TColor TPigment::getColor( const TPoint3 &p ) const {
  error("TPigment::getColor: YOU SHOULD NEVER SEE THIS\n");
  return TColor::black;
}

void TPigment::print( const char *msg ) const {
  THandle::print(msg);
  printf("KIND: [%s]\n", pigmentName() );
  anchor.print("anchor: ");
  simple.print("simple: ");
}

char *TPigment::pigmentName() const {
  for ( int i = 0; pigmentSubClassTags[i].tagName != NULL; i++ )
    if ( pigmentSubClassTags[i].tagId == pigmentKind )
      return pigmentSubClassTags[i].tagName;
  static char buf[1024];
  sprintf( buf, "UNKNOWN[%d]", pigmentKind );
  return buf;
}
