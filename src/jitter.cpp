#include <stdlib.h>

#include "jitter.h"

//--float jitterMap[MAX_JITTER_MAP];
//--void jitterInit() {
//--  for ( int i = 0; i < MAX_JITTER_MAP; i++ )
//--    jitterMap[i] = (float)((rand()%256))/256.0f; // 256 differents values  0.0 <= x < 1.0
//--}

/*
 * Efficient Generation of Sampling Jitter Using Look-up Tables
 * by Joseph M. Cychosz
 * from "Graphics Gems", Academic Press, 1990
 */

/*  	Jitter.c - Sampling jitter generation routines.  	    
/*
/*	Description:
/*	    Jitter.c contains the routines for generation of sampling  
/*	    jitter using look-up tables.
/*
/*	Contents:
/*	    Jitter1	Generate random jitter function 1.
/*	    Jitter2	Generate random jitter function 2.			
/*	    JitterInit	Initialize look-up tables.
/* */

float xranf() {
  return (float)((rand()%16384))/16384.0f;
}

#define	NRAN	1024	/* Random number table length	*/

static	float	URANX[NRAN],		/* Random number tables		*/
  URANY[NRAN];
static	int		IRAND[NRAN];	/* Shuffle table	*/
static	int		MASK = NRAN-1;	/* Mask for jitter mod function */

/*  	Jitter1 - Generate random jitter. 	*/
void	jitter1( int x, int y, int s, float &xj, float &yj ) {
  //		int	x, y;		/* Pixel location	 */
  //		int	s;		/* Sample number for the pixel */
  //		float	*xj, *yj;	/* Jitter (x,y)	*/
  xj = URANX[ (x + (y<<2) + IRAND[(x+s)&MASK]) & MASK ];
  yj = URANY[ (y + (x<<2) + IRAND[(y+s)&MASK]) & MASK ];
}

/*  	Jitter2 - Generate random jitter.  	*/
void	jitter2( int x, int y, int s, float &xj, float &yj ) {
  //		int	x, y;		/* Pixel location  */
  //		int	s;		/* Sample number for the pixel */
  //		float	*xj, *yj;	/* Jitter (x,y)	*/
  xj = URANX[ ((x | (y<<2)) + IRAND[(x+s)&MASK]) & MASK ];
  yj = URANY[ ((y | (x<<2)) + IRAND[(y+s)&MASK]) & MASK ];
}


/*  	JitterInit - Initialize look-up tables.  	*/
void	jitterInit	() {
  int	i;
  
  for ( i = 0 ; i < NRAN ; i++ ) URANX[i] = xranf();
  for ( i = 0 ; i < NRAN ; i++ ) URANY[i] = xranf();
  for ( i = 0 ; i < NRAN ; i++ ) IRAND[i] = (int) (NRAN *
						   xranf());
}
