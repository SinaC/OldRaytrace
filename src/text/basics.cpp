#include <string.h>

#include "basics.h"

const char *shiftFun( const int n ) {
  static char buffer[4096];
  char *p = &buffer[0];
  for ( int i = 0; i < n; i++ )
    *p++ = ' ';
  *p = '\0';
  return buffer;
}

// add \r for each \n found
void add_carriage_return( char *s ) {
  if ( !strstr( s, "\n" ) )
    return;
  char buf[MAX_STRING_LENGTH];
  char *q = s;
  char *p = buf;

  while ( *q != '\0' ) {
    *p = *q;
    if ( *q == '\n' )
      *(++p) = '\r';
    p++; q++;
  }
  *p = '\0';

  strcpy( s, buf );
}

const char * replaceChar( const char *s, char from, char to ) {
  static char buf[MAX_STRING_LENGTH];
  const char *q = s;
  char *p = buf;

  while ( *q != '\0' ) {
    if ( *q == from )
      *p = to;
    else
      *p = *q;
    p++; q++;
  }
  *p = '\0';

  return buf;
}

void stripChar( char *s, const char c ) {
  char *p = s;
  char *q = s;
  
  while ( *q != '\0' ) {
    if ( *q == c )
      *q++;
    *p++ = *q++;
  }
  *p = '\0';
}

char *str_dup( const char *str ) {
  char *s = new char [strlen(str)+1];
  strcpy( s, str );
  return s;
}

bool str_cmp( const char *astr, const char *bstr ) {
  if ( astr == NULL ) {
    printf( "Str_cmp: null astr.");
    return true;
  }

  if ( bstr == NULL ) {
    printf( "Str_cmp: null bstr." );
    return true;
  }

  for ( ; *astr || *bstr; astr++, bstr++ ) {
    if ( LOWER(*astr) != LOWER(*bstr) )
      return true;
  }

  return false;
}

bool str_prefix( const char *astr, const char *bstr ) {
  if ( astr == NULL ) {
    printf( "Strn_cmp: null astr.");
    return true;
  }

  if ( bstr == NULL ) {
    printf( "Strn_cmp: null bstr." );
    return true;
  }

  for ( ; *astr; astr++, bstr++ ) {
    if ( LOWER(*astr) != LOWER(*bstr) )
      return true;
  }

  return false;
}

bool str_infix( const char *astr, const char *bstr ) {
  int sstr1;
  int sstr2;
  int ichar;
  char c0;

  if ( ( c0 = LOWER(astr[0]) ) == '\0' )
    return false;

  sstr1 = strlen(astr);
  sstr2 = strlen(bstr);

  for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ ) {
    if ( c0 == LOWER(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )
      return false;
  }

  return true;
}

bool str_suffix( const char *astr, const char *bstr ) {
  int sstr1;
  int sstr2;

  sstr1 = strlen(astr);
  sstr2 = strlen(bstr);
  if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
    return false;
  else
    return true;
}
