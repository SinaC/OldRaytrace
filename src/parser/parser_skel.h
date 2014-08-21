#include <stdlib.h>
#include <string.h>
#include "struct_skel.h"

extern char token_image[4096];
extern int scan();
extern NonTerminal* parse();
extern NonTerminal** listAsArray(ListNonTerminal* list, int & out_count);
extern char* shift(int i);
extern int numline;

extern void p_error(const char* msg, ...);
extern void p_warn(const char* msg, ...);
