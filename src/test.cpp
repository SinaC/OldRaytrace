//***********************************************
//***********************************************
//***********************************************
//***********************************************
//**************** TEST BSP *********************
//***********************************************
//***********************************************
//***********************************************
//***********************************************


// include files
#include <fstream>
#include <stdio.h>
#include <time.h>

#include "e:/openPTC/source/ptc.h"

#include "types/basics.h"
#include "text/basics.h"

//--#define __FULLSCREEN__

#include "world.h"
#include "bsp.h"
#include "object.h"
#include "primitives/primitive.h"
#include "primitives/sphere.h"
#include "boundings/aabb.h"
#include "ray.h"
#include "poly3d/camerasimple.h"
#include "poly3d/rendersimple.h"
#include "intersection.h"

static const u32 XRES = 640;
static const u32 YRES = 480;

TCameraSimple cameraSimple;
TRay ray;
TWorld world;

bool key_in( const int k ) {

  switch (k) {
    // move cameraSimple
  case VK_NUMPAD2: cameraSimple.angleX -= 0.05f; break;
  case VK_NUMPAD8: cameraSimple.angleX += 0.05f; break;
  case VK_NUMPAD4: cameraSimple.angleY -= 0.05f; break;
  case VK_NUMPAD6: cameraSimple.angleY += 0.05f; break;
  case VK_NUMPAD9: cameraSimple.angleZ -= 0.05f; break;
  case VK_NUMPAD3: cameraSimple.angleZ += 0.05f; break;
    // move ray.origin
  case 'E': ray.origin += TPoint3( 0, +10.0, 0 ); break;
  case 'D': ray.origin += TPoint3( 0, -10.0, 0 ); break;
  case 'F': ray.origin += TPoint3( +10.0, 0, 0 ); break;
  case 'S': ray.origin += TPoint3( -10.0, 0, 0 ); break;
  case 'Z': ray.origin += TPoint3( 0, 0, -10.0 ); break;
  case 'R': ray.origin += TPoint3( 0, 0, +10.0 ); break;
    // move ray.direction
  case 'U': ray.direction += TVector3( 0, +0.1f, 0 ); break;
  case 'J': ray.direction += TVector3( 0, -0.1f, 0 ); break;
  case 'K': ray.direction += TVector3( +0.1f, 0, 0 ); break;
  case 'H': ray.direction += TVector3( -0.1f, 0, 0 ); break;
  case 'Y': ray.direction += TVector3( 0, 0, -0.1f ); break;
  case 'I': ray.direction += TVector3( 0, 0, +0.1f ); break;

  case VK_ESCAPE: return false; break;
    break;
  }
  return true;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) {
  // message handler
  switch (uMsg) { 
    
  case WM_KEYDOWN: {    
    // handle keypress
    if (wParam==VK_ESCAPE) {
      // exit on escape key
      PostMessage(hWnd,WM_DESTROY,0,0);
    }
    else
      key_in( wParam );
    break;
  }
    
  case WM_CLOSE: {
    // destroy window on close message
    PostMessage(hWnd,WM_DESTROY,0,0);
    break;
  }
    
  case WM_DESTROY: {
    // post quit message
    PostQuitMessage(0);
    break;
  }
    
  default: {
    // default window action
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
  }
  }
  
  // handled
  return 0;
}

HWND create(HINSTANCE hInstance,int nCmdShow) {
#ifdef __FULLSCREEN__

    // fullscreen output style
    DWORD style = WS_POPUP;

    // window position
    const int x = 0;
    const int y = 0;

    // setup window rect to cover the entire screen
    RECT rect = {0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)};

#else

    // windowed output style
    DWORD style = WS_OVERLAPPEDWINDOW;

    // setup window position
    const int x = CW_USEDEFAULT;
    const int y = CW_USEDEFAULT;

    // setup client area rect
    RECT rect = {0,0,XRES,YRES};

    // adjust window rect for window style
    AdjustWindowRect(&rect,style,0);

#endif

    // setup window class
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance,"IDI_MAIN");
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "WNDPROC_EXAMPLE_WINDOW";

    // register window class
    RegisterClass(&wc);

    // create window
    HWND hWnd = CreateWindow("WNDPROC_EXAMPLE_WINDOW","RTRT Engine",WS_OVERLAPPEDWINDOW,
			     x,y,rect.right-rect.left,rect.bottom-rect.top,
			     0,0,hInstance,0);
    
    // check window was created successfully
    if (!hWnd) throw Error("could not create window");

    // show the window
    ShowWindow(hWnd,nCmdShow);

    // return window
    return hWnd;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow) {   

  TObject *o1 = new TObject;
  {
  PrimitiveSphere *p1 = new PrimitiveSphere;
  p1->center = TPoint3( -850, -850, -850 ); p1->radius = 50;
  o1->primitive = p1;
  o1->bounding = new BoundingAABB;
  ((BoundingAABB*)(o1->bounding))->min = TPoint3( -900, -900, -900 );
  ((BoundingAABB*)(o1->bounding))->max = TPoint3( -800, -800, -800 );
  }

  TObject *o2 = new TObject;
  {
  PrimitiveSphere *p2 = new PrimitiveSphere;
  p2->center = TPoint3( 850, 850, 850 ); p2->radius = 50;
  o2->primitive = p2;
  o2->bounding = new BoundingAABB;
  ((BoundingAABB*)(o2->bounding))->min = TPoint3( 800, 800, 800 );
  ((BoundingAABB*)(o2->bounding))->max = TPoint3( 900, 900, 900 );
  }

  TObject *o3 = new TObject;
  {
  PrimitiveSphere *p3 = new PrimitiveSphere;
  p3->center = TPoint3( 0, 0, 0 ); p3->radius = 100;
  o3->primitive = p3;
  o3->bounding = new BoundingAABB;
  ((BoundingAABB*)(o3->bounding))->min = TPoint3( -100, -100, -100 );
  ((BoundingAABB*)(o3->bounding))->max = TPoint3( 100, 100, 100 );
  }

  TObject *o4 = new TObject;
  {
  PrimitiveSphere *p4 = new PrimitiveSphere;
  p4->center = TPoint3( -850, -850, -850 ); p4->radius = 50;
  o4->primitive = p4;
  o4->bounding = new BoundingAABB;
  ((BoundingAABB*)(o4->bounding))->min = TPoint3( -900, -900, 900 );
  ((BoundingAABB*)(o4->bounding))->max = TPoint3( -800, -800, 800 );
  }

  TObject *o5 = new TObject;
  {
  PrimitiveSphere *p5 = new PrimitiveSphere;
  p5->center = TPoint3( 850, 850, -850 ); p5->radius = 50;
  o5->primitive = p5;
  o5->bounding = new BoundingAABB;
  ((BoundingAABB*)(o5->bounding))->min = TPoint3( 800, 800, -800 );
  ((BoundingAABB*)(o5->bounding))->max = TPoint3( 900, 900, -900 );
  }

  TObject *o6 = new TObject;
  {
  PrimitiveSphere *p6 = new PrimitiveSphere;
  p6->center = TPoint3( 75, 0, 0 ); p6->radius = 100;
  o6->primitive = p6;
  o6->bounding = new BoundingAABB;
  ((BoundingAABB*)(o6->bounding))->min = TPoint3( -25, -100, -100 );
  ((BoundingAABB*)(o6->bounding))->max = TPoint3( 175, 100, 100 );
  }

  world.objects.push_back( o1 ); 
  world.objects.push_back( o2 );
  world.objects.push_back( o3 );
  world.objects.push_back( o4 );
  world.objects.push_back( o5 ); 
  world.objects.push_back( o6 );

  //--ray.origin = TPoint3( -500, -500, -500 );
  //--ray.direction = TVector3( 1, 1, 1 );
  ray.origin = TPoint3( -1100, -850, -850 );
  ray.direction = TVector3( 1, 0, 0 );
  ray.update();

  printf("===================\n"
	 "        BSP        \n"
	 "===================\n");
  world.bsp = new BSPTree;
  world.bsp->build( world.objects );
  
  printf("DUMPING BSP\n"
	 "===========\n");
  world.bsp->print("BSP");

  printf("BSP TRAVERSAL CLOSEST\n"
	 "=====================\n");
  TObject *obj;
  float dist;
  if ( world.bsp->closestIntersection( ray, obj, dist ) ) {
    printf("closest intersection: %5.5f\n", dist );
    obj->print("closest object");
  }
  else
    printf("no intersection\n");

  printf("BSP TRAVERSAL FIRST\n"
	 "===================\n");
  if ( world.bsp->firstIntersection( ray, obj ) ) {
    printf("first intersection\n" );
    obj->print("first object");
  }
  else
    printf("no intersection\n");

  cameraSimple.angleX = 0;
  cameraSimple.angleY = 0;
  cameraSimple.angleZ = 0;
  cameraSimple.zModifier = 2000.0f;
  cameraSimple.projectionFactor = 200.0f;

  try {
    // create format
    Format format( 32, 0x00ff0000, 0x0000ff00, 0x000000ff );

    // create output window manually
    HWND hWnd = create( hInstance, nCmdShow );

    // create console
    Console console;
    
#ifdef FULLSCREEN
    // force fullscreen output
    console.option("DirectX");
    console.option("fullscreen output");
#else
    // force windowed output
    console.option("DirectX");
    console.option("windowed output");
    console.option("fixed window");
    console.option("center window");
#endif
    
    // open console on window
    console.open( hWnd, XRES, YRES, format );

    // create surface
    Surface surface( XRES, YRES, format );

    printf("===============\n"
	   "START RENDERING\n"
	   "===============\n");fflush(stdout);
    
    bool continueLoop = true;
    // main loop
    while ( continueLoop ) {

      // msg data
      MSG message;
     
      // process all window messages
      while (PeekMessage(&message,hWnd,0,0,PM_REMOVE)) {
	// handle WM_QUIT message
	if ( message.message == WM_QUIT )
	  continueLoop = false;
	
	// translate and dispatch
	TranslateMessage(&message);
	DispatchMessage(&message);
      }
      
      // lock surface pixels
      u32 *vs = (u32 *)surface.lock();
      memset( vs, 0, XRES*YRES*4 );

      printf("=========\n"
	     "==FRAME==\n"
	     "=========\n");

      // RENDER HERE
      cameraSimple.update();
      ray.update();

      world.bsp->render( vs, XRES, YRES, cameraSimple );
      renderLine( vs, XRES, YRES, ray.origin,
		  ray.origin + ( ray.direction * 2500.0f ),
		  0x00ffffff, cameraSimple );
      TIntersection intersection;
      if ( world.findClosestIntersection( ray, intersection ) )
	intersection.object->bounding->renderSimple( vs, XRES, YRES, 0x000000ff, cameraSimple );


      // unlock surface
      surface.unlock();
      // copy to console
      surface.copy(console);
      // update console
      console.update();
    }

    printf("=======\n"
	   "LEAVING\n"
	   "=======\n");fflush(stdout);

    console.close();
  }
  catch (Error &error) {
    // report error
    error.report();
  }
  
  return 0;
}
