#ifndef __SCENE_PARSER_H__
#define __SCENE_PARSER_H__

extern char currentFilename[1024];

struct Context;
struct TWorld;
struct THandle;
struct RAYTag;

TWorld *parseFile( Context &ctx, FILE *fp );

bool parseSettings( Context &ctx, RAYField *field );

void addThis( Context &ctx, const Value &v );
void addSuper( Context &ctx );

#endif
