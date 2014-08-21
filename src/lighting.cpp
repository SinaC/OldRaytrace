#include <stdio.h>
#include <stdlib.h>

#include "lighting.h"
#include "lights\light.h"

//-- Lighting information
TLighting::TLighting() {
  light = NULL;
  shadow = diffuse = specular = phong = 0.0f;
  visible = false;
}

void TLighting::print( const char *msg ) const {
  printf("%s\n", msg );
  if ( light != NULL )
    light->print("light");
  printf("shadow: %5.5f  diffuse: %5.5f    visible: %d\n", shadow, diffuse, visible );
  printf("specular: %5.5f  phong: %5.5f\n", specular, phong );
  color.print("color: ");
}
