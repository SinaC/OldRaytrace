// include files
#include <fstream>
#include <stdio.h>
#include <time.h>

#include "c:/openPTC/source/ptc.h"

//--#define __FULLSCREEN__
//--#define __USE__MOUSE__

//--#define __SETUP_TEST__

#include "world.h"
#include "all_renderers.h"
#include "main_common.h"
#include "fonts/font.h"
#include "animators/animator.h"
#include "cameras/camera.h"
#include "globals.h"
#include "parser/scene_parser.h"
#include "parser/context.h"
#include "memory.h"
#include "error.h"

#ifdef __USE__MOUSE__
#include "ray.h"
#include "poly3d/wire.h"
#include "object.h"
#include "primitives/primitive.h"
#endif

#define defaultSceneFilename "test.scn"
char sceneFilename[4096];

//static const u32 XRES = 320;
//static const u32 YRES = 240;

static TWorld *world;

static TRenderer *renderer;
//--static RendererQuincunx renderer;
//--static RendererInterpolated renderer;
//--static RendererBilinear renderer;

#ifdef __USE__MOUSE__
static int mouseX, mouseY;
static Console *rayConsolePTR;
static Surface *raySurfacePTR;
static TRay mouseRay;
static const u32 MOUSEXRES = 640;
static const u32 MOUSEYRES = 480;
//#define __TEST_MOUSE__
#ifdef __TEST_MOUSE__
static const u32 testMouseX = 213;
static const u32 testMouseY = 137;
#endif
#endif

#ifdef __USE__MOUSE__
void renderRay( u32 *buf, const u32 width, const u32 height, const TRay &ray, u32 y ) {
  static int depth = 0;
  depth++;

  // fill a square with ray.intersection color
  for ( int yi = 0; yi < 10; yi++ )
    for ( int xi = 0; xi < 10; xi++ )
      *(buf+xi+depth*10+yi*width) = clampRGB32( world->shadeRay( ray ) );
  if ( depth == 1 )
    y += 10;
  
  // trace the rays projected on oY plane
  int ox, oy, tx, ty;
  ox = (int)(ray.origin[__X] + (float)width*0.5f);
  oy = height - (int)(ray.origin[__Z] + (float)height*0.5f);
  if ( ray.intersection.distance < INFINITE ) {
    TVector3 target = ray.target;//ray.origin + ( ( ray.target - ray.origin ) * 5.0f );
    tx = (int)(target[__X] + (float)width*0.5f);
    ty = height - (int)(target[__Z] + (float)height*0.5f);
  }
  else {
    TVector3 target = ray.origin + ( ray.direction * 1000.0f );
    tx = (int)(target[__X] + (float)width*0.5f);
    ty = height - (int)(target[__Z] + (float)height*0.5f);
  }
  
  u32 color;
  switch( ray.kind ) {
  case TRay::RAY_CAMERA: color = clampRGB32( TColor::white ); break;
  case TRay::RAY_REFLECTED: color = clampRGB32( TColor::green ); break;
  case TRay::RAY_TRANSMITTED: color = clampRGB32( TColor::blue ); break;
  case TRay::RAY_SHADOW: color = clampRGB32( TColor::yellow ); break;
  case TRay::RAY_INTERNAL_REFLECTED: color = clampRGB32( TColor::cyan ); break;
  default: case TRay::RAY_NOTHING: color = clampRGB32( TColor::red ); break;
  }
  bresen( buf, width, height, ox, oy, tx, ty, color );

  // dump informations
  if ( ray.id != 0 ) {
    displayString( 0, y, 0x00ffffff, 0x00000000, false, buf, width, height,
		   "%s[%d] (%5.2f,%5.2f,%5.2f)",
		   ray.intersection.object->name, depth,
		   ray.intersection.pointWorldSpace[__X],
		   ray.intersection.pointWorldSpace[__Y],
		   ray.intersection.pointWorldSpace[__Z] ); y += 8;
    displayString( 0, y, 0x00ffffff, 0x00000000, false, buf, width, height,
		   "dir (%5.2f,%5.2f,%5.2f)  ori (%5.2f,%5.2f,%5.2f)",
		   ray.direction[__X],
		   ray.direction[__Y],
		   ray.direction[__Z],
		   ray.origin[__X],
		   ray.origin[__Y],
		   ray.origin[__Z] ); y += 8;
    displayString( 0, y, 0x00ffffff, 0x00000000, false, buf, width, height,
		   "normal (%5.2f,%5.2f,%5.2f)",
		   ray.intersection.normal[__X],
		   ray.intersection.normal[__Y],
		   ray.intersection.normal[__Z] ); y += 8;
    displayString( 0, y, 0x00ffffff, 0x00000000, false, buf, width, height,
		   "[%d] [%d] %s[%d]",
		   ray.intersection.inverse,
		   ray.intersection.additionalInformation.side,
		   (ray.intersection.CSGPrimitive==NULL)?
		   "(NONE)":ray.intersection.CSGPrimitive->name,
		   (ray.intersection.CSGPrimitive==NULL)?
		   -1:ray.intersection.CSGPrimitive->primitiveKind
		   ); y += 8;
  }
  else {
    displayString( 0, y, 0x00ffffff, 0x00000000, false, buf, width, height,
		   "atmosphere" ); y += 8;
    displayString( 0, y, 0x00ffffff, 0x00000000, false, buf, width, height,
		   "dir (%5.2f,%5.2f,%5.2f)  ori (%5.2f,%5.2f,%5.2f)",
		   ray.direction[__X],
		   ray.direction[__Y],
		   ray.direction[__Z],
		   ray.origin[__X],
		   ray.origin[__Y],
		   ray.origin[__Z] ); y += 8;
  }

  // recursive call for reflection and transmission
  if ( ray.reflectedRay != NULL ) {
    displayString( 0, y, 0x00ffffff, 0x00000000, false, buf, width, height,
		   "reflected ray:" ); y += 8;
    renderRay( buf, width, height, *ray.reflectedRay, y );
  }
  if ( ray.transmittedRay != NULL ) {
    displayString( 0, y, 0x00ffffff, 0x00000000, false, buf, width, height,
		   "transmitted ray:" ); y += 8;
    renderRay( buf, width, height, *ray.transmittedRay, y );
  }

  depth--;
}

void renderRay( const TRay &ray ) {

  u32 *vs = (u32*)raySurfacePTR->lock();
  u32 width = raySurfacePTR->width();
  u32 height = raySurfacePTR->height();

  ray.print("SHOOTED RAY: ");

  memset( vs, 0, width*height*4 );
  renderRay( vs, width, height, ray, 0 );

  //--  displayString( 0, 400, 0x00ffffff, 0x00000000, false, vs, width, height,
  //--		 "%3d, %3d",
  //--		 mouseX, mouseY );

  raySurfacePTR->unlock();
  raySurfacePTR->copy(*rayConsolePTR);
  rayConsolePTR->update();
}
#endif

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  // message handler
  switch (uMsg) { 

#ifdef __USE__MOUSE__
  case WM_LBUTTONDOWN: {
    if ( !globalSettings::mouseOn )
      break;
    // click on left button
    mouseX = (short)LOWORD(lParam);
    mouseY = (short)HIWORD(lParam);
    // shoot a ray
    if ( mouseX >= 0
	 && mouseX < XRES
	 && mouseY >= 0
	 && mouseY < YRES ) {
      mouseRay.clear();

#ifdef __TEST_MOUSE__
      mouseX = testMouseX; mouseY = testMouseY;
#endif

      const TCamera *currentCamera = world->currentCamera;
      
      TVector3 topLeft, rightStep, upStep;
      currentCamera->computeDirectionVector( XRES, YRES, topLeft, rightStep, upStep );
      
      mouseRay.origin = currentCamera->location; mouseRay.kind = TRay::RAY_CAMERA;
      mouseRay.direction = topLeft - (((float)mouseY)*upStep) + (((float)mouseX)*rightStep);
      mouseRay.direction.normalize();
      
      world->traceRayNoShading( mouseRay, 1.0f );
      renderRay( mouseRay );
    }

    SetCapture(hWnd);
    break;
  }
    
  case WM_MOUSEMOVE : {
    mouseX = (short)LOWORD(lParam);
    mouseY = (short)HIWORD(lParam);
    break;
  }
#endif
    
  case WM_KEYDOWN: {    
    // handle keypress
    if ( !key_in( wParam, *world ) )
      PostMessage(hWnd,WM_DESTROY,0,0); // exit on escape key
    break;
  }
    
  case WM_CLOSE: {
    // destroy window on close message
    PostMessage(hWnd,WM_DESTROY,0,0);
    break;
  }
    
  case WM_DESTROY: {
    // post quit message
    //--    PostQuitMessage(0);
    PostMessage(hWnd,WM_QUIT,0,0);
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

HWND create( HINSTANCE hInstance, int nCmdShow ) {
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
    RECT rect = {0,0,globalSettings::width,globalSettings::height};

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
    //--if (!hWnd) throw Error("could not create window");
	return NULL;

    // show the window
    ShowWindow(hWnd,nCmdShow);

    // return window
    return hWnd;
}

bool ptc_key_in( const Key &key ) {
	  static const enum EStates {
	    STATE_CAMERA_LOC,
	    STATE_CAMERA_LOOKAT
	};
	static int currentState = STATE_CAMERA_LOC;
	static TVector3 *current = &( world->currentCamera->location );
	static int screenshotCount = 0;
	switch ( key.code() ) {
		// PAUSE
		case Key::PAUSE: globalSettings::pauseOn = !globalSettings::pauseOn; break;
		// STEP
		case Key::SUBTRACT: globalSettings::moveStep -= 1.0f; break;
		case Key::ADD: globalSettings::moveStep += 1.0f; break;
		// FOV
		case Key::INSERT: globalSettings::lightAttenuation *= 10.0f; break;
		//--case Key::DELETE: globalSettings::lightAttenuation /= 10.0f; break;
		case 0x7F: globalSettings::lightAttenuation /= 10.0f; break;
		// Move & target choice
		case Key::D:
			if ( currentState == STATE_CAMERA_LOC ) {
				current = &( world->currentCamera->lookat );
				currentState = STATE_CAMERA_LOOKAT;
			}
			else {
				current = &( world->currentCamera->location );
				currentState = STATE_CAMERA_LOC;
			}
			break;
		// Change current camera
		case Key::C: {
			int idx = 0;
			int i = 0;
			for ( vector<TCamera *>::const_iterator iterC = world->cameras.begin(); iterC != world->cameras.end(); iterC++, i++ )
				if ( *iterC == world->currentCamera ) {
					idx = i;
					break;
				}
			i = ( idx + 1 ) % world->cameras.size();
			world->currentCamera = world->cameras[i];
			break;
		}
		// Move camera
		case Key::END: world->currentCamera->fov += globalSettings::moveStep; break;
		case Key::HOME: world->currentCamera->fov -= globalSettings::moveStep; break;
		case Key::LEFT:
			if ( currentState == STATE_CAMERA_LOC )
				*current += TVector3( globalSettings::moveStep, 0, 0 ) * world->currentCamera->getU();
			else
				*current += TVector3( globalSettings::moveStep, 0, 0 );
			break;
		case Key::RIGHT:
			if ( currentState == STATE_CAMERA_LOC )
				*current += TVector3( -globalSettings::moveStep, 0, 0 ) * world->currentCamera->getU();
			else
				*current += TVector3( -globalSettings::moveStep, 0, 0 );
			break;
		case Key::UP:
			if ( currentState == STATE_CAMERA_LOC )
				*current += TVector3( 0, 0, globalSettings::moveStep ) * world->currentCamera->getN();
			else
				*current += TVector3( 0, 0, globalSettings::moveStep );
			break;
		case Key::DOWN:
			if ( currentState == STATE_CAMERA_LOC )
				*current += TVector3( 0, 0, -globalSettings::moveStep ) * world->currentCamera->getN();
			else
				*current += TVector3( 0, 0, -globalSettings::moveStep );
			break;
		case Key::PAGEUP:
			if ( currentState == STATE_CAMERA_LOC )
				*current += TVector3( 0, globalSettings::moveStep, 0 ) * world->currentCamera->getV();
			else
				*current += TVector3( 0, globalSettings::moveStep, 0 );
			break;
		case Key::PAGEDOWN:
			if ( currentState == STATE_CAMERA_LOC )
				*current += TVector3( 0, -globalSettings::moveStep, 0 ) * world->currentCamera->getV();
			else
				*current += TVector3( 0, -globalSettings::moveStep, 0 );
			break;
	    // Global switches
		case Key::F1: globalSettings::shadingOn = !globalSettings::shadingOn; break;
		case Key::F2: globalSettings::shadowOn = !globalSettings::shadowOn; break;
		case Key::F3: globalSettings::reflectionOn = !globalSettings::reflectionOn; break;
		case Key::F4: globalSettings::transmissionOn = !globalSettings::transmissionOn; break;
		case Key::F5: globalSettings::atmosphericOn = !globalSettings::atmosphericOn; break;
		case Key::F6: globalSettings::shadowCacheOn = !globalSettings::shadowCacheOn; break;
		case Key::F7: globalSettings::whichMapping = ( globalSettings::whichMapping + 1 ) % globalSettings::MAPPING_MAX; break;
		case Key::F8: globalSettings::whichRenderingScreenshot = ( globalSettings::whichRenderingScreenshot + 1 ) % TRenderer::RENDERER_MAX; break;
		case Key::F9: globalSettings::diffuseOn = !globalSettings::diffuseOn; break;
		case Key::F11: globalSettings::ambientOn = !globalSettings::ambientOn; break;

	    // Dump
		case Key::SPACE:
			world->print("WORLD DUMP:");
			break;

		// Screenshot
		case Key::S: {
			char filename[1024];
			//sprintf( filename, "screen%4d.tga", screenshotCount );
			char tmp[1024];
			sprintf( tmp, "%4d", screenshotCount );
			int len = strlen(tmp);
			for ( int i = 0; i < len; i++ )
				if ( tmp[i] == ' ' )
					tmp[i] = '0';
				else
					break;
			sprintf( filename, "screen%s.tga", tmp );
			takeScreenshot( *world, filename );
			screenshotCount++;
			break;
		}

		// Quit
		case Key::ESCAPE: return false; break;
	  }
  return true;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow) {
  printf("===============\n"
	 "INITIALIZING...\n"
	 "===============\n"); fflush(stdout);

  // FIXME: be smarter ... tokenize lpCmdLine and get the right one
  if ( lpCmdLine != NULL && strlen(lpCmdLine) > 0 && nCmdShow > 0 ) {
//	printf("command line: %s\n", lpCmdLine );
	int i = 0;
	char *tok = strtok( lpCmdLine, " " );
	do {
//		printf("==>token[%d]: %s\n", i, tok ); fflush(stdout);
		switch( i ) {
		case 0: strcpy( sceneFilename, tok ); break;
		case 1: strcpy( globalSettings::path, tok ); break;
		}
		tok = strtok( NULL, " " );
		i++;
	} while ( tok != NULL );
//    strcpy( sceneFilename, lpCmdLine ); // 1st arg: scene filename
//	if ( nCmdShow > 1 )
//		strcpy( globalSettings::path, lpCmdLine );
  }
  else
    strcpy( sceneFilename, defaultSceneFilename );

//  globalSettings::print( "==============\n"
//			 "GLOBAL SETTING AFTER COMMAND LINE PARSING\n"
//			 "=========================================" );

  testProc();

  initRaytracing();

//--  world = new TWorld;
//--  if ( !initWorld( *world ) )
//--    error("Error while initializing world!\n");

  // Load world
  Context ctx;
  ctx.pushscope();
  world = readWorld( ctx, sceneFilename );
  ctx.popscope();
  world->setup();
  world->print("WORLD DUMP");
  fflush(stdout);

#ifdef __SETUP_TEST__
  shutdownRaytracing();
  return -1;
#endif

  // Create Renderer
  // TODO: create correct renderer using whichRendering
  switch ( globalSettings::whichRendering ) {
	  case TRenderer::RENDERER_NORMAL: renderer = new TRenderer(); break;
	  case TRenderer::RENDERER_BILINEAR: renderer = new RendererBilinear(); break;
	  case TRenderer::RENDERER_QUINCUNX: renderer = new RendererQuincunx(); break;
	  case TRenderer::RENDERER_FLIPQUAD: renderer = new RendererFlipquad(); break;
	  case TRenderer::RENDERER_GRID: renderer = new RendererGrid(); break;
	  case TRenderer::RENDERER_CORNER: renderer = new RendererCorner(); break;
	  case TRenderer::RENDERER_DIAMOND: renderer = new RendererDiamond(); break;
	  case TRenderer::RENDERER_RGSS2x2: renderer = new RendererRGSS(); break;
	  case TRenderer::RENDERER_INTERPOLATED: renderer = new RendererInterpolated(); break;
	  case TRenderer::RENDERER_QUINCUNX_ADAPTATIVE_COLOR:
	  case TRenderer::RENDERER_QUINCUNX_ADAPTATIVE_PRIMITIVE:
	  default:
		  renderer = new TRenderer();
  }
  renderer->setup( NULL, globalSettings::width, globalSettings::height );
  //--renderer.setup( NULL, XRES, YRES, 8 );

  globalSettings::print( "==============\n"
			 "GLOBAL SETTING\n"
			 "==============" );

  try {
    // create format
    Format format( 32, 0x00ff0000, 0x0000ff00, 0x000000ff ); // 32 bits 888
    //Format format( 16, 0xF800,0x7E0,0x1F ); // 16 bits 565
    //Format format( 16, 0x7C00,0x3E0,0x1F ); // 15 bits 555

    // create output window manually
    HWND hWnd = create( hInstance, nCmdShow );

#ifdef __USE__MOUSE__
    Console rayConsole;
    if ( globalSettings::mouseOn ) {
      // This console is used to render mouse ray, same for surface
      // Create raySurface and rayConsole
      rayConsole.option("DirectX");
      rayConsole.option("windowed output");
      rayConsole.option("fixed window");
      rayConsole.option("center window");
      rayConsole.open( "shooted ray", MOUSEXRES, MOUSEYRES, format );
    }
    Surface raySurface( MOUSEXRES, MOUSEYRES, format );
    raySurfacePTR = &raySurface;
    rayConsolePTR = &rayConsole;
#endif

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
    console.open( hWnd, globalSettings::width, globalSettings::height, format );

    // create surface
    Surface surface( globalSettings::width, globalSettings::height, format );

    printf("===============\n"
	   "START RENDERING\n"
	   "===============\n");fflush(stdout);

    // create timer
    Timer timer;
    
    // start timer
    timer.start();

    float before = 0, after = 0;
    long frameCount = 0, localFrameCount = 0;
    bool continueLoop = true;
    // main loop
    while ( continueLoop ) {

//-- OLD WAY
//      // msg data
//      MSG message;
//     
//      // process all window messages
//      while (PeekMessage(&message,hWnd,0,0,PM_REMOVE)) {
//	// handle WM_QUIT message
//	if ( message.message == WM_QUIT )
//	  continueLoop = false;
//	
//	// translate and dispatch
//	TranslateMessage(&message);
//	DispatchMessage(&message);
//      }
//-- NEW WAY
		while ( console.key() )
			continueLoop = ptc_key_in( console.read() );

		if ( continueLoop == false )
		  break;
      
      // lock surface pixels
      //u32 *vs = (u32 *)surface.lock();
      renderer->surface = (u32 *)surface.lock();

      // RENDER HERE
      frameCount++; localFrameCount++;
      stats.clear();
      if ( !globalSettings::pauseOn
			&& world->animator != NULL )
			world->animator->animate();
      world->currentCamera->update();
      world->precalcFrame();
      before += timer.time();
      //--renderer.render( 160, 100, *world, *world->currentCamera );
      //--renderer.renderAdaptativePrimitive( *world, *world->currentCamera, 16 );
      //--renderer.renderAdaptativeColor( *world, *world->currentCamera, 16 );
      renderer->render( *world );
#ifdef __USE__MOUSE__
#ifdef __TEST_MOUSE__
      if ( globalSettings::mouseOn )
		*(renderer->surface+testMouseX+testMouseY*renderer->width) = 0x00FF0000;
#endif
#endif
      after += timer.time();
      const float fps = (float)localFrameCount / ( after - before );
      displayString( 160, 0, 0x00ffffff, 0x00000000, false,
		     renderer->surface, renderer->width, renderer->height, "fps: %4.2f", fps );
      stats.print("FRAME STATISTICS");

      // reset timer
      if ( localFrameCount % 64 ) {
	localFrameCount = 0;
	before = 0;
	after = 0;
      }

      // unlock surface
      surface.unlock();
      // copy to console
      surface.copy(console);
      // update console
      console.update();
    }

    // get current time from timer
    const double time = timer.time();

    printf("=======\n"
	   "LEAVING\n"
	   "=======\n");fflush(stdout);

    printf("time: %5.5f\n"
	   "#frames: %ld\n"
	   "fps: %5.5f\n",
	   time, frameCount, (float)frameCount / time );

    char tmpBuf[255];
    sprintf( tmpBuf,"%5.2f fps", (float)frameCount / time );
    MessageBox( NULL, tmpBuf ,"fps report",MB_OK );

    console.close();
  }
  catch (Error &error) {
    // report error
    error.report();
  }

  shutdownRaytracing();
  
  return 0;
}
