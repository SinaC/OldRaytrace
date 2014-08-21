#include <stdlib.h>
#include <stdio.h>

#include "functions.h"
#include "value.h"
#include "../math/math.h"
#include "hashing.h"
#include "../text/basics.h"
#include "context.h"
#include "parser_skel.h"

//--#define __DEBUG__

//----------------------------------------
//-- Functions accessible from RAYCallExpr

static Value sinFct( const int numParams, Value *params ) {
  return sinf(params[0].asFloat());
}

static Value cosFct( const int numParams, Value *params ) {
  return cosf(params[0].asFloat());
}

static Value sqrtFct( const int numParams, Value *params ) {
  return sqrtf(params[0].asFloat());
}

static Value fracFct( const int numParams, Value *params ) {
  float f = params[0].asFloat();
  return (f - (int)f);
}

static Value minFct( const int numParams, Value *params ) {
  float a = params[0].asFloat();
  float b = params[1].asFloat();
  return (MIN(a,b));
}

static Value maxFct( const int numParams, Value *params ) {
  float a = params[0].asFloat();
  float b = params[1].asFloat();
  return (MAX(a,b));
}

static Value absFct( const int numParams, Value *params ) {
  return fabsf(params[0].asFloat());
}

static Value printFct( const int numParams, Value *params ) {
  printf("PRINT: %s\n", params[0].asStr() );
  return Value((long)0);
}

static Value modFct( const int numParams, Value *params ) {
  return fmodf( params[0].asFloat(), params[1].asFloat() );
}

//-----------------
//-- Function table

void addPredefined( FUN *f, const char *name, const int numParams ) {
  FCT_DATA *nf = new FCT_DATA;
  nf->predFct = f;
  nf->name = str_dup( name );
  nf->nbparm = numParams;
  nf->code = NULL;
  hashPut( nf, functionHash );
}

void initPredefinedFunctions() {
  addPredefined( sinFct, "sin", 1 );
  addPredefined( cosFct, "cos", 1 );
  addPredefined( sqrtFct, "sqrt", 1 );
  addPredefined( fracFct, "frac", 1 );
  addPredefined( minFct, "min", 2 );
  addPredefined( maxFct, "max", 2 );
  addPredefined( absFct, "abs", 1 );
  addPredefined( printFct, "print", 1 );
  addPredefined( modFct, "mod", 2 );
}

Value FCT_DATA::execute( Context &ctx, Value *params ) const {
#ifdef __DEBUG__
  printf("FCT_DATA::execute  new context + pushscope\n"); fflush(stdout);
#endif
  // create a new context
  Context newCtx;

  // push context
  newCtx.pushscope();

#ifdef __DEBUG__
  printf("FCT_DATA::execute  add 'result' and params\n"); fflush(stdout);
#endif
  
  // add variable result, function result must be assigned to "result"
  Value val = newCtx.newVar("result");
  
  // add function parameters
  for (int i = 0; i< nbparm; i++) {
    Value val = newCtx.newVar( parmname[i] );
    val.setValue(params[i]);
  }

#ifdef __DEBUG__
  printf("FCT_DATA::execute  execution\n"); fflush(stdout);
#endif
  
  // execute function
  code->execute(newCtx);

#ifdef __DEBUG__
  printf("FCT_DATA::execute  gather result\n"); fflush(stdout);
#endif

  // gather function result
  Value res;
  res = *newCtx["result"];
  
#ifdef __DEBUG__
  printf("FCT_DATA::execute  popscope\n"); fflush(stdout);
#endif

  // pop context
  newCtx.popscope();

  return res;
}

Value FCT_DATA::eval( Context &ctx ) const {
#ifdef __DEBUG__
  printf("FCT_DATA::eval  pushscope\n"); fflush(stdout);
#endif
  ctx.pushscope();

  // eval params
  Value *parms = new Value [nbparm];
  for (int i = 0; i < nbparm; i++) {
    Value *slot = ctx[parmname[i]];
    if ( slot == NULL )
      p_error("Function [%s] parameter([%d]) [%s] not found in context!",
	      name, i, parmname[i] );
    parms[i] = Value(slot);
  }
#ifdef __DEBUG__
  printf("FCT_DATA::eval  param evaluated\n"); fflush(stdout);
#endif

  // eval function
  Value val = FCT_DATA::execute( ctx, parms );

#ifdef __DEBUG__
  printf("FCT_DATA::eval  function executed\n"); fflush(stdout);
#endif

  ctx.popscope();

#ifdef __DEBUG__
  printf("FCT_DATA::eval  popscope\n"); fflush(stdout);
#endif

  delete [] parms;

  // return evaluated value
  return val;
}

void FCT_DATA::print( const char *msg ) const {
  printf("%s\n", msg );
  char buf[1024];
  buf[0] = '\0';
  for ( int i = 0; i < nbparm; i++ ) {
    strcat( buf, parmname[i] );
    if ( i < nbparm-1 )
      strcat( buf, ", ");
  }
  printf("name [%s]  numParams [%d]  params [%s]  predFct [%s]\n",
	 name, nbparm, buf, predFct==NULL?"false":"true");
  if ( code )
    code->dump(1);
}
