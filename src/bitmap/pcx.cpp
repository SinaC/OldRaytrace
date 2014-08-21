#include <windows.h>
#include <stdio.h>

#include "bitmap.h"

//--#define __DEBUG__

//----------------------------------------------------------------------------------------------
// Read PCX image files from version Z-Soft 5.0 with 256 colors.

// PCX header (128 bytes)
struct tagHeaderPCX {
                               // byte  description
  unsigned char yMan;          // 0   Manufacturer     10 = ZSoft
  unsigned char yVer;          // 1   Version
  unsigned char yEnc;          // 2   Encoding
  unsigned char yBpP;          // 3   Bits Per Pixel
  WORD uXMin;                  // 4   XMin
  WORD uYMin;                  // 6   Ymin
  WORD uXMax;                  // 8   XMax
  WORD uYMax;                  // 10  YMax
  WORD uHRes;                  // 12  Horizontal Resolution - disable
  WORD uVRes;                  // 14  Verticle Resolution   - disable
  unsigned char ayPal [ 48 ];  // 16  Color palette setting
  unsigned char yRes;          // 64  Reserved
  unsigned char yCPl;          // 65  Number of color planes
  WORD uBLi;                   // 66  Bytes per line
  WORD uTCo;                   // 68  1 = Color    2 = Grayscale
  unsigned char ayNone [ 58 ]; // 70  Blank
};

bool TBitmap::loadPCX( const char *filename ) {
#ifdef __DEBUG__
  printf("loadPCX\n");
#endif

  // open file
  FILE *fp;
  if ( ( fp = fopen( filename, "rb" ) ) == NULL )
    return false;

#ifdef __DEBUG__
  printf("file opened\n");
#endif

  // read header
  tagHeaderPCX header;
  if ( fread( &header, 1, sizeof(tagHeaderPCX), fp ) != sizeof(tagHeaderPCX) )
    return false;

#ifdef __DEBUG__
  printf("header read\n");
#endif

  // get image size and create bitmap
  width = header.uXMax - header.uXMin + 1;
  height = header.uYMax - header.uYMin + 1;
  const u32 size = width*height;
  data = new u32 [size];

  // read palette
  palette = new u8 [768];
  // 768 before EOF
  fseek ( fp, -768, SEEK_END );
  // read pal
  if ( fread ( palette, 1, 768, fp ) < 768 )
    return false;

#ifdef __DEBUG__
  printf("palette read\n");
#endif

  // read image
  fseek ( fp, 128, SEEK_SET );
  u32 *now = data,
    *end = data+size;
  // while image not totally loaded
  while ( now < end ) {
    u8 val; // read color or number of occurences
    if ( fread( &val, 1, 1, fp ) != 1 )
      return false;
    if ( val < 192 ) // value read is less than 192 -> color
      *(now++) = getPaletteColor( val );
    else {
      val &= 0x3F; // get number of occurences, 6 first bits valid
      u8 col; // read color
      if ( fread( &col, 1, 1, fp ) != 1 )
	return false;
      for ( u8 i = 0; i < val; i++ )
	*(now++) = getPaletteColor(col);
    }
  }
  // close file
  fclose(fp);

#ifdef __DEBUG__
  printf("done\n");
#endif

  return true;
}
