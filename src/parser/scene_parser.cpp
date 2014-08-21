#include <stdio.h>

#include "../world.h"
#include "parser.h"
#include "parser_skel.h"
#include "context.h"

//--#define __DEBUG__
//--#define __DEBUG2__

char currentFilename[1024];


//----------------------------
//--------- Add this and add super
void addThis( Context &ctx, const Value &v ) {
  // add 'this' in context
  ctx.addVar( "this", v );
}

// FIXME: find another way to do this
//  super.super.<field> doesn't work  because a handle doesn't have 'super' as qualified expression
//  we cannot add a special case in RAYQualifyExpr because a scope is not linked to a handle
void addSuper( Context &ctx ) {
//--  // add 'super' in context: link to this of previous context scope layer
//--  if ( ctx.numscope <= 1 )
//--    p_error("Cannot add SUPER: no previous scope layer!");
//--  Value *previousThis = ctx.scopes[ctx.numscope-2].getVar("this");
//--  //--  if ( previousThis == NULL )
//--  //--    p_error("Cannot find THIS in previous scope layer!");
//--  if ( previousThis != NULL )
//--    ctx.addVar( "super", Value(previousThis) );
}

//-----------------------------
//--------- MODULE READ & PARSE

#define BIG_SIZE (4*1024*1024)
static char large_buffer[BIG_SIZE];
extern const char *cur_pos;
static RAYModule *readModule( FILE *fp ) {
  int actual_size = 0;
  large_buffer[0] = '\0';
  actual_size = fread(large_buffer, 1, BIG_SIZE, fp);
  large_buffer[actual_size] = '\0';

  cur_pos = large_buffer;
#ifdef __DEBUG2__
  printf("large_buffer: [%s]\n", large_buffer ); fflush(stdout);
#endif
#ifdef __DEBUG__
  printf("reading module...\n"); fflush( stdout );
#endif
  RAYModule* module;
  module = (RAYModule*)parse();
#ifdef __DEBUG__
  printf("...done.\n"); fflush(stdout);
#endif
#ifdef __DEBUG2__
  module->dump(1); fflush( stdout );
#endif

  return module;
}

static TWorld *parseModule( Context &ctx, RAYModule *module ) {
  extern TWorld *parsedWorld; // declare in execute.cpp
  for ( int i = 0; i < module->instructions_count; i++ ) {
#ifdef __DEBUG__
    printf("parseModule: EXECUTING [%x]:\n", module->instructions[i] ); fflush(stdout);
#endif
#ifdef __DEBUG2__
    printf("dumping:\n" ); fflush(stdout);
    module->instructions[i]->dump(1);
    fflush(stdout);
#endif
#ifdef __DEBUG__
    printf("executing:\n" ); fflush(stdout);
#endif
    module->instructions[i]->execute( ctx );
#ifdef __DEBUG__
    printf("DONE.\n"); fflush( stdout );
#endif
  }
#ifdef __DEBUG__
  printf("DUMPING CONTEXT:"); fflush(stdout);
  ctx.dump();
#endif
  return parsedWorld;
}


//-----------------------------
//--------- WORLD READ & PARSE
TWorld *parseFile( Context &ctx, FILE *fp ) {
  RAYModule *module = readModule( fp );
#ifdef __DEBUG__
  printf("parsing module...\n"); fflush(stdout);
#endif
  TWorld *wld = parseModule( ctx, module );
#ifdef __DEBUG__
  printf("...done.\n"); fflush(stdout);
#endif
  return wld;
}


//----------------------------
//----------------------------
//-------PARSE METHODS--------
//----------------------------
//----------------------------
#include "tag.h"

#include "../handle.h"
#include "../all_textures.h"
#include "../all_interiors.h"
#include "../all_finishes.h"
#include "../all_cameras.h"
#include "../all_primitives.h"
#include "../all_lights.h"
#include "../all_boundings.h"
#include "../all_ambients.h"
#include "../all_pigments.h"
#include "../all_atmospherics.h"
#include "../colormap.h"
//--#include "../all_patterns.h"
#include "../animators/animator.h"
#include "../all_movers.h"
#include "../bitmap/bitmap.h"
#include "../all_normals.h"
#include "../object.h"
#include "../globals.h"
#include "../renderers/renderer.h"
#include "../all_turbulences.h"


//--------------------------
//---- General parse methods
//-------------------------
// extract fields from object and fill handle
static bool parseHandle( Context &ctx, RAYRecord *record, THandle *handle ) {
  ctx.pushscope();

  addThis( ctx, handle );
  addSuper( ctx );

#ifdef __DEBUG__
  printf("parseHandle...(record, handle) %x %x\n",
	 record, handle ); fflush(stdout);
  printf("handleKind: %s\n",
	 handle->handleName() ); fflush(stdout);
#endif
  for ( int i = 0; i < record->fields_count; i++ ) {
#ifdef __DEBUG__
    printf("parsing field [%s]\n", record->fields[i]->fieldName->image ); fflush(stdout);
#endif
    if ( !handle->parse( ctx, record->fields[i] ) )
      p_error("Unknown token [%s] for handle [%s] kind [%s]!",
	      record->fields[i]->fieldName->image,
	      handle->name, handle->handleName());
  }

  ctx.popscope();

#ifdef __DEBUG__
  printf("...done -> dumping:\n" ); fflush(stdout);
  handle->print("DUMPED HANDLE:");
#endif
  return true;
}

//-----------------------
//----- Sub-Class parsing
//-----------------------
// direct definition or "pointer" to a handle
THandle *THandle::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("THandle::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
    printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // find kind of handle
  int handleKind = TTagList::findId( record->handleName->image, handleSubClassTags );
#ifdef __DEBUG__
  printf("handleKind: %d\n", handleKind ); fflush(stdout);
#endif
  switch( handleKind ) {
    //---- TObject
  case HANDLE_OBJECT: {
    TObject *object = new TObject;
    parseHandle( ctx, record, object );
    return object;
    break;
  }
    //---- TTexture
  case HANDLE_TEXTURE: {
    //--TODO    return TTexture::parseSubclass( ctx, record );
    TTexture *texture = new TTexture;
    parseHandle( ctx, record, texture );
    return texture;
    break;
  }
    //---- TInterior
  case HANDLE_INTERIOR: {
    //--TODO    return TInterior::parseSubclass( ctx, record );
    TInterior *interior = new TInterior;
    parseHandle( ctx, record, interior );
    return interior;
    break;
  }
    //---- TFinish
  case HANDLE_FINISH: {
    //--TODO    return TFinish::parseSubclass( ctx, record );
    TFinish *finish = new TFinish;
    parseHandle( ctx, record, finish );
    return finish;
    break;
  }
    //---- TCamera
  case HANDLE_CAMERA: {
    return TCamera::parseSubclass( ctx, record );
    break;
  }
    //---- TPrimitive
  case HANDLE_PRIMITIVE: {
    return TPrimitive::parseSubclass( ctx, record );
    break;
  }
    //---- TLight
  case HANDLE_LIGHT: {
    return TLight::parseSubclass( ctx, record );
    break;
  }
    //---- TBounding
  case HANDLE_BOUNDING: {
    return TBounding::parseSubclass( ctx, record );
    break;
  }
    //---- TAmbient
  case HANDLE_AMBIENT: {
    return TAmbient::parseSubclass( ctx, record );
    break;
  }
    //---- TPigment
  case HANDLE_PIGMENT: {
    return TPigment::parseSubclass( ctx, record );
    break;
  }
    //---- TAtmospheric
  case HANDLE_ATMOSPHERIC: {
    return TAtmospheric::parseSubclass( ctx, record );
    break;
  }
    //---- TColormap
  case HANDLE_COLORMAP: {
    TColorMap *colormap = new TColorMap;
    parseHandle( ctx, record, colormap );
    return colormap;
    break;
  }
    //---- TAnimator
  case HANDLE_ANIMATOR: {
    TAnimator *animator = new TAnimator;
    parseHandle( ctx, record, animator );
    return animator;
    break;
  }
    //---- TMover
  case HANDLE_MOVER: {
    return TMover::parseSubclass( ctx, record );
    break;
  }
    //---- TTurbulence
  case HANDLE_TURBULENCE: {
    return TTurbulence::parseSubclass( ctx, record );
    break;
  }
    //---- TBitmapColor
  case HANDLE_BITMAP: {
    TBitmapColor *bitmap = new TBitmapColor;
    parseHandle( ctx, record, bitmap );
    return bitmap;
    break;
  }
    //---- MetaSubObject
  case HANDLE_META: {
    return MetaSubObject::parseSubclass( ctx, record );
    break;
  }
    //---- BlobSubObject
  case HANDLE_BLOB: {
    return BlobSubObject::parseSubclass( ctx, record );
    break;
  }
    //---- TNormal
  case HANDLE_NORMAL: {
    return TNormal::parseSubclass( ctx, record );
    break;
  }
    
  default: p_error("Unknown Handle kind [%s]", record->handleName->image );
  }
  return NULL;
}

THandle *TPrimitive::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TPrimitive::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Primitive type must be specified!");
  int primitiveKind = TTagList::findId( record->subHandleName->image, primitiveSubClassTags );
#ifdef __DEBUG__
  printf("primitiveKind: %d\n", primitiveKind ); fflush(stdout);
#endif
  switch ( primitiveKind ) {
    //-- PrimitivePlane
  case PRIMITIVE_PLANE: {
    PrimitivePlane *plane = new PrimitivePlane;
    parseHandle( ctx, record, plane );
    return plane;
    break;
  }
    //-- PrimitiveSphere
  case PRIMITIVE_SPHERE: {
    PrimitiveSphere *sphere = new PrimitiveSphere;
    parseHandle( ctx, record, sphere );
    return sphere;
    break;
  }
    //-- PrimitiveBox
  case PRIMITIVE_BOX: {
    PrimitiveBox *box = new PrimitiveBox;
    parseHandle( ctx, record, box );
    return box;
    break;
  }
    //-- PrimitiveTorus
  case PRIMITIVE_TORUS: {
    PrimitiveTorus *torus = new PrimitiveTorus;
    parseHandle( ctx, record, torus );
    return torus;
    break;
  }
    //-- PrimitiveBlob
  case PRIMITIVE_BLOB: {
    PrimitiveBlob *blob = new PrimitiveBlob;
    parseHandle( ctx, record, blob );
    return blob;
    break;
  }
    //-- PrimitiveCylinder
  case PRIMITIVE_CYLINDER: {
    PrimitiveCylinder *cylinder = new PrimitiveCylinder;
    parseHandle( ctx, record, cylinder );
    return cylinder;
    break;
  }
    //-- PrimitiveCapsule
  case PRIMITIVE_CAPSULE: {
    PrimitiveCapsule *capsule = new PrimitiveCapsule;
    parseHandle( ctx, record, capsule );
    return capsule;
    break;
  }
    //-- PrimitiveCSG
  case PRIMITIVE_CSG: {
    PrimitiveCSG *csg = new PrimitiveCSG;
    parseHandle( ctx, record, csg );
    return csg;
    break;
  }
    //-- PrimitiveHeart
  case PRIMITIVE_HEART: {
    PrimitiveHeart *heart = new PrimitiveHeart;
    parseHandle( ctx, record, heart );
    return heart;
    break;
  }
    //-- PrimitiveQuadric
  case PRIMITIVE_QUADRIC: {
    PrimitiveQuadric *quadric = new PrimitiveQuadric;
    parseHandle( ctx, record, quadric );
    return quadric;
    break;
  }
    //-- PrimitiveJulia
  case PRIMITIVE_JULIA: {
    PrimitiveJulia *julia = new PrimitiveJulia;
    parseHandle( ctx, record, julia );
    return julia;
    break;
  }
    //-- PrimitiveMetaobjects
  case PRIMITIVE_META: {
    PrimitiveMetaObjects *meta = new PrimitiveMetaObjects;
    parseHandle( ctx, record, meta );
    return meta;
    break;
  }
    //-- PrimitiveTransformed
  case PRIMITIVE_TRANSFORMED: {
    PrimitiveTransformed *transformed = new PrimitiveTransformed;
    parseHandle( ctx, record, transformed );
    return transformed;
    break;
  }
    //-- PrimitiveSuperellipsoid
  case PRIMITIVE_SUPERELLIPSOID: {
    PrimitiveSuperellipsoid *superellipsoid = new PrimitiveSuperellipsoid;
    parseHandle( ctx, record, superellipsoid );
    return superellipsoid;
    break;
  }
    //-- PrimitiveSor
  case PRIMITIVE_SOR: {
    PrimitiveSor *sor = new PrimitiveSor;
    parseHandle( ctx, record, sor );
    return sor;
    break;
  }
    //-- PrimitiveCone
  case PRIMITIVE_CONE: {
    PrimitiveCone *cone = new PrimitiveCone;
    parseHandle( ctx, record, cone );
    return cone;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown primitive type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

THandle *TLight::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TLight::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Light type must be specified!");
  int lightKind = TTagList::findId( record->subHandleName->image, lightSubClassTags );
#ifdef __DEBUG__
  printf("lightKind: %d\n", lightKind ); fflush(stdout);
#endif
  switch ( lightKind ) {
    //-- LightDot
  case LIGHT_DOT: {
    LightDot *dot = new LightDot;
    parseHandle( ctx, record, dot );
    return dot;
    break;
  }
    //-- LightSquare
  case LIGHT_SQUARE: {
    LightSquare *square = new LightSquare;
    parseHandle( ctx, record, square );
    return square;
    break;
  }
//--TODO    //-- LightSpherical
//--TODO  case LIGHT_SPHERICAL: {
//--TODO    LightSpherical *spherical = new LightSpherical;
//--TODO    parseHandle( ctx, record, spherical );
//--TODO    return spherical;
//--TODO    break;
//--TODO  }
    // already declared handle  TODO
  default: {
    p_error("Unknown light type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

THandle *TBounding::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TBounding::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Bounding type must be specified!");
  int boundingKind = TTagList::findId( record->subHandleName->image, boundingSubClassTags );
#ifdef __DEBUG__
  printf("boundingKind: %d\n", boundingKind ); fflush(stdout);
#endif
  switch ( boundingKind ) {
    //-- BoundingAABB
  case BOUNDING_AABB: {
    BoundingAABB *aabb = new BoundingAABB;
    parseHandle( ctx, record, (TBounding*)aabb );
    return (TBounding*)aabb;
    break;
  }
    //-- BoundingSphere
  case BOUNDING_SPHERE: {
    BoundingSphere *sphere = new BoundingSphere;
    parseHandle( ctx, record, (TBounding*)sphere );
    return (TBounding*)sphere;
    break;
  }
    //-- BoundingSlab
//--TODO  case BOUNDING_SLAB: {
//--TODO    BoundingSlabs3 *slab = new BoundingSlabs3;
//--TODO    parseHandle( ctx, record, slab );
//--TODO    return (TBounding*)slab;
//--TODO    break;
//--TODO  }
    // already declared handle  TODO
  default: {
    p_error("Unknown bounding type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

THandle *TAmbient::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TAmbient::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Ambient type must be specified!");
  int ambientKind = TTagList::findId( record->subHandleName->image, ambientSubClassTags );
#ifdef __DEBUG__
  printf("ambientKind: %d\n", ambientKind ); fflush(stdout);
#endif
  switch ( ambientKind ) {
    //-- AmbientSolid
  case AMBIENT_SOLID: {
    AmbientSolid *solid = new AmbientSolid;
    parseHandle( ctx, record, solid );
    return solid;
    break;
  }
    //-- AmbientHemispherical
  case AMBIENT_HEMISPHERICAL: {
    AmbientHemispherical *hemispherical = new AmbientHemispherical;
    parseHandle( ctx, record, hemispherical );
    return hemispherical;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown ambient type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

THandle *TPigment::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TPigment::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Pigment type must be specified!");
  int pigmentKind = TTagList::findId( record->subHandleName->image, pigmentSubClassTags );
#ifdef __DEBUG__
  printf("pigmentKind: %d\n", pigmentKind ); fflush(stdout);
#endif
  switch ( pigmentKind ) {
    //-- PigmentSolid
  case PIGMENT_SOLID: {
    PigmentSolid *solid = new PigmentSolid;
    parseHandle( ctx, record, solid );
    return solid;
    break;
  }
    //-- PigmentNoisy
  case PIGMENT_NOISY: {
    PigmentNoisy *noisy = new PigmentNoisy;
    parseHandle( ctx, record, noisy );
    return noisy;
    break;
  }
    //-- PigmentSkymap
  case PIGMENT_SKYMAP: {
    PigmentSkymap *skymap = new PigmentSkymap;
    parseHandle( ctx, record, skymap );
    return skymap;
    break;
  }
    //-- PigmentChecker:
  case PIGMENT_CHECKER: {
    PigmentChecker *checker = new PigmentChecker;
    parseHandle( ctx, record, checker );
    return checker;
    break;
  }
    //-- PigmentCubemap:
  case PIGMENT_CUBEMAP: {
    PigmentCubemap *cubemap = new PigmentCubemap;
    parseHandle( ctx, record, cubemap );
    return cubemap;
    break;
  }
    //-- PigmentCylindrical:
  case PIGMENT_CYLINDRICAL: {
    PigmentCylindrical *cylindrical = new PigmentCylindrical;
    parseHandle( ctx, record, cylindrical );
    return cylindrical;
    break;
  }
    //-- PigmentGranite:
  case PIGMENT_GRANITE: {
    PigmentGranite *granite = new PigmentGranite;
    parseHandle( ctx, record, granite );
    return granite;
    break;
  }
    //-- PigmentMandel:
  case PIGMENT_MANDEL: {
    PigmentMandel *mandel = new PigmentMandel;
    parseHandle( ctx, record, mandel );
    return mandel;
    break;
  }
    //-- PigmentMarble:
  case PIGMENT_MARBLE: {
    PigmentMarble *marble = new PigmentMarble;
    parseHandle( ctx, record, marble );
    return marble;
    break;
  }
    //-- PigmentPlanar:
  case PIGMENT_PLANAR: {
    PigmentPlanar *planar = new PigmentPlanar;
    parseHandle( ctx, record, planar );
    return planar;
    break;
  }
    //-- PigmentSpherical:
  case PIGMENT_SPHERICAL: {
    PigmentSpherical *spherical = new PigmentSpherical;
    parseHandle( ctx, record, spherical );
    return spherical;
    break;
  }
    //-- PigmentTorusmap:
  case PIGMENT_TORUSMAP: {
    PigmentTorusmap *torusmap = new PigmentTorusmap;
    parseHandle( ctx, record, torusmap );
    return torusmap;
    break;
  }
    //-- PigmentWood:
  case PIGMENT_WOOD: {
    PigmentWood *wood = new PigmentWood;
    parseHandle( ctx, record, wood );
    return wood;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown pigment type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

THandle *TAtmospheric::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TAtmospheric::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Atmospheric type must be specified!");
  int atmosphericKind = TTagList::findId( record->subHandleName->image, atmosphericSubClassTags );
#ifdef __DEBUG__
  printf("atmosphericKind: %d\n", atmosphericKind ); fflush(stdout);
#endif
  switch ( atmosphericKind ) {
    //-- AtmosphericFog
  case ATMOSPHERIC_FOG: {
    AtmosphericFog *fog = new AtmosphericFog;
    parseHandle( ctx, record, fog );
    return fog;
    break;
  }
    //-- AtmosphericGround
  case ATMOSPHERIC_GROUND: {
    AtmosphericGround *ground = new AtmosphericGround;
    parseHandle( ctx, record, ground );
    return ground;
    break;
  }
    //-- AtmosphericVolumetric
  case ATMOSPHERIC_VOLUMETRIC: {
    AtmosphericVolumetric *volumetric = new AtmosphericVolumetric;
    parseHandle( ctx, record, volumetric );
    return volumetric;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown atmospheric type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

//--TODOTHandle *TInterior::parseSubclass( Context &ctx, RAYRecord *record ) {
//--TODO#ifdef __DEBUG__
//--TODO  printf("TInterior::parseSubclass\n"); fflush(stdout);
//--TODO  // record->handleName: kind of handle
//--TODO  if ( record->handleName )
//--TODO    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
//--TODO  // record->subHandleName: name of handle subclass or already defined handle
//--TODO  if ( record->subHandleName )
//--TODO      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
//--TODO#endif
//--TODO  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
//--TODO  // if no subclass: normal interior
//--TODO  if ( record->subHandleName == NULL ) {
//--TODO    TInterior *interior = new TInterior;
//--TODO    parseHandle( ctx, record, interior );
//--TODO    return interior;
//--TODO  }
//--TODO  int interiorKind = TTagList::findId( record->subHandleName->image, interiorSubClassTags );
//--TODO#ifdef __DEBUG__
//--TODO  printf("interiorKind: %d\n", interiorKind ); fflush(stdout);
//--TODO#endif
//--TODO  switch ( interiorKind ) {
//--TODO    //-- Interior
//--TODO  case INTERIOR_SIMPLE: {
//--TODO    TInterior *interior = new TInterior;
//--TODO    parseHandle( ctx, record, interior );
//--TODO    return interior;
//--TODO    break;
//--TODO  }
//--TODO    //-- InteriorPattern
//--TODO  case INTERIOR_PATTERN: {
//--TODO    InteriorPattern *pattern = new InteriorPattern;
//--TODO    parseHandle( ctx, record, pattern );
//--TODO    return pattern;
//--TODO    break;
//--TODO  }
//--TODO    // already declared handle  TODO
//--TODO  default: {
//--TODO    p_error("Unknown interior type [%s]", record->subHandleName->image );
//--TODO    return NULL;
//--TODO    break;
//--TODO  }
//--TODO  }
//--TODO  return NULL;
//--TODO}
//--TODO
//--TODOTHandle *TFinish::parseSubclass( Context &ctx, RAYRecord *record ) {
//--TODO#ifdef __DEBUG__
//--TODO  printf("TFinish::parseSubclass\n"); fflush(stdout);
//--TODO  // record->handleName: kind of handle
//--TODO  if ( record->handleName )
//--TODO    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
//--TODO  // record->subHandleName: name of handle subclass or already defined handle
//--TODO  if ( record->subHandleName )
//--TODO      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
//--TODO#endif
//--TODO  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
//--TODO  // if no subclass: normal finish
//--TODO  if ( record->subHandleName == NULL ) {
//--TODO    TFinish *finish = new TFinish;
//--TODO    parseHandle( ctx, record, finish );
//--TODO    return finish;
//--TODO  }
//--TODO  int finishKind = TTagList::findId( record->subHandleName->image, finishSubClassTags ); 
//--TODO#ifdef __DEBUG__
//--TODO  printf("finishKind: %d\n", finishKind ); fflush(stdout);
//--TODO#endif
//--TODO  switch ( finishKind ) {
//--TODO    //-- Finish
//--TODO  case FINISH_SIMPLE: {
//--TODO    TFinish *finish = new TFinish;
//--TODO    parseHandle( ctx, record, finish );
//--TODO    return finish;
//--TODO    break;
//--TODO  }
//--TODO    //-- FinishPattern
//--TODO  case FINISH_PATTERN: {
//--TODO    FinishPattern *pattern = new FinishPattern;
//--TODO    parseHandle( ctx, record, pattern );
//--TODO    return pattern;
//--TODO    break;
//--TODO  }
//--TODO    // already declared handle  TODO
//--TODO  default: {
//--TODO    p_error("Unknown finish type [%s]", record->subHandleName->image );
//--TODO    return NULL;
//--TODO    break;
//--TODO  }
//--TODO  }
//--TODO  return NULL;
//--TODO}
//--TODO
//--TODOTHandle *TTexture::parseSubclass( Context &ctx, RAYRecord *record ) {
//--TODO#ifdef __DEBUG__
//--TODO  printf("TTexture::parseSubclass\n"); fflush(stdout);
//--TODO  // record->handleName: kind of handle
//--TODO  if ( record->handleName )
//--TODO    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
//--TODO  // record->subHandleName: name of handle subclass or already defined handle
//--TODO  if ( record->subHandleName )
//--TODO      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
//--TODO#endif
//--TODO  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
//--TODO  // if no subclass: normal texture
//--TODO  if ( record->subHandleName == NULL ) {
//--TODO    TTexture *texture = new TTexture;
//--TODO    parseHandle( ctx, record, texture );
//--TODO    return texture;
//--TODO  }
//--TODO  int textureKind = TTagList::findId( record->subHandleName->image, textureSubClassTags );
//--TODO#ifdef __DEBUG__
//--TODO  printf("textureKind: %d\n", textureKind ); fflush(stdout);
//--TODO#endif
//--TODO  switch ( textureKind ) {
//--TODO    //-- Texture
//--TODO  case TEXTURE_SIMPLE: {
//--TODO    TTexture *texture = new TTexture;
//--TODO    parseHandle( ctx, record, texture );
//--TODO    return texture;
//--TODO    break;
//--TODO  }
//--TODO    //-- TexturePattern
//--TODO  case TEXTURE_PATTERN: {
//--TODO    TexturePattern *pattern = new TexturePattern;
//--TODO    parseHandle( ctx, record, pattern );
//--TODO    return pattern;
//--TODO    break;
//--TODO  }
//--TODO    // already declared handle  TODO
//--TODO  default: {
//--TODO    p_error("Unknown texture type [%s]", record->subHandleName->image );
//--TODO    return NULL;
//--TODO    break;
//--TODO  }
//--TODO  }
//--TODO  return NULL;
//--TODO}

THandle *TMover::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TMover::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Mover type must be specified!");
  int moverKind = TTagList::findId( record->subHandleName->image, moverSubClassTags );
#ifdef __DEBUG__
  printf("moverKind: %d\n", moverKind ); fflush(stdout);
#endif
  switch ( moverKind ) {
    //-- MoverAddV
  case MOVER_ADDV: {
    MoverAddV *add = new MoverAddV;
    parseHandle( ctx, record, add );
    return add;
    break;
  }
    //-- MoverCircle
  case MOVER_CIRCLE: {
    MoverCircle *circle = new MoverCircle;
    parseHandle( ctx, record, circle );
    return circle;
    break;
  }
    //-- MoverLinearF
  case MOVER_LINEAR: {
    MoverLinearF *linear = new MoverLinearF;
    parseHandle( ctx, record, linear );
    return linear;
    break;
  }
    //-- MoverBounce
  case MOVER_BOUNCE: {
    MoverBounce *bounce = new MoverBounce;
    parseHandle( ctx, record, bounce );
    return bounce;
    break;
  }
    //-- MoverLissajou
  case MOVER_LISSAJOU: {
    MoverLissajou *lissajou = new MoverLissajou;
    parseHandle( ctx, record, lissajou );
    return lissajou;
    break;
  }
    //-- MoverBezier
  case MOVER_BEZIER: {
    MoverBezier *bezier = new MoverBezier;
    parseHandle( ctx, record, bezier );
    return bezier;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown mover type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

THandle *TNormal::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TNormal::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  // if no subclass: normal normal
  if ( record->subHandleName == NULL ) {
    TNormal *normal = new TNormal;
    parseHandle( ctx, record, normal );
    return normal;
  }
  int normalKind = TTagList::findId( record->subHandleName->image, normalSubClassTags ); 
#ifdef __DEBUG__
  printf("normalKind: %d\n", normalKind ); fflush(stdout);
#endif
  switch ( normalKind ) {
//--TODO    //-- Normal
//--TODO  case NORMAL_SIMPLE: {
//--TODO    TNormal *normal = new TNormal;
//--TODO    parseHandle( ctx, record, normal );
//--TODO    return normal;
//--TODO    break;
//--TODO  }
    //-- NormalBumps
  case NORMAL_BUMPS: {
    NormalBumps *bumps = new NormalBumps;
    parseHandle( ctx, record, bumps );
    return bumps;
    break;
  }
    //-- NormalTurbulence
  case NORMAL_TURBULENCE: {
    NormalTurbulence *turbulence = new NormalTurbulence;
    parseHandle( ctx, record, turbulence );
    return turbulence;
    break;
  }
//--TODO    //-- NormalPattern
//--TODO  case NORMAL_PATTERN: {
//--TODO    NormalPattern *pattern = new NormalPattern;
//--TODO    parseHandle( ctx, record, pattern );
//--TODO    return pattern;
//--TODO    break;
//--TODO  }
//--TODO    //-- NormalFunction
//--TODO  case NORMAL_FUNCTION: {
//--TODO    NormalFunction *function = new NormalFunction;
//--TODO    parseHandle( ctx, record, function );
//--TODO    return function;
//--TODO    break;
//--TODO  }
    // already declared handle  TODO
  default: {
    p_error("Unknown normal type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

THandle *MetaSubObject::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("MetaSubObject::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Meta type must be specified!");
  int metaKind = TTagList::findId( record->subHandleName->image, metaSubClassTags );
#ifdef __DEBUG__
  printf("metaKind: %d\n", metaKind ); fflush(stdout);
#endif
  switch ( metaKind ) {
    //-- MetaPlane
  case META_PLANE: {
    MetaPlane *plane = new MetaPlane;
    parseHandle( ctx, record, plane );
    return plane;
    break;
  }
    //-- MetaSphere
  case META_SPHERE: {
    MetaSphere *sphere = new MetaSphere;
    parseHandle( ctx, record, sphere );
    return sphere;
    break;
  }
    //-- MetaCylinder
  case META_CYLINDER: {
    MetaCylinder *cylinder = new MetaCylinder;
    parseHandle( ctx, record, cylinder );
    return cylinder;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown meta type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

THandle *BlobSubObject::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("BlobSubObject::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Blob type must be specified!");
  int blobKind = TTagList::findId( record->subHandleName->image, blobSubClassTags );
#ifdef __DEBUG__
  printf("blobKind: %d\n", blobKind ); fflush(stdout);
#endif
  switch ( blobKind ) {
    //-- BlobSphere
  case BLOB_SPHERE: {
    BlobSphere *sphere = new BlobSphere;
    parseHandle( ctx, record, sphere );
    return sphere;
    break;
  }
    //-- BlobCylinder
  case BLOB_CYLINDER: {
    BlobCylinder *cylinder = new BlobCylinder;
    parseHandle( ctx, record, cylinder );
    return cylinder;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown blob type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}
THandle *TTurbulence::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TTurbulence::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Turbulence type must be specified!");
  int turbulenceKind = TTagList::findId( record->subHandleName->image, turbulenceSubClassTags );
#ifdef __DEBUG__
  printf("turbulenceKind: %d\n", turbulenceKind ); fflush(stdout);
#endif
  switch ( turbulenceKind ) {
    //-- TurbulenceNoise
  case TURBULENCE_NOISE: {
    TurbulenceNoise *noise = new TurbulenceNoise;
    parseHandle( ctx, record, (TTurbulence*)noise );
    return (TTurbulence*)noise;
    break;
  }
    //-- TurbulenceSphere
  case TURBULENCE_PERLIN: {
    TurbulencePerlin *perlin = new TurbulencePerlin;
    parseHandle( ctx, record, (TTurbulence*)perlin );
    return (TTurbulence*)perlin;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown turbulence type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}
THandle *TCamera::parseSubclass( Context &ctx, RAYRecord *record ) {
#ifdef __DEBUG__
  printf("TCamera::parseSubclass\n"); fflush(stdout);
  // record->handleName: kind of handle
  if ( record->handleName )
    printf("handle: %s\n", record->handleName->image ); fflush( stdout );
  // record->subHandleName: name of handle subclass or already defined handle
  if ( record->subHandleName )
      printf("subHandle: %s\n", record->subHandleName->image ); fflush( stdout );
#endif
  // get subclass kind (in fact, it can be  a handle subclass   or   an already declared handle)
  if ( record->subHandleName == NULL )
    p_error("A Camera type must be specified!");
  int cameraKind = TTagList::findId( record->subHandleName->image, cameraSubClassTags );
#ifdef __DEBUG__
  printf("cameraKind: %d\n", cameraKind ); fflush(stdout);
#endif
  switch ( cameraKind ) {
    //-- CameraClassic
  case CAMERA_CLASSIC: {
    CameraClassic *classic = new CameraClassic;
    parseHandle( ctx, record, (TCamera*)classic );
    return (TCamera*)classic;
    break;
  }
    //-- CameraPerspective
  case CAMERA_PERSPECTIVE: {
    CameraPerspective *perspective = new CameraPerspective;
    parseHandle( ctx, record, (TCamera*)perspective );
    return (TCamera*)perspective;
    break;
  }
    // already declared handle  TODO
  default: {
    p_error("Unknown camera type [%s]", record->subHandleName->image );
    return NULL;
    break;
  }
  }
  return NULL;
}

//--------------------------
//----- One field parsing
//--------------------------
//---- TWorld
bool TWorld::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, worldTags );
  switch( tagId ) {
    // objects: list of TObject
  case TAG_world_objects: {
    ValueList *l = field->value->eval( ctx ).asList();
    //--    numObjects = l->size;
    //--    objects = new TPrimitive *[numObjects];
    //--    for ( int i = 0; i < numObjects; i++ ) {
    for ( int i = 0; i < l->size; i++ ) {
      THandle *handle = l->elems[i].asHandle();
      if ( handle->handleKind != THandle::HANDLE_OBJECT )
	p_error("An Object was expected in world.objects!");
      //--      objects[i] = (TPrimitive*)handle;
      objects.push_back( (TObject*)handle );
    }
    break;
  }
    // cameras: list of TCamera
  case TAG_world_cameras: {
    ValueList *l = field->value->eval( ctx ).asList();
    //--    numCameras = l->size;
    //--    cameras = new TCamera *[numCameras];
    //--    for ( int i = 0; i < numCameras; i++ ) {
    for ( int i = 0; i < l->size; i++ ) {
      THandle *handle = l->elems[i].asHandle();
      if ( handle->handleKind != THandle::HANDLE_CAMERA )
	p_error("A Camera was expected in world.cameras!");
      //--      cameras[i] = (TCamera*)handle;
      cameras.push_back( (TCamera*)handle );
    }
    break;
  }
    // lights: list of TLight
  case TAG_world_lights: {
    ValueList *l = field->value->eval( ctx ).asList();
    //--    numLights = l->size;
    //--    lights = new TLight *[numLights];
    //--    for ( int i = 0; i < numLights; i++ ) {
    for ( int i = 0; i < l->size; i++ ) {
      THandle *handle = l->elems[i].asHandle();
      if ( handle->handleKind != THandle::HANDLE_LIGHT )
	p_error("A Light was expected in world.lights!");
      //--      lights[i] = (TLight*)handle;
      lights.push_back( (TLight*)handle );
    }
    break;
  }
    // currentCamera: pointer to TCamera
  case TAG_world_currentCamera: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_CAMERA )
      p_error("A Camera was expected in world.currentCamera!");
    currentCamera = (TCamera*)handle;
    break;
  }
    // ambient: TAmbient
  case TAG_world_ambient: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_AMBIENT )
      p_error("An Ambient was expected in world.ambient!");
    ambient = (TAmbient*)handle;
    break;
  }
    // background: TPigment
  case TAG_world_background: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_PIGMENT )
      p_error("A Pigment was expected in world.background!");
    background = (TPigment*)handle;
    break;
  }
    // atmospheric: TAtmospheric
  case TAG_world_atmospheric: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_ATMOSPHERIC )
      p_error("An Atmospheric was expected in world.atmospheric!");
    atmospheric = (TAtmospheric*)handle;
    break;
  }
    // atmosphericIOR: float
  case TAG_world_atmosphereIOR: atmosphereIOR = field->value->eval( ctx ).asFloat(); break;
    // animator: TAnimator
  case TAG_world_animator: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_ANIMATOR )
      p_error("An Animator was expected in world.animator!");
    animator = (TAnimator*)handle;
    break;
  }
    // default: error
    //--  default: p_error("Unknown World token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}

//----THandle
bool THandle::parse( Context &ctx, RAYField *field ) {
  p_error("Handle doesn't have specific fields!!");
  return false;
}

//----TObject
bool TObject::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, objectTags );
  switch( tagId ) {
    // primitive: TPrimitive
  case TAG_object_primitive: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_PRIMITIVE )
      p_error("A Primitive was expected in object.primitive!");
    primitive = (TPrimitive*)handle;
    break;
  }
    // bounding: TBounding
  case TAG_object_bounding: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_BOUNDING )
      p_error("A Bounding was expected in object.bounding!");
    bounding = (TBounding*)handle;
    break;
  }
    // texture: TTexture
  case TAG_object_texture: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_TEXTURE )
      p_error("A Texture was expected in object.texture!");
    texture = (TTexture*)handle;
    break;
  }
  default: return false;
  }
  return true;
}

//----TTexture
bool TTexture::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, textureTags );
  switch( tagId ) {
    // pigment: TPigment
  case TAG_texture_pigment: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_PIGMENT )
      p_error("A Pigment was expected in texture.pigment!");
    pigment = (TPigment*)handle;
    break;
  }
    // finish: TFinish
  case TAG_texture_finish: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_FINISH )
      p_error("A Finish was expected in texture.finish!");
    finish = (TFinish*)handle;
    break;
  }
    // interior: TInterior
  case TAG_texture_interior: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_INTERIOR )
      p_error("An Interior was expected in texture.interior!");
    interior = (TInterior*)handle;
    break;
  }
    // normal: TNormal
  case TAG_texture_normal: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_NORMAL )
      p_error("An Normal was expected in texture.normal!");
    normal = (TNormal*)handle;
    break;
  }
    // default: error
    //--  default: p_error("Unknown Texture token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}
//--TODObool TexturePattern::parse( Context &ctx, RAYField *field ) {
//--TODO  const char *tagName = field->fieldName->image;
//--TODO  int tagId = TTagList::findId( tagName, texturePatternTags );
//--TODO  switch( tagId ) {
//--TODO    // pattern: TPattern
//--TODO  case TAG_texturePattern_pattern: {
//--TODO    THandle *handle = field->value->eval( ctx ).asHandle();
//--TODO    if ( handle->handleKind != THandle::HANDLE_PATTERN )
//--TODO      p_error("A Pattern was expected in pattern.pattern!");
//--TODO    pattern = (TPattern*)handle;
//--TODO    pattern->storedHandleKind = THandle::HANDLE_TEXTURE;
//--TODO    break;
//--TODO  }
//--TODO    // default: no need to call TTexture::parse because we'll only use pattern as texture
//--TODO  default: {
//--TODO    //--      p_error("Unknown Texture::Pattern token: %s\n", tagName );
//--TODO      return false;
//--TODO    }
//--TODO  }
//--TODO  return true;
//--TODO}

//----TInterior
bool TInterior::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, interiorTags );
  switch( tagId ) {
    // IOR: float
  case TAG_interior_ior: ior = field->value->eval( ctx ).asFloat(); break;
	// Absorption: TColor
  case TAG_interior_absorption: absorption = field->value->eval( ctx ).asColor(); break;
    // default: error
    //--  default: p_error("Unknown Interior token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}
//--TODObool InteriorPattern::parse( Context &ctx, RAYField *field ) {
//--TODO  const char *tagName = field->fieldName->image;
//--TODO  int tagId = TTagList::findId( tagName, interiorPatternTags );
//--TODO  switch( tagId ) {
//--TODO    // pattern: TPattern
//--TODO  case TAG_interiorPattern_pattern: {
//--TODO    THandle *handle = field->value->eval( ctx ).asHandle();
//--TODO    if ( handle->handleKind != THandle::HANDLE_PATTERN )
//--TODO      p_error("A Pattern was expected in pattern.pattern!");
//--TODO    pattern = (TPattern*)handle;
//--TODO    pattern->storedHandleKind = THandle::HANDLE_INTERIOR;
//--TODO    break;
//--TODO  }
//--TODO    // default: no need to call TInterior::parse because we'll only use pattern as interior
//--TODO  default: {
//--TODO    //--      p_error("Unknown Interior::Pattern token: %s\n", tagName );
//--TODO      return false;
//--TODO    }
//--TODO  }
//--TODO  return true;
//--TODO}

//----TFinish
bool TFinish::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, finishTags );
  switch( tagId ) {
    // reflection: float
  case TAG_finish_reflection: reflectionFactor = field->value->eval( ctx ).asFloat(); break;
    // transmission: float
  case TAG_finish_transmission: transmissionFactor = field->value->eval( ctx ).asFloat(); break;
    // ambient: float
  case TAG_finish_ambient: ambientFactor = field->value->eval( ctx ).asFloat(); break;
    // diffuse: float
  case TAG_finish_diffuse: diffuseFactor = field->value->eval( ctx ).asFloat(); break;
    // specular: float
  case TAG_finish_specular: specularFactor = field->value->eval( ctx ).asFloat(); break;
    // roughness: float
  case TAG_finish_roughness: specularRoughness = field->value->eval( ctx ).asFloat(); break;
    // phong: float
  case TAG_finish_phong: phongFactor = field->value->eval( ctx ).asFloat(); break;
    // phongSize: float
  case TAG_finish_phongSize: phongSize = field->value->eval( ctx ).asFloat(); break;
    //--TODO    // metallic: float
    //--TODO  case TAG_finish_metallic: metallic = field->value->eval( ctx ).asFloat(); break;
    // brilliance: float
  case TAG_finish_brilliance: brilliance = field->value->eval( ctx ).asFloat(); break;
    // default: error
    //--  default: p_error("Unknown Finish token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}
//--TODObool FinishPattern::parse( Context &ctx, RAYField *field ) {
//--TODO  const char *tagName = field->fieldName->image;
//--TODO  int tagId = TTagList::findId( tagName, finishPatternTags );
//--TODO  switch( tagId ) {
//--TODO    // pattern: TPattern
//--TODO  case TAG_finishPattern_pattern: {
//--TODO    THandle *handle = field->value->eval( ctx ).asHandle();
//--TODO    if ( handle->handleKind != THandle::HANDLE_PATTERN )
//--TODO      p_error("A Pattern was expected in pattern.pattern!");
//--TODO    pattern = (TPattern*)handle;
//--TODO    pattern->storedHandleKind = THandle::HANDLE_FINISH;
//--TODO    break;
//--TODO  }
//--TODO    // default: no need to call TFinish::parse because we'll only use pattern as finish
//--TODO  default: {
//--TODO    //--      p_error("Unknown Finish::Pattern token: %s\n", tagName );
//--TODO      return false;
//--TODO    }
//--TODO  }
//--TODO  return true;
//--TODO}

//----TCamera
bool TCamera::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, cameraTags );
  switch( tagId ) {
    // location: TVector3
  case TAG_camera_location: location = field->value->eval( ctx ).asVector3(); break;
    // lookat: TVector3
  case TAG_camera_lookat: lookat = field->value->eval( ctx ).asVector3(); break;
    // vertical: TVector3
  case TAG_camera_vertical: vertical = field->value->eval( ctx ).asVector3(); break;
    // fov: float
  case TAG_camera_FOV: fov = field->value->eval( ctx ).asFloat(); break;
    // default: error
    //--  default: p_error("Unknown Camera token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}
bool CameraClassic::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, cameraClassicTags );
  switch( tagId ) {
    // default: TCamera::parse
  default: return TCamera::parse( ctx, field );
  }
  return true;
}
bool CameraPerspective::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, cameraPerspectiveTags );
  switch( tagId ) {
    // default: TCamera::parse
  default: return TCamera::parse( ctx, field );
  }
  return true;
}

//----TPrimitive
bool TPrimitive::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveTags );
  switch( tagId ) {
    // invalid tag: error
  case TAG_INVALID: return false;
    //--  case TAG_INVALID: p_error("Unknown Primitive token: %s\n", tagName ); return false;
    // valid tag
  default: additionalInformation.setBit( tagId );
  }
  return true;
}
bool PrimitivePlane::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitivePlaneTags );
  switch( tagId ) {
    // normal: TVector3
  case TAG_primitivePlane_normal: m_Normal = field->value->eval( ctx ).asVector3(); break;
    // distanceOrigin: float
  case TAG_primitivePlane_distance: m_Distance = field->value->eval( ctx ).asFloat(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveSphere::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveSphereTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveSphere_center: m_Center = field->value->eval( ctx ).asVector3(); break;
    // radius: float
  case TAG_primitiveSphere_radius: m_Radius = field->value->eval( ctx ).asFloat(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveBox::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveBoxTags );
  switch( tagId ) {
    // min: TVector3
  case TAG_primitiveBox_min: m_Min = field->value->eval( ctx ).asVector3(); break;
    // max: TVector3
  case TAG_primitiveBox_max: m_Max = field->value->eval( ctx ).asVector3(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}

bool PrimitiveTorus::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveTorusTags );
  switch( tagId ) {
//--TODO    // angle: float
//--TODO  case TAG_primitiveTorus_angle: vangle = field->value->eval( ctx ).asFloat(); break;
//--TODO    // ustart: float
//--TODO  case TAG_primitiveTorus_ustart: ustart = field->value->eval( ctx ).asFloat(); break;
//--TODO    // ustop: float
//--TODO  case TAG_primitiveTorus_ustop: ustop = field->value->eval( ctx ).asFloat(); break;
    // radius1: float
  case TAG_primitiveTorus_radius1: R1 = field->value->eval( ctx ).asFloat(); break;
    // radius2: float
  case TAG_primitiveTorus_radius2: R2 = field->value->eval( ctx ).asFloat(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool BlobSubObject::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, blobTags );
  switch( tagId ) {
    // origin: TVector3
  case TAG_blob_origin: origin = field->value->eval( ctx ).asVector3(); break;
    // strength: float
  case TAG_blob_strength: c[2] = field->value->eval( ctx ).asFloat(); break;
    // default: error
    //--  default: p_error("Unknown BlobSubObject token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}
bool BlobSphere::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, blobSphereTags );
  switch( tagId ) {
    // radius2: float
  case TAG_blobsphere_radius2: radius2 = field->value->eval( ctx ).asFloat(); break;
    // default: BlobSubObject::parse
  default: return BlobSubObject::parse( ctx, field );
  }
  return true;
}
bool BlobCylinder::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, blobCylinderTags );
  switch( tagId ) {
    // radius2: float
  case TAG_blobcylinder_radius2: radius2 = field->value->eval( ctx ).asFloat(); break;
    // length: float
  case TAG_blobcylinder_len: len = field->value->eval( ctx ).asFloat(); break;
    // default: BlobSubObject::parse
  default: return BlobSubObject::parse( ctx, field );
  }
  return true;
}
bool PrimitiveBlob::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveBlobTags );
  switch( tagId ) {
    // blobs: list of BlobSubObject
  case TAG_primitiveBlob_blobs: {
    ValueList *l = field->value->eval( ctx ).asList();
    for ( int i = 0; i < l->size; i++ ) {
      THandle *handle = l->elems[i].asHandle();
      if ( handle->handleKind != THandle::HANDLE_BLOB )
	p_error("A blob was expected in blob.blobs!");
      blobs.push_back( (BlobSubObject*)handle );
    }
    break;
  }
    // threshold: float
  case TAG_primitiveBlob_threshold: threshold = field->value->eval( ctx ).asFloat(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveCylinder::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveCylinderTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveCylinder_center: center = field->value->eval( ctx ).asVector3(); break;
    // axis: TVector3
  case TAG_primitiveCylinder_axis: axis = field->value->eval( ctx ).asVector3(); break;
    // radius: float
  case TAG_primitiveCylinder_radius: radius = field->value->eval( ctx ).asFloat(); break;
    // height: float (+ capped set to true)
  case TAG_primitiveCylinder_height: height = field->value->eval( ctx ).asFloat(); capped = true; break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveCapsule::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveCapsuleTags );
  switch( tagId ) {
    // p1: TVector3
  case TAG_primitiveCapsule_p1: p1 = field->value->eval( ctx ).asVector3(); break;
    // p2: TVector3
  case TAG_primitiveCapsule_p2: p2 = field->value->eval( ctx ).asVector3(); break;
    // radius: float
  case TAG_primitiveCapsule_radius: radius = field->value->eval( ctx ).asFloat(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveCSG::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveCsgTags );
  switch( tagId ) {
    // operation: string
  case TAG_primitiveCsg_operation: {
    const char *s = field->value->eval( ctx ).asStr();
    int op = TTagList::findId( s, primitiveCsgOperations );
    if ( op < 0 )
      p_error("Invalid CSG operation!");
    operation = op;
    break;
  }
    // primitives: list of TPrimitive
  case TAG_primitiveCsg_primitiveList: {
    ValueList *l = field->value->eval( ctx ).asList();
    for ( int i = 0; i < l->size; i++ ) {
      // list element can be a couple (TPrimitive, boolean) or a TPrimitive
      if ( l->elems[i].typ == SVT_LIST ) {
	ValueList *couple = l->elems[i].asList();
	if ( couple->size != 2 )
	  p_error("A couple (primitive, boolean) was expected in CSG.primitives!");
	THandle *handle = couple->elems[0].asHandle();
	if ( handle->handleKind != THandle::HANDLE_PRIMITIVE )
	  p_error("A primitive was expected in couple of CSG.primitives!");
	TPrimitive *primitive = (TPrimitive*)handle;
	bool inverse = couple->elems[1].asBool();
	if ( inverse )
	  primitive->additionalInformation.setBit( PRIMITIVE_INVERSE );
	else
	  primitive->additionalInformation.removeBit( PRIMITIVE_INVERSE );
	primitiveList.push_back( primitive );
      }
      else {
	THandle *handle = l->elems[i].asHandle();
	if ( handle->handleKind != THandle::HANDLE_PRIMITIVE )
	  p_error("A primitive was expected in CSG.primitives!");
	primitiveList.push_back( (TPrimitive*)handle );
      }
    }
    break;
  }
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveHeart::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveHeartTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveHeart_center: center = field->value->eval( ctx ).asVector3(); break;
    // radius: float
  case TAG_primitiveHeart_radius: radius = field->value->eval( ctx ).asFloat(); break;
    // step: float
  case TAG_primitiveHeart_step: tStep = field->value->eval( ctx ).asFloat(); break;
    // refinement: int
  case TAG_primitiveHeart_refinement: refinementSteps = field->value->eval( ctx ).asInt(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveQuadric::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveQuadricTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveQuadric_center: center = field->value->eval( ctx ).asVector3(); break;
    // radius: float
  case TAG_primitiveQuadric_radius: radius = field->value->eval( ctx ).asFloat(); break;
    // matrix: TMatrix3
  case TAG_primitiveQuadric_A: {
    ValueList *l = field->value->eval( ctx ).asList();
    if ( l->size != 3 )
      p_error("A 3x3 matrix was expected in quadric.matrix!");
    for ( int i = 0; i < 3; i++ ) {
      ValueList *row = l->elems[i].asList();
      if ( row->size != 3 )
	p_error("A 3x3 matrix was expected in quadric.matrix!");
      for ( int j = 0; j < 3; j++ )
	A.m[j][i] = row->elems[j].asFloat();
    }
    break;
  }
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveJulia::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveJuliaTags );
  switch( tagId ) {
    // param: TVector4
  case TAG_primitiveJulia_param: juliaParm = field->value->eval( ctx ).asVector4(); break;
    // slice: couple ( TVector4, float )
  case TAG_primitiveJulia_slice: {
    ValueList *couple = field->value->eval( ctx ).asList();
    if ( couple->size != 2 )
      p_error("A couple (vector4, float) was expected in julia.slice!");
    slice = couple->elems[0].asVector4();
    sliceDist = couple->elems[1].asFloat();
    break;
  }
    // exitValue: float
  case TAG_primitiveJulia_exitValue: exitValue = field->value->eval( ctx ).asFloat(); break;
    // n: int
  case TAG_primitiveJulia_n: n = field->value->eval( ctx ).asInt(); break;
    // precision: float
  case TAG_primitiveJulia_precision: precision = field->value->eval( ctx ).asFloat(); break;
    // center: TVector3
  case TAG_primitiveJulia_center: center = field->value->eval( ctx ).asVector3(); break;
    // radius: float
  case TAG_primitiveJulia_radius2: radius2 = SQR(field->value->eval( ctx ).asFloat()); break;
    //--    // exponent: TVector2
    //--  case TAG_primitiveJulia_exponent: exponent = field->value->eval( ctx ).asVector2(); break;
    // type: string
  case TAG_primitiveJulia_type: {
    const char *s = field->value->eval( ctx ).asStr();
    int typeId = TTagList::findId( s, primitiveJuliaTypes );
    if ( typeId < 0 )
      p_error("Invalid julia type!");
    algebra = typeId;
    break;
  }
    // subtype: string
  case TAG_primitiveJulia_subType: {
    const char *s = field->value->eval( ctx ).asStr();
    int subTypeId = TTagList::findId( s, primitiveJuliaSubTypes );
    if ( subTypeId < 0 )
      p_error("Invalid julia sub-type!");
    subType = subTypeId;
    break;
  }
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveTransformed::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveTransformedTags );
  switch( tagId ) {
    // primitive: TPrimitive
  case TAG_primitiveTransformed_primitive: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_PRIMITIVE )
      p_error("A primitive was expected in transformed.primitive!");
    primitive = (TPrimitive*)handle;
    break;
  }
    // transformation: TMatrix4
  case TAG_primitiveTransformed_transformation: {
    ValueList *l = field->value->eval( ctx ).asList();
    if ( l->size != 4 )
      p_error("A 4x4 matrix was expected in transformed.transformation!");
    for ( int j = 0; j < 4; j++ ) {
      ValueList *row = l->elems[j].asList();
      if ( row->size != 4 )
	p_error("A 4x4 matrix was expected in transformed.transformation!");
      for ( int i = 0; i < 4; i++ )
	transform.matrix.m[j][i] = row->elems[i].asFloat();
    }
    transform.inverse.invert( transform.matrix );
    break;
  }
    // rotate: TVector3
  case TAG_primitiveTransformed_rotate: {
    TTransform localTransform;
    localTransform.computeRotationTransform( field->value->eval( ctx ).asVector3() );
    transform.composeTransforms( localTransform );
    break;
  }
    // translate: TVector3
  case TAG_primitiveTransformed_translate: {
    TTransform localTransform;
    localTransform.computeTranslationTransform( field->value->eval( ctx ).asVector3() );
    transform.composeTransforms( localTransform );
    break;
  }
    // scale: TVector3
  case TAG_primitiveTransformed_scale: {
    TTransform localTransform;
    localTransform.computeScalingTransform( field->value->eval( ctx ).asVector3() );
    transform.composeTransforms( localTransform );
    break;
  }
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool MetaSubObject::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, metaTags );
  switch( tagId ) {
    // loc: TVector3
  case TAG_meta_loc: loc = field->value->eval( ctx ).asVector3(); break;
    // strength: float
  case TAG_meta_strength: strength = field->value->eval( ctx ).asFloat(); break;
    // default: error
  default: return false;
  }
  return true;
}
bool MetaSphere::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, metaSphereTags );
  switch( tagId ) {
    // default: MetaSubObject::parse
  default: return MetaSubObject::parse( ctx, field );
  }
  return true;
}
bool MetaCylinder::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, metaCylinderTags );
  switch( tagId ) {
    // default: MetaSubObject::parse
  default: return MetaSubObject::parse( ctx, field );
  }
  return true;
}
bool MetaPlane::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, metaPlaneTags );
  switch( tagId ) {
    // slant: float
  case TAG_metaplane_slant: slant = field->value->eval( ctx ).asFloat(); break;
    // default: MetaSubObject::parse
  default: return MetaSubObject::parse( ctx, field );
  }
  return true;
}
bool PrimitiveMetaObjects::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveMetaTags );
  switch( tagId ) {
    // threshold: float
  case TAG_primitiveMeta_threshold: threshold = field->value->eval( ctx ).asFloat(); break;
    // subs: list of MetaSubObject
  case TAG_primitiveMeta_subs: {
    ValueList *l = field->value->eval( ctx ).asList();
    for ( int i = 0; i < l->size; i++ ) {
      THandle *handle = l->elems[i].asHandle();
      if ( handle->handleKind != THandle::HANDLE_META )
	p_error("A meta was expected in meta.meta!");
      subs[num_subs++] = (MetaSubObject*)handle;
    }
    break;
  }
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveSuperellipsoid::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveSuperellipsoidTags );
  switch( tagId ) {
    // e: float
  case TAG_primitiveSuperellipsoid_e: e = field->value->eval( ctx ).asFloat(); break;
    // n: float
  case TAG_primitiveSuperellipsoid_n: n = field->value->eval( ctx ).asFloat(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveSor::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveSorTags );
  switch( tagId ) {
    // points: list of TVector2
  case TAG_primitiveSor_points: {
    ValueList *l = field->value->eval( ctx ).asList();
    number = l->size;
    TVector2 *pts = new TVector2 [l->size];
    for ( int i = 0; i < l->size; i++ )
      pts[i] = l->elems[i].asVector2();
    create( pts ); // create spline segments from list of points
    delete [] pts;
    break;
  }
//--TODO    // open: bool
//--TODO  case TAG_primitiveSor_open: n = field->value->eval( ctx ).asFloat(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}
bool PrimitiveCone::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, primitiveConeTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_primitiveCone_center: center = field->value->eval( ctx ).asVector3(); break;
    // axis: TVector3
  case TAG_primitiveCone_axis: axis = field->value->eval( ctx ).asVector3(); break;
    // angle: float
  case TAG_primitiveCone_angle: angle = field->value->eval( ctx ).asFloat(); break;
    // default: TPrimitive::parse
  default:
    return TPrimitive::parse( ctx, field );
  }
  return true;
}

//----TLight
bool TLight::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, lightTags );
  switch( tagId ) {
    // color: TColor
  case TAG_light_color: color = field->value->eval( ctx ).asColor(); break;
    // brightness: float
  case TAG_light_brightness: brightness = field->value->eval( ctx ).asFloat(); break;
//--TODO    // powerOn: boolean
//--TODO  case TAG_light_powerOn: powerOn = field->value->eval( ctx ).asBool(); break;
//--TODO    // looksLike: TPrimitive
//--TODO  case TAG_light_looksLike: {
//--TODO    THandle *handle = field->value->eval( ctx ).asHandle();
//--TODO    if ( handle->handleKind != THandle::HANDLE_PRIMITIVE )
//--TODO      p_error("A Primitive was expected in light.looksLike!");
//--TODO    looksLike = (TPrimitive*)handle;
//--TODO    looksLike->additional.setBit( TPrimitive::PRIMITIVE_CAST_NO_SHADOW );
//--TODO    looksLike->additional.setBit( TPrimitive::PRIMITIVE_CAST_NO_SELF_SHADOW );
//--TODO    looksLike->additional.setBit( TPrimitive::PRIMITIVE_NO_LIGHTING );
//--TODO    break;
//--TODO  }
//--TODO    // noShadow: boolean
//--TODO  case TAG_light_no_shadow: additional.setBit( LIGHT_CAST_NO_SHADOW ); break;
    // volume: TGeometry
  case TAG_light_volume: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_PRIMITIVE )
      p_error("A primitive was expected in light.volume!");
    volume = (TPrimitive*)handle;
    break;
  }
    // default: error
    //--default: p_error("Unknown Light token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}
bool LightDot::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, lightDotTags );
  switch( tagId ) {
    // location: TVector3
  case TAG_lightDot_location: location = field->value->eval( ctx ).asVector3(); break;
    // attenuation: boolean
  case TAG_lightDot_attenuation: attenuation = field->value->eval( ctx ).asBool(); break;
    // default: TLight::parse
  default:
    if ( !TLight::parse( ctx, field ) ) {
      //--p_error("Unknown Light::Dot token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool LightSquare::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, lightSquareTags );
  switch( tagId ) {
    //--    // location: TVector3
    //--  case TAG_lightSquare_location: location = field->value->eval( ctx ).asVector3(); break;
    // dir: TVector3
  case TAG_lightSquare_dir: dir = field->value->eval( ctx ).asVector3(); break;
    // axis: couple of TVector3
  case TAG_lightSquare_axis: {
    ValueList *couple = field->value->eval( ctx ).asList();
    if ( couple->size != 2 )
      p_error("A couple (vector3, vector3) was expected in square.axis!");
    axis1 = couple->elems[0].asVector3();
    axis2 = couple->elems[1].asVector3();
    break;
  }
    // numdots: couple of int
  case TAG_lightSquare_numdots: {
    ValueList *couple = field->value->eval( ctx ).asList();
    if ( couple->size != 2 )
      p_error("A couple (int, int) was expected in square.numdots!");
    numDots1 = couple->elems[0].asInt();
    numDots2 = couple->elems[1].asInt();
    break;
  }
  case TAG_lightSquare_jitter: jitter = field->value->eval( ctx ).asBool(); break;
    // default: LightDot::parse
  default:
    if ( !LightDot::parse( ctx, field ) ) {
      //--p_error("Unknown Light::Square token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
//--TODObool LightSpherical::parse( Context &ctx, RAYField *field ) {
//--TODO  const char *tagName = field->fieldName->image;
//--TODO  int tagId = TTagList::findId( tagName, lightSphericalTags );
//--TODO  switch( tagId ) {
//--TODO    // diameter: float
//--TODO    case TAG_lightSpherical_diameter: diameter = field->value->eval( ctx ).asFloat(); break;
//--TODO    // default: LightDot::parse
//--TODO  default:
//--TODO    if ( !LightDot::parse( ctx, field ) ) {
//--TODO      //--p_error("Unknown Light::Spherical token: %s\n", tagName );
//--TODO      return false;
//--TODO    }
//--TODO  }
//--TODO  return true;
//--TODO}

//----TBounding
bool TBounding::parse( Context &ctx, RAYField *field ) {
  return false;
}
bool BoundingAABB::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, boundingAABBTags );
  switch( tagId ) {
    // min: TVector3
  case TAG_boundingAABB_min: m_Min = field->value->eval( ctx ).asVector3(); break;
    // max: TVector3
  case TAG_boundingAABB_max: m_Max = field->value->eval( ctx ).asVector3(); break;
    // default: TBounding::parse
  default:
    if ( !TBounding::parse( ctx, field ) ) {
      //--p_error("Unknown Bounding::AABB token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool BoundingSphere::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, boundingSphereTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_boundingSphere_center: m_Center = field->value->eval( ctx ).asVector3(); break;
    // radius: float
  case TAG_boundingSphere_radius2: m_Radius2 = field->value->eval( ctx ).asFloat(); break;
    // default: TBounding::parse
  default:
    if ( !TBounding::parse( ctx, field ) ) {
      //--p_error("Unknown Bounding::Sphere token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
//--TODObool BoundingSlabs3::parse( Context &ctx, RAYField *field ) {
//--TODO  const char *tagName = field->fieldName->image;
//--TODO  int tagId = TTagList::findId( tagName, boundingSlabsTags );
//--TODO  switch( tagId ) {
//--TODO    // slabs: triple of Triple ( TVector3, float, float )
//--TODO  case TAG_boundingSlabs_slabs: {
//--TODO    ValueList *slabsList = field->value->eval( ctx ).asList();
//--TODO    if ( slabsList->size != 3 )
//--TODO      p_error("3 slabs were expected in boundingSlabs.slabs");
//--TODO    for ( int i = 0; i < slabsList->size; i++ ) {
//--TODO      ValueList *triple = slabsList->elems[i].asList();
//--TODO      if ( triple->size != 3 )
//--TODO	p_error("A slab must contains 3 elements: direction, min, max");
//--TODO      slabs[i].direction = triple->elems[0].asVector3();
//--TODO      slabs[i].min = triple->elems[1].asFloat();
//--TODO      slabs[i].max = triple->elems[2].asFloat();
//--TODO    }
//--TODO    break;
//--TODO  }
//--TODO    // default: TBounding::parse
//--TODO  default:
//--TODO    if ( !TBounding::parse( ctx, field ) ) {
//--TODO      //--p_error("Unknown Bounding::Slabs token: %s\n", tagName );
//--TODO      return false;
//--TODO    }
//--TODO  }
//--TODO  return true;
//--TODO}

//----TAmbient
bool TAmbient::parse( Context &ctx, RAYField *field ) {
  return false;
}
bool AmbientSolid::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, ambientSolidTags );
  switch( tagId ) {
    // color: TColor
  case TAG_ambientSolid_color: color = field->value->eval( ctx ).asColor(); break;
    // default: TAmbient::parse
  default:
    if ( !TAmbient::parse( ctx, field ) ) {
      //--p_error("Unknown Ambient::Solid token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool AmbientHemispherical::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, ambientHemisphericalTags );
  switch( tagId ) {
    // center: TVector3
  case TAG_ambientHemispherical_center: center = field->value->eval( ctx ).asVector3(); break;
    // up: TVector3
  case TAG_ambientHemispherical_up: up = field->value->eval( ctx ).asVector3(); break;
    // colorTop: TColor
  case TAG_ambientHemispherical_colorTop: colorTop = field->value->eval( ctx ).asColor(); break;
    // colorBottom: TColor
  case TAG_ambientHemispherical_colorBottom: colorBottom = field->value->eval( ctx ).asColor(); break;
    // default: TAmbient::parse
  default:
    if ( !TAmbient::parse( ctx, field ) ) {
      //--p_error("Unknown Ambient::Hemispherical token: %s\n", tagName );
      return false;
    }
  }
  return true;
}

//----TPigment
bool TPigment::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentTags );

  switch( tagId ) {
    // anchor: TVector3
  case TAG_pigment_anchor: anchor = field->value->eval( ctx ).asVector3(); break;
    // simple: TColor
  case TAG_pigment_simple: simple = field->value->eval( ctx ).asColor(); break;
    // default: error
    //--  default: p_error("Unknown Pigment token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}
bool PigmentSolid::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentSolidTags );

  switch( tagId ) {
    // color: TColor
  case TAG_pigmentSolid_color: color = field->value->eval( ctx ).asColor(); break;
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Solid token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentNoisy::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentNoisyTags );
  switch( tagId ) {
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Noisy token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentSkymap::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentSkymapTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentSkymap_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_BITMAP )
      p_error("A Bitmap was expected in sky.map!");
    map = (TBitmapColor*)handle;
    break;
  }
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Sky token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentCubemap::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentCubemapTags );
  switch( tagId ) {
    // maps: list of 6 TBitmapColor
  case TAG_pigmentCubemap_maps: {
    ValueList *l = field->value->eval( ctx ).asList();
    if ( l->size != 6 )
      p_error("A list of 6 Bitmap(right,left,up,down,front,back) was expected in cubemap.maps");
    for ( int i = 0; i < 6; i++ ) {
      THandle *handle = l->elems[i].asHandle();
      if ( handle->handleKind != THandle::HANDLE_BITMAP )
	p_error("A Bitmap was expected in cubemap.map[%d]!", i );
      maps[i] = (TBitmapColor*)handle;
    }
    break;
  }
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Sky token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentCylindrical::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentCylindricalTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentCylindrical_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_BITMAP )
      p_error("A Bitmap was expected in sky.map!");
    map = (TBitmapColor*)handle;
    break;
  }
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Sky token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentSpherical::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentSphericalTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentSpherical_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_BITMAP )
      p_error("A Bitmap was expected in sky.map!");
    map = (TBitmapColor*)handle;
    break;
  }
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Sky token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentTorusmap::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentTorusmapTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentTorusmap_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_BITMAP )
      p_error("A Bitmap was expected in sky.map!");
    map = (TBitmapColor*)handle;
    break;
  }
    // gradient: TVector3
  case TAG_pigmentTorusmap_gradient: gradient = field->value->eval( ctx ).asVector3(); break;
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Sky token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentPlanar::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentPlanarTags );
  switch( tagId ) {
    // map: TBitmapColor
  case TAG_pigmentPlanar_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_BITMAP )
      p_error("A Bitmap was expected in sky.map!");
    map = (TBitmapColor*)handle;
    break;
  }
    // gradient: TVector3
  case TAG_pigmentPlanar_gradient: gradient = field->value->eval( ctx ).asVector3(); break;
    // size: float
  case TAG_pigmentPlanar_size: size = field->value->eval( ctx ).asFloat(); break;
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Sky token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentChecker::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentCheckerTags );

  switch( tagId ) {
    // white: TColor
  case TAG_pigmentChecker_white: colorWhite = field->value->eval( ctx ).asColor(); break;
    // black: TColor
  case TAG_pigmentChecker_black: colorBlack = field->value->eval( ctx ).asColor(); break;
    // size: TVector3
  case TAG_pigmentChecker_size: size = field->value->eval( ctx ).asVector3(); break;
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Checker token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentGranite::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentGraniteTags );

  switch( tagId ) {
    // map: TColorMap
  case TAG_pigmentGranite_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_COLORMAP )
      p_error("A Colormap was expected in granite.map!");
    map = (TColorMap*)handle;
    break;
  }
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Granite token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentMandel::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentMandelTags );

  switch( tagId ) {
    // map: TColorMap
  case TAG_pigmentMandel_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_COLORMAP )
      p_error("A Colormap was expected in mandel.map!");
    map = (TColorMap*)handle;
    break;
  }
    // iterations: int
  case TAG_pigmentMandel_iterations: iterations = field->value->eval( ctx ).asInt(); break;
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Mandel token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentMarble::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentMarbleTags );

  switch( tagId ) {
    // map: TColorMap
  case TAG_pigmentMarble_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_COLORMAP )
      p_error("A Colormap was expected in marble.map!");
    map = (TColorMap*)handle;
    break;
  }
    // turbulence: TTurbulence
  case TAG_pigmentMarble_turbulence: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_TURBULENCE )
      p_error("A Colormap was expected in marble.turbulence!");
    turbulence = (TTurbulence*)handle;
    break;
  }
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Marble token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool PigmentWood::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, pigmentWoodTags );

  switch( tagId ) {
    // map: TColorMap
  case TAG_pigmentWood_map: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_COLORMAP )
      p_error("A Colormap was expected in wood.map!");
    map = (TColorMap*)handle;
    break;
  }
    // turbulence: TTurbulence
  case TAG_pigmentWood_turbulence: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_TURBULENCE )
      p_error("A Colormap was expected in wood.turbulence!");
    turbulence = (TTurbulence*)handle;
    break;
  }
    // default: TPigment::parse
  default:
    if ( !TPigment::parse( ctx, field ) ) {
      //--p_error("Unknown Pigment::Wood token: %s\n", tagName );
      return false;
    }
  }
  return true;
}

//----TAtmospheric
bool TAtmospheric::parse( Context &ctx, RAYField *field ) {
  return false;
}
bool AtmosphericFog::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, atmosphericFogTags );
  switch( tagId ) {
    // density: float
  case TAG_atmosphericFog_density: density = field->value->eval( ctx ).asFloat(); break;
    // color: TColor
  case TAG_atmosphericFog_color: color = field->value->eval( ctx ).asColor(); break;
    // filter: float
  case TAG_atmosphericFog_filter: filter = field->value->eval( ctx ).asFloat(); break;
    // turbulence: TTurbulence
  case TAG_atmosphericFog_turbulence: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_TURBULENCE )
      p_error("A Turbulence was expected in fog.turbulence!");
    turbulence = (TTurbulence*)handle;
    break;
  }
    // depth: float
  case TAG_atmosphericFog_turbulenceDepth: turbulenceDepth = field->value->eval( ctx ).asFloat(); break;
    // default: TAtmospheric::parse
  default:
    if ( !TAtmospheric::parse( ctx, field ) ) {
      //--p_error("Unknown Atmospheric::Fog token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool AtmosphericGround::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, atmosphericGroundTags );
  switch( tagId ) {
    // offset: float
  case TAG_atmosphericGround_offset: offset = field->value->eval( ctx ).asFloat(); break;
    // altitude: float
  case TAG_atmosphericGround_altitude: altitude = field->value->eval( ctx ).asFloat(); break;
    // up: TVector3
  case TAG_atmosphericGround_up: up = field->value->eval( ctx ).asVector3(); break;
    // default: AtmosphericFog::parse
  default:
    if ( !AtmosphericFog::parse( ctx, field ) ) {
      //--p_error("Unknown Atmospheric::Ground token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool AtmosphericVolumetric::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, atmosphericVolumetricTags );
  switch( tagId ) {
    // volume: TPrimitive
  case TAG_atmosphericVolumetric_volume: {
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_PRIMITIVE )
      p_error("A Primitive was expected in volumetric.volume!");
    volume = (TPrimitive*)handle;
    break;
  }
    // default: AtmosphericFog::parse
  default:
    if ( !AtmosphericFog::parse( ctx, field ) ) {
      //--p_error("Unknown Atmospheric::Volumetric token: %s\n", tagName );
      return false;
    }
  }
  return true;
}

//----TColormap
bool TColorMap::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, colormapTags );
  switch( tagId ) {
    // map: list of couple ( float, TColor )
  case TAG_colormap_map: {
    ValueList *l = field->value->eval( ctx ).asList();
    nbItems = l->size;
    map = new TColormapItem [l->size];
    for ( int i = 0; i < l->size; i++ ) {
      ValueList *couple = l->elems[i].asList();
      if ( couple->size != 2 )
	p_error("A list of couple was expected in colormap.map!");
      map[i].startValue = couple->elems[0].asFloat();
      map[i].color = couple->elems[1].asColor();
    }
    break;
  }
//--TODO    // function: FCT_DATA
//--TODO  case TAG_colormap_function: function = field->value->eval( ctx ).asFct(); break;
    // default: error
    //--  default: p_error("Unknown Colormap token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}

//----TTransformation
bool TAnimator::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, animatorTags );
  switch( tagId ) {
    // movers: list of TMover
  case TAG_animator_movers: {
    ValueList *l = field->value->eval( ctx ).asList();
    for ( int i = 0; i < l->size; i++ ) {
      THandle *handle = l->elems[i].asHandle();
      if ( handle->handleKind != THandle::HANDLE_MOVER )
	p_error("A Mover was expected in animator.movers!");
      movers.push_back( (TMover*)handle );
    }
//--    numMovers = l->size;
//--    movers = new TMover * [numMovers];
//--    for ( int i = 0; i < numMovers; i++ ) {
//--      THandle *handle = l->elems[i].asHandle();
//--      if ( handle->handleKind != THandle::HANDLE_MOVER )
//--	p_error("A Mover was expected in animator.movers!");
//--      movers[i] = (TMover*)handle;
//--    }
    break;
  }
    // speed: float
  case TAG_animator_speed: speed = field->value->eval( ctx ).asFloat(); break;
    // default: error
    //--  default: p_error("Unknown Animator token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}

//----TMover
bool TMover::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, moverTags );
  switch( tagId ) {
    // dest: list of TVector3 or float (depends on kind of mover)
  case TAG_mover_dest: {
    ValueList *l = field->value->eval( ctx ).asList();
    for ( int i = 0; i < l->size; i++ )
      if ( vMove )
	dest.push_back( l->elems[i].asVector3Pointer() );
      else
	dest.push_back( l->elems[i].asFloatPointer() );
//--    numDest = l->size;
//--    dest = new void* [numDest];
//--    for ( int i = 0; i < numDest; i++ ) {
//--      if ( vMove )
//--	dest[i] = (l->elems[i].asVector3Pointer());
//--      else
//--	dest[i] = (l->elems[i].asFloatPointer());
//--    }
    break;
  }
    // default: error
    //--  default: p_error("Unknown Mover token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}

bool MoverAddV::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, moverAddTags );
  switch( tagId ) {
    //    // source1: pointer to TVector3
    //  case TAG_moverAdd_source1: source1 = field->value->eval( ctx ).asVector3Pointer(); break;
    //    // source1: pointer to TVector3
    //  case TAG_moverAdd_source2: source2 = field->value->eval( ctx ).asVector3Pointer(); break;
    //    // default: TMover::parse
    // source: list of TVector3 pointer
  case TAG_moverAdd_source: {
    ValueList *l = field->value->eval( ctx ).asList();
    for ( int i = 0; i < l->size; i++ )
      source.push_back( l->elems[i].asVector3Pointer() );
    break;
  }
  default:
    if ( !TMover::parse( ctx, field ) ) {
      //--      p_error("Unknown Mover::AddV token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool MoverCircle::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, moverCircleTags );
  switch( tagId ) {
    // source: TVector3
  case TAG_moverCircle_source: source = field->value->eval( ctx ).asVector3(); break;
    // axis: TVector3
  case TAG_moverCircle_axis: axis = field->value->eval( ctx ).asVector3(); break;
    // speed: float
  case TAG_moverCircle_speed: speed = field->value->eval( ctx ).asFloat(); break;
    // default: TMover::parse
  default:
    if ( !TMover::parse( ctx, field ) ) {
      //--      p_error("Unknown Mover::Circle token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool MoverLinearF::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, moverLinearFTags );
  switch( tagId ) {
    // zero: float
  case TAG_moverLinearF_zero: zero = field->value->eval( ctx ).asFloat(); break;
    // factor: float
  case TAG_moverLinearF_factor: fact = field->value->eval( ctx ).asFloat(); break;
    // default: TMover::parse
  default:
    if ( !TMover::parse( ctx, field ) ) {
      //--      p_error("Unknown Mover::LinearF token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool MoverBounce::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, moverBounceTags );
  switch( tagId ) {
    // gravity: float
  case TAG_moverBounce_gravity: gravity = field->value->eval( ctx ).asFloat(); break;
    // speed: float
  case TAG_moverBounce_speed: speed = field->value->eval( ctx ).asFloat(); break;
    // height: float
  case TAG_moverBounce_height: height = field->value->eval( ctx ).asFloat(); break;
    // default: TMover::parse
  default:
    if ( !TMover::parse( ctx, field ) ) {
      //--      p_error("Unknown Mover::Bounce token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool MoverLissajou::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, moverLissajouTags );
  switch( tagId ) {
    // pulse: TVector3
  case TAG_moverLissajou_pulse: pulse = field->value->eval( ctx ).asVector3(); break;
    // phase: TVector3
  case TAG_moverLissajou_phase: phase = field->value->eval( ctx ).asVector3(); break;
    // amplitude: TVector3
  case TAG_moverLissajou_amplitude: ampl = field->value->eval( ctx ).asVector3(); break;
    // shift: TVector3
  case TAG_moverLissajou_shift: shift = field->value->eval( ctx ).asVector3(); break;
    // default: TMover::parse
  default:
    if ( !TMover::parse( ctx, field ) ) {
      //--      p_error("Unknown Mover::Lissajou token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool MoverBezier::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, moverBezierTags );
  switch( tagId ) {
    // segments: list of triple( TVector3, TVector3, float )
  case TAG_moverBezier_segments: {
    ValueList *l = field->value->eval( ctx ).asList();
    numSegments = l->size;
    segments = new MoverBezier::TSegment [l->size];
    for ( int i = 0; i < l->size; i++ ) {
      ValueList *triplet = l->elems[i].asList();
      if ( triplet->size != 3 )
	p_error("A list of triple (point, vector, time) was expected in bezier.segments!");
      segments[i].point = triplet->elems[0].asVector3();
      segments[i].vector = triplet->elems[1].asVector3();
      segments[i].time = triplet->elems[2].asFloat();
    }
    break;
  }
    // default: TMover::parse
  default:
    if ( !TMover::parse( ctx, field ) ) {
      //-      p_error("Unknown Mover::Bezier token: %s\n", tagName );
      return false;
    }
  }
  return true;
}

//----TNormal
bool TNormal::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, normalTags );
  switch( tagId ) {
    // default: error
    //--  default: p_error("Unknown Normal token: %s\n", tagName ); return false;
  default: return false;
  }
  return true;
}
bool NormalBumps::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, normalBumpsTags );
  switch( tagId ) {
    // bumps: TBumps
  case TAG_normalBumps_bumps: bumps = field->value->eval( ctx ).asFloat(); break;
  default:
    if ( !TNormal::parse( ctx, field ) ) {
      //--      p_error("Unknown Normal::Bumps token: %s\n", tagName );
      return false;
    }
    break;
  }
  return true;
}
bool NormalTurbulence::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, normalTurbulenceTags );
  switch( tagId ) {
    // factor: float
  case TAG_normalTurbulence_factor: factor = field->value->eval( ctx ).asFloat(); break;
    // turbulence: TTurbulence
  case TAG_normalTurbulence_turbulence: { 
    THandle *handle = field->value->eval( ctx ).asHandle();
    if ( handle->handleKind != THandle::HANDLE_TURBULENCE )
      p_error("A Turbulence was expected in turbulence.turbulence!");
    turbulence = (TTurbulence*)handle;
    break;
  }
  default:
    if ( !TNormal::parse( ctx, field ) ) {
      //--      p_error("Unknown Normal::Turbulence token: %s\n", tagName );
      return false;
    }
    break;
  }
  return true;
}
//--TODObool NormalPattern::parse( Context &ctx, RAYField *field ) {
//--TODO  const char *tagName = field->fieldName->image;
//--TODO  int tagId = TTagList::findId( tagName, normalPatternTags );
//--TODO  switch( tagId ) {
//--TODO    // pattern: TPattern
//--TODO  case TAG_normalPattern_pattern: {
//--TODO    THandle *handle = field->value->eval( ctx ).asHandle();
//--TODO    if ( handle->handleKind != THandle::HANDLE_PATTERN )
//--TODO      p_error("A Pattern was expected in pattern.pattern!");
//--TODO    pattern = (TPattern*)handle;
//--TODO    pattern->storedHandleKind = THandle::HANDLE_NORMAL;
//--TODO    break;
//--TODO  }
//--TODO    // default: no need to call TNormal::parse because we'll only use pattern as normal
//--TODO  default: {
//--TODO    //--      p_error("Unknown Normal::Pattern token: %s\n", tagName );
//--TODO      return false;
//--TODO    }
//--TODO  }
//--TODO  return true;
//--TODO}
//--TODObool NormalFunction::parse( Context &ctx, RAYField *field ) {
//--TODO  const char *tagName = field->fieldName->image;
//--TODO  int tagId = TTagList::findId( tagName, normalFunctionTags );
//--TODO  switch( tagId ) {
//--TODO    // function: TFunction
//--TODO  case TAG_normalFunction_function: function = field->value->eval( ctx ).asFct(); break;
//--TODO    // default: no need to call TNormal::parse because we'll only use function as normal
//--TODO  default: {
//--TODO    //--      p_error("Unknown Normal::Function token: %s\n", tagName );
//--TODO      return false;
//--TODO    }
//--TODO  }
//--TODO  return true;
//--TODO}

//----TTurbulence
bool TTurbulence::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, turbulenceTags );
  switch( tagId ) {
    // turbulence: TVector3
  case TAG_turbulence_turbulence: turbulence = field->value->eval( ctx ).asVector3(); break;
    // octaves: int
  case TAG_turbulence_octaves: octaves = field->value->eval( ctx ).asInt(); break;
    // lambda: float
  case TAG_turbulence_lambda: lambda = field->value->eval( ctx ).asFloat(); break;
    // omega: float
  case TAG_turbulence_omega: omega = field->value->eval( ctx ).asFloat(); break;
    // default
  default: return false;
  }
  return true;
}
bool TurbulenceNoise::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, turbulenceNoiseTags );
  switch( tagId ) {
    // default
  default:
    if ( !TTurbulence::parse( ctx, field ) ) {
      //--      p_error("Unknown TurbulenceNoise::Bumps token: %s\n", tagName );
      return false;
    }
  }
  return true;
}
bool TurbulencePerlin::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, turbulencePerlinTags );
  switch( tagId ) {
    // default
  default:
    if ( !TTurbulence::parse( ctx, field ) ) {
      //--      p_error("Unknown TurbulencePerlin::Bumps token: %s\n", tagName );
      return false;
    }
  }
  return true;
}

//----TBitmapColor
bool TBitmapColor::parse( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, bitmapTags );
  switch( tagId ) {
    // pcx: string
  case TAG_bitmap_pcx: {
    if ( dataColor != NULL )
      p_error("bitmap already loaded!");
    const char *s = field->value->eval( ctx ).asStr();
    char fName[1024];
    sprintf( fName, "%stextures/%s", globalSettings::path, s );
    if ( !loadPCX( fName ) )
      p_error("Couldn't load texture: %s\n", fName );
    setup(); // create dataColor
    break;
  }
    // tga: string
  case TAG_bitmap_tga: {
    if ( dataColor != NULL )
      p_error("bitmap already loaded!");
    const char *s = field->value->eval( ctx ).asStr();
    char fName[1024];
    sprintf( fName, "%stextures/%s", globalSettings::path, s );
    if ( !loadTGA( fName ) )
      p_error("Couldn't load texture: %s\n", fName );
    setup(); // create dataColor
    break;
  }
    // bmp: string
  case TAG_bitmap_bmp: {
    if ( dataColor != NULL )
      p_error("bitmap already loaded!");
    const char *s = field->value->eval( ctx ).asStr();
    char fName[1024];
    sprintf( fName, "%stextures/%s", globalSettings::path, s );
    if ( !loadBMP( fName ) )
      p_error("Couldn't load texture: %s\n", fName );
    setup(); // create dataColor
    break;
  }
    // default
  default: return false;
  }
  return true;
}

//----Settings
bool parseSettings( Context &ctx, RAYField *field ) {
  const char *tagName = field->fieldName->image;
  int tagId = TTagList::findId( tagName, globalSettings::settingsTags );

  switch( tagId ) {
    // lightAttenuation: float
  case globalSettings::TAG_settings_lightAttenuation: globalSettings::lightAttenuation = field->value->eval( ctx ).asFloat(); break;
    // maxReflection: int
  case globalSettings::TAG_settings_maxReflection: globalSettings::maxReflectionDepth = field->value->eval( ctx ).asInt(); break;
    // maxTransmission: int
  case globalSettings::TAG_settings_maxTransmission: globalSettings::maxTransmissionDepth = field->value->eval( ctx ).asInt(); break;
    // shading: bool
  case globalSettings::TAG_settings_shading: globalSettings::shadingOn = field->value->eval( ctx ).asBool(); break;
    // shadow: bool
  case globalSettings::TAG_settings_shadow: globalSettings::shadowOn = field->value->eval( ctx ).asBool(); break;
    // shadowCache: bool
  case globalSettings::TAG_settings_shadow_cache: globalSettings::shadowCacheOn = field->value->eval( ctx ).asBool(); break;
    // reflection: bool
  case globalSettings::TAG_settings_reflection: globalSettings::reflectionOn = field->value->eval( ctx ).asBool(); break; 
    // transmission: bool
  case globalSettings::TAG_settings_transmission: globalSettings::transmissionOn = field->value->eval( ctx ).asBool(); break;
    // atmospheric: bool
  case globalSettings::TAG_settings_atmospheric: globalSettings::atmosphericOn = field->value->eval( ctx ).asBool(); break;
    // rendering: string
  case globalSettings::TAG_settings_rendering: {
    const char *renderName = field->value->eval( ctx ).asStr();
    int renderId = TTagList::findId( renderName, TRenderer::rendererKindTags );
    if ( renderId < 0 )
      p_error("Invalid rendering [%s] method!", renderName );
    globalSettings::whichRendering = renderId;
    break;
  }
    // screenshot: string
  case globalSettings::TAG_settings_screenshot: {
    const char *renderName = field->value->eval( ctx ).asStr();
    int renderId = TTagList::findId( renderName, TRenderer::rendererKindTags );
    if ( renderId < 0 )
      p_error("Invalid screenshot rendering [%s] method!", renderName );
    globalSettings::whichRenderingScreenshot = renderId;
    break;
  }
    // mouse: bool
  case globalSettings::TAG_settings_mouse: globalSettings::mouseOn = field->value->eval( ctx ).asBool(); break;
    // diffuse: bool
  case globalSettings::TAG_settings_diffuse: globalSettings::diffuseOn = field->value->eval( ctx ).asBool(); break;
    // ambient: bool
  case globalSettings::TAG_settings_ambient: globalSettings::ambientOn = field->value->eval( ctx ).asBool(); break;
    // moveStep: float
  case globalSettings::TAG_settings_moveStep: globalSettings::moveStep = field->value->eval( ctx ).asFloat(); break;

	// width; int
  case globalSettings::TAG_settings_width: globalSettings::width = field->value->eval( ctx ).asInt(); break;
	// height; int
  case globalSettings::TAG_settings_height: globalSettings::height = field->value->eval( ctx ).asInt(); break;
	  	// screenshotwidth: int
  case globalSettings::TAG_settings_screenshot_width: globalSettings::screenshotWidth = field->value->eval( ctx ).asInt(); break;
	// screenshotheight; int
  case globalSettings::TAG_settings_screenshot_height: globalSettings::screenshotHeight = field->value->eval( ctx ).asInt(); break;

    // optimizing: list of string
  case globalSettings::TAG_settings_optimizations: {
    ValueList *l = field->value->eval( ctx ).asList();
    for ( int i = 0; i < l->size; i++ ) {
      const char *s = l->elems[i].asStr();
      const int optId = TTagList::findId( s, globalSettings::optimizationsTags );
      if ( optId == -1 )
	p_warn("optimizations: invalid value [%s].\n", s );
      else
	globalSettings::optimizations.setBit( optId );
    }
    break;
  }
    // default
  default: return false;
  }
  return true;
}
