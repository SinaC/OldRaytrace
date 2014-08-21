#ifndef __RAY_CONTEXT_H__
#define __RAY_CONTEXT_H__

#include "value.h"
#include "parser.h"

////////////////////////// Context //////////////////////////////////

#define MAX_SCOPE_VARS (256)
#define MAX_SCOPES (32)

class Scope {
  int numvar;

  Value* val[MAX_SCOPE_VARS];
  const char* name[MAX_SCOPE_VARS];
 public:

  Value* newVar(const char* n);
	
  Value* getVar(const char* n);

  void clean();

  void dump() const;

  Scope() {
    numvar = 0;
  };
};


struct Context {

  int numscope;

  Scope scopes[MAX_SCOPES];
  
 public:

  Context(){
    numscope = 0;
  }

  Value* operator [](const char* n) {
    for (int i = numscope-1; i>=0; i--) {
      Value* res = scopes[i].getVar(n);
      if (res != NULL) {
	return res;
      }
    }
    return NULL;
  }

  Value newVar(const char* name);
  void addVar( const char *varName, RAYExpression *varValue );
  void addVar( const char *varName, const Value &val );

  void dump() const;

  void pushscope();
  void popscope();

  void clean();
};


// global context
//--extern Context rayContext;

#endif
