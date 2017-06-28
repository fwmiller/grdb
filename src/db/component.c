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
}

void
component_insert_vertex(component_t c, vertex_t v)
{
	vertex_t w;

	assert (c != NULL);
	assert (v != NULL);

	if (c->v == NULL) {
		/* Insert vertex into empty component */
		c->v = v;
		return;
	}
	/* Insert at the end of the double linked list */
	for (w = c->v; w->next != NULL; w = w->next);
	w->next = v;
	v->prev = w;
}

vertex_t
component_find_vertex_by_id(component_t c, vertexid_t id)
{
	vertex_t v;

	assert (c != NULL);

	for (v = c->v; v != NULL; v = v->next)
		if (v->id == id)
			return v;

	return NULL;
}

edge_t
component_find_edge_by_ids(component_t c, vertexid_t id1, vertexid_t id2)
{
	edge_t e;

	assert (c != NULL);

	for (e = c->e; e != NULL; e = e->next)
		if (e->id1 == id1 && e->id2 == id2)
			return e;

	return NULL;
}

void
component_insert_edge(component_t c, edge_t e)
{
	edge_t f;

	if (c->e == NULL) {
		/* Insert edge into empty component edge set */
		c->e = e;
		return;
	}
	/* Insert at the end of the double linked list */
	for (f = c->e; f->next != NULL; f = f->next);
	f->next = e;
	e->prev = f;
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
