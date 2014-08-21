#ifndef __MOVER_H__
#define __MOVER_H__

#include <vector>
using namespace std;

#include "../types/basics.h"
#include "../math/vector3.h"
#include "../handle.h"

struct TMover: public THandle {
  //----- Datas
  u32 moverKind;
  static const enum ELightKind {
    MOVER_NOTHING,
    MOVER_ADDV,
    MOVER_CIRCLE,
    MOVER_LINEAR,
    MOVER_BOUNCE,
    MOVER_LISSAJOU,
    MOVER_BEZIER
  };
  bool vMove; // vector(true)  float(false)
  //--  int numDest;
  //--  void** dest;
  vector<void*> dest;

  //----- Constructor
  TMover();
  //----- Destructor
  ~TMover();

  //----- Setup
  // setup called once in world initialization
  virtual void setup() {};

  //----- Methods
  void move( const float time );
  virtual TVector3 vmove( const float time ) { return TVector3::zeroVector; };
  virtual float fmove( const float time ) { return 0; };

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // find substruct type and parse
  static THandle* parseSubclass( Context &ctx, RAYRecord *record );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  // dump mover information to standard output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  char *moverName() const;
  //--tag
  static const enum EMoverTags {
    TAG_mover_dest
  };
  static const TTagList moverTags [];
  static const TTagList moverSubClassTags [];
};

// pure virtual struct
struct MoverFloat : public TMover {
 public:
  //----- Constructor
  MoverFloat() { TMover::TMover(); vMove = false; }
};

// pure virtual struct
struct MoverVector : public TMover {
 public:
  //----- Constuctor
  MoverVector() { TMover::TMover(); vMove = true; }
};

#endif
