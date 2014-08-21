#include <stdio.h>
#include <string.h>

#include "hashing.h"
#include "value.h"
#include "../text/basics.h"
#include "functions.h"
#include "parser_skel.h"

//#define VERBOSE

FCT_DATA * functionHash[HASH_FCT];

void hashInit() {
  for (int i=0;i<HASH_FCT;i++)
    functionHash[i] = NULL;
}

int hash_s(const char * s, int tblsize) {  
  unsigned int i;
  long res = 0;
  
  for (i = 0; i<strlen(s); i++)
    res += (s[i] | 0x20)*i; // Case insensitive
  return res % tblsize;   
}

FCT_DATA *hashGet( const char *s, FCT_DATA **c ) {
  int i = hash_s(s,HASH_FCT);

  FCT_DATA * res = c[i];
  if (res) {
    for (;res && str_cmp(res->name,s);res = res->next) /* empty */
      ;
    if (res)
      return res;
    else
      return NULL;
  } 
  else
    return NULL;
}

void hashPut( FCT_DATA *fct, FCT_DATA **c ) {
  int idx = hash_s(fct->name,HASH_FCT);
  for ( FCT_DATA *f = c[idx]; f != NULL; f = f->next )
    if ( !str_cmp( f->name, fct->name ) )
      p_error("Function [%s] already defined!");
  fct->next = c[idx];
  c[idx] = fct;
}

void hashDump( FCT_DATA **c ) {
  for ( int i = 0; i < HASH_FCT; i++ )
    for ( FCT_DATA *f = c[i]; f != NULL; f = f->next )
      printf("function [%s] [%d]\n", f->name, f->nbparm );
}
