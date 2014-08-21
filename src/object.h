#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "types/basics.h"
#include "handle.h"

struct TCameraSimple;
struct TBounding;
struct TPrimitive;
struct TTexture;

// An object is an instance of a primitive
struct TObject: public THandle {
  //----- Datas
  TPrimitive *primitive; // primitive representing the object
  TBounding *bounding;   // bounding volume englobing the object
  TTexture *texture;     // object color/texture/refection/ior/...

  //----- Constructor
  TObject();
  //----- Destructor
  ~TObject();

  //----- Setup
  virtual void setup();

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;
  virtual void renderSimple( u32 *screen, const u32 width, const u32 height, const u32 color, const TCameraSimple &camera ) const;

  //***** Private
  private:
  //--tag
  static const enum EObjectTags {
    TAG_object_primitive,
    TAG_object_bounding,
    TAG_object_texture
  };
  static const TTagList objectTags [];
};

#endif
