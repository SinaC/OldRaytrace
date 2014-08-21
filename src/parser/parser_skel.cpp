#include "parser_skel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../memory.h"

//--#define __DEBUG__

char token_image[4096];

extern char currentFilename[1024];
extern int numline;
extern int rule_depth_table[];
extern int goto_entry[];
extern int map_token(int);
extern NonTerminal* reduce_rule(int, stack_elem*);

extern const int* action_table[];
extern const int* goto_table[];

static stack_elem parse_stack[1024];
static int parse_top = 0;

extern int initialState;


//////////////////////////////////////////////////////

void p_warn(const char* msg, ...) {
  va_list argptr;

  static char buf[1024]; 

  va_start(argptr, msg);
  vsprintf(buf, msg, argptr);
  printf("PARSE WARNING: %s\n", buf); 
  va_end(argptr);
}

void p_error(const char* msg, ...) {
  va_list argptr;

  static char buf[1024]; 

  va_start(argptr, msg);
  vsprintf(buf, msg, argptr);
  printf("PARSE ERROR: %s\n", buf); 
  va_end(argptr);

  shutdownRaytracing();
  exit(-1);
}

//////////////////////////////////////////////////////////////

NonTerminal** listAsArray(ListNonTerminal* list, int & out_count) {

  int length = 0;
  ListNonTerminal* p = list;
  while (p != NULL) {
    length++;
    p = p->tail;
  }

  NonTerminal** result = new NonTerminal*[length];

  p = list;
  for (int i=length-1; i>=0; i--) {
    result[i] = p->head;
    p = p->tail;
  }
  
  out_count = length;
  return result;
}


char* shift(int i) {
  static char result[256];
  memset(result, ' ', i);
  result[i] = 0;
  return result;
}


//////////////////////////////////////////////////////////////

static void push(int state, int elem, char* tok_image) {
  parse_stack[parse_top].elem = elem;
  parse_stack[parse_top].state = state;
  delete[] parse_stack[parse_top].tok_image;
  if ( tok_image == NULL )
    parse_stack[parse_top].tok_image = NULL;
  else {
    char* s = new char[strlen(tok_image)+1];
    strcpy(s, tok_image);
    parse_stack[parse_top].tok_image = s;
  }
  parse_top++;
}

static void pop(int i) {
  parse_top -= i;
  if (parse_top < 0) {
    printf("bardaf\n");
    exit(-1);
  }
}

static void set_top_nt(NonTerminal* nt) {
  if ( parse_top <= 0 )
    p_error("NULL top state, reduce instead of shift!!");
  parse_stack[parse_top-1].elem = (int) nt;
}

static int get_top_state() {
#ifdef __DEBUG__
  printf("get_top_state: parse_top=%d  parse_stack.state=%d\n",
	 parse_top, parse_stack[parse_top-1].state );
#endif
  if ( parse_top <= 0 )
    p_error("NULL top state, reduce instead of shift!!");
  return parse_stack[parse_top-1].state;
}

NonTerminal* parse() {
  int token;
  //--  int state = 1;
  int state = initialState;

  numline = 1;

  token = scan();

#ifdef __DEBUG__
  //--printf("=== parse()  token_image [%x]\n", (int)token_image);
  printf("=== parse()  token_image [%s]  token [%d]\n", token_image, token ); fflush(stdout);
#endif

  while (1) {

#ifdef __DEBUG__
    printf("\nIn state %d, l-a %d [%c]\n", state, token, (token>=32)?token:' ');  
    fflush(stdout);
    printf("Stack = ( ");
    for (int i = 0; i<parse_top; i++)
      printf("%d ", parse_stack[i].state );
    printf(" )\n");
    fflush(stdout);
#endif   

    int action = action_table[state - 1][map_token(token) - 1];
#ifdef __DEBUG__
    printf("action[%d, %d] = %d\n", state-1, map_token(token)-1, action);
    fflush(stdout);
#endif
    if (action == 0) {
      // parse error
      printf("parse error in state %d file: %s  line %d (%s)\n", state, currentFilename, numline, token_image );
      exit(-1);
    }
    else if (action > 0) {
      //shift
#ifdef __DEBUG__
      printf("shitfing token %d [%c]\n", token, (token>=32)?token:' ');
      printf("token_image: %s\n", token_image);
      fflush(stdout);
#endif
      push(state, token, token_image);
      token = scan();
      state = action;
#ifdef __DEBUG__
      printf("ok\n");
      fflush(stdout);
#endif
    }
    else { // action < 0
      // reduce
      int rule = -action;
      int depth = rule_depth_table[rule-1];
#ifdef __DEBUG__
      printf("reducing rule %d, depth %d\n", rule-1, depth);  
      fflush(stdout);
#endif
      NonTerminal* nt = reduce_rule(rule, parse_stack+parse_top-depth);
      push(state, 0, NULL); // push a dummy state, in case depth = 0
      pop(depth);
      set_top_nt(nt);
      state = get_top_state();
#ifdef __DEBUG__
      printf("Poping to state = %d\n", state);
      fflush(stdout);
#endif
      state = goto_table[state - 1][goto_entry[rule-1]];
      if (rule == 1) {
#ifdef __DEBUG__
	printf("success!\n");
	fflush(stdout);
#endif
	return nt;
      }
    }
  }
}
