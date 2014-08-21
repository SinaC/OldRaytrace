#ifndef __LIGHTING_H__
#define __LIGHTING_H__

#include "color.h"

struct TLight;

// Lighting
struct TLighting {
  //----- Datas
  const TLight *light; // light, lighting has been computed
  float shadow; // amount of shadow 1.0: no shadow  0.0: full shadow
  float diffuse; // amount of diffuse light
  float specular; // amount of specular light
  float phong; // amount of phong light
  bool visible; // intersection visible from light
  TColor color; // color of intersection from light

  //----- Constructor
  TLighting();

  //----- Output
  void print( const char *msg ) const;
};

#endif
