//----------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Bitmap TGA file I/O
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------

#ifndef _IMPORTTGABITMAP_H_
#define _IMPORTTGABITMAP_H_

#include "Types.h"
#include "ARGBPixelBuffer.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma pack(1)

// Header for tga files
struct TGA_Header
{
  u8			idlength;
  u8			colormaptype;
  u8			imagetype;
  u16			colormapstart;		// Color map specification
  u16			colormaplength;
  u8			colormapdepth;
  u16			xoffset;			// Image specification
  u16			yoffset;
  u16			width;
  u16			height;
  u8			pixeldepth;
  u8			attributebits:4;
  u8			originx:1;
  u8			originy:1;
  u8			unused:2;
};

// TGA image type
enum TGA_ImageType
  {
    IMAGETYPE_NOIMAGE = 0,
    IMAGETYPE_COLORMAPPEDIMAGE = 1,
    IMAGETYPE_TRUECOLORIMAGE = 2,
    IMAGETYPE_MONOCHROMEIMAGE = 3,
    IMAGETYPE_ENCODEDCOLORMAPPEDIMAGE = 9,
    IMAGETYPE_ENCODEDTRUECOLORIMAGE = 10,
    IMAGETYPE_ENCODEDMONOCHROMEIMAGE = 11
  };

// Developper data directory
struct TGA_DevDirectory
{
  u16			numtags;
};

// Developper data directory entry
struct TGA_DevDirectoryEntry
{
  u16			tag;
  u8			*data;
  u32			size;
};
/*
  #define TGA_DevDirEntryPtrDA CDynArray<TGA_DevDirectoryEntry>
*/

// Extension
struct TGA_Extension
{
  u16			size;
  char		authorname[41];
  char		authorcomments[324];
  u16			savemonth;
  u16			saveday;
  u16			saveyear;
  u16			savehour;
  u16			saveminute;
  u16			savesecond;
  char		jobname[41];
  u16			jobhour;
  u16			jobminute;
  u16			jobsecond;
  char		softwareid[41];
  u16			softwareversionnum;
  char		softwareversionletter;
  u32			keycolor;
  u16			aspectrationumerator;
  u16			aspectratiodenominator;
  u16			gammanumerator;
  u16			gammadenominator;
  u32			colorcorrectionoffset;
  u32			postagestampoffset;
  u32			scanlineoffset;
  u8			attributetype;
};

// Footer for tga files
struct TGA_Footer
{
  u32			extensionoffset;
  u32			devdirectoryoffset;
  char		signature[16];
  char		periodchar;
  u8			nullterminator;
};

#pragma pack()

// Structure that points sub-structures of a TGA file
struct TGA_Composition
{
  TGA_Header *			header;
  u8 *					imageid;
  u8 *					colormapdata;
  u8 *					imagedata;
  TGA_DevDirectory *		devdirectory;
  /*	
    TGA_DevDirEntryPtrDA	devdirentries;	
  */	
  TGA_Extension *			extension;
  u8 *					scanlinetable;
  u8 *					postagestampimage;
  u16 *					colorcorrectiontable;
  TGA_Footer *			footer;
};

struct TGA_Palette
{
  u32						col[256];
};


//----------------------------------------------------------------------------------------------------------------------------------------------------------
// IO functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------
bool			ImportTGABitmap		( char *_filename, ARGBPixelBuffer &_buffer );

bool			ReadTGAFile			( char *_filename, TGA_Composition &_composition );
bool			FreeTGAFile			( TGA_Composition &_composition );

bool			DecodePalette		( TGA_Composition &_composition, TGA_Palette &_palette );
bool			DecodePixARGB		( TGA_Composition &_composition, ARGBPixelBuffer &_argbbuffer );
bool			DecodePixIntensity	( TGA_Composition &_composition, ARGBPixelBuffer &_argbbuffer );
bool			DecodePixPalette	( TGA_Composition &_composition, ARGBPixelBuffer &_argbbuffer, TGA_Palette &_palette );

#endif // _IMPORTTGABITMAP_H_
