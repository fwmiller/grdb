#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

void
graph_init(graph_t g)
{
	assert (g != NULL);
	memset(g, 0, sizeof(struct graph));
}

void
graph_insert_vertex(graph_t g, vertex_t v)
{
	vertex_t w;

	assert (g != NULL);
	assert (v != NULL);

	if (g->v == NULL) {
		/* Insert vertex into empty graph */
		g->v = v;
		return;
	}
	/* Insert at the end of the double linked list */
	for (w = g->v; w->next != NULL; w = w->next);
	w->next = v;
	v->prev = w;
}

vertex_t
graph_find_vertex_by_id(graph_t g, vertexid_t id)
{
	vertex_t v;

	assert (g != NULL);

	for (v = g->v; v != NULL; v = v->next)
		if (v->id == id)
			return v;

	return NULL;
}

edge_t
graph_find_edge_by_ids(graph_t g, vertexid_t id1, vertexid_t id2)
{
	edge_t e;

	assert (g != NULL);

	for (e = g->e; e != NULL; e = e->next)
		if (e->id1 == id1 && e->id2 == id2)
			return e;

	return NULL;
}

void
graph_insert_edge(graph_t g, edge_t e)
{
	edge_t f;

	if (g->e == NULL) {
		/* Insert edge into empty graph edge set */
		g->e = e;
		return;
	}
	/* Insert at the end of the double linked list */
	for (f = g->e; f->next != NULL; f = f->next);
	f->next = e;
	e->prev = f;
}

void
graph_print(graph_t g, int with_tuples)
{
	vertex_t v;
	edge_t e;

	assert (g != NULL);

	printf("({");
	/* Vertices */
	for (v = g->v; v != NULL; v = v->next) {
		vertex_print(v);
		if (v->tuple != NULL && with_tuples)
			tuple_print(v->tuple);
		if (v->next != NULL)
			printf(",");
	}

	printf("},{");
	/* Edges */
	for (e = g->e; e != NULL; e = e->next) {
		edge_print(e);
		if (e->tuple != NULL && with_tuples)
			tuple_print(e->tuple);
		if (e->next != NULL)
			printf(",");
	}
	printf("})");
}
