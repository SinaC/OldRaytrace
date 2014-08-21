#include <stdio.h>

#include "context.h"
#include "parser_skel.h"

//--#define __DEBUG__


// Global context
//--Context rayContext;

//--------------------------
//----------------- Scope

Value* Scope::newVar(const char* n) {
  if ( numvar >= MAX_SCOPE_VARS-1 ) {
    printf("Scope layer has too much variables: %d [max: %d]\n", numvar, MAX_SCOPE_VARS );
    printf("DUMPING SCOPE:\n");
    dump();
    p_error("DONE");
  }

  if ( getVar(n) != NULL )
    p_error("Variable %s has already been declared in this scope.", n );

  Value* res = new Value((long int)0);
  val[numvar] = res;
  name[numvar] = n;
  numvar++;
  return res;
}

Value* Scope::getVar(const char* n) {
  for (int i = 0; i<numvar; i++) {
    if (!str_cmp(n, name[i]))
      return val[i];
  }
  return NULL;
}

void Scope::clean() {
  for (int i =0; i<numvar; i++)
    delete val[i];
  numvar = 0;
}

void Scope::dump() const {
  for ( int i = 0; i < numvar; i++ )
    printf(" [%s]=[%s]\n", name[i], val[i]->asStr() );
}

//--------------------------
//------------------ Context

void Context::addVar( const char *varName, RAYExpression *varValue ) {
#ifdef __DEBUG__
  printf("CONTEXT::addVar:  %s\n", varName );
#endif

  Value v = varValue->eval(*this);
  Value var = newVar(varName);
  var.setValue(v);
}

void Context::addVar( const char *varName, const Value &val ) {
#ifdef __DEBUG__
  printf("CONTEXT::addVar:  %s\n", varName );
#endif

  Value var = newVar(varName);
  var.setValue(val);
}

Value Context::newVar(const char* name) {
  Value* ref = scopes[numscope-1].newVar(name);
  return Value(ref);
}

void Context::pushscope() {
  numscope++;

#ifdef __DEBUG__
  printf("Context::pushscope\n");
#endif

  if ( numscope >= MAX_SCOPES ) {
    printf("Scope depth overloaded: %d [max= %d]", numscope, MAX_SCOPES );
    dump();
    exit(-1);
  }
}

void Context::popscope() {    
#ifdef __DEBUG__
  printf("Context::popscope\n");
#endif

  if ( numscope <= 0 )
    p_error("Scope depth underloaded" );

  numscope--;
  scopes[numscope].clean();
}

void Context::dump() const {
  printf("CONTEXT DUMP:\n");
  for ( int i = 0; i < numscope; i++ ) {
    printf("SCOPE [%d]\n", i );
    scopes[i].dump();
  }
}

void Context::clean() {
  while ( numscope > 0 )
    popscope();
}
