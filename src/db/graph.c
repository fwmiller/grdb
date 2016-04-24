#include <assert.h>
#include <stdio.h>
#include "graph.h"

void
graph_insert_vertex(vertex_t *g, vertex_t v)
{
	assert (g != NULL);
	assert (v != NULL);

	if (*g == NULL) {
		/* Insert vertex into empty graph */
		*g = v;
		return;
	}
	/* Insert at the front of the double linked list */
	(*g)->prev = v;
	v->next = *g;
	*g = v;
}

vertex_t
graph_find_vertex_by_id(vertex_t g, vertexid_t id)
{
	vertex_t v;

	assert (g != NULL);

	for (v = g; v != NULL; v = v->next)
		if (v->id == id)
			return v;

	return NULL;
}

void
graph_insert_edge(vertex_t g, vertexid_t v1, vertexid_t v2)
{
	vertex_t v;

	assert (g != NULL);

	/* Search graph for v1 */
	v = graph_find_vertex_by_id(g, v1);
	if (v == NULL)
		return;

	/* Insert v2 into v1 adj set */
	vertexid_set_insert(&(v->adj), v2);
}

void
graph_print(vertex_t g)
{
	vertex_t v;

	assert (g != NULL);

	for (v = g; v != NULL; v = v->next) {
		vertex_print(v);
		printf("\n");
	}
}
