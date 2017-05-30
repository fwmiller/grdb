#include <assert.h>
#include <string.h>
#include "graph.h"

void
graph_init(graph_t g)
{
	assert (g != NULL);
	memset(g, 0, sizeof(struct graph));
}

void
graph_insert_component(graph_t g, component_t c)
{
	component_t d;

	if (g->c == NULL) {
		g->c = c;
		return;
	}
	for (d = g->c; d->next != NULL; d = d->next);
	d->next = c;
}
