#ifndef __KDTREE_H__
#define __KDTREE_H__

#include <vector>
using namespace std;

#include "../types/basics.h"
#include "../math/vector3.h"

struct TObject;
struct TRay;
struct TCameraSimple;

struct BinNode {
  //----- Datas
  TPoint3      min, max;      /* extent of node */
  vector<TObject *> members;
  struct BinNode *child[2];  /* pointers to children nodes, if any */
  
  //----- Methods
  /* distance to the plane which subdivides the children */
  float  (*distanceToDivisionPlane)( const TPoint3 &, const TRay & );
  
  /* children near/far ordering relative to a input point */
  void    (*getChildren)( BinNode *, const TPoint3 &,
			  BinNode *&, BinNode *& );
  
  void subdivide( int depth, int MaxDepth, int MaxListLength, int axis );
  bool geomInNode( const TObject *obj ) const;
  bool pointInNode( const TPoint3 &pt ) const;
  bool leaf() const;
  
  //----- Output
  void print( const char *msg ) const;
  void render( u32 *screen, const u32 width, const u32 height, const TCameraSimple &camera );
};

struct KDTree {
  //***** Private *****
private:
  //----- Datas
  struct TStackElem {
    BinNode   *node;
    float     min, max;
  };
  
  struct TStack {
    int       stackPtr;
    TStackElem stack[50];
    
    void init();
    void push( BinNode *node, float min, float max );
    void pop( BinNode *&node, float &min, float &max );
  };

  TPoint3     min, max;       /* extent of the entire bin tree */
  vector<TObject *> members;
  int         MaxDepth;      /* max allowed depth of the tree */
  int         MaxListLength; /* max object allowed in a leaf node */
  BinNode    *root;          /* root of the entire bin tree */

  //***** Public *****
  public:
  //----- Datas

  //----- Constructor
  KDTree();
  //----- Destructor
  ~KDTree();

  //----- Methods
  void build( const vector<TObject *> objectPool );
  bool findClosestIntersectionCamera( const TRay &ray, TObject *&obj, float &distance ) const;
  bool findClosestIntersection( const TRay &ray, TObject *&obj, float &distance ) const;
  bool findFirstIntersection( const TRay &ray, const float maxDist2, TObject *&obj ) const;

  //----- Output
  void print( const char *) const;
  void render( u32 *screen, const u32 width, const u32 height, const TCameraSimple &camera );
};

#endif
