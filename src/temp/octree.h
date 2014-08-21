#ifndef __OCTREE_H__
#define __OCTREE_H__

#include <vector>

#include "primitives/primitive.h"
#include "boundings/bounding.h"
#include "boundings/aabb.h"

struct TRay;

struct TOctree {
  //***** Private *****
 private:
  //----- Methods
  void createChildCubes( BoundingAABB *nodes );
  
  //----- Datas
  BoundingAABB  node;
  TOctree      *children[8];
  TOctree      *parent;
  int           position; // 0 --> 7, which child are we
  std::vector<const TPrimitive*> primitives;

  //***** Public *****
  public:
  //----- Datas

  //----- Constructor
  TOctree();
  //----- Destructor
  ~TOctree();

  //----- Methods
  const TOctree *findNode( const TPoint3 &p ) const;
  bool traversal( const TRay &ray ) const;

  bool build( const std::vector<const TPrimitive*> primitivePool,
	      const BoundingAABB &nodeSize, const int threshold );

  //bool buildCollisionList( std::vector<TPrimitive*> &tlist, const Sphere &ps)
  void render();
  void release();

  //----- Output
  void print( const char *msg ) const;
};

#endif
