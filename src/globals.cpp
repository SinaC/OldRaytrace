#include <stdlib.h>
#include <stdio.h>

//--#define __TEST_PROC__

#ifdef __TEST_PROC__
#include "partitions/uniformgrid.h"
#include "partitions/bspheretree.h"
#include "partitions/bboxtree.h"
#include "math/vector3.h"
#include "object.h"
#include "primitives/box.h"
#include "primitives/sphere.h"
#endif

#include "globals.h"
#include "renderers/renderer.h"

//----- Global settings

const TTagList globalSettings::settingsTags [] = {
  { "lightAttenuation", TAG_settings_lightAttenuation },
  { "maxReflectionDepth", TAG_settings_maxReflection },
  { "maxTransmissionDepth", TAG_settings_maxTransmission },
  { "shading", TAG_settings_shading },
  { "shadow", TAG_settings_shadow },
  { "shadowCache", TAG_settings_shadow_cache },
  { "reflection", TAG_settings_reflection },
  { "transmission", TAG_settings_transmission },
  { "atmospheric", TAG_settings_atmospheric },
  { "rendering", TAG_settings_rendering },
  { "screenshot", TAG_settings_screenshot },
  { "mouse", TAG_settings_mouse },
  { "diffuse", TAG_settings_diffuse },
  { "ambient", TAG_settings_ambient },
  { "moveStep", TAG_settings_moveStep },
  { "optimizations", TAG_settings_optimizations },
  { "width", TAG_settings_width },
  { "height", TAG_settings_height },
  { "screenshotwidth", TAG_settings_screenshot_width },
  { "screenshotheight", TAG_settings_screenshot_height },
  { NULL, -1 }
};

bool globalSettings::pauseOn = false;
bool globalSettings::shadingOn = true;
bool globalSettings::shadowOn = true;
bool globalSettings::reflectionOn = false;
bool globalSettings::transmissionOn = false;
bool globalSettings::atmosphericOn = false;
bool globalSettings::shadowCacheOn = false;
bool globalSettings::mouseOn = false;
bool globalSettings::diffuseOn = true;
bool globalSettings::ambientOn = true;

int globalSettings::whichMapping = globalSettings::MAPPING_NORMAL;

//--int globalSettings::whichRendering = TRenderer::RENDERER_BILINEAR;
int globalSettings::whichRendering = TRenderer::RENDERER_NORMAL;
int globalSettings::whichRenderingScreenshot = TRenderer::RENDERER_QUINCUNX;

float globalSettings::lightAttenuation = 0.000005f;

int globalSettings::maxReflectionDepth = 4;
int globalSettings::maxTransmissionDepth = 4;

float globalSettings::moveStep = 5.0f;

u32 globalSettings::width = 320;
u32 globalSettings::height = 240;
u32 globalSettings::screenshotWidth = 640;
u32 globalSettings::screenshotHeight = 480;

//char globalSettings::path[1024] = "C:/Documents and Settings/Loraine/Mes documents/SinaC/sources/newray/data/";
char globalSettings::path[1024] = "D:/Projects/Personal/newray/data/";

const TTagList globalSettings::optimizationsTags [] = {
  { "first_ray", OPTIMIZE_FIRST_RAY },
  { "kdtree", OPTIMIZE_KDTREE },
  { "uniform_grid", OPTIMIZE_UNIFORM_GRID },
  { NULL, -1 }
};
TBitvector globalSettings::optimizations;

void globalSettings::print( const char *msg ) {
  printf("%s\n", msg );

  printf("PAUSE: %s\n", pauseOn?"ON":"OFF" );
  printf("SHADING: %s\n", shadingOn?"ON":"OFF" );
  printf("SHADOWS: %s\n", shadowOn?"ON":"OFF" );
  printf("REFLECTIONS: %s\n", reflectionOn?"ON":"OFF" );
  printf("TRANSMISSIONS: %s\n", transmissionOn?"ON":"OFF" );
  printf("ATMOSPHERIC: %s\n", atmosphericOn?"ON":"OFF" );
  printf("SHADOW CACHE: %s\n", shadowCacheOn?"ON":"OFF" );
  printf("MOUSE: %s\n", mouseOn?"ON":"OFF" );
  printf("DIFFUSE: %s\n", diffuseOn?"ON":"OFF" );
  printf("AMBIENT: %s\n", ambientOn?"ON":"OFF" );
  printf("MAPPING: [%d]\n", whichMapping );
  printf("LIGHT ATTENUATION: [%5.5f]\n", lightAttenuation );
  printf("REFLECTION DEPTH: [%2d]\n", maxReflectionDepth );
  printf("TRANSMISSION DEPTH: [%2d]\n", maxTransmissionDepth );
  printf("MOVE STEP: [%5.5f]\n", moveStep );
  printf("RENDERING: [%s]\n", TTagList::findName( whichRendering, TRenderer::rendererKindTags ) );
  printf("RENDERING SCREENSHOT: [%s]\n", TTagList::findName( whichRenderingScreenshot, TRenderer::rendererKindTags ) );
  printf("PATH: [%s]\n", path );
  printf("OPTIMIZATIONS: [%s]\n", TTagList::findName( optimizations, optimizationsTags ) );
}

//----- Statistics
TStatistic stats;

//----- Test procedure

void testProc() {
#ifndef __TEST_PROC__
  return;
#else
  printf("===============\n"
	 "-->TEST PROC...\n"
	 "===============\n"); fflush(stdout);

  int n = 10;
  int i;

  printf("=======\n"
	 "BSPHERE\n"
	 "=======\n");
  printf("CREATING LIST...\n"); fflush(stdout);

  BSphereTree **eSphere = new BSphereTree *[n];
  for ( i = 0; i < n; i++ ) {
    eSphere[i] = new BSphereTree();
    eSphere[i]->entries = 0;
    //--    eSphere[i]->center = TVector3( 0, 0, 0 );
    //--    eSphere[i]->radius2 = (float)SQR(i+1);
    eSphere[i]->sphere.m_Center = TVector3( 2.0f*(float)(i+1), 0, 0 );
    eSphere[i]->sphere.m_Radius2 = 1.0f;
    eSphere[i]->node = (BSphereTree**)((int)i+0x00FF0000);
  }

  printf("BUILDING TREE...\n"); fflush(stdout);

  BSphereTree *rootSphere = BSphereTree::build( n, eSphere );

  printf("DUMPING TREE...\n"); fflush(stdout);

  if ( rootSphere )
  rootSphere->print("ROOT");

  initializeBSphereCode();
  // compute intersection
  deinitializeBSphereCode();

  //------------------------------------------

  printf("====\n"
	 "BBOX\n"
	 "====\n");

  printf("CREATING LIST...\n"); fflush(stdout);

  AABBTree **eBox = new AABBTree *[n];
  for ( i = 0; i < n; i++ ) {
    eBox[i] = new AABBTree();
    eBox[i]->infinite = false;
    eBox[i]->entries = 0;
    eBox[i]->box.lowerLeft = TPoint3( (float)(i+1), 1.0f, 1.0f );
    eBox[i]->box.lengths = TVector3( 1, 1, 1 );
    //--eBox[i]->node = (AABBTree**)((int)i+0x00FF0000);
    TObject *obj = new TObject();
    PrimitiveBox *box = new PrimitiveBox();
    box->m_Min = eBox[i]->box.lowerLeft;
    box->m_Max = eBox[i]->box.lowerLeft + eBox[i]->box.lengths;
    box->setup();
    obj->primitive = box;
    eBox[i]->node = (AABBTree**)obj;
  }

  printf("BUILDING TREE...\n"); fflush(stdout);

  AABBTree *rootBox = AABBTree::build( n, eBox, 0, NULL );

  printf("DUMPING TREE...\n"); fflush(stdout);

  if ( rootBox )
    rootBox->print("ROOT");

  printf("COMPUTING INTERSECTION...\n"); fflush(stdout);

  initializeAABBCode();
  TRay rayBox;
  //--rayBox.origin = TPoint3( 1.5f, 1.5f, 1.5f );
  //--rayBox.direction = TVector3( 1.0f, 0.0f, 0.0f );
  //--  rayBox.origin = TPoint3( 15.5f, 1.5f, 1.5f );
  //--  rayBox.direction = TVector3( -1.0f, 0.0f, 0.0f );
  rayBox.origin = TPoint3( 5.0f, 1.5f, 0.0f );
  rayBox.direction = TVector3( 0.0f, 0.0f, 1.0f );
  rayBox.update();
  
  TIntersection inter;
  bool found = rootBox->intersectionRay( rayBox, inter );

  printf("INTERSECTION COMPUTED...\n"); fflush(stdout);

  if ( found )
    inter.print("intersection: ");
  else
    printf("NO intersection\n");

  printf("============\n"
	 "UNIFORM GRID\n"
	 "============\n");

  printf("CREATING LIST...\n"); fflush(stdout);

  vector<TObject*> objList;
  UniformGrid testGrid;
  BoundingAABB extends;
  extends.m_Min = TVector3( -10, -10, -10 );
  extends.m_Max = TVector3( 10, 10, 10 );

  {
  TObject *obj = new TObject();
  PrimitiveSphere *prim = new PrimitiveSphere();
  prim->m_Center = TVector3( 0, 0, 0 );
  prim->m_Radius = 0.1f;
  prim->setup();
  obj->primitive = prim;

  objList.push_back(obj);
  }

  printf("BUILDING GRID...\n"); fflush(stdout);

  testGrid.build( objList, 2, extends );

  printf("DUMPING GRID...\n"); fflush(stdout);

  testGrid.print("GRID");


  printf("COMPUTING INTERSECTION...\n"); fflush(stdout);

  TRay rayGrid;
  //--rayGrid.origin = TPoint3( 1.5f, 1.5f, 1.5f );
  //--rayGrid.direction = TVector3( 1.0f, 0.0f, 0.0f );
  //--  rayGrid.origin = TPoint3( 15.5f, 1.5f, 1.5f );
  //--  rayGrid.direction = TVector3( -1.0f, 0.0f, 0.0f );
  rayGrid.origin = TPoint3( -9.0f, 8.0f, -8.0f );
  rayGrid.direction = TVector3( 0,0,0 ) - rayGrid.origin;
  rayGrid.update();
  
  float rDist;
  TObject *rObj;
  found = testGrid.findClosestIntersection( rayGrid, rObj, rDist );

  printf("INTERSECTION COMPUTED...\n"); fflush(stdout);

  if ( found ) {
    printf("INTERSECTION:\n");
    printf("distance: %5.5f\n", rDist );
    rObj->print("object intersected: ");
  }
  else
    printf("NO INTERSECTION\n");

  printf("===============\n"
	 "<--TEST PROC...\n"
	 "===============\n"); fflush(stdout);

  deinitializeAABBCode();
  exit(-1);
#endif
}
