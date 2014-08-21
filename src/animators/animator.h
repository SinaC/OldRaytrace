#ifndef __ANIMATORS_H__
#define __ANIMATORS_H__

#include <vector>
using namespace std;

#include "../handle.h"

struct TMover;

struct TAnimator: public THandle {
  //----- Datas
  private:
  float time;
  public:
  //**  protected:
  float speed;
  //  TMover **movers;
  //  int numMovers;
  vector<TMover*> movers;
  
  //----- Constructor
  TAnimator();
  //----- Destructor
  ~TAnimator();

  //----- Setup
  // setup called once in world initialization
  virtual void setup();

  //----- Methods
  void animate();
  void reset();

  //----- Input
  // parse one field
  virtual bool parse( Context &ctx, RAYField *field );
  // qualified expression
  virtual bool qualifiedExpression( Value &res, const char *fieldName ) const;

  //----- Output
  virtual void print( const char *msg ) const;

  //***** Private
  private:
  //--tag
  static const enum EAnimatorTags {
    TAG_animator_movers,
    TAG_animator_speed
  };
  static const TTagList animatorTags [];
};

#endif
