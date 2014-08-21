#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../world.h"
#include "struct_skel.h"
#include "parser.h"
#include "../text/basics.h"
#include "value.h"
#include "../math/vector3.h"
#include "../math/vector4.h"
#include "../color.h"
#include "parser_skel.h"
#include "context.h"
#include "../math/math.h"
#include "../handle.h"
#include "scene_parser.h"
#include "../math/math.h"
#include "functions.h"
#include "hashing.h"
#include "../math/vector2.h"
#include "../globals.h"

//--#define __DEBUG__
//--#define __DEBUG2__
//--#define __DEBUG3__


//****************************
//** Instructions

void RAYInstruction::execute( Context &ctx ) const {
  printf("do NOTHING; whoops\n"); fflush( stdout );
}

void RAYCompound::execute( Context &ctx ) const {
  ctx.pushscope();
  for (int i=0; i<instr_count; i++) {
#ifdef __DEBUG3__
    printf("RAYCompound::execute [%d]\n", i ); fflush(stdout);
    instr[i]->dump(1);    
#endif
    instr[i]->execute(ctx);
  }
  ctx.popscope();
}

void RAYVarDecl::execute( Context &ctx ) const {

 Value newvar = ctx.newVar(ident->image);
 if (defval != NULL)
   newvar.setValue(defval->eval(ctx));
}

#define MAX_LOOP (10000)
void RAYWhileInstr::execute( Context &ctx ) const {
  int loopCount = 0;
  while (cond->eval(ctx).asInt() != 0) {
    if ( loopCount > MAX_LOOP )
      p_error("Max loop iteration exceeded, %d", MAX_LOOP );
    
    body->execute(ctx);

    loopCount++;
  }
}

void RAYIfInstr::execute( Context &ctx ) const {
  if ( cond->eval(ctx).asBool() == true )
    thenpart->execute(ctx);
  else
    if ( elsepart )
      elsepart->execute(ctx);
}

void RAYExprInstr::execute( Context &ctx ) const {
  expr->eval(ctx);
}


//****************************
//** Expressions

//-------------------------------------------
//------------------ RAYMethodCallExpr::eval()
Value RAYMethodCallExpr::eval( Context &ctx ) const {

  Value leftval = left->eval(ctx).get();
  const char* rightval = ident->image;

  //Value parms[params_count];
  Value parms[MAX_FCT_PARM];

  for (int i = 0; i<params_count; i++)
    parms[i] = params[i]->eval(ctx);

  switch(leftval.typ) {
    // methods on vector: magnitudeSquared, magnitude, normalize
  case SVT_VECTOR2:
    if ( !strcmp( rightval, "magnitudeSquared" ) )
      return leftval.asVector2().magnitudeSquared();
    if ( !strcmp( rightval, "magnitude" ) )
      return leftval.asVector2().magnitude();
    if ( !strcmp( rightval, "normalize" ) ) {
      TVector2 v = leftval.asVector2();
      v.normalize();
      return v;
    }
    // methods on vector3: magnitudeSquared, magnitude, normalize
  case SVT_VECTOR3:
    if ( !strcmp( rightval, "magnitudeSquared" ) )
      return leftval.asVector3().magnitudeSquared();
    if ( !strcmp( rightval, "magnitude" ) )
      return leftval.asVector3().magnitude();
    if ( !strcmp( rightval, "normalize" ) ) {
      TVector3 v = leftval.asVector3();
      v.normalize();
      return v;
    }
    // methods on vector4: magnitudeSquared, magnitude, normalize
  case SVT_VECTOR4:
    if ( !strcmp( rightval, "magnitudeSquared" ) )
      return leftval.asVector4().magnitudeSquared();
    if ( !strcmp( rightval, "magnitude" ) )
      return leftval.asVector4().magnitude();
    if ( !strcmp( rightval, "normalize" ) ) {
      TVector4 v = leftval.asVector4();
      v.normalize();
      return v;
    }
    break;
    // methods on list: size(), includes(list), get(n), unique(), uniqueEquiv(), index(x), random(), flat()
  case SVT_LIST:
    if (!strcmp(rightval, "size")) {
      return Value(leftval.asList()->size);
    }
//--TODO    else if (!strcmp(rightval, "includes")) {
//--TODO      if (params_count != 1)
//--TODO	p_error("includes() expects one parameter");
//--TODO      return leftval.asList()->includes(parms[0].asList());
//--TODO    } 
    else if (!strcmp(rightval, "get")) {
      if (params_count != 1)
	p_error("get() expects one parameter");
      return leftval.asList()->get(parms[0].asInt());
    }
//--TODO    else if (!strcmp(rightval, "unique")) {
//--TODO      return leftval.asList()->unique();
//--TODO    }

//--TODO    else if (!strcmp(rightval, "uniqueEquiv")) {
//--TODO      return leftval.asList()->uniqueEquiv();
//--TODO    }

//--TODO    else if (!strcmp(rightval, "index")) {
//--TODO      if (params_count != 1)
//--TODO	p_error("index expects one parameter");
//--TODO      return leftval.asList()->index(parms[0]);
//--TODO    }
    //--TODO    else if (!strcmp(rightval, "flat")) {
    //--TODO      if (params_count != 0)
    //--TODO	p_error("random expects no parameter");
    //--TODO      return leftval.asList()->flat();
    //--TODO    }
    break;
    // methods on string: size(), words(), asInt(), pad(n), startsWith(string), tokenize(c)
  case SVT_STR:
    if (!strcmp(rightval, "size")) {
      return Value(strlen(leftval.asStr()));
    }
//--TODO    else if (!strcmp(rightval, "words")) {
//--TODO      return wordize(leftval.asStr());
//--TODO    } 

//--TODO    else if (!strcmp(rightval,"asInt")){
//--TODO      if (is_number(leftval.asStr()))
//--TODO	return Value(atoi(leftval.asStr()));
//--TODO      else
//--TODO	return Value((long)-1);
//--TODO    }
    else if (!strcmp(rightval,"startsWith")){
      if (params_count != 1 ) 
	p_error("startsWith expects one parameter");
      //log_stringf("%s %s", leftval.asStr(), parms[0].asStr() );
      return !str_prefix(parms[0].asStr(),leftval.asStr());
    }
    else if (!strcmp(rightval,"pad")){
      if (params_count != 1 ) 
	p_error("pad() expects one parameter");

      char format[64];
      char formatted[64];
      sprintf(format,"%%%lds", parms[0].asInt());
      sprintf(formatted,format,leftval.asStr());
      return Value(formatted);
    }
//--TODO    else if (!strcmp(rightval,"tokenize") ) {
//--TODO      if (params_count != 1)
//--TODO	p_error("tokenize expects one parameter");
//--TODO      return wordize(leftval.asStr(), parms[0].asStr());
//--TODO    }
    break;
    // methods on int: asStr(), pad(n)
  case SVT_INT:
    if (!strcmp(rightval, "asStr")) {
      char s[512];
      sprintf(s, "%ld", leftval.asInt());
      return Value(s);
    }
    else if (!strcmp(rightval,"pad")){
      if (params_count != 1 ) 
	p_error("pad() expects one parameter");

      char format[64];
      char formatted[64];
      sprintf(format,"%%%ldd", parms[0].asInt());
      sprintf(formatted,format,leftval.asInt());
      return Value(formatted);
    }
    break;
  }
  
  p_error("Method (%s) not found in this context (%s) [type: %d]",
	  rightval, leftval.asStr(), leftval.typ);
  return Value::nullValue;
}


//------------------------------------
//------------------ RAYFunction::Eval
Value RAYFunction::eval( Context &ctx ) const {
#ifdef __DEBUG__
  printf("RAYFunction::eval()  nbparm[%d]\n", params_count ); fflush(stdout);
#endif
  FCT_DATA *fct = new FCT_DATA;

  fct->nbparm = params_count;
  for ( int i = 0; i < params_count; i++ )
    fct->parmname[i] = str_dup( params[i]->image );
  fct->code = body;
  //fct->name is set in RAYDeclare

  return Value(fct);
}


//------------------------------------
//------------------ RAYCallExpr::Eval

//--- general function call
Value functionCall( Context &ctx, const char *name, const int numParams, Value *params ) {

  FCT_DATA *fct = hashGet( name, functionHash );
  if ( fct == NULL )
    p_error("Function [%s] not found!", name );
  if ( fct->nbparm != numParams )
    p_error("Wrong number of parameters in function call (%s)[%d/%d] ", 
	    fct->name, numParams, fct->nbparm );
  Value res;
  // predefined function
  if ( fct->predFct )
    res = fct->predFct( numParams, params );
  // user-defined function
  else
    res = fct->execute( ctx, params );

  return res;
}

//--- RAYCallExpr::eval
Value RAYCallExpr::eval( Context &ctx ) const {
#ifdef __DEBUG2__
  printf("RAYCallExpr::eval  [%s] [%d]\n", left->image, params_count ); fflush(stdout);
#endif

  // eval params
  Value *parms = new Value [params_count];
  for (int i = 0; i < params_count; i++)
    parms[i] = params[i]->eval(ctx);

  // search function identifier in context: a parameter a function can be a function
  //  so, we have to use the real param and not the formal param
  Value* slot = ctx[left->image];

  Value res;
  // left->image is not a parameter of a function
  if ( slot == NULL || slot->typ != SVT_FCT )
    // "execute" function
    res = functionCall( ctx, left->image, params_count, parms );
  else
    // "execute" function
    res = functionCall( ctx, slot->val.fct->name, params_count, parms );

  delete [] parms;

  return res;
} 

//-----------------------------------
//------------------ EXPRESSION::EVAL

Value RAYBooleanLiteral::eval( Context &ctx ) const {
  return Value((long)value);
}

Value RAYStringLiteral::eval( Context &ctx ) const {
  return Value(image);
}

Value RAYIntegerLiteral::eval( Context &ctx ) const {
  return Value((long)value);
}

Value RAYFloatLiteral::eval( Context &ctx ) const {
  return Value(value);
}

Value RAYVector2::eval( Context &ctx ) const {
  TVector2 *vector = new TVector2( X->eval(ctx).get().asFloat(),
				   Y->eval(ctx).get().asFloat() );
  return vector;
}

Value RAYVector3::eval( Context &ctx ) const {
  TVector3 *vector = new TVector3( X->eval(ctx).get().asFloat(),
				   Y->eval(ctx).get().asFloat(),
				   Z->eval(ctx).get().asFloat() );
  return vector;
}

Value RAYVector4::eval( Context &ctx ) const {
  TVector4 *vector = new TVector4( X->eval(ctx).get().asFloat(),
				   Y->eval(ctx).get().asFloat(),
				   Z->eval(ctx).get().asFloat(),
				   W->eval(ctx).get().asFloat() );
  return vector;
}

Value RAYColorRGB::eval( Context &ctx ) const {
  TColor *color = new TColor( R->eval(ctx).get().asFloat(),
			      G->eval(ctx).get().asFloat(),
			      B->eval(ctx).get().asFloat() );
  return color;
}

Value RAYList::eval( Context &ctx ) const {
  if (elems_count == 0)
    return ValueList::emptyList();
  ValueList* l = ValueList::newList(elems_count);
  for (int i = 0; i<elems_count; i++)
    l->elems[i] = elems[i]->eval(ctx).get();
  return l;
}

// FIXME
Value RAYSuper::eval( Context &ctx ) const {
  return Value::nullValue;
}

Value RAYIdentifier::eval( Context &ctx ) const {
  // search in context, if not found search in 'this' qualified expression
  Value* slot = ctx[image];
  if ( slot != NULL )
    return Value(slot);

  //--#ifdef __DEBUG__
  //--    printf("Identifier [%s] not found...dumping context.\n", image ); fflush(stdout);
  //--    ctx.dump();
  //--#endif
  //--    p_error("Identifier [%s] not found in context.", image );
  
  // if not found in context, is it a qualified expression of this ?
  Value *actor = ctx["this"];
  if ( actor == NULL )
    p_error("'this' not found!!!!");
  if ( actor->typ != SVT_HANDLE )
    p_error("'this' is not a THandle but a [%d]", actor->typ );
  Value res;
  if ( actor->asHandle()->qualifiedExpression( res, image ) )
    return res;
#ifdef __DEBUG__
  printf("Identifier [%s] not found...dumping context.\n", image ); fflush(stdout);
  ctx.dump();
#endif
  p_error("Identifier [%s] not found in context.", image );
  return Value::nullValue;
}

Value RAYUnaryExpr::eval( Context &ctx ) const {
  switch(op) {
  case '-':
    return - expr->eval( ctx );
  case '!':
    return ! expr->eval( ctx );
  default:
    p_error("parsed unknown unary operator.");
  }
  return Value::nullValue;
}

Value RAYBinaryExpr::eval( Context &ctx ) const {
  switch(op) {
  case '^':
    return powf( left->eval(ctx).asFloat(), right->eval(ctx).asFloat() ); // float ^ float
  case '|':
    return left->eval(ctx).asVector3() | right->eval(ctx).asVector3(); // dot product
  case '#':
    return crossProduct( left->eval(ctx).asVector3(), right->eval(ctx).asVector3() ); // cross product
  case '[':
    return left->eval(ctx).asList()->get(right->eval(ctx).asInt()); // list element extraction
  case '+':
    return left->eval(ctx) + right->eval(ctx);
  case '-':
    return left->eval(ctx) - right->eval(ctx);
  case '*':
    return left->eval(ctx) * right->eval(ctx);
  case '/':
    return left->eval(ctx) / right->eval(ctx);
  case '=': {
    Value res = left->eval(ctx);
    res.setValue(right->eval(ctx));
    return res;
  }
  case RAY_AND:
    return left->eval(ctx).asInt() && right->eval(ctx).asInt();
  case RAY_OR:
    return left->eval(ctx).asInt() || right->eval(ctx).asInt();
  case '<':
    return left->eval(ctx) < right->eval(ctx);
  case '>':
    return left->eval(ctx) > right->eval(ctx);
  case RAY_LE:
    return left->eval(ctx) <= right->eval(ctx);
  case RAY_GE:
    return left->eval(ctx) >= right->eval(ctx);
  case RAY_EQ:
    return left->eval(ctx) == right->eval(ctx);
  case RAY_NE:
    return left->eval(ctx) != right->eval(ctx);
  default:
    p_error("parsed unknown binary operator.");
  }
  return Value::nullValue;
}

//--CONFLICT WITH RAYRecordValue RAYTernaryExpr::eval() const {
//--CONFLICT WITH RAYRecord  if ( condition->eval().asBool() )
//--CONFLICT WITH RAYRecord    return valueTrue->eval();
//--CONFLICT WITH RAYRecord  else
//--CONFLICT WITH RAYRecord    return valueFalse->eval();
//--CONFLICT WITH RAYRecord}

Value RAYExpression::eval( Context &ctx ) const {
  p_error("Eval is not to be called on abstract expressions.");
  return Value::nullValue;
}

//---------------------------------------
//------------------ QUALIFIED EXPRESSION
// Vector2.X, .Y, .Z
bool findVector2Qualifier( Value &res, const TVector2 &vect, const char *fieldName ) {
  if ( !str_cmp( fieldName, "X" ) ) {
    res = &vect.v[0];
    return true;
  }
  else if ( !str_cmp( fieldName, "Y" ) ) {
    res = &vect.v[1];
    return true;
  }
  return false;
}
// Vector3.X, .Y, .Z
bool findVector3Qualifier( Value &res, const TVector3 &vect, const char *fieldName ) {
  if ( !str_cmp( fieldName, "X" ) ) {
    res = &vect.v[0];
    return true;
  }
  else if ( !str_cmp( fieldName, "Y" ) ) {
    res = &vect.v[1];
    return true;
  }
  else if ( !str_cmp( fieldName, "Z" ) ) {
    res = &vect.v[2];
    return true;
  }
  return false;
}
// Vector4.X, .Y, .Z
bool findVector4Qualifier( Value &res, const TVector4 &vect, const char *fieldName ) {
  if ( !str_cmp( fieldName, "X" ) ) {
    res = &vect.v[0];
    return true;
  }
  else if ( !str_cmp( fieldName, "Y" ) ) {
    res = &vect.v[1];
    return true;
  }
  else if ( !str_cmp( fieldName, "Z" ) ) {
    res = &vect.v[2];
    return true;
  }
  else if ( !str_cmp( fieldName, "W" ) ) {
    res = &vect.v[3];
    return true;
  }
  return false;
}
// Color.R, .G, .B
bool findColorQualifier( Value &res, const TColor &color, const char *fieldName ) {
  if ( !str_cmp( fieldName, "R" ) ) {
    res = color.r();
    return true;
  }
  else if ( !str_cmp( fieldName, "G" ) ) {
    res = color.g();
    return true;
  }
  else if ( !str_cmp( fieldName, "B" ) ) {
    res = color.b();
    return true;
  }
  return false;
}
// Handle: depends on handle.kind
bool findHandleQualifier( Value &res, const THandle *handle, const char *fieldName ) {
  if ( handle == NULL )
    return false;
  return handle->qualifiedExpression( res, fieldName );
}

bool findQualifier( Value &res, const Value &enclosing, const char *fieldName ) {
  Value v = enclosing.get();
  // findField: check if 'leftval' has a field named 'ident->image'
  //  depends on 'leftval.typ'
  //    float: nothing
  //    integer: nothing
  //    string: nothing
  //    list: nothing
  //    vector: X, Y, Z
  //    color: R, G, B
  //    handle: texture: solid: color, anchor
  //                     noisy: step, numOctaves
  //                     ...
  //            material: reflection
  //                      refraction
  //                      ...
  //            ...
  switch ( v.typ ) {
  case SVT_VECTOR2: return findVector2Qualifier( res, v.asVector2(), fieldName ); break;
  case SVT_VECTOR3: return findVector3Qualifier( res, v.asVector3(), fieldName ); break;
  case SVT_VECTOR4: return findVector4Qualifier( res, v.asVector4(), fieldName ); break;
  case SVT_COLOR: return findColorQualifier( res, v.asColor(), fieldName ); break;
  case SVT_HANDLE: return findHandleQualifier( res, v.asHandle(), fieldName ); break;
  default: // int, float, ...
    return false;
  }
  return false;
}
Value RAYQualifyExpr::eval( Context &ctx ) const {
  Value leftval = left->eval( ctx );
  Value res;

  bool found = findQualifier( res, leftval, ident->image );
  if ( !found )
    p_error("unknown qualifier [%s] for expression [%s]", ident->image, leftval.asStr() );
  return res;
}

//------------------------------------
//------------------ RECORD EXPRESSION
Value RAYRecord::eval( Context &ctx ) const {
  //--  RAYIdentifier* handleName;
  //--  RAYIdentifier* subHandleName;
  //--  RAYField** fields;
  //--    RAYIdentifier* fieldName;
  //--    RAYExpression* value;
  return THandle::parseSubclass( ctx, (RAYRecord *)this );
}


//****************************
//** Commands

void RAYCommand::execute( Context &ctx ) const {
  printf("do NOTHING; whoops\n"); fflush( stdout );
}

void RAYInclude::execute( Context &ctx ) const {
  // RAYInclude fields: RAYStringLiteral *filename

  // Save current info
  char saveFilename[1024];
  strcpy( saveFilename, currentFilename );
  int saveNumline;
  saveNumline = numline;

  const char *localFilename = filename->eval( ctx ).asStr();

  // Overwrite current info
  //-- FIXME include_path
  //--strcpy( currentFilename, include_path );
  //--strcat( currentFilename, filename );
  sprintf( currentFilename, "%sscenes/%s", globalSettings::path, localFilename );
  //--  strcpy( currentFilename, localFilename );
  numline = 0;

  FILE *fp;
  if ( ( fp = fopen( currentFilename, "r" ) ) == NULL )
    p_error( "Can't load include file: %s", currentFilename );
#ifdef __DEBUG__
  printf("RAYInclude::execute\n");
  printf("include file: %s   parsing...\n", currentFilename ); fflush(stdout);
#endif

  parseFile( ctx, fp );

#ifdef __DEBUG__
  printf("...done\n"); fflush(stdout);
#endif
  fclose(fp);

  // ReUse saved info
  strcpy( currentFilename, saveFilename );
  numline = saveNumline;
}

void RAYDeclare::execute( Context &ctx ) const {
  // RAYDeclare fields: RAYIdentifier *ident
  //                    RAYExpression *defval
  
#ifdef __DEBUG__
  printf("RAYDeclare::execute  defval: %x\n", defval ); fflush(stdout);
#endif

  // evaluate default value
  Value defValue = defval->eval( ctx );

  // add handle name if expression is a THandle
  if ( defValue.typ == SVT_HANDLE ) {
    if ( defValue.val.h == NULL )
      p_error("Declaration leads to a NULL handle!");
    defValue.val.h->name = str_dup( ident->image );
  }
  // add function name if expression is a FCT_DATA
  else if ( defValue.typ == SVT_FCT ) {
    if ( defValue.val.fct == NULL )
      p_error("Declaration leads to a NULL function!");
    defValue.val.fct->name = str_dup( ident->image );
    // and add in function hash table
    hashPut( defValue.val.fct, functionHash );
#ifdef __DEBUG__
    printf("function hash dump:\n"); fflush( stdout );
    hashDump( functionHash ); fflush( stdout );
#endif
  }

  // create the variable
  Value newvar = ctx.newVar(ident->image);

  // and assign default value
#ifdef __DEBUG__
  printf("adding expression [%s] value: [%s] typ [%d]\n",
	 ident->image, defValue.asStr(), defValue.typ ); fflush(stdout);
#endif
  newvar.setValue(defValue);
#ifdef __DEBUG2__
  printf("Dumping context:\n"); fflush( stdout );
  rayContext.dump(); fflush(stdout);
#endif
}

TWorld *parsedWorld = NULL;
void RAYWorld::execute( Context &ctx ) const {
  // RAYWorld fields: RAYField** fields
  // RAYField fields: RAYIdentifier *fieldName
  //                  RAYExpression *value;

#ifdef __DEBUG__
  printf("RAYWorld::execute...\n"); fflush(stdout);
#endif

  if ( parsedWorld != NULL )
    p_error("World TAG already found! Only one world may be defined!\n");

  parsedWorld = new TWorld;

  ctx.pushscope();
  addThis( ctx, parsedWorld );
  addSuper( ctx );

  for ( int i = 0; i < fields_count; i++ ) {
#ifdef __DEBUG__
    printf("parsing world FIELD [%s]...\n", fields[i]->fieldName->image ); fflush(stdout);
#endif
    
    if ( !parsedWorld->parse( ctx, fields[i] ) )
      p_error("error while parsing world FIELD [%s]", fields[i]->fieldName->image );

#ifdef __DEBUG__
    printf("...done\n" ); fflush( stdout );
#endif
  }

  ctx.popscope();

#ifdef __DEBUG__
  printf("...RAYWorld::execute done\n" ); fflush( stdout );
#endif
}

void RAYSettings::execute( Context &ctx ) const {
#ifdef __DEBUG__
  printf("RAYSettings::execute...\n"); fflush(stdout);
#endif

  ctx.pushscope();

  for ( int i = 0; i < fields_count; i++ ) {
#ifdef __DEBUG__
    printf("parsing settings FIELD [%s]...\n", fields[i]->fieldName->image ); fflush(stdout);
#endif

    if ( !parseSettings( ctx, fields[i] ) )
      p_error("error while parsing setting FIELD [%s]", fields[i]->fieldName->image );

#ifdef __DEBUG__
    printf("...done\n" ); fflush( stdout );
#endif
  }

  ctx.popscope();

#ifdef __DEBUG__
  printf("...RAYSettings::execute done\n" ); fflush( stdout );
#endif
}
