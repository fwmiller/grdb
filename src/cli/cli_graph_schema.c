#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

void cli_graph_schema_add(schema_type_t st, char *cmdline, int *pos);

void
cli_graph_schema(char *cmdline, int *pos)
{
	char s[BUFSIZE];
	graph_t g;
	int cnt;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strcmp(s, "vertex") == 0 || strcmp(s, "v") == 0) {
		cli_graph_schema_add(VERTEX, cmdline, pos);
		return;
	}
	if (strcmp(s, "edge") == 0 || strcmp(s, "e") == 0) {
		cli_graph_schema_add(EDGE, cmdline, pos);
		return;
	}
	/* Display all graph schemas */
	for (g = graphs, cnt = 0; g != NULL; g = g->next, cnt++) {
		if (g->sv != NULL || g->se != NULL) {
			if (g == current)
				printf(">");
			printf("graph %d\n", cnt);
			if (g->sv != NULL) {
				printf("Sv = ");
				schema_print(g->sv);
				printf("\n");
			}
			if (g->se != NULL) {
				printf("Se = ");
				schema_print(g->se);
				printf("\n");
			}
		}
	}
}
