#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void
edge_init(edge_t e)
{
	assert(e != NULL);

	e->id1 = 0;
	e->id2 = 0;
	e->prev = NULL;
	e->next = NULL;
	e->tuple = NULL;
}

void
edge_set_vertices(edge_t e, vertexid_t id1, vertexid_t id2)
{
	assert(e != NULL);

	e->id1 = id1;
	e->id2 = id2;
}

void
edge_print(edge_t e)
{
	assert(e != NULL);

	//printf("(%04llx,%04llx)", e->id1, e->id2);
	printf("(%llu,%llu)", e->id1, e->id2);
}
