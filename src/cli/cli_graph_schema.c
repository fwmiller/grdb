#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

#if _DEBUG
void bufdump(char *buf, int size);
#endif

#define CLI_GRAPH_CREATE_TUPLE(SCHEMA, ELEMENT)				\
{									\
	tuple_t t;							\
	t = (tuple_t) malloc(sizeof(struct tuple));			\
	assert (t != NULL);						\
	tuple_init(t, SCHEMA);						\
	(ELEMENT)->tuple = t;						\
}

#define CLI_GRAPH_MODIFY_TUPLE(SCHEMA, ELEMENT, OLD_SCHEMA_SIZE)	\
{									\
	tuple_t t;							\
	t = (tuple_t) malloc(sizeof(struct tuple));			\
	assert (t != NULL);						\
	tuple_init(t, SCHEMA);						\
	memcpy(t->buf, (ELEMENT)->tuple->buf, OLD_SCHEMA_SIZE);		\
	tuple_delete((ELEMENT)->tuple);					\
	(ELEMENT)->tuple = t;						\
}

static void
cli_graph_update_tuples(schema_type_t st, int old_schema_size)
{
	if (st == VERTEX) {
		vertex_t v;

		/* Update all vertices in the graph */
		for (v = current->v; v != NULL; v = v->next)
			if (v->tuple == NULL) {
				CLI_GRAPH_CREATE_TUPLE(current->sv, v);
#if _DEBUG
				printf("create old_schema_size %d\n", old_schema_size);
				bufdump(v->tuple->buf, v->tuple->len);
#endif
			} else {
				CLI_GRAPH_MODIFY_TUPLE(
					current->sv, v, old_schema_size);
#if _DEBUG
				printf("modify old_schema_size %d\n", old_schema_size);
				bufdump(v->tuple->buf, v->tuple->len);
#endif
			}

	} else if (st == EDGE) {
		edge_t e;

		/* Update all edges in the graph */
		for (e = current->e; e != NULL; e = e->next)
			if (e->tuple == NULL) {
				CLI_GRAPH_CREATE_TUPLE(current->se, e);
#if _DEBUG
				printf("create old_schema_size %d\n", old_schema_size);
				bufdump(e->tuple->buf, e->tuple->len);
#endif
			} else {
				CLI_GRAPH_MODIFY_TUPLE(
					current->se, e, old_schema_size);
#if _DEBUG
				printf("modify old_schema_size %d\n", old_schema_size);
				bufdump(e->tuple->buf, e->tuple->len);
#endif
			}
	}
}

static void
cli_graph_schema_add(schema_type_t st, char *cmdline, int *pos)
{
	char type[BUFSIZE];
	char name[BUFSIZE];
	int i, old_schema_size = 0;

	if (st == EDGE && current->se != NULL)
		old_schema_size = schema_size(current->se);
	else if (st == VERTEX && current->sv != NULL)
		old_schema_size = schema_size(current->sv);

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
			if (st == EDGE) {
				if (current->se == NULL)
					schema_init(&(current->se));
				schema_attribute_insert(current->se, attr);
			} else if (st == VERTEX) {
				if (current->sv == NULL)
					schema_init(&(current->sv));
				schema_attribute_insert(current->sv, attr);
			}
			break;
		}
	}
	printf("update tuples\n");
	cli_graph_update_tuples(st, old_schema_size);
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
				printf("\n");
			}
		}
	}
}
