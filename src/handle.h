#ifndef __HANDLE_H__
#define __HANDLE_H__

#include "types/basics.h"
#include "parser/tag.h"

struct RAYField;
struct RAYRecord;
struct Value;
struct Context;

struct THandle {
  //----- Datas
  char *name;     // handle's name
  u32 handleKind; // kind of handle: camera, light, primitive, ...
  static const enum EHandleKind {
    HANDLE_NOTHING,
    HANDLE_CAMERA,
    HANDLE_LIGHT,
    HANDLE_OBJECT,
    HANDLE_PRIMITIVE,
    HANDLE_BOUNDING,
    HANDLE_AMBIENT,
    HANDLE_TEXTURE,
    HANDLE_PIGMENT,
    HANDLE_FINISH,
    HANDLE_ATMOSPHERIC,
    HANDLE_INTERIOR,
    HANDLE_COLORMAP,
    //    HANDLE_PATTERN,
    HANDLE_ANIMATOR,
    HANDLE_MOVER,
    HANDLE_BITMAP,
    HANDLE_NORMAL,
    HANDLE_TURBULENCE,
    HANDLE_META, // PrimitiveMetaObjects sub-class MetaSubObject
    HANDLE_BLOB, // PrimitiveBlob sub-class BlobSubObject
    HANDLE_WORLD // World isn't a parsable handle sub-class
  };

  public:

  //----- Constructor
  THandle();

  //----- Setup
  // setup called once in world initialization
  virtual void setup();
  // check enough informations have been parsed
  virtual bool check() const;

  //----- Methods
  // return handle name
  char *handleName() const;
  char *handleName( const u32 kind ) const;

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find subclass type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  // dump information to the standard output
  virtual void print( const char *msg ) const;
  // stop program execution
  void error( const char *msg, ... ) const;

  //----- New/Delete
  void* operator new( size_t size );
  void operator delete( void* obj );
  
  void* operator new[]( size_t size );
  void operator delete[]( void* obj );

  //----- Private
  private:
  //-- tag
  static const enum EHandleTags {
    TAG_handle_name
  };
  static const TTagList handleTags [];
  static const TTagList handleSubClassTags [];
};

#endif
