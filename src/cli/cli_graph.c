#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

static void
cli_graph_vertex(char *cmdline, int *pos)
{
	vertexid_t id;
	vertex_t v;
	char s[BUFSIZE];
	int i, n = 1;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	if (strlen(s) > 0)
		n = atoi(s);

	for (i = 0; i < n; i++) {
		id = vertexid_rand();

		/* XXX Check for duplicate id */

		printf("add vertex %04llx to graph %d\n", id, currgraph);

		v = (vertex_t) malloc(sizeof(struct vertex));
		assert(v != NULL);
		vertex_alloc(v);
		vertex_set_id(v, id);
		graph_insert_vertex(&(graphs[currgraph]), v);
	}
}

static void
cli_graph_edge(char *cmdline, int *pos)
{
	vertex_t g, v1, v2;
	int i, n, v1idx, v2idx;

	char s[BUFSIZE];
	int j, m = 1;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	if (strlen(s) > 0)
		m = atoi(s);

	g = graphs[currgraph];

	for (j = 0; j < m; j++) {
		/* Pick two random vertices in the current graph */
		for (v1 = g, n = 0; v1 != NULL; v1 = v1->next, n++);
		v1idx = rand() % n;
		v2idx = rand() % n;
		for (v1 = g, i = 0;
		     v1 != NULL && i < v1idx;
		     v1 = v1->next, i++);
		for (v2 = g, i = 0;
		     v2 != NULL && i < v2idx;
		     v2 = v2->next, i++);

		printf("add edge (%04llx,%04llx) to graph %d\n",
		       v1->id, v2->id, currgraph);

		vertex_edge_insert(v1, v2->id);
	}
}

static void
cli_graph_print()
{
	int i;

	for (i = 0; i < MAX_GRAPHS; i++)
		if (graphs[i] != NULL) {
			if (i == currgraph)
				printf(">");
			printf("graph %d\n", i);
			graph_print(graphs[i]);
		}
}

void
cli_graph(char *cmdline)
{
	char s[BUFSIZE];
	int pos = 1;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, &pos, " ", s);

	if (strcmp(s, "vertex") == 0)
		cli_graph_vertex(cmdline, &pos);

	if (strcmp(s, "edge") == 0)
		cli_graph_edge(cmdline, &pos);

	else if (strlen(s) == 0)
		cli_graph_print();
}
