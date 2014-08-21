#ifndef __HASHING_H__
#define __HASHING_H__

void hashInit();

#define HASH_FCT	(128)

struct FCT_DATA;

extern FCT_DATA * functionHash[HASH_FCT];

FCT_DATA *hashGet(const char *s, FCT_DATA **c );
void hashPut( FCT_DATA *f, FCT_DATA **c );
void hashDump( FCT_DATA **c );

#endif
