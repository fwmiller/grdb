#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

void
cli_graph_edge(char *cmdline, int *pos)
{
	vertex_t v, w;
	edge_t e;
	char s[BUFSIZE];
	int i, j;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	if (strlen(s) == 0) {
		printf("Missing vertex id\n");
		return;
	}
	i = atoi(s);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	if (strlen(s) == 0) {
		printf("Missing vertex id\n");
		return;
	}
	j = atoi(s);

	v = graph_find_vertex_by_id(current, i);
	w = graph_find_vertex_by_id(current, j);

	if (v == NULL && w == NULL) {
		printf("At least one vertex must exist in graph\n");
		return;
	}
	if (v == NULL) {
		// Create a new vertex with i as its id
		v = (vertex_t) malloc(sizeof(struct vertex));
		assert (v != NULL);
		vertex_init(v);
		v->id = i;
		graph_insert_vertex(current, v);

	} else if (w == NULL) {
		// Create a new vertex with j as its id
		w = (vertex_t) malloc(sizeof(struct vertex));
		assert (w != NULL);
		vertex_init(w);
		w->id = j;
		graph_insert_vertex(current, w);
	}
	e = (edge_t) malloc(sizeof(struct edge));
	assert (e != NULL);
	edge_init(e);
	edge_set_vertices(e, i, j);
	graph_insert_edge(current, e);
}
