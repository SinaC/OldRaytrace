#include <stdio.h>

#include "math/math.h"
#include "color.h"

const TColor TColor::white( 1.0, 1.0, 1.0 );
const TColor TColor::black( 0.0, 0.0, 0.0 );
const TColor TColor::red( 1.0, 0.0, 0.0 );
const TColor TColor::green( 0.0, 1.0, 0.0 );
const TColor TColor::blue( 0.0, 0.0, 1.0 );
const TColor TColor::yellow( 1.0, 1.0, 0.0 );
const TColor TColor::cyan( 0.0, 1.0, 1.0 );
const TColor TColor::magenta( 1.0, 0.0, 1.0 );
const TColor TColor::lightGrey( 0.66f, 0.66f, 0.66f );
const TColor TColor::darkGrey( 0.33f, 0.33f, 0.33f );

void TColor::print( const char *msg ) const {
  printf("%s\n", msg );
  printf("R: %5.2f  G: %5.2f  B: %5.2f\n", RED, GREEN, BLUE );
  fflush(stdout);
}

//  A Fast HSL-to-RGB Transform
//  by Ken Fishkin
//  from "Graphics Gems", Academic Press, 1990

// RGB-HSL transforms.
// Ken Fishkin, Pixar Inc., January 1989.

// given r,g,b on [0 ... 1],
// return (h,s,l) on [0 ... 1]
void RGB2HSL( float r, float g, float b, float &h,float &s, float &l ) {
  float v;
  float m;
  float vm;
  float r2, g2, b2;
  
  v = MAX(r,g);
  v = MAX(v,b);
  m = MIN(r,g);
  m = MIN(m,b);
  
  if ((l = (m + v) / 2.0f) <= 0.0f) return;
  if ((s = vm = v - m) > 0.0f) {
    s /= (l <= 0.5f) ? (v + m ) :
      (2.0f - v - m) ;
  } else
    return;
  
  float oneOverVm = 1/vm;
  r2 = (v - r) * oneOverVm;
  g2 = (v - g) * oneOverVm;
  b2 = (v - b) * oneOverVm;
  
  if (r == v)
    h = (g == m ? 5.0f + b2 : 1.0f - g2);
  else if (g == v)
    h = (b == m ? 1.0f + r2 : 3.0f - b2);
  else
    h = (r == m ? 3.0f + g2 : 5.0f - r2);
  
  h /= 6.0f;
}

// given h,s,l on [0 ... 1],
// return r,g,b on [0 ... 1]
void HSL2RGB( float h, float sl, float l, float &r, float &g, float &b) {
  float v;
  
  v = (l <= 0.5f) ? (l * (1.0f + sl)) : (l + sl - l * sl);
  if (v <= 0.0f) {
    r = g = b = 0.0f;
  }
  else {
    float m;
    float sv;
    int sextant;
    float fract, vsf, mid1, mid2;
    
    m = l + l - v;
    sv = (v - m ) / v;
    h *= 6.0f;
    sextant = (int)h;	
    fract = h - sextant;
    vsf = v * sv * fract;
    mid1 = m + vsf;
    mid2 = v - vsf;
    switch (sextant) {
    case 0: r = v; g = mid1; b = m; break;
    case 1: r = mid2; g = v; b = m; break;
    case 2: r = m; g = v; b = mid1; break;
    case 3: r = m; g = mid2; b = v; break;
    case 4: r = mid1; g = m; b = v; break;
    case 5: r = v; g = m; b = mid2; break;
    }
  }
}
