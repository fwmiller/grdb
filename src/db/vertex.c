#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

void
vertex_init(vertex_t v)
{
	assert(v != NULL);
	memset(v, 0, sizeof(struct vertex));
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
