#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void
vertex_init(vertex_t v)
{
	assert(v != NULL);

	v->id = 0;
	v->prev = NULL;
	v->next = NULL;
	v->tuple = NULL;
}

void
vertex_set_id(vertex_t v, vertexid_t id)
{
	assert(v != NULL);

	v->id = id;
}

void
vertex_print(vertex_t v)
{
	assert(v != NULL);

	//printf("%04llx", v->id);
	printf("%llu", v->id);
}
