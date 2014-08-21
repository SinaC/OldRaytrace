#include <malloc.h>
#include <vector>
using namespace std;

#include "memory.h"
#include "handle.h"

#define __DEBUG__

bool shutdownOn = false;
vector<THandle*> memoryAllocated;

void *THandle::operator new( size_t size ) {
  THandle *h = (THandle*) malloc( size );
#ifdef __DEBUG__
  printf("THandle::operator new (%d)  -> (%x)\n", size, (int)h ); fflush(stdout);
#endif
  memoryAllocated.push_back( h );
  return h;
}

void *THandle::operator new[] ( size_t size ) {
#ifdef __DEBUG__
  printf("THandle::operator new[] (%d)\n", size ); fflush(stdout);
#endif
  return THandle::operator new( size );
}

void THandle::operator delete ( void *obj ) {
#ifdef __DEBUG__
  printf("THandle::operator delete (%x) shutdownOn: %d\n", (int)obj, shutdownOn ); fflush(stdout);
#endif

  // if shutdownOn is false
  //   NOP: memory allocated with THandle::operator new will be freed in shutdown_raytracing()
  // else
  //  delete obj
  if ( shutdownOn ) {
#ifdef __DEBUG__
    printf("THandle::operator delete   free (%x)\n", (int)obj ); fflush(stdout);
#endif
    if ( ((THandle*)obj)->name != NULL )
      delete [] ((THandle*)obj)->name;
    free( obj );
  }
}

void THandle::operator delete[] ( void *obj ) {
#ifdef __DEBUG__
  printf("THandle::operator delete[] shutdownOn: %d\n", shutdownOn ); fflush(stdout);
#endif

  THandle::operator delete( obj );
}


void shutdownRaytracing() {
  printf("\n"
	 "===========\n"
	 "SHUTDOWN...\n"
	 "===========\n"); fflush(stdout);
  shutdownOn = true;
  for ( vector<THandle*>::const_iterator iter = memoryAllocated.begin();
	iter != memoryAllocated.end();
	iter++ )
    delete (*iter);
  memoryAllocated.clear();
}
