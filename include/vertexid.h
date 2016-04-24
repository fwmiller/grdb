#ifndef __VERTEXID_H
#define __VERTEXID_H


#include "types.h"

/* Global vertex id type */
typedef u64_t vertexid_t;


/* A vertex id set is a dynamic array of vertex ids */
struct vertexid_set {
	int capacity;		/* Current capacity */
	int n;			/* Current usage */
	vertexid_t *arr;	/* Array of vertex ids */
};

typedef struct vertexid_set *vertexid_set_t;


vertexid_t vertexid_rand();

void vertexid_set_alloc(vertexid_set_t s);
void vertexid_set_free(vertexid_set_t s);
void vertexid_set_insert(vertexid_set_t s, vertexid_t id);
void vertexid_set_remove(vertexid_set_t s, vertexid_t id);
void vertexid_set_print(vertexid_set_t s);


#endif
