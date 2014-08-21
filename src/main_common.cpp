#include <windows.h>

#include "world.h"
#include "types/basics.h"
#include "cameras/camera.h"
#include "globals.h"
#include "main_common.h"
#include "parser/scene_parser.h"
#include "parser/context.h"
#include "jitter.h"
#include "math/cycloidal.h"
#include "renderers/renderer.h"
#include "renderers/bilinear.h"
#include "renderers/corner.h"
#include "renderers/diamond.h"
#include "renderers/rgss.h"
#include "renderers/quincunx.h"
#include "renderers/grid.h"
#include "renderers/interpolated.h"
#include "renderers/flipquad.h"
#include "turbulences/perlin.h"
#include "turbulences/noise.h"
#include "bitmap/bitmap.h"
#include "fonts/font.h"
#include "error.h"

bool takeScreenshot( const TWorld &world, const char *filename ) {
  static const u32 width = globalSettings::screenshotWidth;
  static const u32 height = globalSettings::screenshotHeight;
  static bool allocated = false;
  static u32 *data;
  if ( !allocated ) {
    data = new u32 [width*height];
    allocated = true;
  }

  stats.clear();
  int before = GetTickCount();
  switch ( globalSettings::whichRenderingScreenshot ) {
  case TRenderer::RENDERER_NORMAL: {
    TRenderer r; r.setup( data, width, height );
    r.render( world );
    break;
  }
  case TRenderer::RENDERER_BILINEAR: {
    RendererBilinear r; r.setup( data, width, height );
    r.render( world );
    break;
  }
  case TRenderer::RENDERER_QUINCUNX: {
    RendererQuincunx r; r.setup( data, width , height );
    r.render( world );
    break;
  }
  case TRenderer::RENDERER_FLIPQUAD: {
    RendererFlipquad r; r.setup( data, width, height );
    r.render( world );
    break;
  }
  case TRenderer::RENDERER_GRID: {
    RendererGrid r; r.setup( data, width, height, 2 );
    r.render( world );
    break;
  }
  case TRenderer::RENDERER_CORNER: {
    RendererCorner r; r.setup( data, width, height );
    r.render( world );
    break;
  }
  case TRenderer::RENDERER_DIAMOND: {
    RendererDiamond r; r.setup( data, width, height );
    r.render( world );
    break;
  }
  case TRenderer::RENDERER_RGSS2x2: {
    RendererRGSS r; r.setup( data, width, height );
    r.render( world );
    break;
  }
  case TRenderer::RENDERER_QUINCUNX_ADAPTATIVE_COLOR: {
    RendererQuincunx r; r.setup( data, width, height );
    r.renderAdaptativeColor( world, 16 );
    break;
  }
  case TRenderer::RENDERER_QUINCUNX_ADAPTATIVE_PRIMITIVE: {
    RendererQuincunx r; r.setup( data, width, height );
    r.renderAdaptativePrimitive( world, 16 );
    break;
  }
  case TRenderer::RENDERER_INTERPOLATED: {
    RendererInterpolated r; r.setup( data, width, height, 8 );
    r.render( world );
    break;
  }
  }
  int after = GetTickCount();

  printf("rendering: %d\n", globalSettings::whichRenderingScreenshot );
  printf("%d ticks -> %5.5f sec. to generate\n", after-before, (float)(after-before)/1000.0f);
  stats.print("Statistics:");

  if ( !TBitmap::saveTGA( data,
			  width, height,
			  filename ) )
    //--    throw Error("could not open image file for writing");
    return false;
  return true;
}

bool key_in( const int k, TWorld &world ) {
  static const enum EStates {
    STATE_CAMERA_LOC,
    STATE_CAMERA_LOOKAT
  };
  static int currentState = STATE_CAMERA_LOC;
  static TVector3 *current = &( world.currentCamera->location );
  static int screenshotCount = 0;

  switch (k) {
    // PAUSE
  case VK_PAUSE: globalSettings::pauseOn = !globalSettings::pauseOn; break;
    // STEP
  case VK_SUBTRACT: globalSettings::moveStep -= 1.0f; break;
  case VK_ADD: globalSettings::moveStep += 1.0f; break;
    // FOV
  case VK_INSERT: globalSettings::lightAttenuation *= 10.0f; break;
  case VK_DELETE: globalSettings::lightAttenuation /= 10.0f; break;
    // Move & target choice
  case 'D':
    if ( currentState == STATE_CAMERA_LOC ) {
      current = &( world.currentCamera->lookat );
      currentState = STATE_CAMERA_LOOKAT;
    }
    else {
      current = &( world.currentCamera->location );
      currentState = STATE_CAMERA_LOC;
    }
    break;
    // Change current camera
  case 'C': {
    int idx = 0;
    int i = 0;
    for ( vector<TCamera *>::const_iterator iterC = world.cameras.begin();
	  iterC != world.cameras.end();
	  iterC++, i++ )
      if ( *iterC == world.currentCamera ) {
	idx = i;
	break;
      }
    i = ( idx + 1 ) % world.cameras.size();
    world.currentCamera = world.cameras[i];
    break;
  }
    // Move camera
  case VK_END: world.currentCamera->fov += globalSettings::moveStep; break;
  case VK_HOME: world.currentCamera->fov -= globalSettings::moveStep; break;
  case VK_LEFT:
    if ( currentState == STATE_CAMERA_LOC )
      *current += TVector3( globalSettings::moveStep, 0, 0 ) * world.currentCamera->getU();
    else
      *current += TVector3( globalSettings::moveStep, 0, 0 );
    break;
  case VK_RIGHT:
    if ( currentState == STATE_CAMERA_LOC )
      *current += TVector3( -globalSettings::moveStep, 0, 0 ) * world.currentCamera->getU();
    else
      *current += TVector3( -globalSettings::moveStep, 0, 0 );
    break;
  case VK_UP:
    if ( currentState == STATE_CAMERA_LOC )
      *current += TVector3( 0, 0, globalSettings::moveStep ) * world.currentCamera->getN();
    else
      *current += TVector3( 0, 0, globalSettings::moveStep );
    break;
  case VK_DOWN:
    if ( currentState == STATE_CAMERA_LOC )
      *current += TVector3( 0, 0, -globalSettings::moveStep ) * world.currentCamera->getN();
    else
      *current += TVector3( 0, 0, -globalSettings::moveStep );
    break;
  case VK_PRIOR:
    if ( currentState == STATE_CAMERA_LOC )
      *current += TVector3( 0, globalSettings::moveStep, 0 ) * world.currentCamera->getV();
    else
      *current += TVector3( 0, globalSettings::moveStep, 0 );
    break;
  case VK_NEXT:
    if ( currentState == STATE_CAMERA_LOC )
      *current += TVector3( 0, -globalSettings::moveStep, 0 ) * world.currentCamera->getV();
    else
      *current += TVector3( 0, -globalSettings::moveStep, 0 );
    break;

    // Global switches
  case VK_F1: globalSettings::shadingOn = !globalSettings::shadingOn; break;
  case VK_F2: globalSettings::shadowOn = !globalSettings::shadowOn; break;
  case VK_F3: globalSettings::reflectionOn = !globalSettings::reflectionOn; break;
  case VK_F4: globalSettings::transmissionOn = !globalSettings::transmissionOn; break;
  case VK_F5: globalSettings::atmosphericOn = !globalSettings::atmosphericOn; break;
  case VK_F6: globalSettings::shadowCacheOn = !globalSettings::shadowCacheOn; break;
  case VK_F7: globalSettings::whichMapping = ( globalSettings::whichMapping + 1 ) % globalSettings::MAPPING_MAX; break;
  case VK_F8: globalSettings::whichRenderingScreenshot = ( globalSettings::whichRenderingScreenshot + 1 ) % TRenderer::RENDERER_MAX; break;
  case VK_F9: globalSettings::diffuseOn = !globalSettings::diffuseOn; break;
  case VK_F11: globalSettings::ambientOn = !globalSettings::ambientOn; break;

    // Dump
  case ' ':
    world.print("WORLD DUMP:");
    break;

    // Screenshot
  case 'S': {
    char filename[1024];
    sprintf( filename, "screen%d.tga", screenshotCount );
    takeScreenshot( world, filename );
    screenshotCount++;
    break;
  }

    // Quit
  case VK_ESCAPE: return false; break;
    break;
  }
  return true;
}

void initRaytracing() {
  jitterInit();
  cycloidalInit();
  noiseInit();
  perlinInit();
  readFont( "font.dat" );
}

TWorld *readWorld( Context &ctx, const char *filename ) {
  FILE *fp;

  char localFName[1024];
  sprintf( localFName, "%sscenes/%s", globalSettings::path, filename );

  printf("---------------------------------------------------------------\n"
	 "READING WORLD...\n"); fflush(stdout);
  if ( ( fp = fopen( localFName, "r" ) ) == NULL )
    g_error("Can't load scene file: %s\n", localFName );

  extern char currentFilename[1024];
  strcpy( currentFilename, filename );

  TWorld *wld = parseFile( ctx, fp );

  fclose(fp);

  printf("...DONE\n"
	 "---------------------------------------------------------------\n"); fflush(stdout);
  if ( wld == NULL )
    g_error("Tag WORLD not found!!!\n");
  return wld;
}
