#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

void
cli_graph_edge(char *cmdline, int *pos)
{
	struct vertex v, w;
	vertex_t v1, w1;
	struct edge e;
	tuple_t t;
	char s[BUFSIZE];
	int i, j;
	int gidx, cidx;

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
#if _DEBUG
	printf("cli_graph_edge: add edge (%d,%d) to current component\n",
		i, j);
#endif
	vertex_init(&v);
	vertex_set_id(&v, i);
	v1 = component_find_vertex_by_id(current_component, &v);
#if _DEBUG
	if (v1 == NULL)
		printf("cli_graph_edge: vertex %d not found\n", i);
#endif

	vertex_init(&w);
	vertex_set_id(&w, j);
	w1 = component_find_vertex_by_id(current_component, &w);
#if _DEBUG
	if (w1 == NULL)
		printf("cli_graph_edge: vertex %d not found\n", j);
#endif

	if (v1 == NULL && w1 == NULL) {
		printf("At least one vertex must exist in component\n");
		return;
	}
	/* Persistence... */
	gidx = graphs_get_index(current_graph);
	cidx = components_get_index(current_graph, current_component);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/e", grdbdir, gidx, cidx);
#if _DEBUG
	printf("cli_graph_edge: open edge file %s\n", s);
#endif
	current_component->efd = open(s, O_RDWR | O_CREAT, 0644);
	if (current_component->efd < 0) {
#if _DEBUG
		printf("cli_graph_edge: ");
		printf("open edge file failed (%s)\n", strerror(errno));
#endif
		return;
	}
	edge_init(&e);
	edge_set_vertices(&e, i, j);
	edge_write(&e, current_component->efd);
/*
	e = (edge_t) malloc(sizeof(struct edge));
	assert (e != NULL);
	edge_init(e);
	edge_set_vertices(e, i, j);
*/
	/* Create the edge tuple based on its schema */
/*
	if (current_component->se != NULL) {
		t = (tuple_t) malloc(sizeof(struct tuple));
		assert (t != NULL);
		tuple_init(t, current_component->se);
		e->tuple = t;
	}
	component_insert_edge(current_component, e);
*/
}
