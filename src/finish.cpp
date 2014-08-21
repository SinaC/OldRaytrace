#include <stdio.h>

#include "finish.h"

const TTagList TFinish::finishTags [] = {
  { "reflection", TAG_finish_reflection },
  { "transmission", TAG_finish_transmission },
  { "ambient", TAG_finish_ambient },
  { "diffuse", TAG_finish_diffuse },
  { "specular", TAG_finish_specular },
  { "roughness", TAG_finish_roughness },
  { "phong", TAG_finish_phong },
  { "phongSize", TAG_finish_phongSize },
  { "brilliance", TAG_finish_brilliance },
  { NULL, -1 }
};

TFinish::TFinish() {
  THandle::THandle(); handleKind = HANDLE_FINISH;
  reflectionFactor = 0.0f; // reflection coefficent
  transmissionFactor = 0.0f; // transmittion coefficent
  ambientFactor = 0.1f; // how ambient lighting affect material  0 -> 1
  diffuseFactor = 0.6f; // how direct lighting affect material  0 -> 1
  specularFactor = 0.0f; // specular highlights  0 -> 1
  specularRoughness = 1/0.05f; // size of highlights  0 -> 250
  phongFactor = 0.0f; // phong highlights  0 -> 1
  phongSize = 40.0f; // size of highlights  0 -> 250
  brilliance = 0.0f; // tightness of the basic diffuse illumination (better if used with specular)  0 -> 250
}

void TFinish::setup() {
}

void TFinish::print( const char *msg ) const {
  THandle::print( msg );
  printf( "  reflectionFactor: %5.2f  transmissionFactor: %5.2f\n",
	  reflectionFactor, transmissionFactor );
  printf( "  ambientFactor: %5.2f  diffuseFactor: %5.2f\n"
	  "  specularFactor: %5.2f  specularExponent: %5.2f\n"
	  "  phongFactor: %5.2f  phongSize: %5.2f\n",
	  ambientFactor, diffuseFactor,
	  specularFactor, specularRoughness,
	  phongFactor, phongSize );
  printf( "  brillance: %5.2f\n",
	  brilliance );
}
