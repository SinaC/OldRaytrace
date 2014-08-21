#include <windows.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "parser_skel.h"
#include "../text/basics.h"
#include "parser.h"


//--#define __DEBUG__

int token;
const char* cur_pos;
int numline;
extern char currentFilename[1024];


#define NOTHING (-666)
#define COMMENT (-999)

int scan() {
  int res=NOTHING; 

  int dotCount = 0; // one dot allowed in number

  const char* start;
  token_image[0] = '\0';

  do {
    while (isspace(*cur_pos)) {
      if ( *cur_pos == '\n' )
	numline++;
      cur_pos++;
    }

    switch (*cur_pos) {   

    case '\0' :
      res = E_O_F;
      break;

    case '<':
      cur_pos++;
      if (*cur_pos == '=') {
	cur_pos++;
	res = RAY_LE;	
      }
      else
	res = '<';  
      break;

    case '>':
      cur_pos++;
      if (*cur_pos == '=') {
	cur_pos++;
	res = RAY_GE;
      }
      else
	res = '>';  
      break;

    case '/':
    case '&':
    case '|':
    case '=':
      res = NOTHING;
      if (*(cur_pos+1) == *cur_pos) {
	// the two chars are equal.
	cur_pos++;
	switch (*cur_pos) {
	case '&':
	  res = RAY_AND;
	  break;
	case '|':
	  res = RAY_OR;
	  break;
	case '=':
	  res = RAY_EQ;
	  break;
	case '/':
	  while ((*cur_pos) != '\n')
	    cur_pos++;
	  numline++;
    	  res = COMMENT;
	  break;
	}
	cur_pos++;
      }
      
      if (res != NOTHING)
	break;
      // ELSE FALL THROUGH

    case ':': // pseudo-inheritance
    case '.': // qualify expression
    case '#': // cross product
    case '^': // exponentiation
    case '[': // list element opening tag
    case ']': // list element closing tag
    case '-':
    case '(':
    case ')':
    case '{':
    case '}':
    case ';':
    case ',':
    case '+':
    case '*':
      res = *cur_pos;
      cur_pos++;
      break;

    case '!':      
      if (*(cur_pos+1) == '=') {
	cur_pos++;
	res = RAY_NE;
      }
      else
	res = '!';
      cur_pos++;
      break;

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      start = cur_pos;

      while (( !isspace(*cur_pos) && !ispunct(*cur_pos)) 
	     || ( *cur_pos == '.' && dotCount++==0 ) ) // one dot allowed
	cur_pos++;

      strncpy(token_image, start, cur_pos-start);
      token_image[cur_pos-start] = '\0';

      if ( dotCount > 0 ) // if dot found -> float
	res = RAY_FLOAT;
      else                // else -> integer
	res = RAY_INTEGER;
      break;

    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
    case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
    case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
    case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
    case 'Y': case 'Z':
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
    case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
    case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
    case 's': case 't': case 'u': case 'v': case 'w': case 'x':
    case 'y': case 'z':
    case '_':

      start = cur_pos;
      while ((!isspace(*cur_pos) && !ispunct(*cur_pos))||*cur_pos=='_')
	cur_pos++;
    
      strncpy(token_image, start, cur_pos-start);
      token_image[cur_pos-start] = '\0';

      if (!str_cmp("function", token_image))
	res = RAY_FUNCTION;
      else if (!str_cmp("if", token_image))
	res = RAY_IF;
      else if (!str_cmp("else", token_image))
	res = RAY_ELSE;
      else if (!str_cmp("var", token_image))
	res = RAY_VAR;
      else if (!str_cmp("while", token_image))
	res = RAY_WHILE;
      else if ( !str_cmp( token_image, "rgb" ) )
	res = RAY_COLOR;
      else if ( !str_cmp( token_image, "vector2" ) )
	res = RAY_VECTOR2;
      else if ( !str_cmp( token_image, "vector" )
		|| !str_cmp( token_image, "vector3" ) )
	res = RAY_VECTOR3;
      else if ( !str_cmp( token_image, "vector4" ) )
	res = RAY_VECTOR4;
      else if ( !str_cmp( token_image, "include" ) )
	res = RAY_INCLUDE;
      else if ( !str_cmp( token_image, "declare" ) )
	res = RAY_DECLARE;
      else if ( !str_cmp( token_image, "world" ) )
	res = RAY_WORLD;
      else if ( !str_cmp( token_image, "true" ) || !str_cmp( token_image, "false") )
	res = RAY_BOOLEAN;
      else if ( !str_cmp( token_image, "settings" ) )
	res = RAY_SETTINGS;
      else 
	res = RAY_IDENT;
      break;

    case '\'': {
      cur_pos++;
      start = cur_pos;
      bool found = false;
      while (*cur_pos != '\'') {
	if ( *cur_pos == '\n' )
	  numline++;
	if ( *cur_pos == '\\' ) { // skip backslash
	  found = true;
	  cur_pos++;
	}
	cur_pos++;
      }
      strncpy(token_image, start, cur_pos-start);
      token_image[cur_pos-start] = '\0';
      // add \r for each \n found
      //add_carriage_return( token_image );

      //remove backslash
      if ( found ) stripChar( token_image, '\\' );

      cur_pos++;
      res = RAY_STRING;
      break;
    }

    }
  
    token=res;
    //    log_stringf("Found token: %d '%c' [%s]\n", res, res, token_image);
  } while (res == COMMENT);  

#ifdef __DEBUG__
  printf("=== scan()  token_image [%s]  res: [%d]  numline: %d\n", token_image, res, numline ); fflush(stdout);
#endif

  if ( res == NOTHING )
    p_error("Unknown token: [%s] [%s] [line %d]", token_image, currentFilename, numline );

  return res;
}
