#include <assert.h>
#include <stdlib.h>
#include "cli.h"

void
cli_graph_new(char *cmdline, int *pos)
{
	component_t g;
	vertex_t v;

	/* Create first vertex in component */
	v = (vertex_t) malloc(sizeof(struct vertex));
	assert (v != NULL);
	vertex_init(v);
	v->id = 1;

	/* Create new component */
	g = (component_t) malloc(sizeof(struct component));
	assert (g != NULL);
	component_init(g);
	component_insert_vertex(g, v);

	if (graphs == NULL)
		current = g;
	cli_graphs_insert(g);
}
