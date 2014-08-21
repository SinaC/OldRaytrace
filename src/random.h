#ifndef __RANDOM_H__
#define __RANDOM_H__

#define RNDMASK       0x7FFF
#define RNDMULTIPLIER ((DBL)0.000030518509476)

// Macro to create random number in the [0; 1] range.

#define FRand() ((DBL)POV_RAND()*RNDMULTIPLIER)

inline int MyRand() {
  static unsigned long int next_rand = 1;
  next_rand = next_rand * 1812433253L + 12345L;
  return((int)(next_rand >> 16) & RNDMASK);
}

#endif
