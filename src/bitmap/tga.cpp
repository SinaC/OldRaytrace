#include <stdio.h>
//--#include <fstream>

#include "bitmap.h"
#include "../color.h"

//--#define __DEBUG__

bool TBitmap::loadTGA( const char *filename ) {
  FILE *fp;
//--  u8 kTGAHeader[12];//--={0,0,2,0,0,0,0,0,0,0,0,0};      // Uncompressed TGA Header
  u32 bpp;
  u8 buf;

  // open file
  if ( ( fp = fopen( filename, "rb" ) ) == NULL )
    return false;

#ifdef __DEBUG__
  printf("file opened\n");
#endif

  // read header
  for ( u8 j = 0; j < 12; j++ ) {
    if ( fread( &buf, 1, 1, fp ) != 1 ) return false;
    //--    if ( buf != kTGAHeader[j] ) // not an uncompressed TGA
    //--      return false;
  }

#ifdef __DEBUG__
  printf("header 1st part read\n");
#endif

  // read width in little endian
  if ( fread( &buf, 1, 1, fp ) != 1 ) return false;
  width = buf;
  if ( fread( &buf, 1, 1, fp ) != 1 ) return false;
  width = width + 256 * buf;
  // read height in little endian
  if ( fread( &buf, 1, 1, fp ) != 1 ) return false;
  height = buf;
  if ( fread( &buf, 1, 1, fp ) != 1 ) return false;
  height = height + 256 * buf;
  // read bits per pixel
  if ( fread( &buf, 1, 1, fp ) != 1 ) return false;
  bpp = buf;

  // read extra byte: 18th byte of TGA header
  if ( fread( &buf, 1, 1, fp ) != 1 ) return false;

#ifdef __DEBUG__
  printf("header 2nd part read\n");
#endif

  if ( bpp == 8 ) {
    // Read Palette
    palette = new u8 [768];
    if ( fread ( palette, 1, 768, fp ) < 768 ) {
      fclose(fp);
      return false;
    }

#ifdef __DEBUG__
    printf("palette read\n");
#endif
  }

  // bpp is now BYTES per pixel
  bpp /= 8;

  // allocate image
  const u32 size = width*height;
  data = new u32 [size];
  u8 *image = new u8 [size * bpp];
  // read image in temp buffer
  if ( fread( image, 1, size*bpp, fp ) != size*bpp )
    return false;

#ifdef __DEBUG__
  printf("image read\n");
#endif

  // pixels are stored in BGRA
  u32 *dot = data;
  if ( bpp == 1 ) {
    u8 *img = image;
    for ( u32 i = 0; i < size; i++ ) {
      *(dot++) = getPaletteColor( *img );
      img++;
    }
  }
  else {
    //--    u8 *img = image;
    //--    for ( u32 i = 0; i < size; i++ ) {
    //--      *(dot++) = packRGB32( *(img+2), *(img+1), *(img+0) );
    //--      img+=bpp;
    //--    }
    // don't forget a vertical flip
    u8 *img = image + bpp * size-1;
    for ( u32 i = 0; i < size; i++ ) {
      *(dot++) = packRGB32( *(img), *(img-1), *(img-2) );
      img-=bpp;
    }
  }
  delete [] image;

  fclose( fp );

#ifdef __DEBUG__
  printf("done\n");
#endif

  return true;
}

bool TBitmap::saveTGA( const char *filename ) const {
  return saveTGA( data, width, height, filename );
}

bool TBitmap::saveTGA( const u32 *buffer, const u32 width, const u32 height, const char *filename ) {
  // open image file for writing
  //--ofstream os( filename, ios::out|ios::binary );
	FILE *os = fopen( filename, "wb" );

  // check image file stream
  if ( !os ) 
    return false;

  // generate the header for a true color targa image
  u8 header[18] = { 0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  
  // set targa image width
  header[12] = width & 0xFF;
  header[13] = width >> 8;
  
  // set targa image height
  header[14] = height & 0xFF;
  header[15] = height >> 8;
  
  // set bits per pixel
  header[16] = 24;
  
  // write tga header
  //--os.write( (const char*)header, 18 );
  fwrite( header, sizeof(u8), 18, os );

  // write image pixels (only last 24 bits of each pixel)
  for ( u32 y = 0; y < height; y++ )
    for ( u32 x = 0; x < width; x++ )
      //--os.write( (char*)&buffer[ x + (height-y-1) * width ], 3 );
	  fwrite( (u8*)&buffer[x+(height-y-1)*width], sizeof(u8), 3, os );

  //--os.close();
  fclose(os);
  
  return true;
}
