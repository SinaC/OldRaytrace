#include <stdio.h>

#include "bounce.h"

const TTagList MoverBounce::moverBounceTags [] = {
  { "gravity", TAG_moverBounce_gravity },
  { "speed", TAG_moverBounce_speed },
  { "height", TAG_moverBounce_height },
  { NULL, -1 }
};

MoverBounce::MoverBounce() {
  MoverFloat(); moverKind = MOVER_BOUNCE;
}

float MoverBounce::fmove( const float time ) {
  float delta = time-time0;
  time0 = time;
  speed -= gravity;

  height += speed*delta;

  if ( height+speed < 0 ) {
    speed -= gravity;
    speed =- speed;
  }

  return height;
}

void MoverBounce::print( const char *msg ) const {
  TMover::print(msg);
  printf("gravity: %5.2f\n"
	 "speed: %5.2f\n"
	 "height: %5.2f\n",
	 gravity, speed, height );
}
