//--#include <fstream>
#include <stdarg.h>
#include <stdio.h>

#include "font.h"
#include "../globals.h"
#include "../error.h"

typedef unsigned char CARDEF[256][8];     // Structure du jeu de caracteres
CARDEF fptr;

static bool fontRead = false;

bool readFont( const char *filename ) {
	char localFName[1024];
	sprintf( localFName, "%sfonts/%s", globalSettings::path, filename );

	  FILE *f = fopen( localFName, "rb" );
	if ( f == NULL ) {
	    g_error("Couldn't open font file: %s\n", localFName );
	    return false;
	}
	for ( int i = 0; i < 255; i++ )
		for ( int j = 0; j < 8; j++ ) {
			int l;
			fscanf( f, "%d\n\r", &l );
			fptr[i][j] = l;
		}
	fclose(f);

  //ifstream f( localFName, ios::in );
  //if ( !f.is_open() ) {
  //  g_error("Couldn't open font file: %s\n", localFName );
  //  return false;
  //}
  //for ( int i = 0; i < 255; i++ )
  //  for ( int j = 0; j < 8; j++ ) {
  //    int l;
  //    f >> l;
  //    fptr[i][j] = l;
  //  }
  fontRead = true;
  return true;
}

static void printChar( const char caractere, const u32 x, const u32 y,
		       const u32 cc, const u32 cf, const bool transparent,
		       u32 *vs, const u32 width, const u32 height ) {
  unsigned char     i, k,                              // Compteur d'iterations
    masque;        // Masque binaire pour dessiner le caractere
  
  u32 *buffer = vs + x + y * width;
  //- Dessine le caractere pixel par pixel ---------------------------*/
  if( transparent )                           // Caractere transparent ?
    for ( i = 0; i < 8; i++ ) { // Oui, ne dess. que pixels du 1er plan
      masque = fptr[caractere][i];         // Motif bin. pour ligne
      for ( k = 0; k < 8; k++, masque <<= 1 )  // Parcourt les colonnes
	if ( masque & 128 )                         // Pixel a dessiner ?
	  *(buffer+i*width+k) = cc;
    }
  else                                     // Non dessine chaque pixel
    for ( i = 0; i < 8; i++ ) {                   // Parcourt les lignes
      masque = fptr[caractere][i];         // Motif bin. pour ligne
      for ( k = 0; k < 8; k++, masque <<= 1 )  // Parcourt les colonnes
	*(buffer+i*width+k) = (masque & 128)?cc:cf;
    }
}


void displayString( const u32 x, const u32 y,
		    u32 cc, const u32 cf, const bool transparent,
		    u32 *vs, const u32 width, const u32 height,
		    const char *s, ... ) {
  if ( !fontRead )
    return;

  va_list argptr;
  static char buf[1024];

  va_start( argptr, s );
  vsprintf( buf, s, argptr );
  va_end( argptr );

  const char *p = buf;
  int xs = x;
  while ( *p ) {
    printChar( *p, xs, y, cc, cf, transparent, vs, width, height );
    xs+=8;
    p++;
  }
}
