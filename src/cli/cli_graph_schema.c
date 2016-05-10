#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

void schema_print(schema_t s);

void
cli_graph_schema(char *cmdline, int *pos)
{
        char s[BUFSIZE];
	int i;

        memset(s, 0, BUFSIZE);
        nextarg(cmdline, pos, " ", s);

	if (strcmp(s, "add") == 0) {
		/* Add an attribute to the schema */

		return;
	}
	/* Display all graph schemas */
	for (i = 0; i < MAX_GRAPHS; i++) {
		if (graphs[i].sv != NULL || graphs[i].se != NULL) {
			if (i == currgraph)
				printf(">");
			printf("graph %d\n", i);
			if (graphs[i].sv != NULL) {
				printf("Sv ");
				schema_print(graphs[i].sv);
			}
			if (graphs[i].se != NULL) {
				printf("\nSe ");
				schema_print(graphs[i].se);
			}
			printf("\n");
		}
	}
}
