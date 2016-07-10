#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

void
cli_graph_vertex(char *cmdline, int *pos)
{
	vertexid_t id;
	vertex_t v;
	char s[BUFSIZE];
	int i, n = 1;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	if (strlen(s) <= 0)
		return;

	/* Check for empty schema */
	if (current->sv == NULL) {
		printf("add a vertex schema\n");
		return;
	}
	n = atoi(s);

	/* Add the specified number of random vertices */
	for (i = 0; i < n; i++) {
		id = vertexid_rand();

		/* XXX Check for duplicate id */

		/* XXX Check for empty schema */
		if (current->sv == NULL) {
			printf("add a vertex schema\n");
			return;
		}

		printf("add vertex %04llx to current graph\n", id);

		v = (vertex_t) malloc(sizeof(struct vertex));
		assert(v != NULL);
		vertex_init(v);
		vertex_set_id(v, id);

		/* Create a new tuple for the vertex */
		v->tuple = malloc(sizeof(struct tuple));
		assert (v->tuple != NULL);
		tuple_init(v->tuple, current->sv);
		graph_insert_vertex(current, v);
	}
}
