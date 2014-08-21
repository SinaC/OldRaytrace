#include <stdlib.h>

#include "classic.h"

const TTagList CameraClassic::cameraClassicTags [] = {
  { NULL, -1 }
};

CameraClassic::CameraClassic() {
  TCamera::TCamera(); cameraKind = CAMERA_CLASSIC;
}

void CameraClassic::print( const char *msg ) const {
  TCamera::print(msg);
}
