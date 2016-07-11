#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

void cli_graph_new(char *cmdline, int *pos);
void cli_graph_edge(char *cmdline, int *pos);
void cli_graph_schema(char *cmdline, int *pos);
void cli_graph_tuple(char *cmdline, int *pos);

static void
cli_graph_print()
{
	graph_t g;
	int cnt = 0;

	for (g = graphs; g != NULL; g = g->next, cnt++) {
		if (g == current)
			printf(">");
		printf("%d:", cnt);
		graph_print(g, 0); /* no tuples */
		printf("\n");
	}
}

void
cli_graph(char *cmdline, int *pos)
{
	char s[BUFSIZE];

	assert (cmdline != NULL);
	assert (pos != NULL);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strcmp(s, "new") == 0 || strcmp(s, "n") == 0)
		cli_graph_new(cmdline, pos);

	else if (strcmp(s, "edge") == 0 || strcmp(s, "e") == 0)
		cli_graph_edge(cmdline, pos);

	else if (strcmp(s, "schema") == 0 || strcmp(s, "s") == 0)
		cli_graph_schema(cmdline, pos);

	else if (strcmp(s, "tuple") == 0 || strcmp(s, "t") == 0)
		cli_graph_tuple(cmdline, pos);

	else if (isdigit(s[0])) {
		graph_t g;
		int i, cnt;

		// Change current graph
		i = atoi(s);
		for (g = graphs, cnt = 0; g != NULL; g = g->next, cnt++)
			if (cnt == i) {
				current = g;
				return;
			}

	} else if (strlen(s) == 0)
		cli_graph_print();
}
