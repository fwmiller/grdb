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
	if (strlen(s) > 0)
		n = atoi(s);

	for (i = 0; i < n; i++) {
		id = vertexid_rand();

		/* XXX Check for duplicate id */

		printf("add vertex %04llx to graph %d\n", id, currgraph);

		v = (vertex_t) malloc(sizeof(struct vertex));
		assert(v != NULL);
		vertex_init(v);
		vertex_set_id(v, id);
		graph_insert_vertex(&(graphs[currgraph]), v);
	}
}
