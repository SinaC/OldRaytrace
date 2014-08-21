#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "../types/basics.h"
#include "../color.h"
#include "../handle.h"

struct TBitmap {
  //----- Datas
  u32 width, height;
  u32 *data;
  u8 *palette;

  //----- Constructor
  TBitmap();
  //----- Destructor
  ~TBitmap();

  //----- Methods
  bool loadTGA( const char *filename );
  bool loadPCX( const char *filename );
  bool loadBMP( const char *filename );
  //--  bool loadGIF( const char *filename );

  bool saveTGA( const char *filename ) const;
  static bool saveTGA( const u32 *buffer, const u32 width, const u32 height, const char *filename );

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  protected:
  u32 getPaletteColor( const u8 palIndex ) const;
};

struct TBitmapColor: public TBitmap, public THandle {
  // We must call  setup  after  loadXXX

  //----- Datas
  TColor *dataColor;

  //----- Constructor
  TBitmapColor();
  //----- Destructor
  ~TBitmapColor();

  //----- Setup
  void setup(); // create dataColor from data and delete data

  //----- Methods
  inline TColor& dotAt( const u32 x, const u32 y ) const {
    return dataColor[x+y*width];
  }
  TColor dotAtBilinear( const float x, const float y ) const;
  TColor dotAtNormalDist( const float x, const float y ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  TColor *getCorner( int x, int y ) const;
  //--tag
  static const enum EBitmapTags {
    TAG_bitmap_pcx,
    TAG_bitmap_tga,
    TAG_bitmap_bmp,
    TAG_bitmap_width,
    TAG_bitmap_height
  };
  static const TTagList bitmapTags [];
};

#endif
