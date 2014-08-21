#ifndef __COLOR_H__
#define __COLOR_H__

#include <math.h>

#include "types/basics.h"

struct TColor {
  //------ Datas
  static const TColor white;
  static const TColor black;
  static const TColor red;
  static const TColor green;
  static const TColor blue;
  static const TColor yellow;
  static const TColor cyan;
  static const TColor magenta;
  static const TColor lightGrey;
  static const TColor darkGrey;

  float RED, GREEN, BLUE;

  //------ Output
  void print( const char *msg ) const;

  //------ Constructor
  TColor() {}
  TColor( const float _r, const float _g, const float _b ) {
    RED = _r;
    GREEN = _g;
    BLUE = _b;
  }
  TColor( const TColor &col ) { RED = col.RED; GREEN = col.GREEN; BLUE = col.BLUE; }

  //------- Get method
  inline float r() const { return RED; };
  inline float g() const { return GREEN; };
  inline float b() const { return BLUE; };
};

inline TColor operator+( const TColor & a, const TColor & b )  {
  return TColor(
  a.RED+b.RED,
  a.GREEN+b.GREEN,
  a.BLUE+b.BLUE);
}

inline TColor operator+( const TColor & a, const float f ) {
  return TColor(
  a.RED+f,
  a.GREEN+f,
  a.BLUE+f);
}

inline void operator+=( TColor & a, const TColor & b ) {
  a.RED += b.RED;
  a.GREEN += b.GREEN;
  a.BLUE += b.BLUE;
}

inline TColor operator-( const TColor & a, const TColor & b ) {
  return TColor(
  a.RED-b.RED,
  a.GREEN-b.GREEN,
  a.BLUE-b.BLUE);
}

inline TColor operator-( const TColor & a, const float f ) {
  return TColor(
  a.RED-f,
  a.GREEN-f,
  a.BLUE-f);
}

inline TColor operator*( const TColor & a, const float f )  {
  return TColor( a.RED*f,
		 a.GREEN*f,
		 a.BLUE*f );
}

inline TColor operator*( const float f, const TColor & a )  {
  return TColor( a.RED*f,
		 a.GREEN*f,
		 a.BLUE*f );
}

inline TColor operator*( const TColor & a, const TColor &b )  {
  return TColor( a.RED*b.RED,
		 a.GREEN*b.GREEN,
		 a.BLUE*b.BLUE );
}

inline void operator*=( TColor & a, const float f ) {
  a.RED = a.RED * f;
  a.GREEN = a.GREEN * f;
  a.BLUE = a.BLUE * f;
}

inline void operator*=( TColor & a, TColor & b ) {
  a.RED *= b.RED;
  a.GREEN *= b.GREEN;
  a.BLUE *= b.BLUE;
}

// compare 2 color
inline bool sameColor( const TColor &c1, const TColor &c2 ) {
  static float colorPrecision = 1.0f/256.0f;
  return (fabsf(c1.RED-c2.RED) < colorPrecision
	  && fabsf(c1.GREEN-c2.GREEN) < colorPrecision
	  && fabsf(c1.BLUE-c2.BLUE) < colorPrecision );
}

// clamp a color
inline u32 clamp32( const float x, const float max ) {
  float tmp = x*max;
  if ( tmp < 0.0f )
    return 0;
  if ( tmp > max )
    return (u32)max;
  return (u32)tmp;
}

inline u16 clamp16( const float x, const float max ) {
  float tmp = x*max;
  if ( tmp < 0.0f )
    return 0;
  if ( tmp > max )
    return (u16)max;
  return (u16)tmp;
}

inline u8 clamp8( const float x, const float max ) {
  float tmp = x*max;
  if ( tmp < 0.0f )
    return 0;
  if ( tmp > max )
    return (u8)max;
  return (u8)tmp;
}

inline u32 clampRGB32( const float r, const float g, const float b ) {
  return (((u32)(r>1.0f? 255 : 255.0f*r))<<16)|
    (((u32)(g>1.0f? 255 : 255.0f*g))<<8)|
    (((u32)(b>1.0f? 255 : 255.0f*b)));
}

inline u32 clampRGB32ExposureGamma( const TColor &col ) {
	static const float invGamma = 0.45f;  // standard sRGB = 1 / 2.2
	static const float exposure = -0.66f;

	float red = col.RED;
	float green = col.GREEN;
	float blue = col.BLUE;

    // photo exposure
	red = 1.0f - expf(red * exposure);
	green = 1.0f - expf(green * exposure);
	blue = 1.0f - expf(blue * exposure);
	
	// gamma correction
	red = powf( red, invGamma );
	green = powf( green, invGamma );
	blue = powf( blue, invGamma );

	return clampRGB32( red, green, blue );
}

inline u32 clampRGB32(const TColor &Col) {
  return (((u32)(Col.RED>1.0f? 255 : 255.0f*Col.RED))<<16)|
    (((u32)(Col.GREEN>1.0f? 255 : 255.0f*Col.GREEN))<<8)|
    (((u32)(Col.BLUE>1.0f? 255 : 255.0f*Col.BLUE)));
}

inline u16 clampRGB16(const TColor &Col) {
  return (((u16)(Col.RED>1.0f? 32 : 32.0f*Col.RED))<<8)+
    (((u16)(Col.GREEN>1.0f? 64 : 64.0f*Col.GREEN))<<4)+
    (((u16)(Col.BLUE>1.0f? 32 : 32.0f*Col.BLUE)));
    //( 16, 0x0000F800,0x000007E0,0x0000001F ) // 16 bits 565
}

inline u16 clampRGB15(const TColor &Col) {
  return (((u16)(Col.RED>1.0f? 32 : 32.0f*Col.RED))<<8)+
    (((u16)(Col.GREEN>1.0f? 32 : 32.0f*Col.GREEN))<<4)+
    (((u16)(Col.BLUE>1.0f? 32 : 32.0f*Col.BLUE)));
    //( 16, 0x00007C00,0x000003E0,0x0000001F ) // 15 bits 555
}

// blend 2 colors
inline u32 blend2RGB( const u32 c, const u32 c1 ) {
  return
    ( ( ( (c&0x00FF0000)+(c1&0x00FF0000) ) >> 1 )&0x00FF0000 ) |
    ( ( ( (c&0x0000FF00)+(c1&0x0000FF00) ) >> 1 )&0x0000FF00 ) |
    ( ( ( (c&0x000000FF)+(c1&0x000000FF) ) >> 1 )&0x000000FF );
}

// blend 4 colors
inline u32 blend4RGB( const u32 c, const u32 c1, const u32 c2, const u32 c3 ) {
  return
    ( ( ( (c&0x00FF0000)+(c1&0x00FF0000)+(c2&0x00FF0000)+(c3&0x00FF0000) ) >> 2 )&0x00FF0000 ) |
    ( ( ( (c&0x0000FF00)+(c1&0x0000FF00)+(c2&0x0000FF00)+(c3&0x0000FF00) ) >> 2 )&0x0000FF00 ) |
    ( ( ( (c&0x000000FF)+(c1&0x000000FF)+(c2&0x000000FF)+(c3&0x000000FF) ) >> 2 )&0x000000FF );
}

inline u32 packRGB32( const u8 r, const u8 g, const u8 b ) {
  return ( r << 16 ) + ( g << 8 ) + b;
}

inline TColor packRGBColor( const u8 r, const u8 g, const u8 b ) {
  return TColor( (float)r/255.0f, (float)g/255.0f, (float)b/255.0f );
}

inline TColor packRGBColor( const u32 rgb ) {
  u8 r = (u8)((rgb&0x00FF0000)>>16);
  u8 g = (u8)((rgb&0x00FF00)>>8);
  u8 b = (u8)((rgb&0x0000FF));
  return packRGBColor( r, g, b );
}

// Fast (?) blending courtesy of Michael Herf (www.stereopsis.com)
// slightly modified   linear interpolation of rgb
// equivalent to  a + alpha * (b-a)/16
inline u32 blend16( const u32 a, const u32 b, u32 alpha ) {
  u32 dstrb = a & 0xff00ff;
  u32 dstag = a >> 8 & 0xff00ff;
  
  u32 srcrb = b & 0xff00ff;
  u32 srcag = b >> 8 & 0xff00ff;
  
  u32 drb = srcrb - dstrb;
  u32 dag = srcag - dstag;
  
  drb *= alpha; dag *= alpha;
  drb >>= 4; dag >>= 4;
  
  const u32 rb = (drb + dstrb) & 0x00ff00ff;
  const u32 ag = (dag + dstag) << 8 & 0xff00ff00;
  
  return rb | ag;
}
inline u32 blend8( const u32 a, const u32 b, u32 alpha ) {
  u32 dstrb = a & 0xff00ff;
  u32 dstag = a >> 8 & 0xff00ff;
  
  u32 srcrb = b & 0xff00ff;
  u32 srcag = b >> 8 & 0xff00ff;
  
  u32 drb = srcrb - dstrb;
  u32 dag = srcag - dstag;
  
  drb *= alpha; dag *= alpha;
  drb >>= 3; dag >>= 3;
  
  const u32 rb = (drb + dstrb) & 0x00ff00ff;
  const u32 ag = (dag + dstag) << 8 & 0xff00ff00;
  
  return rb | ag;
}
inline u32 blend4( const u32 a, const u32 b, u32 alpha ) {
  u32 dstrb = a & 0xff00ff;
  u32 dstag = a >> 8 & 0xff00ff;
  
  u32 srcrb = b & 0xff00ff;
  u32 srcag = b >> 8 & 0xff00ff;
  
  u32 drb = srcrb - dstrb;
  u32 dag = srcag - dstag;
  
  drb *= alpha; dag *= alpha;
  drb >>= 2; dag >>= 2;
  
  const u32 rb = (drb + dstrb) & 0x00ff00ff;
  const u32 ag = (dag + dstag) << 8 & 0xff00ff00;
  
  return rb | ag;
}
inline u32 blend2( const u32 a, const u32 b, u32 alpha ) {
  u32 dstrb = a & 0xff00ff;
  u32 dstag = a >> 8 & 0xff00ff;
  
  u32 srcrb = b & 0xff00ff;
  u32 srcag = b >> 8 & 0xff00ff;
  
  u32 drb = srcrb - dstrb;
  u32 dag = srcag - dstag;
  
  drb *= alpha; dag *= alpha;
  drb >>= 1; dag >>= 1;
  
  const u32 rb = (drb + dstrb) & 0x00ff00ff;
  const u32 ag = (dag + dstag) << 8 & 0xff00ff00;
  
  return rb | ag;
}

#endif
