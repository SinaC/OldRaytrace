#ifndef __RENDERER_QUINCUNX_H__
#define __RENDERER_QUINCUNX_H__

#include "renderer.h"
#include "../color.h"

struct RendererQuincunx: public TRenderer {
  //---- Methods
  bool render( const TWorld &world );
  bool renderAdaptativeColor( const TWorld &world, const u32 step );
  bool renderAdaptativePrimitive( const TWorld &world, const u32 step );
  
  //**** Private
  private:
  //-- based on color
  void fillSquareColorUL( TRay &ray, const TWorld &world,
			  const u32 x, const u32 y, const u32 step,
			  const TColor &colorUL, const TColor &colorDR ) const;
  void fillSquareColorUR( TRay &ray, const TWorld &world,
			  const u32 x, const u32 y, const u32 step,
			  const TColor &colorDL, const TColor &colorUR ) const;
  void fillSquareColorDL( TRay &ray, const TWorld &world,
			  const u32 x, const u32 y, const u32 step,
			  const TColor &colorDL, const TColor &colorUR ) const;
  void fillSquareColorDR( TRay &ray, const TWorld &world,
			  const u32 x, const u32 y, const u32 step,
			  const TColor &colorUL, const TColor &colorDR ) const;
  void fillSquareColor( TRay &ray, const TWorld &world,
			const u32 x, const u32 y, const u32 step,
			const TColor &colorUL, const TColor &colorUR,
			const TColor &colorC,
			const TColor &colorDL, const TColor &colorDR ) const;
  //-- based on primitive intersection
  bool compareIntersectionsUL_DR( const TWorld &world,
				  const u32 x, const u32 y, const u32 step,
				  const TRay &r1, const TRay &r2 ) const;
  bool compareIntersectionsDL_UR( const TWorld &world,
				  const u32 x, const u32 y, const u32 step,
				  const TRay &r1, const TRay &r2 ) const;
  void fillSquarePrimitiveUL( const TWorld &world,
			      const u32 x, const u32 y, const u32 step,
			      TRay &rayUL, TRay &rayDR ) const;
  void fillSquarePrimitiveUR( const TWorld &world,
			      const u32 x, const u32 y, const u32 step,
			      TRay &rayDL, TRay &rayUR ) const;
  void fillSquarePrimitiveDL( const TWorld &world,
			      const u32 x, const u32 y, const u32 step,
			      TRay &rayDL, TRay &rayUR ) const;
  void fillSquarePrimitiveDR( const TWorld &world,
			      const u32 x, const u32 y, const u32 step,
			      TRay &rayUL, TRay &rayDR ) const;
  void fillSquarePrimitive( const TWorld &world,
			    const u32 x, const u32 y, const u32 step,
			    TRay &rayUL, TRay &rayUR,
			    TRay &rayC,
			    TRay &rayDL, TRay &rayDR ) const;
};

#endif
