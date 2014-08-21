#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#define   DIRECTDRAW_VERSION 0x0300
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "world.h"
#include "renderers/renderer.h"
#include "main_common.h"
#include "fonts/font.h"
#include "animators/animator.h"
#include "camera.h"
#include "globals.h"

#define NAME                "dX Raytracing"
#define TITLE               "Direct Draw Basic Raytracing"

HWND                        hWnd;
BOOL                        g_bActive;

LPDIRECTDRAW               g_pDD = NULL;

LPDIRECTDRAWSURFACE	FrontBuffer;
LPDIRECTDRAWSURFACE	BackBuffer;
LPDIRECTDRAWPALETTE     lpddpal      = NULL;		// a pointer to the created dd palette

PALETTEENTRY         color_palette[256];		// holds palette entries

static const u32 XRES = 320;
static const u32 YRES = 240;

static TWorld world;
static TRenderer renderer;


//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------
static void 
ReleaseAllObjects() {
  if (g_pDD != NULL) {
    if (BackBuffer)  BackBuffer->Release(),     BackBuffer = NULL;
    if (FrontBuffer) FrontBuffer->Release(),    FrontBuffer = NULL;
    g_pDD->Release();
    g_pDD = NULL;
  }
}
//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...) {
  char                        szBuff[128];
  va_list                     vl;
  va_start(vl, szError);
  vsprintf(szBuff, szError, vl);
  ReleaseAllObjects();
  MessageBox(hWnd, szBuff, TITLE, MB_OK);
  DestroyWindow(hWnd);
  va_end(vl);
  return hRet;
}

void BuildImage() {
  static int before = 0;
  static int after = 0;
  static frameCount = 0;
  HRESULT                     hRet;
  DDSURFACEDESC              ddsd;
  
  if (FrontBuffer->IsLost() == DDERR_SURFACELOST)
    FrontBuffer->Restore();
  
  ddsd.dwSize = sizeof(ddsd);
  hRet = BackBuffer->Lock(NULL, &ddsd, 0, NULL);

  if (hRet != DD_OK)
    return;
  
  renderer.surface = (u32 *) ddsd.lpSurface;
  stats.clear();
  if ( world.animator != NULL )
    world.animator->animate();
  world.currentCamera->update();
  world.precalcFrame();
  frameCount++;
  before += GetTickCount();
  renderer.render( 160, 100, world, *world.currentCamera );
  after += GetTickCount();
  const float fps = (float)frameCount / (float)((after - before) / 1000.0f);
  displayString( 160, 0, 0x00ffffff, 0x00000000, false,
  		 renderer.surface, renderer.width, renderer.height, "fps: %4.2f", fps );

  // reset timer
  if ( frameCount % 64 ) {
    frameCount = 0;
    before = 0;
    after = 0;
  }

  BackBuffer->Unlock(NULL);
  FrontBuffer->Flip(NULL, DDFLIP_WAIT);
}

void ExitApp() {
  static bool done = FALSE;
  if ( done )
    return;
  ReleaseAllObjects();
  if ( hWnd )
    PostMessage(hWnd, WM_CLOSE, 0, 0);
  done = TRUE;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {

  case WM_ACTIVATE:
    g_bActive = wParam;
    break;

  case WM_CREATE:
    break;

  case WM_SETCURSOR:
    SetCursor(NULL);
    break;

  case WM_KEYDOWN:
    if ( !key_in( wParam, world ) ) // stop application
      ExitApp();
    break;

  case WM_DESTROY:
    ReleaseAllObjects();
    PostQuitMessage(0);
    break;

  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}


//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------

static BOOL 
InitApp(HINSTANCE hInstance, int nCmdShow) {
  WNDCLASS                    wc;
  DDSURFACEDESC              ddsd;
  HRESULT                     err;
  LPDIRECTDRAW                pDD;


  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = 0;
  wc.hCursor = 0; //LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NAME;
  wc.lpszClassName = NAME;
  RegisterClass(&wc);

  hWnd = CreateWindowEx(0,
			NAME,
			TITLE,
			WS_POPUP,
			0,
			0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			NULL,
			NULL,
			hInstance,
			NULL);

  if (!hWnd)
    return FALSE;
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  // /////////////////////////////////////////////////////////////////////////
  // Create the main DirectDraw object
  // /////////////////////////////////////////////////////////////////////////
  err = DirectDrawCreate(NULL, &pDD, NULL);
  if (err != DD_OK)
    return InitFail(hWnd, err, "DirectDrawCreate FAILED");
  
  // Fetch DirectDraw interface
  err = pDD->QueryInterface(IID_IDirectDraw, (LPVOID *) & g_pDD);
  if (err != DD_OK)
    return InitFail(hWnd, err, "QueryInterface FAILED");
  
  // Get exclusive mode
  err = g_pDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
  if (err != DD_OK)
    return InitFail(hWnd, err, "SetCooperativeLevel FAILED");
  
  // Set the video mode
  err = g_pDD->SetDisplayMode( XRES, YRES, 32 );
  if (err != DD_OK)
    return InitFail(hWnd, err, "SetDisplayMode FAILED");
  
  //
  // Create surfaces
  //
  ZeroMemory(&ddsd, sizeof(ddsd));
  ddsd.dwSize = sizeof(ddsd);
  ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  ddsd.dwBackBufferCount = 2;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
    DDSCAPS_FLIP |
    DDSCAPS_COMPLEX |
    DDSCAPS_VIDEOMEMORY;
  
  // try to get a triple buffered video memory surface.
  err = g_pDD->CreateSurface(&ddsd, &FrontBuffer, NULL);
  
  if (err != DD_OK) {
    // try to get a double buffered video memory surface.
    ddsd.dwBackBufferCount = 1;
    err = g_pDD->CreateSurface(&ddsd, &FrontBuffer, NULL);
  }
  
  if (err != DD_OK) {
    // settle for a main memory surface.
    ddsd.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
    err = g_pDD->CreateSurface(&ddsd, &FrontBuffer, NULL);
  }
  
  if (err != DD_OK)
    return InitFail(hWnd, err, "CreateSurface FAILED");
  
  
  // get a pointer to the back buffer
  DDSCAPS caps;
  caps.dwCaps = DDSCAPS_BACKBUFFER;
  err = FrontBuffer->GetAttachedSurface(&caps, &BackBuffer);
  
  if (err != DD_OK)
    return InitFail(hWnd, err, "Back buffer FAILED");

  return TRUE;
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------

int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow) {
  printf("===============\n"
	 "INITIALIZING...\n"
	 "===============\n"); fflush(stdout);

  if ( lpCmdLine != NULL && strlen(lpCmdLine) > 0 && nCmdShow > 0 ) {
    // FIXME: be smarter ... tokenize lpCmdLine and get the right one
  }

  if (!InitApp(hInstance, nCmdShow))
    return FALSE;

  initRaytracing();
  initWorld( world );

  renderer.setup( NULL, XRES, YRES );

  MSG msg;

  printf("==================\n"
	 "START RENDERING...\n"
	 "==================\n"); fflush(stdout);

  int frame = 0, startTime = GetTickCount();
  while (TRUE) {
    if ( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) ) {
      if ( !GetMessage(&msg, NULL, 0, 0) )
	break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else if ( g_bActive ) {
      BuildImage(); // render and raster
      frame++;
    }
  }
  int stopTime = GetTickCount();
  float fps = (float)frame / (float)((stopTime - startTime) / 1000.0f);


  printf("==========\n"
	 "LEAVING...\n"
	 "==========\n"); fflush(stdout);

  printf("===================================\n"
	 "%.2f fps\n"
	 "===================================\n", fps );

  ExitApp();
  return 0;
}
