#ifndef __INIT_H__
#define __INIT_H__

void initRaytracing();

TWorld *readWorld( Context &ctx, const char *filename );

bool key_in( const int k, TWorld &world );
bool takeScreenshot( const TWorld &world, const char *filename );


#endif

