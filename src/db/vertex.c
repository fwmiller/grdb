#include <assert.h>
#include <stdio.h>
#include "vertexid.h"
#include "graph.h"

void
vertex_alloc(vertex_t v)
{
	assert(v != NULL);

	v->id = 0;
	vertexid_set_alloc(&(v->adj));
	v->prev = NULL;
	v->next = NULL;
}

void
vertex_set_id(vertex_t v, vertexid_t id)
{
	assert(v != NULL);

	v->id = id;
}

void
vertex_edge_insert(vertex_t v, vertexid_t id)
{
	assert(v != NULL);

	vertexid_set_insert(&(v->adj), id);
}

void
vertex_edge_remove(vertex_t v, vertexid_t id)
{
	assert(v != NULL);

	vertexid_set_remove(&(v->adj), id);
}

void
vertex_print(vertex_t v)
{
	assert(v != NULL);

	printf("(%04llx -> ", v->id);

	/* Print adjacency set */
	vertexid_set_print(&(v->adj));

	printf(")");
}
