#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "statistics.h"
#include "parser/tag.h"
#include "types/bitvector.h"

#define MAX_DISTANCE (1e7)

struct globalSettings {
  static const enum ERendererKind {
    TAG_settings_lightAttenuation,
    TAG_settings_maxReflection,
    TAG_settings_maxTransmission,
    TAG_settings_shading,
    TAG_settings_shadow,
    TAG_settings_shadow_cache,
    TAG_settings_reflection,
    TAG_settings_transmission,
    TAG_settings_atmospheric,
    TAG_settings_rendering,
    TAG_settings_screenshot,
    TAG_settings_mouse,
    TAG_settings_diffuse,
    TAG_settings_ambient,
    TAG_settings_moveStep,
    TAG_settings_optimizations,
	TAG_settings_width,
	TAG_settings_height,
	TAG_settings_screenshot_width,
	TAG_settings_screenshot_height
  };
  static const TTagList settingsTags [];

  static bool pauseOn;
  static bool shadingOn;
  static bool shadowOn;
  static bool reflectionOn;
  static bool transmissionOn;
  static bool atmosphericOn;
  static bool shadowCacheOn;
  static bool mouseOn;
  static bool diffuseOn;
  static bool ambientOn;

  static const enum EMappingKind {
    MAPPING_NORMAL,
    MAPPING_BILINEAR,
    MAPPING_NORMAL_DISTANCE,
    MAPPING_MAX
  };
  static int whichMapping; // EMappingKind

  static int whichRendering; // TRenderer::ERenderKind
  static int whichRenderingScreenshot; // TRenderer::ERenderKind

  static float lightAttenuation;

  static int maxReflectionDepth;
  static int maxTransmissionDepth;

  static float moveStep;

  static u32 width;
  static u32 height;
  static u32 screenshotWidth;
  static u32 screenshotHeight;

  static char path[1024];

  static const enum EOptimizations {
    OPTIMIZE_FIRST_RAY = 1,
    OPTIMIZE_KDTREE,
    OPTIMIZE_UNIFORM_GRID
  };
  static const TTagList optimizationsTags [];
  static TBitvector optimizations;

  static void print( const char *s );
};

extern TStatistic stats;

void testProc();

#endif
