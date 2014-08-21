#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//#define __DEBUG__

#define   DIRECTDRAW_VERSION 0x0300
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "surface.h"
#include "color.h"
#include "bitmap.h"
#include "camera.h"
#include "frame.h"
#include "matrix.h"
#include "model.h"
#include "light.h"
#include "font.h"
#include "render.h"

// for test
//#include "clipping.h"
//#include "tessel.h"


#define NAME                "dX Engine"
#define TITLE               "Direct Draw Basic Demo Engine"

HWND                        hWnd;
BOOL                        g_bActive;

LPDIRECTDRAW               g_pDD = NULL;

LPDIRECTDRAWSURFACE	FrontBuffer;
LPDIRECTDRAWSURFACE	BackBuffer;

HPALETTE  hpalApp = NULL;

struct TResolution {
  int Width;
  int Height;
  int BPP;
} resolution = {
  640, 480, 32 // 8 bits works but 16 first and 16 last color in palette are not set!!
};

TSurface frontend_surface;       // raster in this surface
TBitmap renderBitmap;            // render in this bitmap
TBitmap renderBitmap2;           // render in this other bitmap
TBitmap informationBitmap;       // output information are dump in this bitmap
TFrame screenFrame;              // list of bitmap and their position on the surface
TBitmap texture;                 // texture test

long num_frames = 0;
clock_t startC, endC;

LOGPALETTE RGBPalette = {
  0x300,
  256
};


Camera camera; // camera
Model model; // an object
TConicalLight light; // a conical light


//void render( TWorld &world, TBitmap &renderBitmap );
//void render( TBitmap &renderBitmap );
//void raster( TSurface *surf, TBitmap &bitmap, const int displayMinX, const int displayMinY );
void raster( TSurface *surf, const TFrame &frame );
void BuildImage() {
  HRESULT                     hRet;
  DDSURFACEDESC              ddsd;
  
  if (FrontBuffer->IsLost() == DDERR_SURFACELOST)
    FrontBuffer->Restore();
  
  ddsd.dwSize = sizeof(ddsd);
  hRet = BackBuffer->Lock(NULL, &ddsd, 0, NULL);
  if (hRet != DD_OK)
    return;
  
  //render( world, renderBitmap );
  //render( renderBitmap );

  // render  in a bitmap (24 bit)
  render( camera, model, screenFrame );
  // raster  in a surface  (convert a 24 bits bitmap to wanted BPP (8, 16, 24, 32))
  frontend_surface.buf = ddsd.lpSurface;
  raster( &frontend_surface, screenFrame ); // takes 65% CPU
  
  BackBuffer->Unlock(NULL);
  FrontBuffer->Flip(NULL, DDFLIP_WAIT);
  num_frames++;
}


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
  if ( hpalApp )
    DeleteObject(hpalApp);
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

void ExitApp() {
  static bool done = FALSE;
  if ( done )
    return;
  ReleaseAllObjects();
  PostMessage(hWnd, WM_CLOSE, 0, 0);
  done = TRUE;
}

void key_in(int k) {
  static Point4 *modifyP = &(camera.lookAt);
  static float step = 10.0;
  switch (k) {
  case VK_ESCAPE:
    ExitApp();
    break;
  case 'C': modifyP = &(camera.position); break;
  case 'L': modifyP = &(camera.lookAt); break;
  case 'I': modifyP = &(light.position); break;
  case VK_LEFT: *modifyP = *modifyP + Point4( -step, 0.0, 0.0, 0.0 ); break;
  case VK_RIGHT: *modifyP = *modifyP + Point4( +step, 0.0, 0.0, 0.0 ); break;
  case VK_UP: *modifyP = *modifyP + Point4( 0.0, +step, 0.0, 0.0 ); break;
  case VK_DOWN: *modifyP = *modifyP + Point4( 0.0, -step, 0.0, 0.0 ); break;
  case 'A': *modifyP = *modifyP + Point4( 0.0, 0.0, -step, 0.0 ); break;
  case 'Q': *modifyP = *modifyP + Point4( 0.0, 0.0, +step, 0.0 ); break;

  case 'D': {
    std::list<TPolygon>::iterator iterP = model.polygonsList.begin();
    while ( iterP != model.polygonsList.end() ) {
      TPolygon *current = &(*iterP);
      TOGGLE_BIT( current->mapping, POLYGON_MAPPING_DOT );
      iterP++;
    }
    break; // dot
  }
  case 'G': {
    std::list<TPolygon>::iterator iterP = model.polygonsList.begin();
    while ( iterP != model.polygonsList.end() ) {
      TPolygon *current = &(*iterP);
      TOGGLE_BIT( current->mapping, POLYGON_MAPPING_GOURAUD );
      iterP++;
    }
    break; // gouraud
  }
  case 'T': {
    std::list<TPolygon>::iterator iterP = model.polygonsList.begin();
    while ( iterP != model.polygonsList.end() ) {
      TPolygon *current = &(*iterP);
      TOGGLE_BIT( current->mapping, POLYGON_MAPPING_TEXTURE );
      iterP++;
    }
    break; // texture
  }
  case 'P': {
    std::list<TPolygon>::iterator iterP = model.polygonsList.begin();
    while ( iterP != model.polygonsList.end() ) {
      TPolygon *current = &(*iterP);
      TOGGLE_BIT( current->mapping, POLYGON_MAPPING_CORRECT );
      iterP++;
    }
    break; // correct
  }
  case 'F': {
    std::list<TPolygon>::iterator iterP = model.polygonsList.begin();
    while ( iterP != model.polygonsList.end() ) {
      TPolygon *current = &(*iterP);
      TOGGLE_BIT( current->mapping, POLYGON_MAPPING_FLAT );
      iterP++;
    }
    break; // flat
  }
  case 'W': {
    std::list<TPolygon>::iterator iterP = model.polygonsList.begin();
    while ( iterP != model.polygonsList.end() ) {
      TPolygon *current = &(*iterP);
      TOGGLE_BIT( current->mapping, POLYGON_MAPPING_WIRE );
      iterP++;
    }
    break; // wire
  }

  case VK_HOME: camera.setFov( camera.getFov()+step); break;
  case VK_END: camera.setFov( camera.getFov()-step); break;
  case VK_ADD: step *= 1.1; break;
  case VK_SUBTRACT: step *= 0.9; break;

  case VK_NUMPAD0: camera.roll( rads(step) ); break;
  case VK_DECIMAL: camera.roll( -rads(step) ); break;
  case VK_NUMPAD1: camera.yaw( rads(step ) ); break;
  case VK_NUMPAD3: camera.yaw( -rads(step ) ); break;
  case VK_NUMPAD7: camera.pitch( rads(step) ); break;
  case VK_NUMPAD4: camera.pitch( -rads(step) ); break;

  case ' ':
    camera.position = Point4( 0, 0, 150, 0 );
    camera.orientation = Vector4( 0, 0, 0, 0 );
    break;
  case 'B':
    camera.position = Point4( 110, 0, 40, 0 );
    camera.orientation = Vector4( 0, 1.22, 0, 0 );
    break;
  case 'X':
    camera.position = Point4( 0, 90, 90, 0 );
    camera.orientation = Vector4( 0.87, 0, 0, 0 );
    break;
  case 'V':
    camera.position = Point4( 0, 0, 150, 0 );
    camera.setLookAt( Point4( -47.17, -68.21, 0, 0 ) );
    break;
  }
}


//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  static int LastX, LastY;
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
    key_in(wParam);
    break;

  case WM_LBUTTONDOWN: {
    LastX = (short)LOWORD(lParam);
    LastY = (short)HIWORD(lParam);
    
    SetCapture(hWnd);
    break;
  }
    
  case WM_MOUSEMOVE: {
    if ( wParam & MK_LBUTTON ) {
      int X = (short)LOWORD(lParam);
      int Y = (short)HIWORD(lParam);
      int DeltaX = LastX - X;
      int DeltaY = LastY - Y;
      
      LastX = X;
      LastY = Y;
     
      //      camera.updatePosition( 1.0, -radiansFrom(DeltaX), -radiansFrom(DeltaY), TRUE );
 
      InvalidateRect(hWnd,0,FALSE);
      UpdateWindow(hWnd);
    }
    break;
  }
    
  case WM_LBUTTONUP: {
    ReleaseCapture();
    break;
  }

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
  err = g_pDD->SetDisplayMode( resolution.Width, resolution.Height, resolution.BPP );
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
  
  // Init display surface
  frontend_surface.bpp = resolution.BPP;
  frontend_surface.width = resolution.Width;
  frontend_surface.height = resolution.Height;
  switch( frontend_surface.bpp ) {
  case 24: case 32:
    frontend_surface.blue_depth = 8;
    frontend_surface.green_depth = 8;
    frontend_surface.red_depth = 8;
    frontend_surface.blue_shift = 0;
    frontend_surface.green_shift = 8;
    frontend_surface.red_shift = 16;
    break;
  case 16:
    frontend_surface.blue_depth = 5;
    frontend_surface.green_depth = 5;
    frontend_surface.red_depth = 5;
    frontend_surface.blue_shift = 0;
    frontend_surface.green_shift = 5;
    frontend_surface.red_shift = 10;
    break;
  case 8: {
    frontend_surface.blue_depth = 2;
    frontend_surface.green_depth = 3;
    frontend_surface.red_depth = 3;
    frontend_surface.blue_shift = 0;
    frontend_surface.green_shift = 2;
    frontend_surface.red_shift = 5;
    // create RGB palette
    int index = 0;
    int rCount = (1<<frontend_surface.red_depth)-1;
    int gCount = (1<<frontend_surface.green_depth)-1;
    int bCount = (1<<frontend_surface.blue_depth)-1;
    for ( int r = 0; r < rCount+1; r++ )
      for ( int g = 0; g < gCount+1; g++ )
	for ( int b = 0; b < bCount+1; b++ ) {
	  RGBPalette.palPalEntry[index].peRed = r*(255/rCount);  // R
	  RGBPalette.palPalEntry[index].peGreen = g*(255/gCount);  // G
	  RGBPalette.palPalEntry[index].peBlue = b*(255/bCount);  // B
	  RGBPalette.palPalEntry[index].peFlags = PC_RESERVED;
	  index++;
      }
    hpalApp = CreatePalette(&RGBPalette);
    HDC Display = GetDC(hWnd);
    SelectPalette(Display,hpalApp,FALSE);
    RealizePalette(Display);
    ReleaseDC(hWnd,Display);
    break;
  }
  }

  int i, j;

  // Read font
  if ( !readFont( "font.dat" ) )
    return InitFail(hWnd, err, "Can't load: font.dat");

  // Create render buffer
  // 256 x proportional height
  renderBitmap.create( 256, ((float)256.0/(float)resolution.Width)*(float)resolution.Height );
  renderBitmap2.create( 256, ((float)256.0/(float)resolution.Width)*(float)resolution.Height );
  // Create information buffer
  informationBitmap.create( 350, resolution.Height/2 );

  // Read a texture
  if ( !texture.loadPCX("texture.pcx") )
  // if ( !texture.loadTGA("C_SCIENC.tga",320,200) )
    return InitFail(hWnd, err, "Can't load: texture.pcx");

  // Create mipmap
  //  if ( !texture.createMipmap() )
  //    printf("Can't create mipmap with texture.\n");

  // Create frame
  screenFrame.addBitmap( "information", &informationBitmap, 270, 10, BITMAP_MOV,
			 FRAME_ADD_BORDER|FRAME_CLEAR_ON_DISPLAY );
  screenFrame.addBitmap( "render", &renderBitmap, 10, 10, BITMAP_MOV,
			 FRAME_ADD_BORDER|FRAME_CLEAR_ON_DISPLAY );
  screenFrame.addBitmap( "render2", &renderBitmap2, 10, 210, BITMAP_MOV,
			 FRAME_ADD_BORDER|FRAME_CLEAR_ON_DISPLAY );

  // Set camera information
  camera.setPosition( 0, 0, 150 );
  camera.setNearFar(10,1000);
  camera.setFov(90);
  camera.setAspect( renderBitmap.getWidth()/renderBitmap.getHeight() );
  camera.setLookAt( Vector4( 0, 0, 0, 0 ) );

  // Set light information
  light.setPosition( 0, 0, 100 );
  light.direction = Vector4( 0, 0, -1, 0 );
  light.angle = M_PI/4;
  light.color = TColor::red;
  light.brightness = 1.0f;
  light.precalc();

  // Set model information
  TPolygon poly;
  TVertex vert;
  vert.position = Point4( -50, -50, 0, 1 );
  vert.color = TColor::red*0.5;
  vert.state = VERTEX_STATE_NOTHING;
  poly.verticesList.push_back( vert ); // put vertex in polygon
  vert.position = Point4( 50, -50, 0, 1 );
  vert.color = TColor::black;
  vert.state = VERTEX_STATE_NOTHING;
  poly.verticesList.push_back( vert ); // put vertex in polygon
  vert.position = Point4( 50, 50, 0, 1 );
  vert.color = TColor::blue*0.5;
  vert.state = VERTEX_STATE_NOTHING;
  poly.verticesList.push_back( vert ); // put vertex in polygon
  vert.position = Point4( -50, 50, 0, 1 );
  vert.color = TColor::black;
  vert.state = VERTEX_STATE_NOTHING;
  poly.verticesList.push_back( vert ); // put vertex in polygon
  //poly.mapping = POLYGON_MAPPING_GOURAUD|POLYGON_MAPPING_DOT;
  poly.mapping = POLYGON_MAPPING_WIRE;
  poly.texture = &texture;
  poly.color = TColor::white;
  poly.state = POLYGON_STATE_NOTHING;
  poly.computeNormal();
  model.polygonsList.push_back(poly); // put polygon in model

  // englobing sphere
  model.computeClippingSphere();

//--  // Clip test
//--  // point    0, 0, 0
//--  // normal   1, 0, 0
//--  printf("BEFORE:\n"); fflush(stdout);
//--  list<TVertex>::iterator iterV = poly.verticesList.begin();
//--  j = 0;
//--  while ( iterV != poly.verticesList.end() ) {
//--    TVertex *vert = &(*iterV);
//--    printf("  Vertices [%d]  pos: %5.2f  %5.2f  %5.2f  texel: %5.2f  %5.2f  color: %3d %3d %3d\n",
//--	   j,
//--	   vert->position[0], vert->position[1], vert->position[2],
//--	   vert->texel.u, vert->texel.v,
//--	   vert->color.r(), vert->color.g(), vert->color.b() ); fflush(stdout);
//--    iterV++;
//--    j++;
//--  }
//--  Plane plane( 1, 0, 0, 0 );
//--  clipToPlane( poly, plane );
//--  printf("AFTER:\n"); fflush(stdout);
//--  iterV = poly.verticesList.begin();
//--  j = 0;
//--  while ( iterV != poly.verticesList.end() ) {
//--    TVertex *vert = &(*iterV);
//--    printf("  Vertices [%d]  pos: %5.2f  %5.2f  %5.2f  texel: %5.2f  %5.2f  color: %3d %3d %3d\n",
//--	   j,
//--	   vert->position[0], vert->position[1], vert->position[2],
//--	   vert->texel.u, vert->texel.v,
//--	   vert->color.r(), vert->color.g(), vert->color.b() ); fflush(stdout);
//--    iterV++;
//--    j++;
//--  }
//--  exit(-1);

  // tesselate -- NO NEED TO TESSELATE, DONE BEFORE RENDERING
  //tesselate( model.polygonsList );

  // Dump model informations
  printf("Sphere: %5.2f %5.2f %5.2f   Radius: %5.2f\n",
	 model.clipCenter[0], model.clipCenter[1], model.clipCenter[2], model.clipRadius );
  std::list<TPolygon>::iterator iterP = model.polygonsList.begin();
  i = 0;
  while ( iterP != model.polygonsList.end() ) {
    TPolygon *poly = &(*iterP);
    printf("Poly [%d] normal: %5.2f %5.2f %5.2f  color: %3d %3d %3d   mapping: %d\n", i,
	   poly->normal[0], poly->normal[1], poly->normal[2],
	   poly->color.r(), poly->color.g(), poly->color.b(),
	   poly->mapping );
    std::list<TVertex>::iterator iterV = poly->verticesList.begin();
    j = 0;
    while ( iterV != poly->verticesList.end() ) {
      TVertex *vert = &(*iterV);
      printf("  Vertices [%d]  pos: %5.2f  %5.2f  %5.2f  color: %3d %3d %3d\n",
	     j,
	     vert->position[0], vert->position[1], vert->position[2],
	     vert->color.r(), vert->color.g(), vert->color.b() );
      iterV++;
      j++;
    }
    iterP++;
    i++;
  }

//--//--  int i;
//--//--  // Read a torus
//--//--  if ( !loadXYZ( "c:\\watcom\\sources\\fromhome\\new3d2\\models\\tor.xyz", torus ) )
//--//--    return InitFail(hWnd, err, "Can't load: tor.xyz");
//--//--  //torus.position = camera.position + camera.distance * TVector_4( 0, 0, 1 ) + TVector_4( 100, 0, 0 );
//--//--  torus.position = TPoint_4( 0, 0, 200 );
//--//--  for ( i = 0; i < torus.numBasePolygons; i++ ) {
//--//--    torus.basePolygons[i].mapping = WIREFRAME_MAP;
//--//--    torus.basePolygons[i].color = TColor( 255, 255, 255 );
//--//--  }
//--//--  // Compute englobing sphere
//--//--  TPoint_4 *vertices = new TPoint_4 [torus.numBaseVertices];
//--//--  for ( i = 0; i < torus.numBaseVertices; i++ )
//--//--    vertices[i] = torus.baseVertices[i].pixel;
//--//--  torus.englobing = TSphere( vertices, torus.numBaseVertices );
//--//--
//--//--  // Read an ellipse
//--//--  if ( !loadXYZ( "c:\\watcom\\sources\\fromhome\\new3d2\\models\\ellipse.xyz", ellipse ) )
//--//--    return InitFail(hWnd, err, "Can't load: ellipse.xyz");
//--//--  //  ellipse.position = TPoint_4( 0, 0, -200 );
//--//--  ellipse.position = camera.position - camera.distance * TVector_4( 0, 0, 3 );
//--//--  for ( i = 0; i < ellipse.numBasePolygons; i++ ) {
//--//--    ellipse.basePolygons[i].mapping = WIREFRAME_MAP;
//--//--    ellipse.basePolygons[i].color = TColor( 255, 255, 255 );
//--//--  }
//--//--  // Compute englobing sphere
//--//--  TPoint_4 *vertices2 = new TPoint_4 [ellipse.numBaseVertices];
//--//--  for ( i = 0; i < ellipse.numBaseVertices; i++ )
//--//--    vertices2[i] = ellipse.baseVertices[i].pixel;
//--//--  ellipse.englobing = TSphere( vertices2, ellipse.numBaseVertices );
//--//--  
//--//--  //  world.numObjects = 2;
//--//--  //  world.objectList[0] = torus;
//--//--  //  world.objectList[1] = ellipse;
//--//--  world.numObjects = 1;
//--//--  world.objectList = new TObject [world.numObjects];
//--//--  world.objectList[0] = torus;

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
  if (!InitApp(hInstance, nCmdShow))
    return FALSE;

  //char *argv[] = {"rt.exe", lpCmdLine};

  MSG msg;

  startC = clock();

  while (TRUE) {
    if ( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) ) {
      if ( !GetMessage(&msg, NULL, 0, 0) )
	break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else if ( g_bActive ) {
      BuildImage(); // render and raster
    }
    else
      WaitMessage();
  }

  endC = clock();
  double fps = (double)num_frames * (double)CLOCKS_PER_SEC / (double)(endC-startC);
  printf("%d in %2.2f sec.\nfps: %2.2f\n",
	 num_frames, ((double)(endC-startC)/(double)CLOCKS_PER_SEC), fps );

  ExitApp();
  return 0;
}


// Raster an image (bitmap) in a surface, BPP conversion
void raster( TSurface *surface, TBitmap &bitmap, const int displayMinX, const int displayMinY ) {
  bitmap.display( surface, displayMinX, displayMinY, BITMAP_MOV, TRUE );
  bitmap.clear();
}

// Raster a frame
void raster( TSurface *surface, const TFrame &frame ) {
  frame.display( surface );
}
