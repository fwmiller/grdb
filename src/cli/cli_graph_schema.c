#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

enum schema_type { VERTEX, EDGE };

typedef enum schema_type schema_type_t;

static void
cli_graph_schema_add(schema_type_t s, char *cmdline, int *pos)
{
	char type[BUFSIZE];
	char name[BUFSIZE];
	int i;

	/* Attribute type */
        memset(type, 0, BUFSIZE);
        nextarg(cmdline, pos, " ", type);

	/* Attribute name */
        memset(name, 0, BUFSIZE);
        nextarg(cmdline, pos, " ", name);

	for (i = 0; i < BASE_TYPES_MAX; i++) {
		if (strcasecmp(type, base_types_str[i]) == 0) {
			attribute_t attr;

			attr = (attribute_t)
				malloc(sizeof(struct attribute));
			assert(attr != NULL);
			schema_attribute_init(attr, i, name);
			schema_attribute_insert(
				(s == EDGE ? &(current->se) : &(current->sv)),
				attr);
			break;
		}
	}
}

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
			}
			if (g->se != NULL) {
				printf("\nSe = ");
				schema_print(g->se);
			}
			printf("\n");
		}
	}
}
