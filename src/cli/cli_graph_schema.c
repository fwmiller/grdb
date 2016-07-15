#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

enum schema_type { VERTEX, EDGE };

typedef enum schema_type schema_type_t;

static void
cli_graph_update_tuples(schema_type_t s)
{
	if (s == VERTEX) {
		vertex_t v;

		/*
		 * Loop over all vertices in the current graph and update
		 *   their tuples
		 */
		for (v = current->v; v != NULL; v = v->next) {
			if (v->tuple == NULL) {
				tuple_t t;

				/* Create a new tuple for the vertex */
				t = (tuple_t) malloc(sizeof(struct tuple));
				assert (t != NULL);
				tuple_init(t, current->sv);
				v->tuple = t;
			}
		}

	} else if (s == EDGE) {
		edge_t e;

		/*
		 * Loop over all edges in the current graph and update
		 *   their tuples
		 */
		for (e = current->e; e != NULL; e = e->next) {
			if (e->tuple == NULL) {
				tuple_t t;

				/* Create a new tuple for the edge */
				t = (tuple_t) malloc(sizeof(struct tuple));
				assert (t != NULL);
				tuple_init(t, current->se);
				e->tuple = t;
			}
		}

	}
}

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

			/*
			 * Create a new schema attribute and insert it
			 *   in the schema for the edge|vertex
			 */
			attr = (attribute_t)
				malloc(sizeof(struct attribute));
			assert(attr != NULL);
			schema_attribute_init(attr, i, name);
			schema_attribute_insert( (s == EDGE ?
					&(current->se) :
					&(current->sv)),
					attr);
			break;
		}
	}
	cli_graph_update_tuples(s);
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
				printf("\n");
			}
			if (g->se != NULL) {
				printf("Se = ");
				schema_print(g->se);
			}
			printf("\n");
		}
	}
}
