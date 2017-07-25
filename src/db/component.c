#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

void
component_init(component_t c)
{
	assert (c != NULL);
	memset(c, 0, sizeof(struct component));

	/* Initial values for file descriptors */
	c->vfd = (-1);
	c->efd = (-1);
}

vertex_t
component_find_vertex_by_id(component_t c, vertex_t v)
{
	int len;

	assert (c != NULL);
	assert (v != NULL);

	/* Assume v was allocated and v->id was set by the caller */
	len = vertex_read(v, c->vfd);
	if (len > 0)
		return v;

	return NULL;
}

void
component_insert_vertex(component_t c, vertex_t v)
{
	assert (c != NULL);
	assert (v != NULL);

	vertex_write(v, c->vfd);
}

edge_t
component_find_edge_by_ids(component_t c, edge_t e)
{
	int len;

	assert (c != NULL);
	assert (e != NULL);

	/*
	 * Assume e was allocated and e->id1 and e->id2 were set by
	 * the caller
	 */
	len = edge_read(e, c->efd);
	if (len > 0)
		return e;

	return NULL;
}

void
component_insert_edge(component_t c, edge_t e)
{
	assert (c != NULL);
	assert (e != NULL);

	edge_write(e, c->efd);
}

void
component_print(component_t c, int with_tuples)
{
	vertex_t v;
	edge_t e;

	assert (c != NULL);

	printf("({");
	/* Vertices */
	for (v = c->v; v != NULL; v = v->next) {
		vertex_print(v);
		if (v->tuple != NULL && with_tuples)
			tuple_print(v->tuple, c->el);
		if (v->next != NULL)
			printf(",");
	}

	printf("},{");
	/* Edges */
	for (e = c->e; e != NULL; e = e->next) {
		edge_print(e);
		if (e->tuple != NULL && with_tuples)
			tuple_print(e->tuple, c->el);
		if (e->next != NULL)
			printf(",");
	}
	printf("})");
}
