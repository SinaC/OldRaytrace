#include <stdlib.h>
#include <stdio.h>

#include "bspheretree.h"
#include "../object.h"
#include "../primitives/primitive.h"
#include "../globals.h"
#include "../types/basics.h"
#include "../error.h"

#define BRANCHING_FACTOR (4)

// Initial number of entries in a priority queue.
#define INITIAL_PRIORITY_QUEUE_SIZE (256)

// Local structures

// Priority queue
struct PriorityQueueSphere {
  //----- Datas
  struct QElem {
    float depth;
    const BSphereTree *node;
  };
  u32 size;
  u32 maxSize;
  QElem *queue;

  //----- Methods
  static PriorityQueueSphere *create( const u32 initSize );
  void insertElem( const float Depth, const BSphereTree *Node );
  void deleteElem( float &Depth, const BSphereTree *&Node );
  void checkAndEnqueue( const BSphereTree *Node, const BSphere &sphere, const TRay &ray );
  void destroy();
};

// Priority queue used for frame level bounding sphere hierarchy.
static PriorityQueueSphere *frameQueue;
// Number of elements
static int maxElements;
// Current axis to sort along.
static int axis;
// Local copy of object list
static BSphereTree **Elements;

void initializeBSphereCode() {
  frameQueue = PriorityQueueSphere::create( INITIAL_PRIORITY_QUEUE_SIZE );
}

void deinitializeBSphereCode() {
  if ( frameQueue != NULL )
    frameQueue->destroy();
  frameQueue = NULL;
}

PriorityQueueSphere *PriorityQueueSphere::create( const u32 initSize ) {
  PriorityQueueSphere *New;

  New = new PriorityQueueSphere;
  New->queue = new QElem [initSize];
  New->size = 0;
  New->maxSize = initSize;

  return New;
}

void PriorityQueueSphere::destroy() {
  if ( queue != NULL )
    delete [] queue;
  size = maxSize = 0;
}

// return closest ray intersection with a bounding sphere tree.
bool BSphereTree::intersectionRay( const TRay &ray, TIntersection &intersection ) const {
  bool found = false;
  float minDist = INFINITE;
  TObject *currentObject = NULL;
  frameQueue->size = 0;

  // Check top node.
  frameQueue->checkAndEnqueue( this, sphere, ray );

  // Check elements in the priority queue.
  while ( frameQueue->size > 0 ) {
    float depth;
    BSphereTree const *intersectionNode;
    frameQueue->deleteElem( depth, intersectionNode );

    // If current intersection is larger than the best intersection found
    // so far our task is finished, because all other bounding spheres in
    // the priority queue are further away.
    if ( depth > minDist )
      break;

    // Check current node.
    if ( intersectionNode->entries ) {
      // This is a node containing leaves to be checked.
      for ( int i = 0; i < intersectionNode->entries; i++)
        frameQueue->checkAndEnqueue( intersectionNode->node[i], intersectionNode->node[i]->sphere, ray );
    }
    else {
      // This is a leaf so test contained object.
      TObject *obj = (TObject*)(intersectionNode->node);
      if ( obj->primitive->intersectionRay( ray, intersection ) )
	if ( intersection.distance < minDist
	     && intersection.distance < MAX_DISTANCE ) { // new intersection is closer
	  minDist = intersection.distance;
	  currentObject = obj;
	  found = true;
	}
    }
  }

  if ( found ) {
    intersection.pointWorldSpace = ray.origin + ( minDist * ray.direction );
    intersection.distance = minDist;
    intersection.object = currentObject;
  }

  return found;
}

// return first ray intersection with a bounding sphere tree.
bool BSphereTree::intersectsRay( const TRay &ray, const float maxDist2 ) const {
  bool found = false;
  float minDist = INFINITE;
  TObject *currentObject = NULL;
  frameQueue->size = 0;

  // Check top node.
  frameQueue->checkAndEnqueue( this, sphere, ray );

  // Check elements in the priority queue.
  while ( frameQueue->size > 0 ) {
    float depth;
    BSphereTree const *intersectionNode;
    frameQueue->deleteElem( depth, intersectionNode );

    // If current intersection is larger than the maximal authorized distance
    // so far our task is finished, because all other bounding spheres in
    // the priority queue are further away.
    if ( depth > maxDist2 )
      break;

    // Check current node.
    if ( intersectionNode->entries ) {
      // This is a node containing leaves to be checked.
      for ( int i = 0; i < intersectionNode->entries; i++)
        frameQueue->checkAndEnqueue( intersectionNode->node[i], intersectionNode->node[i]->sphere, ray );
    }
    else {
      // This is a leaf so test contained object.
      TObject *obj = (TObject*)(intersectionNode->node);
      if ( obj->primitive->intersectsRay( ray, maxDist2 ) )
	return true;
    }
  }
  return false;
}

// Insert an element in the priority queue.
void PriorityQueueSphere::insertElem( const float Depth, const BSphereTree *Node ) {
  // Reallocate priority queue if it's too small.
  if ( size+1 >= maxSize) {
    if ( size+1 >= 10000 )
      g_error("PriorityQueueSphere::insertElem: Priority queue overflow.\n");

    maxSize *= 2;
    QElem *tempQueue = new QElem [maxSize];
    for ( u32 i = 0; i < size; i++ )
      tempQueue[i] = queue[i];
    delete [] queue;
    queue = tempQueue;
  }

  size++;
  QElem *List = queue;
  
  List[size].depth = Depth;
  List[size].node  = Node;
  
  u32 i = size;
  while (i > 1 && List[i].depth < List[i / 2].depth) {
    QElem tmp = List[i];
    List[i] = List[i / 2];
    List[i / 2] = tmp;
    i = i / 2;
  }
}

// Get an element from the priority queue.
// This element will always be the one closest to the ray origin.
void PriorityQueueSphere::deleteElem( float &Depth, const BSphereTree *&Node ) {
  if ( size == 0 )
    g_error("PriorityQueueSphere::deleteElem: priority queue is empty.\n");

  QElem *List = queue;

  Depth = List[1].depth;
  Node  = List[1].node;
  List[1] = List[size];

  size--;
  u32 localSize = size;

  u32 i = 1;
  while ( 2 * i <= localSize ) {
    u32 j;
    if ( 2 * i == localSize )
      j = 2 * i;
    else
      if ( List[2*i].depth < List[2*i+1].depth )
        j = 2 * i;
      else
        j = 2 * i + 1;

    if ( List[i].depth > List[j].depth ) {
      QElem tmp = List[i];
      List[i] = List[j];
      List[j] = tmp;
      i = j;
    }
    else
      break;
  }
}

// If a given ray intersect the object's bounding sphere then add it
// to the priority queue.
void PriorityQueueSphere::checkAndEnqueue( const BSphereTree *Node, const BSphere &sphere, const TRay &ray ) {
  TVector3 rayToCenter = sphere.m_Center - ray.origin;
  float B = ( ray.direction | rayToCenter );
  float C = rayToCenter.magnitudeSquared() - sphere.m_Radius2;
  float discriminant = B*B-C;
  
  if ( discriminant < 0.0f )
    return;
  
  float sqrtDiscr = sqrtf(discriminant);
  // store smallest positive solution
  float dmin = B-sqrtDiscr;
  if ( dmin <= 0.0f )
    dmin = B+sqrtDiscr;
  if ( dmin <= 0.0f )
    return;

  insertElem( dmin, Node );
}

BSphereTree *BSphereTree::createBSphereNode( const int size ) {
  BSphereTree *New = new BSphereTree;
  New->entries = size;
  if ( size != 0 )
    New->node = new BSphereTree * [size];
  else
    New->node = NULL;
  return New;
}

//   Recompute the bounding sphere of a given node in the bounding hierarchy,
//   i. e. find the bounding sphere that encloses the bounding spheres
//   of all nodes.
void BSphere::recomputeBound( BSphereTree **node, const int first, const int last ) {
  m_Radius2 = node[first]->sphere.m_Radius2;
  m_Center = node[first]->sphere.m_Center;
  
  for ( int i = first+1; i < last; i++ )
    merge( node[i]->sphere );
}

static int compElements( const void *in_a, const void *in_b ) {
  float am, bm;

  am = (*(BSphereTree **)in_a)->sphere.m_Center[axis];
  bm = (*(BSphereTree **)in_b)->sphere.m_Center[axis];

  if ( am < bm - EPS )
    return -1;
  else
    if ( am > bm + EPS )
      return 1;
    else
      return 0;
}

int BSphereTree::findAxis( const int first, const int last ) {
  TVector3 mins = TVector3( INFINITE, INFINITE, INFINITE );
  TVector3 maxs = TVector3( -INFINITE, -INFINITE, -INFINITE );

  for ( int i = first; i < last; i++ ) {
    BSphere *sphere = &( Elements[i]->sphere );

    mins[__X] = MIN( mins[__X], sphere->m_Center[__X] );
    maxs[__X] = MAX( maxs[__X], sphere->m_Center[__X] );

    mins[__Y] = MIN( mins[__Y], sphere->m_Center[__Y] );
    maxs[__Y] = MAX( maxs[__Y], sphere->m_Center[__Y] );

    mins[__Z] = MIN( mins[__Z], sphere->m_Center[__Z] );
    maxs[__Z] = MAX( maxs[__Z], sphere->m_Center[__Z] );
  }

  int which = __X;
  float e, d = -INFINITE;

  e = maxs[__X] - mins[__X];
  if ( e > d ) {
    d = e;  which = __X;
  }

  e = maxs[__Y] - mins[__Y];
  if ( e > d ) {
    d = e;  which = __Y;
  }

  e = maxs[__Z] - mins[__Z];
  if ( e > d ) {
    which = __Z;
  }

  return which;
}

//  Generates a table of bounding sphere surface areas.
void BSphereTree::buildAreaTable( const int a, const int b,
				  float *areas ) {
  int dir, imin;
  if (a < b) {
    imin = a;  dir = 1;
  }
  else {
    imin = b;  dir = -1;
  }

  BSphere sphereLocal = Elements[a]->sphere;
  for ( int i = a; i != (b + dir); i += dir ) {
    sphereLocal.merge( Elements[i]->sphere );
    areas[i-imin] = sphereLocal.m_Radius2;
  }
}

int BSphereTree::sortAndSplit( BSphereTree *&Root, int &nElem,
			       const int first, const int last ) {
  int i;

  axis = findAxis( first, last );

  int size = last - first;

  if ( size <= 0 )
    return 1;
  
  // Actually, we could do this faster in several ways. We could use a
  // logn algorithm to find the median along the given axis, and then a
  // linear algorithm to partition along the axis. Oh well.
  qsort((void *)(&Elements[first]), (size_t)size, sizeof(BSphereTree *), compElements);

  // area_left[] and area_right[] hold the surface areas of the bounding
  // spheres to the left and right of any given point. E.g. area_left[i] holds
  // the surface area of the bounding sphere containing Elements 0 through i and
  // area_right[i] holds the surface area of the sphere containing Elements
  // i through size-1.
  float *areaLeft  = new float [size];
  float *areaRight = new float [size];
  
  // Precalculate the areas for speed.
  buildAreaTable( first, last - 1, areaLeft );
  buildAreaTable( last - 1, first, areaRight );

  float bestIndex = areaRight[0] * (float)(size - 3);
  int bestLoc = - 1;

  // Find the most effective point to split. The best location will be
  // the one that minimizes the function N1*A1 + N2*A2 where N1 and N2
  // are the number of objects in the two groups and A1 and A2 are the
  // surface areas of the bounding spheres of the two groups.
  for ( i = 0; i < size - 1; i++ ) {
    float newIndex = (float)(i + 1) * areaLeft[i] + (float)(size - 1 - i) * areaRight[i + 1];

    if ( newIndex < bestIndex ) {
      bestIndex = newIndex;
      bestLoc = i + first;
    }
  }

  delete [] areaLeft;
  delete [] areaRight;

  // Stop splitting if the BRANCHING_FACTOR is reached or
  // if splitting stops being effective.
  if ( size <= BRANCHING_FACTOR
       || bestLoc < 0 ) {
    BSphereTree *cd = BSphereTree::createBSphereNode( size );
    for ( i = 0; i < size; i++ )
      cd->node[i] = Elements[first+i];
    cd->sphere.recomputeBound( cd->node, 0, size );
    Root = cd;

    if ( nElem > maxElements ) {
      // Prim array overrun, increase array by 50%.
      maxElements = ( 3 * maxElements ) / 2;

      // realloc
      BSphereTree **tempElements = new BSphereTree * [maxElements];
      for ( i = 0; i < nElem; i++ )
	tempElements[i] = Elements[i];
      delete [] Elements;
      Elements = tempElements;
    }

    Elements[nElem] = cd;
    nElem++;

    return 1;
  }
  else {
    sortAndSplit( Root, nElem, first, bestLoc + 1 );
    sortAndSplit( Root, nElem, bestLoc + 1, last );
    return 0;
  }
}

// Create the bounding sphere hierarchy for a given set of elements.
// One element consists of an element number (index into the array
// of elements; e.g. blob components, triangles) and a bounding
// sphere enclosing this element (center and squared radius).
BSphereTree * BSphereTree::build( const int _nElem, BSphereTree **_Elements ) {
  BSphereTree *Root;
  int nElem = _nElem;

  // This is a resonable guess at the number of elements needed.
  // This array will be reallocated as needed if it isn't.
  maxElements = 2 * nElem;
  Elements = new BSphereTree * [maxElements];
  for ( int i = 0; i < nElem; i++ )
    Elements[i] = _Elements[i];

  // Do a sort on the elements, with the end result being
  // a tree of objects sorted along the x, y, and z axes.
  if ( nElem > 0 ) {
    int low  = 0;
    int high = nElem;

    while ( sortAndSplit( Root, nElem, low, high ) == 0 ) {
      low  = high;
      high = nElem;
    }
  }

  delete [] Elements;

  return Root;
}

void BSphereTree::destroy() {
  if ( entries > 0 ) { // free children
    for ( int i = 0; i < entries; i++ )
      node[i]->destroy();
    delete [] node;
    entries = 0;
    node = NULL;
  }
}

void BSphereTree::print( const char *msg ) const {
  static int depth = 0;
  depth++;

  printf("%s [%d]\n", msg, depth );
  sphere.print("bounding sphere: ");
  if ( entries <= 0 )
    printf("Node is Leaf   [%p]\n", node );
  else {
    printf("Node has Children [%d]\n", entries );
    for ( int i = 0; i < entries; i++ )
      node[i]->print("NODE");
  }
  depth--;
}
