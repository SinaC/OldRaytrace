#include <stdlib.h>
#include <stdio.h>

#include "bitmap.h"
#include "../color.h"

//#define __DEBUG__


#pragma warning (disable:4018) // '<' : signed/unsigned mismatch

static unsigned readShort( FILE *f ) {
  return (getc(f) | (getc(f) << 8));
}

static unsigned readLong( FILE *f ) {
  return (getc(f) | (getc(f) << 8) | (getc(f) << 16) | (getc(f) << 24));
}

#define BI_RGB  (0)
#define BI_RLE8 (1)
#define BI_RLE4 (2)
#define WIN_OS2_OLD (12)

static bool readBMP1( unsigned char **data, FILE *fp, unsigned width, unsigned height ) {
  int i, bit, pwidth, y;
  int c;
  pwidth = ((width+31)/32)*32; // 4 byte boundary
  for ( y = height-1; y >= 0; y-- )
    for ( i = bit = 0; i < pwidth; i++, bit++ ) {
      if ( ( bit & 7 ) == 0 ) {
	c = getc(fp);
	bit = 0;
      }
      if ( i < width ) {
	data[y][i] = ( c & 0x80) ? 1 : 0;
	c <<= 1;
      }
    }
  return true;
}

static bool readBMP4( unsigned char **data, FILE *fp, unsigned compression, unsigned width, unsigned height) {
  int i, pwidth, nibble, x, y;
  int c, cc;
  
  if ( compression == BI_RGB ) {
    pwidth = ((width+7)/8)*8;
    for ( y = height-1; y >= 0; y-- )
      for ( i = nibble = 0; i < pwidth; i++, nibble++ ) {
	if ( ( nibble & 1 ) == 0 ) {
	  c = fgetc(fp);
	  nibble = 0;
	}
	if ( i < width ) {
	  data[y][i] = ( c & 0xf0 ) >> 4;
	  c <<= 4;
	}
      }
  }
  else if ( compression == BI_RLE4 ) {
    x = 0;
    y = height-1;
    while ( 1 ) {
      c = getc(fp);
      if ( c ) {
	cc = getc(fp);
	for ( i = 0; i < c; i++, x++ )
	  if ( y >= 0 && y < height && x >= 0 && x < width )
	    data[y][x] = ( i & 1 ) ? ( cc & 0x0f) : ( ( cc >> 4 ) & 0x0f );
      }
      else {
	c = getc(fp);
	if ( c==0 ) {
	  x = 0;
	  y--;
	}
	else if ( c==1 )
	  return true;
	else if ( c==2 ) {
	  x += getc(fp);
	  y -= getc(fp);
	}
	else {
	  for ( i = 0; i < c; i++, x++ ) {
	    if ( ( i & 1 ) == 0 )
	      cc = getc(fp);
	    if ( ( y >= 0 ) && ( y < height ) && ( x >= 0 ) && ( x < width ) )
	      data[y][x] = ( i & 1 ) ? ( cc & 0x0f ) : ( ( cc >> 4 ) & 0x0f );
	  }
	  if ( ( c & 3 ) == 1 || ( c & 3 ) == 2 )
	    getc(fp);
	}
      }
    }
  }
  else
    return false;// Error ("Unknown compression scheme in BMP image.\n");
  return true;
}

static bool readBMP8( unsigned char **data, FILE *fp, unsigned compression, unsigned width, unsigned height ) {
  int i, pwidth, x, y;
  int c, cc;
  
  if ( compression == BI_RGB ) {
    pwidth = ( ( width + 3 ) / 4 ) * 4;
    for ( y = 0; y < height; y++ )
      for ( i = 0; i < pwidth; i++ ) {
	c = getc(fp);
	if ( i < width )
	  data[y][i] = c;
      }
  }
  else if ( compression == BI_RLE8 ) {
    x = 0;
    y = height-1;
    while ( 1 ) {
      c = getc(fp);
      if ( c ) {
	cc = getc(fp);
	for ( i = 0; i < c; i++, x++ )
	  if ( y >= 0 && y < height && x >= 0 && x < width )
	    data[y][x] = cc;
      }
      else {
	c = getc(fp);
	if ( c == 0 ) {
	  x = 0;
	  y--;
	}
	else if ( c == 1 )
	  return true;
	else if ( c == 2 ) {
	  x += getc(fp);
	  y -= getc(fp);
	}
	else
	  for ( i = 0; i < c; i++, x++ )
	    if ( y >= 0 && y < height && x >= 0 && x < width )
	      data[y][x] = getc(fp);
      }
    }
  }
  else
    return false;//Error ("Unknown compression scheme in BMP image.\n");
  return true;
}

static bool readBMP24( u32 *data, FILE *fp, unsigned width, unsigned height ) {
  int pad, i, y;
  pad = ( 4 - ( ( width * 3 ) %4 ) ) & 0x03;
  u32 *dot = data;
  for ( y = height-1; y >= 0; y-- ) {
    for ( i = 0; i < width; i++ ) {
      int b = getc(fp);
      int g = getc(fp);
      int r = getc(fp);
      *(dot++) = packRGB32( r, g, b );
    }
    if (fseek(fp, pad, SEEK_CUR)==-1)
      return false;
      //Error("Error reading data from BMP image.\n");
  }
  return true;
}


bool TBitmap::loadBMP( const char *filename ) {
#ifdef __DEBUG__
  printf("TBitmap::loadBMP: entering.\n"); fflush(stdout);
#endif

  FILE *fp;
  if ( ( fp = fopen( filename, "rb" ) ) == NULL )
    return false;//("Error opening BMP image.\n");
  if ( ( getc(fp) != 'B' ) || ( getc(fp) != 'M' ) )
    return false;//("Error reading magic number of BMP image.\n");
  readLong(fp); // file size
  readShort(fp); readShort(fp); // reserved fields
  unsigned dataLocation = readLong(fp);
  unsigned infoSize = readLong(fp);
  unsigned planes, depth, compression, colors;
  // new style
  if ( infoSize != WIN_OS2_OLD ) {
    width = readLong(fp);
    height = readLong(fp);
    planes = readShort(fp);
    depth = readShort(fp);
    compression = readLong(fp);
    readLong(fp); // image size in bytes
    readLong(fp); // horizontal pixels per meter
    readLong(fp); // vertical pixels per meter
    colors = readLong(fp);
    readLong(fp); // number of colors absolutely necessary to display image
  }
  // old style
  else {
    width = readShort(fp);
    height = readShort(fp);
    planes = readShort(fp);
    depth = readShort(fp);
    compression = BI_RGB;
    colors = 0;
  }

  if ( feof(fp) )
    return false; //("Error reading data from BMP image.\n");

  // sanity check
  if ( ( depth != 1 && depth != 4 && depth != 8 && depth != 24 )
       || planes != 1 
       || compression > BI_RLE4
       || ( ( depth == 1 || depth == 24 ) && compression != BI_RGB )
       || ( depth == 4 && compression == BI_RLE8 )
       || ( depth == 8 && compression == BI_RLE4 ) )
    return false; //("Invalid BMP image (depth=%d planes=%d compression=%d).\n", depth, planes, compression);

  // Move to colormap
  if ( infoSize != WIN_OS2_OLD ) {
    for ( int i = 0; i < infoSize-40; i++ )
      getc(fp);
  }

  // Load colormap
  if ( depth != 24 ) {
    int length;
    if ( colors )
      length = colors;
    else
      length = 1 << depth; // max depth 8 -> length 256
    palette = new u8 [length*3];
    for ( int i = 0; i < length; i++ ) {
      int b = getc(fp);
      int g = getc(fp);
      int r = getc(fp);
      if ( infoSize != WIN_OS2_OLD )
	getc(fp);
      palette[i*3+0] = r;
      palette[i*3+1] = g;
      palette[i*3+2] = b;
    }
    if ( feof(fp ) )
      return false; //("Error reading data from BMP image.\n");
  }

  // Create image
  data = new u32 [width*height];

  // Load image
  fseek(fp, dataLocation, SEEK_SET);
  
  if ( depth != 24 ) {
    unsigned char **dataU8 = new unsigned char *[height];
    for ( int j = 0; j < height; j++ )
      dataU8[j] = new unsigned char [width];

    // Fill dataU8
    switch( depth ) {
    case 1: readBMP1( dataU8, fp, width, height ); break;
    case 4: readBMP4( dataU8, fp, compression, width, height ); break;
    case 8: readBMP8( dataU8, fp, compression, width, height ); break;
    }
    // Convert U8 to TColor
    u32 *dot = data + height * width;
    for ( int i = 0; i < height; i++ )
      for ( int j = 0; j < width; j++ )
	*(dot--) = getPaletteColor(dataU8[i][j]);

    for ( int j = 0; j < height; j++ )
      delete [] dataU8[j];
    delete [] dataU8;
  }
  else
    readBMP24( data, fp, width, height );

  fclose(fp);

#ifdef __DEBUG__
  printf("TBitmap::loadBMP: leaving.\n"); fflush(stdout);
#endif
  return true;
}
