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
	tuple_t t;
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

	v = component_find_vertex_by_id(current_component, i);
	w = component_find_vertex_by_id(current_component, j);

	if (v == NULL && w == NULL) {
		printf("At least one vertex must exist in component\n");
		return;
	}
	if (v == NULL) {
		// Create a new vertex with i as its id
		v = (vertex_t) malloc(sizeof(struct vertex));
		assert (v != NULL);
		vertex_init(v);
		v->id = i;

		/* Create the vertex tuple based on its schema */
		if (current_component->sv != NULL) {
			t = (tuple_t) malloc(sizeof(struct tuple));
			assert (t != NULL);
			tuple_init(t, current_component->sv);
			v->tuple = t;
		}
		component_insert_vertex(current_component, v);

	} else if (w == NULL) {
		// Create a new vertex with j as its id
		w = (vertex_t) malloc(sizeof(struct vertex));
		assert (w != NULL);
		vertex_init(w);
		w->id = j;

		/* Create the vertex tuple based on its schema */
		if (current_component->sv != NULL) {
			t = (tuple_t) malloc(sizeof(struct tuple));
			assert (t != NULL);
			tuple_init(t, current_component->sv);
			w->tuple = t;
		}
		component_insert_vertex(current_component, w);
	}
	e = (edge_t) malloc(sizeof(struct edge));
	assert (e != NULL);
	edge_init(e);
	edge_set_vertices(e, i, j);

	/* Create the edge tuple based on its schema */
	if (current_component->se != NULL) {
		t = (tuple_t) malloc(sizeof(struct tuple));
		assert (t != NULL);
		tuple_init(t, current_component->se);
		e->tuple = t;
	}
	component_insert_edge(current_component, e);
}
