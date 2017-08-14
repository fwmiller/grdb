#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

void
cli_graph_edge(char *cmdline, int *pos)
{
	struct vertex v, w;
	vertex_t v1, w1;
	edge_t e;
	tuple_t t;
	char s[BUFSIZE];
	int i, j;

	/* Get the vertex id arguments */
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

	vertex_init(&v);
	vertex_init(&w);
	v.id = i;
	w.id = j;

	v1 = component_find_vertex_by_id(current_component, &v);
	w1 = component_find_vertex_by_id(current_component, &w);

	if (v1 == NULL && w1 == NULL) {
		printf("At least one vertex must exist in component\n");
		return;
	}
	if (v1 == NULL) {
		// Create a new vertex with i as its id
		v1 = (vertex_t) malloc(sizeof(struct vertex));
		assert (v1 != NULL);
		vertex_init(v1);
		v1->id = i;

		/* Create the vertex tuple based on its schema */
		if (current_component->sv != NULL) {
			t = (tuple_t) malloc(sizeof(struct tuple));
			assert (t != NULL);
			tuple_init(t, current_component->sv);
			v1->tuple = t;
		}
		component_insert_vertex(current_component, v1);

	} else if (w1 == NULL) {
		// Create a new vertex with j as its id
		w1 = (vertex_t) malloc(sizeof(struct vertex));
		assert (w1 != NULL);
		vertex_init(w1);
		w1->id = j;

		/* Create the vertex tuple based on its schema */
		if (current_component->sv != NULL) {
			t = (tuple_t) malloc(sizeof(struct tuple));
			assert (t != NULL);
			tuple_init(t, current_component->sv);
			w1->tuple = t;
		}
		component_insert_vertex(current_component, w1);
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
