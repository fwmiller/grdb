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

printf("type [%s] base_types_str [%s]\n", type, base_types_str[i]);

		if (strcasecmp(type, base_types_str[i]) == 0) {
			attribute_t attr;

			attr = (attribute_t)
				malloc(sizeof(struct attribute));
			assert(attr != NULL);
			schema_attribute_init(attr, i, name);
			schema_attribute_insert(
				(s == EDGE ?
				 &(graphs[currgraph].se) :
				 &(graphs[currgraph].sv)), attr);
			break;
		}
	}
}

void
cli_graph_schema(char *cmdline, int *pos)
{
	char s[BUFSIZE];
	int i;

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
