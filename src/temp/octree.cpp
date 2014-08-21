#include <stdlib.h>

#include "text/basics.h"
#include "octree.h"
#include "ray.h"
#include "intersection.h"

#define __DEBUG__

TOctree::TOctree() {
  for ( int i = 0; i < 8; i++ )
    children[i] = NULL;
  //--primitives = NULL; primitiveCount = 0; // REPLACE WITH LIST or DYNAMIC VECTOR
  parent = NULL;
}

TOctree::~TOctree() {
  release();
}

bool TOctree::traversal( const TRay &ray ) const {
  const TOctree *rayPosition = findNode( ray.origin );

  // if no parent: octree root -> no traversal
  if ( rayPosition->parent == NULL )
    return false;

  // which box's side the ray pass thru ?
  TIntersection inter;
  if ( !rayPosition->node.intersection( ray, inter ) )
    return false;

  ray.origin.print("ray origin");
  ray.direction.print("ray direction");
  rayPosition->print("NODE BOX + PRIMITIVE");
  printf("SIDE: %d\n", inter.additionalInformation.side );
  printf("DISTANCE: %10.5f\n", inter.distance );

  // check primitives in node rayPosition
  //-- NO INTERSECTION TEST FOR THE MOMENT

  // get parent box
  const TOctree *parentBox = rayPosition->parent;
  parentBox->node.print("parent");

  // get correct child
  //  position is 0 --> 7
  //   near: 4    far: 0
  //   bottom: 2  top: 0
  //   right: 1   left: 0
  //  inter.additionalInformation.side is 0 --> 6
  //   SIDE_X_MIN,     // x < 0
  //   SIDE_X_MAX,     // x > 0
  //   SIDE_Y_MIN,     // y < 0
  //   SIDE_Y_MAX,     // y > 0
  //   SIDE_Z_MIN,     // z < 0
  //   SIDE_Z_MAX      // z > 0
  int child;
  switch( inter.additionalInformation.side ) {
  case BoundingAABB::SIDE_X_MIN: // going left
    if ( parentBox->children[0] != NULL
	 && parentBox->children[0] != rayPosition )
      // take this children for recursive call
      ;
    else
      // do this test again with rayPosition = parent  and  parent = parent->parent
      ;
  case BoundingAABB::SIDE_X_MAX: // going right
  case BoundingAABB::SIDE_Y_MIN: // going down
  case BoundingAABB::SIDE_Y_MAX: // going up
  case BoundingAABB::SIDE_Z_MIN: // going front
  case BoundingAABB::SIDE_Z_MAX: // going back
  default: return false;
  }

  // construct a new ray starting from intersection with box
  TRay newRay;
  newRay.origin = ray.origin + ( inter.distance + EPS ) * ray.direction;
  newRay.direction = ray.direction;
  newRay.reverseDirection = ray.reverseDirection;
  
  // recursive call with the new ray

  return true;
}

const TOctree *TOctree::findNode( const TPoint3 &p ) const {
  TPoint3 center = ( node.max + node.min ) * 0.5f;
  // near: 4    far: 0
  // bottom: 2  top: 0
  // right: 1   left: 0
  int i = 0;
  if ( p[__X] > center[__X] ) i += 1; // right  x greater
  if ( p[__Y] < center[__Y] ) i += 2; // bottom  y smaller
  if ( p[__Z] > center[__Z] ) i += 4; // near  z greater

#ifdef __DEBUG__
  printf("findNode\n"
	 "--------\n");
  node.print("BOX");
  center.print("CENTER");
  p.print("POINT");
  printf("child: %d\n", i );
#endif

  if ( children[i] == NULL )
    return (const TOctree*)this;
  else
    return children[i]->findNode( p );
}

// REPLACE WITH LIST or DYNAMIC VECTOR
//--bool TOctree::build( const TPrimitive **primitivePool, int _primitiveCount,
bool TOctree::build( const std::vector<const TPrimitive*> primitivePool,
		     const BoundingAABB &nodeSize, const int threshold ) {
  // set the node's bounding box
  node = nodeSize;

  // get number of primitives in pool
  int poolSize = (int)primitivePool.size();

  // if there are fewer primitives than the threshold value,
  // this node become a leaf which stores the primitives
  //--  if ( _primitiveCount <= threshold ) {
  if ( poolSize <= threshold ) {
    //--    primitiveCount = _primitiveCount;
    //--    primitives = new const TPrimitive *[primitiveCount];
    //--    for ( int i = 0; i < primitiveCount; i++ )
    //--      primitives[i] = primitivePool[i];
    for( int i = 0; i < poolSize; i++)
      primitives.push_back( primitivePool[i] );
    return true;
  }

  // create the child cubes
  BoundingAABB childNodes[8];
  createChildCubes( childNodes );
  //--  const TPrimitive **childPrimitives[8];
  //--  bool *childPrimitivesIntersects[8];
  //--  int childPrimitivesCount[8];
  std::vector<const TPrimitive*> childPrimitives[8];

  //--  {
  //--  for ( int i = 0; i < 8; i++ ) {
  //--    childPrimitivesCount[i] = 0;
  //--    childPrimitivesIntersects[i] = new bool [_primitiveCount];
  //--    for ( int j = 0; j < _primitiveCount; j++ )
  //--      childPrimitivesIntersects[i][j] = false;
  //--  }
  //--  }

  // for each child, generate a list of primitives that
  // intersect the child cube;
  {
    //--  for ( int i = 0; i < _primitiveCount; i++ )
  for ( int i = 0; i < poolSize; i++ )
    for ( int j = 0; j < 8; j++ )
      if ( primitivePool[i]->bounding->intersects( childNodes[j] ) ) {
	//--	childPrimitivesIntersects[j][i] = true;
	//--	childPrimitivesCount[j]++;
	childPrimitives[j].push_back(primitivePool[i]);
#ifdef __DEBUG__
	printf("primitive [%d] intersects child [%d]\n", i, j );
	primitivePool[i]->bounding->print("primitive box");
	childNodes[j].print("child box");
#endif
      }
  }
  //--  {
  //--  for ( int i = 0; i < 8; i++ )
  //--    if ( childPrimitivesCount[i] > 0 ) {
  //--      childPrimitives[i] = new const TPrimitive * [childPrimitivesCount[i]];
  //--      int k = 0;
  //--      for ( int j = 0; j < _primitiveCount; j++ )
  //--	if ( childPrimitivesIntersects[j] )
  //--	  childPrimitives[i][k++] = primitivePool[j];
  //--      delete [] childPrimitivesIntersects[i];
  //--    }
  //--    else
  //--      childPrimitives[i] = NULL;
  //--  }
  //--#ifdef __DEBUG__
  //--  {
  //--    printf("DUMPING PRIMITIVES IN CHILDREN\n");
  //--    for ( int i = 0; i < 8; i++ )
  //--      if ( childPrimitivesCount[i] > 0 ) {
  //--	printf("child [%d]\n", i );
  //--	childNodes[i].print("child");
  //--	for ( int j = 0; j < childPrimitivesCount[i]; j++ )
  //--	  childPrimitives[i][j]->print("primitive");
  //--      }
  //--  }
  //--#endif

  // for any children with intersecting triangles, create and
  // recursively build the node;
  for ( int i = 0; i < 8; i++ )
    //--    if ( childPrimitivesCount[i] > 0 ) {
    if ( childPrimitives[i].size() > 0 ) {
      children[i] = new TOctree;
      if ( children[i] == NULL )
  	return false;
      if ( !children[i]->build( childPrimitives[i],//-- childPrimitivesCount[i],
				childNodes[i], threshold ) )
  	return false;
      children[i]->parent = this;
      children[i]->position = i;
    }
  return true;
}

// bool TOctree::buildCollisionList(std::vector<Triangle*> &tlist, const Sphere &ps)
//     {
//         // if the bounding sphere does not intersect this node,
//         // we can disregard it and all of its children;

//         if(!ps.doesBoundingBoxIntersect(m_node))
//         {
//             return true;
//         }

//         // if this node is a leaf node with triangles, add the children
//         // to the potential collision list;  otherwise recursively check;

//         if(m_triangles.size())
//         {
//             int tsize = (int)m_triangles.size();

//             for(int i=0; i<tsize; i++)
//             {
//                 tlist.push_back(m_triangles[i].src());
//             }
//         }
//         else
//         {
//             for(int i=0; i<8; i++)
//             {
//                 if(m_children[i] != NULL)
//                 {
//                     m_children[i]->buildCollisionList(tlist, ps);
//                 }
//             }
//         }

//         return true;
//     }

void TOctree::render() {
//**--**  for(int i=0; i<8; i++)
//**--**      if(m_children[i] != NULL)
//**--**	m_children[i]->render();
//**--**  
//**--**  // render only leaf nodes with triangles;
//**--**  if(m_triangles.size())
//**--**    renderBoundingBox(m_node, COLOR_RGB(255, 255, 255));
}

void TOctree::release() {
  for ( int i = 0; i < 8; i++ )
    if ( children[i] != NULL ) {
      children[i]->release();
      delete children[i];
      children[i] = NULL;
    }
  // REPLACE WITH LIST or DYNAMIC VECTOR
  //--  if ( primitives != NULL )
  //--    delete [] primitives;
  //--  primitives = NULL;
  primitives.clear();
}

void TOctree::createChildCubes( BoundingAABB *nodes ) {
  TPoint3 center = ( node.max + node.min ) * 0.5f;
  
  // top left, near;
  nodes[0].min = TPoint3( node.min[__X],   center[__Y], node.min[__Z] );
  nodes[0].max = TPoint3(   center[__X], node.max[__Y],   center[__Z] );
  // top right, near;
  nodes[1].min = TPoint3(   center[__X],   center[__Y], node.min[__Z] );
  nodes[1].max = TPoint3( node.max[__X], node.max[__Y],   center[__Z] );
  // bottom left, near;
  nodes[2].min = TPoint3( node.min[__X], node.min[__Y], node.min[__Z] );
  nodes[2].max = TPoint3(   center[__X],   center[__Y],   center[__Z] );
  // bottom right, near;
  nodes[3].min = TPoint3(   center[__X], node.min[__Y], node.min[__Z] );
  nodes[3].max = TPoint3( node.max[__X],   center[__Y],   center[__Z] );
  // top left, far;
  nodes[4].min = TPoint3( node.min[__X],   center[__Y],   center[__Z] );
  nodes[4].max = TPoint3(   center[__X], node.max[__Y], node.max[__Z] );
  // top right, far;
  nodes[5].min = TPoint3(   center[__X],   center[__Y],   center[__Z] );
  nodes[5].max = TPoint3( node.max[__X], node.max[__Y], node.max[__Z] );
  // bottom left, far;
  nodes[6].min = TPoint3( node.min[__X], node.min[__Y],   center[__Z] );
  nodes[6].max = TPoint3(   center[__X],   center[__Y], node.max[__Z] );
  // bottom right, far;
  nodes[7].min = TPoint3(   center[__X], node.min[__Y],   center[__Z] );
  nodes[7].max = TPoint3( node.max[__X],   center[__Y], node.max[__Z] );

#ifdef __DEBUG__
  printf("createChildCubes\n"
	 "----------------\n");
  for ( int i = 0; i < 8; i++ )
    nodes[i].print("node");
#endif
}

void TOctree::print( const char *msg ) const {
  static int indent = -1;
  indent++;

  printf("%s%s [%d] [addr:%x]\n", shift(indent), msg, indent, (int)this );
  node.min.print( "box min" );
  node.max.print( "box min" );

  printf("parent: [addr:%x]\n", (int)parent );
  
  {
    //--  for ( int i = 0; i < primitiveCount; i++ )
    //--    primitives[i]->print("primitives in node");
    for ( int i = 0; i < primitives.size(); i++ )
      primitives[i]->print("primitive in node");
  }
  
  {
  int childrenCount = 0;
  for ( int i = 0; i < 8; i++ )
    if ( children[i] != NULL ) {
      char buf[16];
      sprintf( buf, "child[%d]", i );
      children[i]->print( buf );
      childrenCount++;
    }
  if ( childrenCount == 0 )
    printf("%sNO CHILDREN\n", shift(indent) );
  }

  indent--;
}
