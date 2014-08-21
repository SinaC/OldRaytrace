#include <stdlib.h>
#include <stdio.h>

#include "bboxtree.h"
#include "../object.h"
#include "../primitives/primitive.h"
#include "../globals.h"
#include "../types/basics.h"
#include "../error.h"

#define BUNCHING_FACTOR (4)

// Initial number of entries in a priority queue.
#define INITIAL_PRIORITY_QUEUE_SIZE (256)

// Local structures

// Structure used to optimize intersection between ray and bounding boxes
typedef bool TVectorB[3];
struct RayInfo {
  //----- Datas
  TVector3 slabNum;
  TVector3 slabDen;
  TVectorB nonZero;
  TVectorB positive;

  //----- Methods
  void create( const TRay &ray );
};

// Priority queue
struct PriorityQueueBox {
  //----- Datas
  struct QElem {
    float depth;
    const AABBTree *node;
  };
  u32 size;
  u32 maxSize;
  QElem *queue;

  //----- Methods
  static PriorityQueueBox *create( const u32 initSize );
  void insertElem( const float Depth, const AABBTree *Node );
  void deleteElem( float &Depth, const AABBTree *&Node );
  void checkAndEnqueue( const AABBTree *Node, const AABB &box, const RayInfo &rayinfo );
  void destroy();
};

// Current axis to sort along.
static int axis = 0;
// Number of finite elements.
static long maxFiniteCount = 0;
// Priority queue used for frame level bounding box hierarchy.
static PriorityQueueBox *frameQueue;
// Local copy of finite object lists
static AABBTree **Finite;


void initializeAABBCode() {
  frameQueue = PriorityQueueBox::create( INITIAL_PRIORITY_QUEUE_SIZE );
}

void deinitializeAABBCode() {
  if ( frameQueue != NULL )
    frameQueue->destroy();
  frameQueue = NULL;
}

PriorityQueueBox *PriorityQueueBox::create( const u32 initSize ) {
  PriorityQueueBox *New;

  New = new PriorityQueueBox;
  New->queue = new QElem [initSize];
  New->size = 0;
  New->maxSize = initSize;

  return New;
}

void PriorityQueueBox::destroy() {
  if ( queue != NULL )
    delete [] queue;
  size = maxSize = 0;
}

void AABBTree::destroy() {
  if ( entries > 0 ) {
    for ( int i = 0; i < entries; i++ )
      node[i]->destroy();
    delete [] node;
    entries = 0;
    node = NULL;
  }
}

//  Create a bounding box hierarchy from a given list of finite and
//  infinite elements. Each element consists of
//
//    - an infinite flag
//    - a bounding box enclosing the element
//    - a pointer to the structure representing the element (e.g an object)

AABBTree *AABBTree::build( const int _nFinite, AABBTree **_Finite,
			   const int _nInfinite, AABBTree **_Infinite ) {
  int i;
  AABBTree *Root;
  int nFinite = _nFinite;
  int nInfinite = _nInfinite;

  // This is a resonable guess at the number of finites needed.
  // This array will be reallocated as needed if it isn't.
  maxFiniteCount = 2 * nFinite;
  Finite = new AABBTree * [maxFiniteCount];
  for ( i = 0; i < nFinite; i++ )
    Finite[i] = _Finite[i];

  // Now do a sort on the objects, with the end result being
  // a tree of objects sorted along the x, y, and z axes.
  if ( nFinite > 0 ) {
    int low = 0;
    int high = nFinite;

    while ( sortAndSplit( Root, nFinite, low, high ) == 0 ) {
      low = high;
      high = nFinite;
    }

    // Move infinite objects in the first leaf of Root.
    if ( nInfinite > 0 ) {
      // realloc root->node but copy root->node to root->node+1
      AABBTree **tempNode = new AABBTree * [Root->entries+1];
      for ( i = 0; i < Root->entries; i++ )
	tempNode[i+1] = Root->node[i];
      delete Root->node;
      Root->node = tempNode;
      Root->entries++;

      // add infinite object in first leaf and recompute bounding box
      AABBTree *cd = AABBTree::createAABBNode( nInfinite );
      for (i = 0; i < nInfinite; i++)
        cd->node[i] = _Infinite[i];
      cd->box.calcAABB( _Infinite, 0, nInfinite );
      Root->node[0] = cd;
      Root->box.calcAABB( Root->node, 0, Root->entries );

      // Root and first node are infinite.
      Root->infinite = true;
      Root->node[0]->infinite = true;
    }
  }
  else {
    // There are no finite objects and no Root was created.
    // Create it now and put all infinite objects into it.
    if ( nInfinite > 0 ) {
      Root = AABBTree::createAABBNode( nInfinite );
      for (i = 0; i < nInfinite; i++ )
        Root->node[i] = _Infinite[i];
      Root->box.calcAABB( _Infinite, 0, nInfinite );
      Root->infinite = true;
    }
  }

  delete [] Finite;

  return Root;
}

// Intersect a ray with a bounding box tree.
bool AABBTree::intersectionRay( const TRay &ray, TIntersection &intersection ) const {
  bool found = false;
  float minDist = INFINITE;
  TObject *currentObject = NULL;
  frameQueue->size = 0;

  // Create the direction vectors for this ray.
  RayInfo rayInfo;
  rayInfo.create( ray );

  // Check top node.
  frameQueue->checkAndEnqueue( this, box, rayInfo );

  // Check elements in the priority queue.
  while ( frameQueue->size > 0 ) {
    float depth;
    AABBTree const *intersectionNode;
    frameQueue->deleteElem( depth, intersectionNode );

    // If current intersection is larger than the best intersection found
    // so far our task is finished, because all other bounding boxes in
    // the priority queue are further away.
    if ( depth > minDist )
      break;

    // Check current node.
    if ( intersectionNode->entries ) {
      // This is a node containing leaves to be checked.
      for ( int i = 0; i < intersectionNode->entries; i++)
        frameQueue->checkAndEnqueue( intersectionNode->node[i], intersectionNode->node[i]->box, rayInfo );
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

// return first ray intersection with a bounding box tree.
bool AABBTree::intersectsRay( const TRay &ray, const float maxDist2 ) const {
  bool found = false;
  float minDist = INFINITE;
  TObject *currentObject = NULL;
  frameQueue->size = 0;

  // Create the direction vectors for this ray.
  RayInfo rayInfo;
  rayInfo.create( ray );

  // Check top node.
  frameQueue->checkAndEnqueue( this, box, rayInfo );

  // Check elements in the priority queue.
  while ( frameQueue->size > 0 ) {
    float depth;
    AABBTree const *intersectionNode;
    frameQueue->deleteElem( depth, intersectionNode );

    // If current intersection is larger than the maximal authorized distance
    // so far our task is finished, because all other bounding boxes in
    // the priority queue are further away.
    if ( depth > maxDist2 )
      break;

    // Check current node.
    if ( intersectionNode->entries ) {
      // This is a node containing leaves to be checked.
      for ( int i = 0; i < intersectionNode->entries; i++)
        frameQueue->checkAndEnqueue( intersectionNode->node[i], intersectionNode->node[i]->box, rayInfo );
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
void PriorityQueueBox::insertElem( const float Depth, const AABBTree *Node ) {
  // Reallocate priority queue if it's too small.
  if ( size+1 >= maxSize) {
    if ( size+1 >= 10000 )
      g_error("PriorityQueueBox::insertElem: Priority queue overflow.\n");

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
void PriorityQueueBox::deleteElem( float &Depth, const AABBTree *&Node ) {
  if ( size == 0 )
    g_error("PriorityQueueBox::deleteElem: priority queue is empty.\n");

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

// If a given ray intersect the object's bounding box then add it
// to the priority queue.
void PriorityQueueBox::checkAndEnqueue( const AABBTree *Node, const AABB &box, const RayInfo &rayinfo ) {
  float tmin, tmax;
  float dmin, dmax;

  if ( Node->infinite )
    // Set intersection depth to -Max_Distance.
    dmin = -INFINITE;
  else {
    if ( rayinfo.nonZero[__X] ) {
      if ( rayinfo.positive[__X] ) {
        dmin = ( box.lowerLeft[__X] - rayinfo.slabNum[__X]) *  rayinfo.slabDen[__X];
        dmax = dmin + ( box.lengths[__X]  * rayinfo.slabDen[__X] );
        if ( dmax < EPS ) return;
      }
      else {
        dmax = ( box.lowerLeft[__X] - rayinfo.slabNum[__X]) * rayinfo.slabDen[__X];
        if ( dmax < EPS ) return;
        dmin = dmax + (box.lengths[__X]  * rayinfo.slabDen[__X]);
      }
      if (dmin > dmax) return;
    }
    else {
      if ( ( rayinfo.slabNum[__X] < box.lowerLeft[__X] ) ||
           ( rayinfo.slabNum[__X] > box.lengths[__X] + box.lowerLeft[__X] ) )
        return;
      dmin = -INFINITE;
      dmax = INFINITE;
    }

    if ( rayinfo.nonZero[__Y] ) {
      if ( rayinfo.positive[__Y] ) {
        tmin = ( box.lowerLeft[__Y] - rayinfo.slabNum[__Y]) * rayinfo.slabDen[__Y];
        tmax = tmin + ( box.lengths[__Y]  * rayinfo.slabDen[__Y] );
      }
      else {
        tmax = ( box.lowerLeft[__Y] - rayinfo.slabNum[__Y]) * rayinfo.slabDen[__Y];
        tmin = tmax + ( box.lengths[__Y]  * rayinfo.slabDen[__Y] );
      }

      // Unwrap the logic - do the dmin and dmax checks only when tmin and
      // tmax actually affect anything, also try to escape ASAP. Better
      // yet, fold the logic below into the two branches above so as to
      //  compute only what is needed.

      // You might even try tmax < EPSILON first (instead of second) for an
      // early quick out.

      if ( tmax < dmax ) {
        if ( tmax < EPS ) return;
        // check bbox only if tmax changes dmax 
        if ( tmin > dmin ) {
          if ( tmin > tmax ) return;
          // do this last in case it's not needed!
          dmin = tmin;
        }
        else
          if ( dmin > tmax ) return;
        // do this last in case it's not needed!
        dmax = tmax;
      }
      else {
        if ( tmin > dmin ) {
          if ( tmin > dmax ) return;
	  // do this last in case it's not needed!
	  dmin = tmin;
        }
        // else nothing needs to happen, since dmin and dmax did not change!
      }
    }
    else {
      if ( ( rayinfo.slabNum[__Y] < box.lowerLeft[__Y] ) ||
	   ( rayinfo.slabNum[__Y] > box.lengths[__Y] + box.lowerLeft[__Y] ) )
        return;
    }
    
    if ( rayinfo.nonZero[__Z] ) {
      if ( rayinfo.positive[__Z] ) {
        tmin = ( box.lowerLeft[__Z] - rayinfo.slabNum[__Z]) * rayinfo.slabDen[__Z];
	tmax = tmin + ( box.lengths[__Z]  * rayinfo.slabDen[__Z] );
      }
      else {
        tmax = ( box.lowerLeft[__Z] - rayinfo.slabNum[__Z]) * rayinfo.slabDen[__Z];
        tmin = tmax + ( box.lengths[__Z]  * rayinfo.slabDen[__Z] );
      }

      if ( tmax < dmax ) {
        if ( tmax < EPS ) return;
        // check bbox only if tmax changes dmax
        if ( tmin > dmin ) {
          if ( tmin > tmax ) return;
          // do this last in case it's not needed!
          dmin = tmin;
        }
        else
          if ( dmin > tmax ) return;
      }
      else {
        if ( tmin > dmin ) {
          if ( tmin > dmax ) return;
          // do this last in case it's not needed!
          dmin = tmin;
        }
        // else nothing needs to happen, since dmin and dmax did not change!
      }
    }
    else
      if ( ( rayinfo.slabNum[__Z] < box.lowerLeft[__Z] ) ||
           ( rayinfo.slabNum[__Z] > box.lengths[__Z] + box.lowerLeft[__Z] ) )
        return;
  }
  
  insertElem( dmin, Node );
}

//   Calculate the rayinfo structure for a given ray. It's need for
//   intersection the ray with bounding boxes.

void RayInfo::create( const TRay &ray ) {
  float t;

  // Create the direction vectors for this ray
  slabNum = ray.origin;
  if ( (nonZero[__X] = ( (t = ray.direction[__X]) != 0.0)) != 0 ) {
    slabDen[__X] = 1.0f / t;
    positive[__X] = ray.direction[__X] > 0.0f;
  }

  if ( (nonZero[__Y] = ( (t = ray.direction[__Y]) != 0.0)) != 0 ) {
    slabDen[__Y] = 1.0f / t;
    positive[__Y] = ray.direction[__Y] > 0.0f;
  }

  if ( (nonZero[__Z] = ( (t = ray.direction[__Z]) != 0.0)) != 0) {
    slabDen[__Z] = 1.0f / t;
    positive[__Z] = ray.direction[__Z] > 0.0;
  }
}

AABBTree *AABBTree::createAABBNode( const int size ) {
  AABBTree *New = new AABBTree;
  New->infinite = false;
  New->entries = size;
  New->box.lowerLeft = TPoint3::zeroVector;
  New->box.lengths = TPoint3::zeroVector;

  if ( size != 0 )
    New->node = new AABBTree * [size];
  else
    New->node = NULL;

  return New;
}


static int compBoxes( const void *in_a, const void *in_b ) {
  AABB *a, *b;
  float am, bm;

  a = &((*(AABBTree **)in_a)->box);
  b = &((*(AABBTree **)in_b)->box);

  am = 2.0f * a->lowerLeft[axis] + a->lengths[axis];
  bm = 2.0f * b->lowerLeft[axis] + b->lengths[axis];

  if ( am < bm )
    return -1;
  else
    if ( am == bm )
      return 0;
    else
      return 1;
}

// Find the axis along which the elements will be sorted.
int AABBTree::findAxis( const int first, const int last ) {
  TPoint3 mins = TPoint3( INFINITE, INFINITE, INFINITE );
  TPoint3 maxs = TPoint3( -INFINITE, -INFINITE, -INFINITE );

  for ( int i = first; i < last; i++ ) {
    AABB *box = &( Finite[i]->box );

    if ( box->lowerLeft[__X] < mins[__X])
      mins[__X] = box->lowerLeft[__X];
    if ( box->lowerLeft[__X] + box->lengths[__X] > maxs[__X])
      maxs[__X] = box->lowerLeft[__X];
    if ( box->lowerLeft[__Y] < mins[__Y])
      mins[__Y] = box->lowerLeft[__Y];
    if ( box->lowerLeft[__Y] + box->lengths[__Y] > maxs[__Y])
      maxs[__Y] = box->lowerLeft[__Y];
    if ( box->lowerLeft[__Z] < mins[__Z])
      mins[__Z] = box->lowerLeft[__Z];
    if ( box->lowerLeft[__Z] + box->lengths[__Z] > maxs[__Z])
      maxs[__Z] = box->lowerLeft[__Z];
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
  if ( e > d )
    which = __Z;

  return which;
}

//  Calculate the bounding box containing Finite[first] through Finite[last-1].
void AABB::calcAABB( AABBTree **_Finite, const int first, const int last ) {
  TPoint3 bmin = TPoint3( INFINITE, INFINITE, INFINITE );
  TPoint3 bmax = TPoint3( -INFINITE, -INFINITE, -INFINITE );

  for ( int i = first; i < last; i++ ) {
    AABB *box = &( _Finite[i]->box );

    float tmin = box->lowerLeft[__X];
    float tmax = tmin + box->lengths[__X];

    if ( tmin < bmin[__X] ) { bmin[__X] = tmin; }
    if ( tmax > bmax[__X] ) { bmax[__X] = tmax; }

    tmin = box->lowerLeft[__Y];
    tmax = tmin + box->lengths[__Y];

    if ( tmin < bmin[__Y] ) { bmin[__Y] = tmin; }
    if ( tmax > bmax[__Y] ) { bmax[__Y] = tmax; }

    tmin = box->lowerLeft[__Z];
    tmax = tmin + box->lengths[__Z];

    if ( tmin < bmin[__Z] ) { bmin[__Z] = tmin; }
    if ( tmax > bmax[__Z] ) { bmax[__Z] = tmax; }
  }

  createFromMinMax( bmin, bmax );
}

// Construct lowerLeft and Length from bottom lower left and top up right corners
void AABB::createFromMinMax( const TPoint3 &min, const TPoint3 &max ) {
  lowerLeft = min;
  lengths = max - min;
}

// Generates a table of bound box surface areas.
void AABBTree::buildAreaTable( const int a, const int b,
			       float *areas ) {
  int imin, dir;

  if ( a < b ) {
    imin = a;  dir =  1;
  }
  else {
    imin = b;  dir = -1;
  }

  TPoint3 bmin = TPoint3( INFINITE, INFINITE, INFINITE );
  TPoint3 bmax = TPoint3( -INFINITE, -INFINITE, -INFINITE );

  for ( int i = a; i != (b + dir); i += dir ) {
    AABB *box = &( Finite[i]->box );

    float tmin = box->lowerLeft[__X];
    float tmax = tmin + box->lengths[__X];

    if (tmin < bmin[__X]) { bmin[__X] = tmin; }
    if (tmax > bmax[__X]) { bmax[__X] = tmax; }

    tmin = box->lowerLeft[__Y];
    tmax = tmin + box->lengths[__Y];

    if (tmin < bmin[__Y]) { bmin[__Y] = tmin; }
    if (tmax > bmax[__Y]) { bmax[__Y] = tmax; }

    tmin = box->lowerLeft[__Z];
    tmax = tmin + box->lengths[__Z];

    if (tmin < bmin[__Z]) { bmin[__Z] = tmin; }
    if (tmax > bmax[__Z]) { bmax[__Z] = tmax; }

    TVector3 len = bmax - bmin;
    areas[i - imin] = len[__X] * ( len[__Y] + len[__Z] ) + len[__Y] * len[__Z];
  }
}

int AABBTree::sortAndSplit( AABBTree *&Root, int &nFinite,
			    const int first, const int last ) {
  int i;
  
  axis = findAxis( first, last );

  int size = last - first;
  if ( size <= 0 )
    return 1;

  // Actually, we could do this faster in several ways. We could use a
  // logn algorithm to find the median along the given axis, and then a
  // linear algorithm to partition along the axis. Oh well.
  qsort((void *)(&Finite[first]), (size_t)size, sizeof(AABBTree*), compBoxes );

  // area_left[] and area_right[] hold the surface areas of the bounding
  // boxes to the left and right of any given point. E.g. area_left[i] holds
  // the surface area of the bounding box containing Finite 0 through i and
  // area_right[i] holds the surface area of the box containing Finite
  // i through size-1.
  float *areaLeft = new float [size];
  float *areaRight = new float [size];

  // Precalculate the areas for speed.
  buildAreaTable( first, last - 1, areaLeft );
  buildAreaTable( last - 1, first, areaRight );

  float bestIndex = areaRight[0] * (float)(size - 3);
  int bestLoc = -1;

  // Find the most effective point to split. The best location will be
  // the one that minimizes the function N1*A1 + N2*A2 where N1 and N2
  // are the number of objects in the two groups and A1 and A2 are the
  // surface areas of the bounding boxes of the two groups.
  for ( i = 0; i < size - 1; i++ ) {
    float newIndex = (float)(i + 1) * areaLeft[i] + (float)(size - 1 - i) * areaRight[i + 1];

    if ( newIndex < bestIndex ) {
      bestIndex = newIndex;
      bestLoc = i + first;
    }
  }

  delete [] areaLeft;
  delete [] areaRight;

  // Stop splitting if the BUNCHING_FACTOR is reached or
  // if splitting stops being effective.
  if ( size <= BUNCHING_FACTOR
       || bestLoc < 0 ) {
    AABBTree *cd = AABBTree::createAABBNode(size);
    for ( i = 0; i < size; i++ )
      cd->node[i] = Finite[first+i];
    cd->box.calcAABB( Finite, first, last );
    Root = cd;

    if ( nFinite > maxFiniteCount ) {
      // Prim array overrun, increase array by 50%.
      maxFiniteCount = ( 3 * maxFiniteCount ) / 2;

      // realloc
      AABBTree **tempFinite = new AABBTree * [maxFiniteCount];
      for ( i = 0; i < nFinite; i++ )
	tempFinite[i] = Finite[i];
      delete [] Finite;
      Finite = tempFinite;
    }

    Finite[nFinite] = cd;
    nFinite++;

    return 1;
  }
  else {
    sortAndSplit( Root, nFinite, first, bestLoc + 1 );
    sortAndSplit( Root, nFinite, bestLoc + 1, last );
    return 0;
  }
}

void AABBTree::print( const char *msg ) const {
  static int depth = 0;
  depth++;

  printf("%s [%d]\n", msg, depth );
  box.lowerLeft.print("lower left corner: ");
  box.lengths.print("lengths: ");
  if ( entries <= 0 )
    printf("Node is Leaf   [%x]\n", (int)node );
  else {
    printf("Node has Children [%d]\n", entries );
    for ( int i = 0; i < entries; i++ )
      node[i]->print("NODE");
  }
  depth--;
}
