#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

void
cli_graph_edge(char *cmdline, int *pos)
{
	graph_t g;
	vertex_t v1, v2;
	int i, n, v1idx, v2idx;

	char s[BUFSIZE];
	int j, m = 1;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	if (strlen(s) > 0)
		m = atoi(s);

	g = &(graphs[currgraph]);

	for (j = 0; j < m; j++) {
		/* Pick two random vertices in the current graph */
		for (v1 = g->v, n = 0; v1 != NULL; v1 = v1->next, n++);
		v1idx = rand() % n;
		v2idx = rand() % n;
		for (v1 = g->v, i = 0;
		     v1 != NULL && i < v1idx;
		     v1 = v1->next, i++);
		for (v2 = g->v, i = 0;
		     v2 != NULL && i < v2idx;
		     v2 = v2->next, i++);

		printf("add edge (%04llx,%04llx) to graph %d\n",
		       v1->id, v2->id, currgraph);

		vertex_edge_insert(v1, v2->id);
	}
}
