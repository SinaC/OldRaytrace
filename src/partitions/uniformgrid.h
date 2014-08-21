#ifndef __UNIFORM_GRID_H__
#define __UNIFORM_GRID_H__

#include <vector>
using namespace std;

#include "../math/vector3.h"
#include "../boundings/aabb.h"

struct TObject;

struct UniformGrid {
  //---- Datas
  //--  vector<TObject *> *m_Grid; // static vector of dynamic vector
  struct ObjectList {
    ObjectList() : m_Object( 0 ), m_Next( 0 ) {}
    ~ObjectList() { delete m_Next; }

    TObject* m_Object;
    ObjectList* m_Next;
  };
  ObjectList** m_Grid;
  
  BoundingAABB m_Extends;
  u32 m_GridSize;
  u32 m_GridShift; // log2(m_GridSize)
  // data for regular grid stepping
  TVector3 m_SR, m_CW;

  //---- Construtor
  UniformGrid();
  //---- Destructor
  ~UniformGrid();

  //---- Methods
  bool build( const vector<TObject*> &a_ObjList, const u32 a_GridSize, const BoundingAABB &a_Extends );
  bool findClosestIntersectionCamera( const TRay &ray, TObject *&obj, float &distance ) const;
  bool findClosestIntersection( const TRay &ray, TObject *&obj, float &distance ) const;
  bool findFirstIntersection( const TRay &ray, const float maxDist2, TObject *&obj ) const;

  //---- Output
  void print( const char *msg ) const;
};

#endif
