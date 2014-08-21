#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__


void initPredefinedFunctions();


struct Value;
struct RAYCompound;
struct Context;

//--- These functions are declared in a scene
//---  in RAYCallExpr, we first check in context if we find the wanted function
//---  if it doesn't exist, we search in functionTable

// maximum number of parameters of a function
#define MAX_FCT_PARM (16)

typedef Value FUN( const int numParams, Value *params );

struct FCT_DATA {
  FUN *predFct;

  // function's body
  RAYCompound*	code;
  // number of parameters
  int		nbparm;
  // symbolic name of parameters
  const char *	parmname[MAX_FCT_PARM];
  const char *	name;

  FCT_DATA *	next;                     /* Next in hash*/

  Value execute( Context &ctx, Value *params ) const;
  Value eval( Context &ctx ) const;
  
  void print( const char *msg ) const;
};


#endif
