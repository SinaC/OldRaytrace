#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "memory.h"

void g_error(const char* msg, ...) {
  va_list argptr;

  static char buf[1024]; 

  va_start(argptr, msg);
  vsprintf(buf, msg, argptr);
  printf("ERROR: %s", buf); 
  va_end(argptr);

  shutdownRaytracing();
  exit(-1);
}
