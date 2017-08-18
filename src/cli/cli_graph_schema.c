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
	component_t c;
	int ccnt, gcnt = 0;

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
/*
	for (g = graphs; g != NULL; g = g->next, gcnt++) {
		for (c = g->c, ccnt = 0; c != NULL; c = c->next, ccnt++) {
			if (c->sv != NULL || c->se != NULL) {
				if (g == current_graph &&
				    c == current_component)
					printf(">");
				printf("component %d.%d\n", gcnt, ccnt);
				if (c->sv != NULL) {
					printf("Sv = ");
					schema_print(c->sv);
					printf("\n");
				}
				if (c->se != NULL) {
					printf("Se = ");
					schema_print(c->se);
					printf("\n");
				}
			}
		}
	}
*/
}
