#ifndef __BSPHERETREE_H__
#define __BSPHERETREE_H__

#include "../math/vector3.h"
#include "../intersection.h"
#include "../ray.h"
#include "../boundings/sphere.h"

struct BSphereTree;

void initializeBSphereCode();
void deinitializeBSphereCode();

// center and radius^2
struct BSphere: public BoundingSphere {
  //----- Datas
  //--  TPoint3 center; // center of bounding sphere
  //--  float radius2; // radius^2 of bounding sphere
  
  //----- Methods
  void recomputeBound( BSphereTree **Finite, const int first, const int last );
};

// Bounding Sphere Tree, only one element/leaf
struct BSphereTree {
  //----- Datas
  int entries; // number of component, leaf if 0
  BSphere sphere;
  BSphereTree **node; // if node: children, if leaf: element (cast node into correct pointer)
  
  //----- Methods
  static BSphereTree *build( const int nElem, BSphereTree **elements );
  void destroy();
  bool intersectionRay( const TRay &ray, TIntersection &inter ) const;
  bool intersectsRay( const TRay &ray, const float maxDist2 ) const;
  
  //----- Output
  void print( const char *msg ) const;

  //***** Private
  private:
  static BSphereTree *createBSphereNode( const int size );
  static int findAxis( const int first, const int last );
  static void buildAreaTable( const int a, const int b,
			      float *areas );
  static int sortAndSplit( BSphereTree *&Root, int &nElem,
			   const int first, const int last );
};

#endif
