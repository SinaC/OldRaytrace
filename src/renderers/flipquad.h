#ifndef __RENDERER_FLIPQUAD_H__
#define __RENDERER_FLIPQUAD_H__

#include "renderer.h"

struct RendererFlipquad: public TRenderer {
  //---- Methods
  bool render( const TWorld &world );
  //**** Private
  private:
  // pattern 1
  //  X-     up
  // |  X    right
  // X  |    left
  //  -X     down
  inline void computePattern1UP( const int x, const int y,
				 TVector3 &vector ) {
    vector = topLeft - ( upStep * ( y ) ) + ( rightStep * ( x + 0.33333f ) );
  }
  inline void computePattern1LEFT( const int x, const int y,
				   TVector3 &vector ) {
    vector = topLeft - ( upStep * ( y + 0.66666f ) ) + ( rightStep * ( x ) );
  }
  inline void computePattern1RIGHT( const int x, const int y,
				    TVector3 &vector ) {
    vector = topLeft - ( upStep * ( y + 0.33333f ) ) + ( rightStep * ( x + 1.0f ) );
  }
  inline void computePattern1DOWN( const int x, const int y,
				   TVector3 &vector ) {
    vector = topLeft - ( upStep * ( y + 1.0f ) ) + ( rightStep * ( x + 0.66666f ) );
  }
  // pattern 2
  //  -X   up
  // X  |  left
  // |  X  right
  //  X-   down
  inline void computePattern2UP( const int x, const int y,
				 TVector3 &vector ) {
    vector = topLeft - ( upStep * ( y ) ) + ( rightStep * ( x + 0.66666f ) );
  }
  inline void computePattern2LEFT( const int x, const int y,
				   TVector3 &vector ) {
    vector = topLeft - ( upStep * ( y + 0.33333f ) ) + ( rightStep * ( x ) );
  }
  inline void computePattern2RIGHT( const int x, const int y,
				    TVector3 &vector ) {
    vector = topLeft - ( upStep * ( y + 0.66666f ) ) + ( rightStep * ( x + 1.0f ) );
  }
  inline void computePattern2DOWN( const int x, const int y,
				   TVector3 &vector ) {
    vector = topLeft - ( upStep * ( y + 1.0f ) ) + ( rightStep * ( x + 0.33333f ) );
  }

  // compute direction vector in function of x and y (choose correct pattern)
  inline void computeDirectionUP( const int x, const int y,
				  TVector3 &vector ) {
    if ( ( x + y ) % 2 == 0 )
      computePattern1UP( x, y, vector );
    else
      computePattern2UP( x, y, vector );
  }
  inline void computeDirectionLEFT( const int x, const int y,
				    TVector3 &vector ) {
    if ( ( x + y ) % 2 == 0 )
      computePattern1LEFT( x, y, vector );
    else
      computePattern2LEFT( x, y, vector );
  }
  inline void computeDirectionRIGHT( const int x, const int y,
				     TVector3 &vector ) {
    if ( ( x + y ) % 2 == 0 )
      computePattern1RIGHT( x, y, vector );
    else
      computePattern2RIGHT( x, y, vector );
  }
  inline void computeDirectionDOWN( const int x, const int y,
				    TVector3 &vector ) {
    if ( ( x + y ) % 2 == 0 )
      computePattern1DOWN( x, y, vector );
    else
      computePattern2DOWN( x, y, vector );
  }
};

#endif
