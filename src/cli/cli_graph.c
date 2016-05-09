#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

void cli_graph_vertex(char *cmdline, int *pos);
void cli_graph_edge(char *cmdline, int *pos);
void cli_graph_schema(char *cmdline, int *pos);

static void
cli_graph_print()
{
	int i;

	for (i = 0; i < MAX_GRAPHS; i++) {
		if (i == currgraph)
			printf(">");
		printf("graph %d\n", i);
		graph_print(&(graphs[i]));
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

	else if (strcmp(s, "edge") == 0)
		cli_graph_edge(cmdline, &pos);

	else if (strcmp(s, "schema") == 0)
		cli_graph_schema(cmdline, &pos);

	else if (strlen(s) == 0)
		cli_graph_print();
}
