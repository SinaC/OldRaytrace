#ifndef __BBOXTREE_H__
#define __BBOXTREE_H__

#include "../math/vector3.h"
#include "../intersection.h"
#include "../ray.h"

struct AABBTree;

void initializeAABBCode();
void deinitializeAABBCode();

// lower left corner and extents
struct AABB {
  //----- Datas
  TPoint3 lowerLeft;
  TVector3 lengths;

  //----- Methods
  void calcAABB( AABBTree **Finite, const int first, const int last );
  void createFromMinMax( const TPoint3 &min, const TPoint3 &max );
};

// Bounding Box Tree, only one element/leaf
struct AABBTree {
  //----- Datas
  bool infinite; // flag if node contains infinite elements
  int entries; // number of sub-node in node, leaf if 0
  AABB box; // bounding box of this node
  AABBTree **node; // if node: children, if leaf: element (cast node into correct pointer)

  //----- Methods
  static AABBTree *build( const int _nFinite, AABBTree **_Finite,
			  const int _nInfinite, AABBTree **_Infinite );
  void destroy();
  bool intersectionRay( const TRay &ray, TIntersection &inter ) const;
  bool intersectsRay( const TRay &ray, const float maxDist2 ) const;

  //----- Output
  void print( const char *msg ) const;

  //***** Private
  private:
  static AABBTree *createAABBNode( const int size );
  static int findAxis( const int first, const int last );
  static void buildAreaTable( const int a, const int b,
			      float *areas );
  static int sortAndSplit( AABBTree *&Root, int &nFinite,
			   const int first, const int last );
};

#endif
