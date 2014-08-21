#ifndef __POLY3D_CAMERASIMPLE_H__
#define __POLY3D_CAMERASIMPLE_H__

#include "../types/basics.h"
#include "../math/vector3.h"

struct TCameraSimple {
  //----- Datas
  float angleX;
  float angleY;
  float angleZ;
  float zModifier;
  float projectionFactor;
 
  void update(); // set private datas using public datas
  void convert3Dto2D( const TPoint3 &p, float &x, float &y ) const;
 
  //***** Private *****
  private:
  //----- Datas
  float mr00;
  float mr10;
  float mr20;
  float mr01;
  float mr11;
  float mr21;
  float mr02;
  float mr12;
  float mr22;
};

#endif
