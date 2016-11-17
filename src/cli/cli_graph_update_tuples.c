#include <assert.h>
#if _DEBUG
#include <stdio.h>
#endif
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
cli_graph_update_vertex_tuples(int old_schema_size)
{
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
}

void
cli_graph_update_edge_tuples(int old_schema_size)
{
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

void
cli_graph_update_tuples(schema_type_t st, int old_schema_size)
{
	if (st == VERTEX)
		cli_graph_update_vertex_tuples(old_schema_size);
	else if (st == EDGE)
		cli_graph_update_edge_tuples(old_schema_size);
}
