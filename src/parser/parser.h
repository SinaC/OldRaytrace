#ifndef __RAY_PARSER_H__
#define __RAY_PARSER_H__

#include "value.h"
#include "../text/basics.h"
#include "struct_skel.h"

struct Context;

#define E_O_F (-1)
#define RAY_INCLUDE (-2)
#define RAY_DECLARE (-3)
#define RAY_WORLD (-4)
#define RAY_SETTINGS (-5)
#define RAY_WHILE (-6)
#define RAY_IF (-7)
#define RAY_ELSE (-8)
#define RAY_VAR (-9)
#define RAY_FUNCTION (-10)
#define RAY_IDENT (-11)
#define RAY_SUPER (-12)
#define RAY_VECTOR2 (-13)
#define RAY_VECTOR3 (-14)
#define RAY_VECTOR4 (-15)
#define RAY_COLOR (-16)
#define RAY_LE (-17)
#define RAY_GE (-18)
#define RAY_EQ (-19)
#define RAY_NE (-20)
#define RAY_AND (-21)
#define RAY_OR (-22)
#define RAY_BOOLEAN (-23)
#define RAY_STRING (-24)
#define RAY_INTEGER (-25)
#define RAY_FLOAT (-26)
struct RAYModule;

struct RAYCommand;

struct RAYInclude;

struct RAYDeclare;

struct RAYWorld;

struct RAYSettings;

struct RAYField;

struct RAYInstruction;

struct RAYCompound;

struct RAYExprInstr;

struct RAYWhileInstr;

struct RAYIfInstr;

struct RAYVarDecl;

struct RAYExpression;

struct RAYMethodCallExpr;

struct RAYFunction;

struct RAYRecord;

struct RAYCallExpr;

struct RAYQualifyExpr;

struct RAYIdentifier;

struct RAYSuper;

struct RAYList;

struct RAYVector2;

struct RAYVector3;

struct RAYVector4;

struct RAYColorRGB;

struct RAYUnaryExpr;

struct RAYBinaryExpr;

struct RAYBooleanLiteral;

struct RAYStringLiteral;

struct RAYIntegerLiteral;

struct RAYFloatLiteral;

struct RAYModule: NonTerminal{
  virtual void dump(int);
  virtual ~RAYModule();

RAYCommand** instructions;int instructions_count;RAYModule(){
instructions = 0; 

}};
struct RAYCommand: NonTerminal{
  virtual void dump(int);
  virtual ~RAYCommand();

       virtual void execute( Context &ctx ) const;
    
RAYCommand(){
}};
struct RAYInclude: RAYCommand{
  virtual void dump(int);
  virtual ~RAYInclude();


       virtual void execute( Context &ctx ) const;
    
RAYStringLiteral* filename;RAYInclude(){
filename = 0; 

}};
struct RAYDeclare: RAYCommand{
  virtual void dump(int);
  virtual ~RAYDeclare();


       virtual void execute( Context &ctx ) const;
    
RAYIdentifier* ident;RAYExpression* defval;RAYDeclare(){
ident = 0; 

defval = 0; 

}};
struct RAYWorld: RAYCommand{
  virtual void dump(int);
  virtual ~RAYWorld();


       virtual void execute( Context &ctx ) const;
    
RAYField** fields;int fields_count;RAYWorld(){
fields = 0; 

}};
struct RAYSettings: RAYCommand{
  virtual void dump(int);
  virtual ~RAYSettings();


       virtual void execute( Context &ctx ) const;
    
RAYField** fields;int fields_count;RAYSettings(){
fields = 0; 

}};
struct RAYField: NonTerminal{
  virtual void dump(int);
  virtual ~RAYField();

RAYIdentifier* fieldName;RAYExpression* value;RAYField(){
fieldName = 0; 

value = 0; 

}};
struct RAYInstruction: NonTerminal{
  virtual void dump(int);
  virtual ~RAYInstruction();

       virtual void execute( Context &ctx ) const;
    
RAYInstruction(){
}};
struct RAYCompound: RAYInstruction{
  virtual void dump(int);
  virtual ~RAYCompound();


       virtual void execute( Context &ctx ) const;
    
RAYInstruction** instr;int instr_count;RAYCompound(){
instr = 0; 

}};
struct RAYExprInstr: RAYInstruction{
  virtual void dump(int);
  virtual ~RAYExprInstr();


       virtual void execute( Context &ctx ) const;
    
RAYExpression* expr;RAYExprInstr(){
expr = 0; 

}};
struct RAYWhileInstr: RAYInstruction{
  virtual void dump(int);
  virtual ~RAYWhileInstr();


       virtual void execute( Context &ctx ) const;
    
RAYInstruction* body;RAYExpression* cond;RAYWhileInstr(){
body = 0; 

cond = 0; 

}};
struct RAYIfInstr: RAYInstruction{
  virtual void dump(int);
  virtual ~RAYIfInstr();


       virtual void execute( Context &ctx ) const;
    
RAYExpression* cond;RAYInstruction* thenpart;RAYInstruction* elsepart;RAYIfInstr(){
cond = 0; 

thenpart = 0; 

elsepart = 0; 

}};
struct RAYVarDecl: RAYInstruction{
  virtual void dump(int);
  virtual ~RAYVarDecl();


       virtual void execute( Context &ctx ) const;
    
RAYExpression* defval;RAYIdentifier* ident;RAYVarDecl(){
defval = 0; 

ident = 0; 

}};
struct RAYExpression: NonTerminal{
  virtual void dump(int);
  virtual ~RAYExpression();
 
       virtual Value eval( Context &ctx ) const;
    
RAYExpression(){
}};
struct RAYMethodCallExpr: RAYExpression{
  virtual void dump(int);
  virtual ~RAYMethodCallExpr();

 
       virtual Value eval( Context &ctx ) const;
    
RAYExpression* left;RAYIdentifier* ident;RAYExpression** params;int params_count;RAYMethodCallExpr(){
left = 0; 

ident = 0; 

params = 0; 

}};
struct RAYFunction: RAYExpression{
  virtual void dump(int);
  virtual ~RAYFunction();

 
       virtual Value eval( Context &ctx ) const;
    
RAYIdentifier** params;int params_count;RAYCompound* body;RAYFunction(){
params = 0; 

body = 0; 

}};
struct RAYRecord: RAYExpression{
  virtual void dump(int);
  virtual ~RAYRecord();

 
       virtual Value eval( Context &ctx ) const;
    
RAYIdentifier* handleName;RAYIdentifier* subHandleName;RAYField** fields;int fields_count;RAYRecord(){
handleName = 0; 

subHandleName = 0; 

fields = 0; 

}};
struct RAYCallExpr: RAYExpression{
  virtual void dump(int);
  virtual ~RAYCallExpr();

 
       virtual Value eval( Context &ctx ) const;
    
RAYIdentifier* left;RAYExpression** params;int params_count;RAYCallExpr(){
left = 0; 

params = 0; 

}};
struct RAYQualifyExpr: RAYExpression{
  virtual void dump(int);
  virtual ~RAYQualifyExpr();

 
       virtual Value eval( Context &ctx ) const;
    
RAYExpression* left;RAYIdentifier* ident;RAYQualifyExpr(){
left = 0; 

ident = 0; 

}};
struct RAYIdentifier: RAYExpression{
  virtual void dump(int);
  virtual ~RAYIdentifier();

      const char* image;
     
 
       virtual Value eval( Context &ctx ) const;
    
RAYIdentifier(){
}};
struct RAYSuper: RAYIdentifier{
  virtual void dump(int);
  virtual ~RAYSuper();

      const char* image;
     

 
       virtual Value eval( Context &ctx ) const;
    
RAYSuper(){
}};
struct RAYList: RAYExpression{
  virtual void dump(int);
  virtual ~RAYList();

 
       virtual Value eval( Context &ctx ) const;
    
RAYExpression** elems;int elems_count;RAYList(){
elems = 0; 

}};
struct RAYVector2: RAYExpression{
  virtual void dump(int);
  virtual ~RAYVector2();

 
       virtual Value eval( Context &ctx ) const;
    
RAYExpression* X;RAYExpression* Y;RAYVector2(){
X = 0; 

Y = 0; 

}};
struct RAYVector3: RAYExpression{
  virtual void dump(int);
  virtual ~RAYVector3();

 
       virtual Value eval( Context &ctx ) const;
    
RAYExpression* X;RAYExpression* Y;RAYExpression* Z;RAYVector3(){
X = 0; 

Y = 0; 

Z = 0; 

}};
struct RAYVector4: RAYExpression{
  virtual void dump(int);
  virtual ~RAYVector4();

 
       virtual Value eval( Context &ctx ) const;
    
RAYExpression* X;RAYExpression* Y;RAYExpression* Z;RAYExpression* W;RAYVector4(){
X = 0; 

Y = 0; 

Z = 0; 

W = 0; 

}};
struct RAYColorRGB: RAYExpression{
  virtual void dump(int);
  virtual ~RAYColorRGB();

 
       virtual Value eval( Context &ctx ) const;
    
RAYExpression* R;RAYExpression* G;RAYExpression* B;RAYColorRGB(){
R = 0; 

G = 0; 

B = 0; 

}};
struct RAYUnaryExpr: RAYExpression{
  virtual void dump(int);
  virtual ~RAYUnaryExpr();

 
       virtual Value eval( Context &ctx ) const;
    
int op;RAYExpression* expr;RAYUnaryExpr(){
op = 0; 

expr = 0; 

}};
struct RAYBinaryExpr: RAYExpression{
  virtual void dump(int);
  virtual ~RAYBinaryExpr();

 
       virtual Value eval( Context &ctx ) const;
    
int op;RAYExpression* left;RAYExpression* right;RAYBinaryExpr(){
op = 0; 

left = 0; 

right = 0; 

}};
struct RAYBooleanLiteral: RAYExpression{
  virtual void dump(int);
  virtual ~RAYBooleanLiteral();

      bool value;
    
 
       virtual Value eval( Context &ctx ) const;
    
RAYBooleanLiteral(){
}};
struct RAYStringLiteral: RAYExpression{
  virtual void dump(int);
  virtual ~RAYStringLiteral();

       const char* image;
     
 
       virtual Value eval( Context &ctx ) const;
    
RAYStringLiteral(){
}};
struct RAYIntegerLiteral: RAYExpression{
  virtual void dump(int);
  virtual ~RAYIntegerLiteral();

       int value;
     
 
       virtual Value eval( Context &ctx ) const;
    
RAYIntegerLiteral(){
}};
struct RAYFloatLiteral: RAYExpression{
  virtual void dump(int);
  virtual ~RAYFloatLiteral();

       float value;
     
 
       virtual Value eval( Context &ctx ) const;
    
RAYFloatLiteral(){
}};


#endif
