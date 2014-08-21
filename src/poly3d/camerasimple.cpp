#include "camerasimple.h"

void TCameraSimple::update() {
  float sa = sinf( angleX );
  float sb = sinf( angleY );
  float sg = sinf( angleZ );

  float ca = cosf( angleX );
  float cb = cosf( angleY );
  float cg = cosf( angleZ );

  mr00 = cb * cg;
  mr10 = -ca * sg - sa * sb * cg;
  mr20 = sa * sg  - ca * sb * cg;
  mr01 = cb * sg;
  mr11 = ca * cg  - sa * sb * sg;
  mr21 = -sa * cg - ca * sb * sg;
  mr02 = sb;
  mr12 = sa * cb;
  mr22 = ca * cb;
}

void TCameraSimple::convert3Dto2D( const TPoint3 &p, float &x, float &y ) const {
  float z = p[__Z];
  x = p[__X];
  y = p[__Y];
 
  float x1, y1, z1;

  x1 = x * mr00 + y * mr10 + z * mr20;
  y1 = x * mr01 + y * mr11 + z * mr21;
  
  z1 = x * mr02 + y * mr12 + z * mr22;


  z = z1 - zModifier;
  x = -x1 * projectionFactor / z;
  y = y1 * projectionFactor / z;
}
