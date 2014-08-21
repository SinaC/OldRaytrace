#ifndef __BASICS_TEXT_H__
#define __BASICS_TEXT_H__

#include <stdio.h>

#define MAX_STRING_LENGTH (1024)

const char *shiftFun( const int n );
void add_carriage_return( char *s );
const char * replaceChar( const char *s, char from, char to );
void stripChar( char *s, const char c );

#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))


// Duplicate a string
char *str_dup( const char *str );

// Compare strings, case insensitive.
// Return TRUE if different
//   (compatibility with historical functions).
bool str_cmp( const char *astr, const char *bstr );

// Compare strings, case insensitive, for prefix matching.
// Return TRUE if astr not a prefix of bstr
//   (compatibility with historical functions).
bool str_prefix( const char *astr, const char *bstr );

// Compare strings, case insensitive, for match anywhere.
// Returns TRUE is astr not part of bstr.
//   (compatibility with historical functions).
bool str_infix( const char *astr, const char *bstr );

// Compare strings, case insensitive, for suffix matching.
// Return TRUE if astr not a suffix of bstr
//   (compatibility with historical functions).
bool str_suffix( const char *astr, const char *bstr );

#endif
